#include "ffmpegmanager.h"
#include <QDebug>
#include <QtConcurrent>
#include <QDateTime>
#include <QAudioDevice>
#include <QMediaDevices>

bool FFmpegManager::s_init = false;
FFmpegManager::FFmpegManager(QObject *parent)
    : QObject{parent}
{
    // 初始化flush packet
    m_flushPacket = av_packet_alloc();
    m_flushPacket->data = (uint8_t *)m_flushPacket;

    m_swrContext = nullptr;
    m_audioCodecContext = nullptr;
    m_formatContext = nullptr;
    m_audioDevice = nullptr;
    m_audioSink = nullptr;
    m_audioStreamIndex = -1;
    m_hasAudio = false;
    m_decoderPaused = false;
    m_seekPosition = -1;
    m_volume = 1.0f;
    m_muted = false;
    m_audioPts     = 0;
    m_audioPtsBase = 0;
    initFFmpeg();
}

FFmpegManager::~FFmpegManager()
{
    av_packet_free(&m_flushPacket);
    close();
    cleanFFmpeg();
}

bool FFmpegManager::open(const QString &filePath)
{
    close();
    // 打开输入文件
    int ret = avformat_open_input(&m_formatContext, filePath.toUtf8().constData(), nullptr, nullptr);
    if (ret < 0) 
    {
        qDebug() << "ffmpeg open fail:" << ret;
        return false;
    }
    // 查找流信息
    ret = avformat_find_stream_info(m_formatContext, nullptr);
    if (ret < 0) 
    {
        qDebug() << "The stream info cannot be found:" << ret;
        close();
        return false;
    }
    // 查找音频流
    m_audioStreamIndex = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (m_audioStreamIndex < 0) 
    {
        qDebug() << "The audio stream cannot be found";
        close();
        return false;
    }
    // 获取音频流
    AVStream *audioStream = m_formatContext->streams[m_audioStreamIndex];
    // 查找解码器
    const AVCodec *codec = avcodec_find_decoder(audioStream->codecpar->codec_id);
    if (!codec) 
    {
        qDebug() << "The decoder cannot be found";
        close();
        return false;
    }
    // 分配解码器上下文
    m_audioCodecContext = avcodec_alloc_context3(codec);
    if (!m_audioCodecContext) 
    {
        qDebug() << "codec alloc context fail";
        close();
        return false;
    }
    // 复制参数到解码器上下文
    ret = avcodec_parameters_to_context(m_audioCodecContext, audioStream->codecpar);
    if (ret < 0) 
    {
        qDebug() << "codec parameters to context fail";
        close();
        return false;
    }
    // 打开解码器
    ret = avcodec_open2(m_audioCodecContext, codec, nullptr);
    if (ret < 0) 
    {
        qDebug() << "audio codec open fail:" << ret;
        close();
        return false;
    }

    // 获取音频信息
    m_sampleRate = m_audioCodecContext->sample_rate;
    int channels = m_audioCodecContext->ch_layout.nb_channels;
    AVSampleFormat srcFmt = m_audioCodecContext->sample_fmt;

    // 初始化重采样（输出固定为 16位 立体声）
    if (srcFmt != AV_SAMPLE_FMT_S16 || channels != 2) 
    {
        m_swrContext = swr_alloc();
        if (!m_swrContext) 
        {
            qDebug() << "swr_alloc failed";
            close();
            return false;
        }

        AVChannelLayout in_ch_layout;
        av_channel_layout_default(&in_ch_layout, channels);
        AVChannelLayout out_ch_layout = AV_CHANNEL_LAYOUT_STEREO; // 定义输出通道布局（立体声）
        enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;// 目标采样格式

        // 设置重采样器参数
        int ret = swr_alloc_set_opts2(&m_swrContext,
                                      &out_ch_layout, out_sample_fmt, m_sampleRate,
                                      &in_ch_layout, srcFmt, m_sampleRate,
                                      0, nullptr);
        if (ret < 0) 
        {
            qDebug() << "swr_alloc_set_opts2 failed:" << ret;
            close();
            return false;
        }

        ret = swr_init(m_swrContext);
        if (ret < 0) 
        {
            qDebug() << "swr_init failed:" << ret;
            close();
            return false;
        }
    }

    // 计算时长
    int duration;
    if (audioStream->duration == AV_NOPTS_VALUE) 
    {
        duration = m_formatContext->duration;
    } 
    else 
    {
        AVRational time_base = audioStream->time_base;
        duration = av_rescale_q(audioStream->duration, time_base, AV_TIME_BASE_Q);
    }

    duration /= 1000; // 转换为毫秒
    m_hasAudio = true;
    qDebug() << "avformat open success:"
            << "rate:" << m_sampleRate
            << "channels:" << channels
            << "format:" << av_get_sample_fmt_name(srcFmt)
            << "duration:" << duration << "ms";

    return true;
}

void FFmpegManager::close()
{
    stop();
    if (m_swrContext) 
    {
        swr_free(&m_swrContext);
        m_swrContext = nullptr;
    }
    
    if (m_audioCodecContext) 
    {
        avcodec_free_context(&m_audioCodecContext);
        m_audioCodecContext = nullptr;
    }
    
    if (m_formatContext) 
    {
        avformat_close_input(&m_formatContext);
        m_formatContext = nullptr;
    }
    m_hasAudio = false;
    cleanupAudioOutput();
}

bool FFmpegManager::isOpen()
{
    return m_hasAudio;
}

bool FFmpegManager::play()
{
    if (!m_hasAudio || m_audioState == AUDIO_PLAYING) 
        return false;

    if(!initAudioOutput())
        return false;

    m_audioState = AUDIO_PLAYING;
    QtConcurrent::run([this]() { decodeLoop(); });
    return true;
}

void FFmpegManager::pause()
{
    m_audioState = AUDIO_PAUSED;
    if (m_audioSink) 
        m_audioSink->suspend();
}

void FFmpegManager::stop()
{
    m_audioState = AUDIO_STOPPED;
    m_audioPts     = 0;
    m_audioPtsBase = 0;

    if (m_audioSink) 
        m_audioSink->stop();
}

void FFmpegManager::resume()
{
    m_audioState = AUDIO_PLAYING;
    if (m_audioSink) 
        m_audioSink->resume();
}

void FFmpegManager::seek(qint64 position)
{
    m_seekPosition = position;
}

void FFmpegManager::setVolume(float volume)
{
    m_volume = volume / 100.0f;
    m_muted = false;
    if (m_audioSink)
        m_audioSink->setVolume(m_volume);
}

void FFmpegManager::setMuted(bool muted)
{
    m_muted = muted;
    float volume = m_muted ? 0.0f : m_volume.load();
    if (m_audioSink)
        m_audioSink->setVolume(volume);
}

qint64 FFmpegManager::getAudioClock() const
{
    if (m_audioState != AUDIO_PLAYING || m_audioPtsBase == 0)
        return m_audioPts;
    return QDateTime::currentMSecsSinceEpoch() - m_audioPtsBase;
}

void FFmpegManager::initFFmpeg()
{
    if (!s_init) 
    {
        avformat_network_init();
        s_init = true;
    }
}

void FFmpegManager::cleanFFmpeg()
{
    if (s_init)
        avformat_network_deinit();
}

bool FFmpegManager::initAudioOutput()
{
    if (!m_hasAudio) 
        return false;

    if(!m_audioSink)
    {
        m_audioFormat.setSampleRate(m_sampleRate);
        m_audioFormat.setChannelCount(2);          // 强制立体声
        m_audioFormat.setSampleFormat(QAudioFormat::Int16);
        QAudioDevice device = QMediaDevices::defaultAudioOutput();
        m_audioSink = new QAudioSink(device, m_audioFormat, this);
        float volume = m_muted ? 0.0f : m_volume.load();
        m_audioSink->setVolume(volume);
        connect(m_audioSink, &QAudioSink::stateChanged, [this](QAudio::State state) {
            qDebug() << "Audio state changed:" << state;
        });
    }


    return true;
}

void FFmpegManager::cleanupAudioOutput()
{
    if (m_audioSink)
    {
        m_audioSink->stop();
        delete m_audioSink;
        m_audioSink = nullptr;
    }
    m_audioDevice = nullptr;
}

void FFmpegManager::decodeLoop()
{
    qDebug() << "audio startTime:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    AVPacket *packet = av_packet_alloc();
    while (true)
    {
        if (m_audioState == AUDIO_PAUSED) 
        {
            QThread::msleep(100);
            continue;
        }

        if (m_seekPosition >= 0) 
        {
            // 将目标时间（毫秒）转换为音频流的 time_base 单位
            int64_t seekTarget = av_rescale_q(m_seekPosition, AV_TIME_BASE_Q, m_formatContext->streams[m_audioStreamIndex]->time_base);
            // 执行跳转（向后查找关键帧）
            av_seek_frame(m_formatContext, m_audioStreamIndex, seekTarget, AVSEEK_FLAG_BACKWARD);
            // 清理解码器内部缓冲，避免跳转后残留旧数据
            avcodec_flush_buffers(m_audioCodecContext);
            m_seekPosition = -1;
            // 更新音频时钟到新的位置
            m_audioPts = m_seekPosition.load();
            m_audioPtsBase = QDateTime::currentMSecsSinceEpoch() - m_seekPosition;
        }

        if (m_audioState == AUDIO_STOPPED) 
            break;

        // 从文件中读取一个数据包
        int ret = av_read_frame(m_formatContext, packet);
        if(ret == AVERROR_EOF) 
        {
            qDebug() << "EOF audio stream finished";
            break;
        }
        else if (ret < 0) 
        {
            char errbuf[128];
            av_strerror(ret, errbuf, sizeof(errbuf));
            qDebug() << "av_read_frame error:" << ret << errbuf;
            break;
        }

        // 只处理属于音频流的数据包
        if (packet->stream_index == m_audioStreamIndex) 
        {
            QByteArray audioData;      // 解码后的 PCM 数据
            qint64 posMs;              // 该帧的时间戳（毫秒）
            if (decodeAndResample(packet, audioData, posMs)) 
            {
                // 如果音量不为 1 且未静音，对 PCM 样本进行音量调节
                if (m_volume != 1.0f && !audioData.isEmpty() && !m_muted) 
                {
                    int16_t *samples = (int16_t*)audioData.data();
                    int cnt = audioData.size() / sizeof(int16_t);
                    for (int i = 0; i < cnt; ++i) 
                    {
                        samples[i] = (int16_t)(samples[i] * m_volume);
                    }
                }
                writeAudioData(audioData);
                // 更新音频时钟
                m_audioPts = posMs;
                if (m_audioPtsBase == 0)
                    m_audioPtsBase = QDateTime::currentMSecsSinceEpoch() - posMs;
            }
        }

        // 释放数据包，避免内存泄漏
        av_packet_unref(packet);
    }

    qDebug() << "audio endTime:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    av_packet_free(&packet);
    m_audioState = AUDIO_STOPPED;
}

bool FFmpegManager::decodeAndResample(AVPacket *pkt, QByteArray &out, qint64 &posMs)
{
    if (!m_audioCodecContext) 
        return false;

    // 将压缩数据包送入解码器
    int ret = avcodec_send_packet(m_audioCodecContext, pkt);
    if (ret < 0) 
        return false;

    // 分配一个帧对象，用于存放解码后的数据
    AVFrame *frame = av_frame_alloc();
    if (!frame) 
        return false;

    // 从解码器接收一个解码后的帧
    ret = avcodec_receive_frame(m_audioCodecContext, frame);
    if (ret < 0) 
    {
        av_frame_free(&frame);
        return false;
    }

    // 获取该帧的 PTS（显示时间戳），并转换为毫秒
    if (frame->pts != AV_NOPTS_VALUE && m_formatContext) 
    {
        AVStream *st = m_formatContext->streams[m_audioStreamIndex];
        posMs = av_rescale_q(frame->pts, st->time_base, AV_TIME_BASE_Q) / 1000;
    } 
    else
     {
        posMs = 0;
    }

    // 重采样部分：确保最终输出为 16位 立体声
    if (!m_swrContext && frame->format == AV_SAMPLE_FMT_S16 && frame->ch_layout.nb_channels == 2) 
    {
        // 如果原始格式已经是 16位立体声，直接拷贝数据
        int sz = av_samples_get_buffer_size(nullptr, frame->ch_layout.nb_channels, frame->nb_samples,
                                            (AVSampleFormat)frame->format, 1);
        out.resize(sz);
        memcpy(out.data(), frame->data[0], sz);
    } 
    else if (m_swrContext) 
    {
        // 需要重采样（可能是格式/通道数不匹配）
        // 计算目标样本数（考虑重采样器内部缓冲）
        int64_t dst_nb_samples = av_rescale_rnd(swr_get_delay(m_swrContext, frame->sample_rate) +
                                                frame->nb_samples,
                                                m_sampleRate, frame->sample_rate, AV_ROUND_UP);
        int dst_nb_channels = 2;  // 目标通道数固定为立体声
        uint8_t **dst_data = nullptr;
        int dst_linesize;

        // 分配目标缓冲区
        ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
                                                 dst_nb_samples, AV_SAMPLE_FMT_S16, 0);
        if (ret < 0) 
        {
            av_frame_free(&frame);
            return false;
        }

        // 执行重采样
        ret = swr_convert(m_swrContext, dst_data, dst_nb_samples,
                          (const uint8_t**)frame->data, frame->nb_samples);
        if (ret < 0) 
        {
            av_freep(&dst_data[0]);
            av_freep(&dst_data);
            av_frame_free(&frame);
            return false;
        }

        // 获取重采样后实际数据的大小
        int dst_bufsize = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
                                                     ret, AV_SAMPLE_FMT_S16, 1);
        out.resize(dst_bufsize);
        memcpy(out.data(), dst_data[0], dst_bufsize);

        // 释放临时缓冲区
        av_freep(&dst_data[0]);
        av_freep(&dst_data);
    }

    av_frame_free(&frame);
    return true;
}

void FFmpegManager::writeAudioData(const QByteArray &data)
{
    if (data.isEmpty() || m_muted) 
        return;

    if (!m_audioDevice) 
    {
        m_audioDevice = m_audioSink->start();
        if (!m_audioDevice) 
        {
            qDebug() << "Failed to start audio device";
            return;
        }
    }

    const char *ptr = data.constData();
    qint64 remaining = data.size();
    while (remaining > 0) 
    {
        qint64 written = m_audioDevice->write(ptr, remaining);
        if (written < 0) {
            qDebug() << "Write error:" << m_audioSink->error();
            return;
        }
        remaining -= written;
        ptr += written;
        if (remaining > 0) 
        {
            QThread::msleep(10);
        }
    }
}

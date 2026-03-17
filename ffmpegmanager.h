#ifndef FFMPEGMANAGER_H
#define FFMPEGMANAGER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QAudioOutput>
#include <QAudioSink>
#include <QAudioFormat>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavcodec/packet.h>
    #include <libavutil/avutil.h>
    #include <libavutil/opt.h>
    #include <libswresample/swresample.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/samplefmt.h>
}

class FFmpegManager : public QObject
{
    Q_OBJECT
public:
    enum AudioState {
        AUDIO_STOPPED = 0,
        AUDIO_PLAYING,
        AUDIO_PAUSED,
        AUDIO_ERROR
    };
    
    explicit FFmpegManager(QObject *parent = nullptr);
    ~FFmpegManager();
    // 音频文件操作
    bool open(const QString &filePath);
    void close();
    bool isOpen();
    // 播放控制
    bool play();
    void pause();
    void stop();
    void resume();
    void seek(qint64 position);
    // 音量控制
    void setVolume(float volume); 
    void setMuted(bool muted);

    qint64 getAudioClock() const;

private:
    void initFFmpeg();
    void cleanFFmpeg();
    bool initAudioOutput();
    void cleanupAudioOutput();
    void decodeLoop();
    bool decodeAndResample(AVPacket *packet, QByteArray &output, qint64 &positionMs);
    void writeAudioData(const QByteArray &data);

signals:
    void audioStateChanged(FFmpegManager::AudioState state);
    void audioPositionChanged(qint64 positionMs);

private:
    AVPacket *m_flushPacket;
    AVFormatContext *m_formatContext;
    AVCodecContext *m_audioCodecContext;
    SwrContext *m_swrContext;
    int m_audioStreamIndex;
    static bool s_init;

    // 音频信息
    bool m_hasAudio;
    int m_sampleRate;
    qint64 m_duration;
    
    // 播放状态
    std::atomic<int> m_audioState; 
    std::atomic<bool> m_decoderPaused;
    
    // 跳转控制
    std::atomic<qint64> m_seekPosition;
    std::atomic<qint64> m_audioPts;
    std::atomic<qint64> m_audioPtsBase;
    
    // 音量控制
    std::atomic<float> m_volume;
    std::atomic<bool> m_muted;
    
    // Qt音频输出
    QAudioSink *m_audioSink;
    QAudioFormat m_audioFormat;
    QAudioOutput *m_audioOutput;// Qt6使用QAudioSink，Qt5使用QAudioOutput
    QIODevice *m_audioDevice;
};

#endif // FFMPEGMANAGER_H

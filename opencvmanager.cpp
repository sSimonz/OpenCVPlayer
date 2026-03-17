#include "opencvmanager.h"
#include <QDebug>

OpenCVManager::OpenCVManager(QObject *parent)
    : QObject{parent}
{
    
}

OpenCVManager::~OpenCVManager()
{
    
}

QImage OpenCVManager::cvMatToImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC4: {
        QImage image(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_ARGB32);
        return image.copy();
    }
    case CV_8UC3: {
        QImage image(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_RGB888);
        return image.rgbSwapped().copy();
    }
    case CV_8UC1: {
        QImage image(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_Grayscale8);
        return image.copy();
    }
    default:
        return QImage();
    }
}

cv::Mat OpenCVManager::image2cvMat(const QImage &image)
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, COLOR_BGRA2BGR);
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
    case QImage::Format_Grayscale8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        qDebug() << __FILE__ << __LINE__ << "error to image2Mat !!!  imge.format =" << image.format();
        return mat;
    }
}

int OpenCVManager::formatToFourCC(const QString &format)
{
    if (format == "avi")
        return cv::VideoWriter::fourcc('M', 'J', 'P', 'G'); // MJPG编码的AVI
    else if (format == "mp4")
        return cv::VideoWriter::fourcc('m', 'p', '4', 'v'); // MP4V编码
    else if (format == "mov")
        return cv::VideoWriter::fourcc('m', 'p', '4', 'v'); // 也适用于MOV
    else if (format == "mkv")
        return cv::VideoWriter::fourcc('X', '2', '6', '4'); // H.264编码
    else
        return cv::VideoWriter::fourcc('M', 'J', 'P', 'G'); // 默认MJPG
}

bool OpenCVManager::open(const QString &filePath)
{
    stop();
    m_video = new cv::VideoCapture(filePath.toStdString());
    if(!m_video->isOpened())
    {
        stop();
        return false;
    }

    m_currentFrame = 0;
    m_duration = 0;
    m_totalFrame = static_cast<int>(m_video->get(cv::CAP_PROP_FRAME_COUNT));
    m_fps = static_cast<int>(m_video->get(cv::CAP_PROP_FPS));
    if(m_fps <= 0)// 默认25fps
        m_fps = 25;

    qint64 duration = static_cast<qint64>(m_totalFrame * 1000 / m_fps);
    emit videoOpened(duration);
    return true;
}

void OpenCVManager::stop()
{
    if(m_video)
    {
        m_video->release();
        delete m_video;
        m_video = nullptr;
    }
}

bool OpenCVManager::isOpen()
{
    return m_video && m_video->isOpened();
}

bool OpenCVManager::capture(const QString &filePath)
{
    QMutexLocker locker(&m_mutex);
    if(!isOpen())
        return false;

    cv::Mat mat;
    if (!m_video->retrieve(mat, 0))// 解码当前帧
        return false;

    return cv::imwrite(filePath.toStdString(), mat);
}

bool OpenCVManager::isRecording()
{
    return m_videoWriter && m_videoWriter->isOpened();
}

bool OpenCVManager::startRecord(const QString &filePath, const QString &format)
{
    QMutexLocker locker(&m_mutex);
    if(!isOpen())
        return false;
        
    int fourcc = formatToFourCC(format);
    cv::Size frameSize(static_cast<int>(m_video->get(cv::CAP_PROP_FRAME_WIDTH)),
                        static_cast<int>(m_video->get(cv::CAP_PROP_FRAME_HEIGHT)));
    m_videoWriter = new cv::VideoWriter(filePath.toStdString(), fourcc, m_fps, frameSize, true);
    return true;
}

void OpenCVManager::stopRecord()
{
    QMutexLocker locker(&m_mutex);
    if(m_videoWriter)
    {
        m_videoWriter->release();
        delete m_videoWriter;
        m_videoWriter = nullptr;
    }
}

void OpenCVManager::seek(qint64 position)
{
    QMutexLocker locker(&m_mutex);
    if(!isOpen())
        return;

    int targetFrame = static_cast<int>((position * m_fps) / 1000.0);
    if(targetFrame < m_totalFrame)
    {
        m_video->set(cv::CAP_PROP_POS_FRAMES, targetFrame);
        m_currentFrame = targetFrame;
        m_duration = position;
    }
}

 bool OpenCVManager::refreshFrame()
 {
    QMutexLocker locker(&m_mutex);
    if(!isOpen())
        return false;

    if(m_currentFrame >= m_totalFrame)
        return false;
    
    cv::Mat mat;
    m_video->read(mat);// 读取当前帧，指向下一帧
    if(mat.empty())
        return false;

    // 录像
    if(isRecording())
    {
        m_videoWriter->write(mat);
    }
    // cv::resize(mat, mat, cv::Size(m_videoSize.width(), m_videoSize.height()));
    QImage image = cvMatToImage(mat);
    m_currentFrame++;
    m_duration = static_cast<qint64>((m_currentFrame * 1000.0) / m_fps);
    emit frameReady(image, m_duration);

    return true;
 }

 bool OpenCVManager::skipFrame()
 {
    QMutexLocker locker(&m_mutex);
    if(!isOpen())
        return false;

    if(m_currentFrame >= m_totalFrame)
        return false;

    m_video->grab();// 跳过当前帧，指向下一帧
    m_currentFrame++;
    m_duration = static_cast<qint64>((m_currentFrame * 1000.0) / m_fps);
    return true;
 }

 int OpenCVManager::delayTime()
 {
     return 1000 / m_fps;
 }

 qint64 OpenCVManager::getCurrentPosition()
 {
     return m_duration;
 }


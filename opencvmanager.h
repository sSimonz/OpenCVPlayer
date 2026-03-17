#ifndef OPENCVMANAGER_H
#define OPENCVMANAGER_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include "opencv2/opencv.hpp"

using namespace cv;
class OpenCVManager : public QObject
{
    Q_OBJECT
public:
    explicit OpenCVManager(QObject *parent = nullptr);
    ~OpenCVManager();
    static QImage cvMatToImage(const cv::Mat &mat);// OpenCV的Mat转QImage
    static cv::Mat image2cvMat(const QImage &image);// QImage转OpenCV的Mat
    static int formatToFourCC(const QString &format);// 获取格式对应的FourCC编码
    bool open(const QString &filePath);
    void stop();
    bool isOpen();
    bool capture(const QString &filePath);
    bool isRecording();
    bool startRecord(const QString &filePath, const QString &format);
    void stopRecord();
    void seek(qint64 position);
    bool refreshFrame();
    bool skipFrame();
    int delayTime();
    qint64 getCurrentPosition();

signals:
    void frameReady(const QImage &image, qint64 duration);
    void videoOpened(qint64 duration);

private:
    cv::VideoCapture *m_video;// 播放器
    cv::VideoWriter *m_videoWriter;  // 录制器

    int m_fps;
    int m_totalFrame;
    int m_currentFrame;
    int m_duration;
    QMutex m_mutex;
};

#endif // OPENCVMANAGER_H

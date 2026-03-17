#ifndef WIDGET_H
#define WIDGET_H

#include "BaseUI/digibasewidget.h"
#include <QElapsedTimer>
#include "opencvmanager.h"
#include "ffmpegmanager.h"

using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QTimer;
class DigiVolumeControl;
class DigiBaseMenu;
class Widget : public DigiBaseWidget
{
    Q_OBJECT

    enum PlayState
    {
        STATE_STOP = 0,
        STATE_PLAY,
        STATE_PAUSE
    };

    enum RecordState
    {
        RECORD_STOP = 0,
        RECORD_RECORDING
    };

    enum FileType
    {
        FILE_VIDEO = 0,
        FILE_PICTURE,
        FILE_OTHER
    };

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void captrueFrame();// 刷新当前帧

protected:
    bool eventFilter(QObject *watched, QEvent *event);// 捕捉滑动条事件

private:
    void init();
    void createConnect();
    bool play();
    void pause();
    void resume();
    void stop();
    QString formatTime(int ms);
    QString getFileName(FileType type);

private slots:
    void onPlayVideoRsp(const QString &filePath);
    void onBtnPlayClicked();
    void onScreenShotRsp();
    void onRecordRsp();
    void onVolumePopup();
    void onSpeedCtrlPopup();
    void onRecordTimeout();
    void onFrameReady(const QImage &image, qint64 duration);
    void onVideoOpened(qint64 duration);
    void onVolumeRsp(int volume);
    void onVolumeControl(bool b);

private:
    Ui::Widget *ui;
    QString m_filePath;
    OpenCVManager *m_video;
    std::atomic<int> m_playState;
    std::atomic<bool> m_bRecording;
    QString m_recordFormat;
    QSize m_videoSize;
    std::atomic<int> m_jmpToPos;
    bool m_bSliderHover;
    bool m_bSliderPress;
    std::atomic<double> m_playSpeed;
    QTimer *m_recordTimer;
    QElapsedTimer m_recordElapsedTimer;
    DigiVolumeControl *m_volumeControl;
    DigiBaseMenu *m_speedCtrlMenu;
    int m_duration;

    FFmpegManager *m_audio;
};
#endif // WIDGET_H

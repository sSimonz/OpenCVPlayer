#include "widget.h"
#include "ui_widget.h"

#include <QTimer>
#include <QThread>
#include <QtConcurrent>
#include <QMouseEvent>
#include "BaseUI/digimsgpopup.h"
#include "BaseUI/digivolumecontrol.h"
#include "BaseUI/digibasemenu.h"

Widget::Widget(QWidget *parent)
    : DigiBaseWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    createConnect();
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->m_slider)
    {
        switch (event->type())
        {
        case QEvent::Enter:
        {
            m_bSliderHover = true;
            return false;
        }
        case QEvent::Leave:
        {
            m_bSliderHover = false;
            QString curTime = formatTime(m_duration);
            ui->m_labCurTime->setText(curTime);
            return false;
        }
        case QEvent::MouseButtonPress:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                m_bSliderPress = true;
            return false;
        }
        case QEvent::MouseButtonRelease:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)	//判断左键
            {
                m_bSliderPress = false;
                int dur = ui->m_slider->maximum() - ui->m_slider->minimum();
                double per = (1.0*mouseEvent->x() / ui->m_slider->width());
                int pos = ui->m_slider->minimum() + dur * per;
                if(pos != ui->m_slider->sliderPosition())
                {
                    ui->m_slider->setValue(pos);
                    m_jmpToPos = pos;
                    m_audio->seek(pos);
                }
            }
            return false;
        }
        case QEvent::MouseMove:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            int dur = ui->m_slider->maximum() - ui->m_slider->minimum();
            double per = (1.0*mouseEvent->x() / ui->m_slider->width());
            int pos = ui->m_slider->minimum() + dur * per;
            QString curTime = formatTime(pos);
            ui->m_labCurTime->setText(curTime);
            return false;
        }
        default:
            return DigiBaseWidget::eventFilter(watched, event);
        }
    }

    return DigiBaseWidget::eventFilter(watched, event);
}

void Widget::init()
{
    m_video = new OpenCVManager(this);
    m_audio = new FFmpegManager(this);
    ui->m_slider->setValueVisible(false);
    ui->m_slider->installEventFilter(this);
    ui->m_slider->setMouseTracking(true);
    ui->m_playWidget->SetBGColor(ColorManager::GetColor(ColorManager::CN_VIEWSCREEN_BACKGROUND));
    ui->m_btnFullScreen->setToolTip("Full Screen");
    ui->m_btnPlay->setToolTip("Play");
    ui->m_btnScreenShot->setToolTip("Screenshot");
    ui->m_btnRecord->setToolTip("Record");
    ui->m_btnVolume->setToolTip("Volume");
    m_jmpToPos = -1;
    m_playState = STATE_STOP;
    m_bRecording = false;
    m_recordFormat = "avi";
    m_bSliderHover = false;
    m_bSliderPress = false;
    m_volumeControl = new DigiVolumeControl(this);
    m_speedCtrlMenu = nullptr;
    m_playSpeed = 1.0;
    m_recordTimer = new QTimer(this);
    m_recordTimer->setInterval(1000);
    SetScale(1.0);
}

void Widget::createConnect()
{
    connect(ui->m_playWidget, &PlayWidget::fileDropped, this, &Widget::onPlayVideoRsp);
    connect(ui->m_btnScreenShot, &QPushButton::clicked, this, &Widget::onScreenShotRsp);
    connect(ui->m_btnRecord, &QPushButton::clicked, this, &Widget::onRecordRsp);
    connect(ui->m_btnPlay, &QPushButton::clicked, this, &Widget::onBtnPlayClicked);
    connect(ui->m_btnSpeed, &QPushButton::clicked, this, &Widget::onSpeedCtrlPopup);
    connect(ui->m_btnVolume, &QPushButton::clicked, this, &Widget::onVolumePopup);
    connect(m_recordTimer, &QTimer::timeout, this, &Widget::onRecordTimeout);
    connect(m_video, &OpenCVManager::frameReady, this, &Widget::onFrameReady);
    connect(m_video, &OpenCVManager::videoOpened, this, &Widget::onVideoOpened);
    connect(m_volumeControl, &DigiVolumeControl::setVolumeReq, this, &Widget::onVolumeRsp);
    connect(m_volumeControl, &DigiVolumeControl::btnVolumeChanged, this, &Widget::onVolumeControl);
}

bool Widget::play()
{
    if(m_filePath.isEmpty())
        return false;

    m_video->open(m_filePath);
    m_audio->open(m_filePath);
    m_duration = 0;
    m_playState = STATE_PLAY;
    m_videoSize = ui->m_playWidget->size();
    QtConcurrent::run([this]() { captrueFrame(); });
    m_audio->play();
    ui->m_btnPlay->setToolTip("Pause");
    return true;
}

void Widget::pause()
{
    m_playState = STATE_PAUSE;
    m_audio->pause();
    ui->m_btnPlay->setToolTip("Play");
}

void Widget::resume()
{
    m_playState = STATE_PLAY;
    m_audio->resume();
    ui->m_btnPlay->setToolTip("Pause");
}   

void Widget::stop()
{
    m_playState = STATE_STOP;
    m_video->stop();
    m_audio->stop();
}

QString Widget::formatTime(int ms)
{
    QTime time(0, 0, 0);
    time = time.addMSecs(ms);
    QString curTime;
    if(ms > 60*60*1000)
        curTime = time.toString("hh:mm:ss");
    else
        curTime = time.toString("mm:ss");

    return curTime;
}

QString Widget::getFileName(FileType type)
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    // 如果有正在播放的视频，使用视频文件名作为前缀
    QString baseName = type == FILE_PICTURE ? "screenshot" : "video";
    QString suffix = type == FILE_PICTURE ? "jpg" : m_recordFormat;
    if (!m_filePath.isEmpty()) 
    {
        QFileInfo fileInfo(m_filePath);
        baseName = fileInfo.baseName();
    }
    return QString("%1/%2_%3.%4").arg(dir).arg(baseName).arg(timestamp).arg(suffix);
}

void Widget::onPlayVideoRsp(const QString &filePath)
{
    m_filePath = filePath;
    if(m_playState != STATE_STOP)
        stop();

    play();
}

void Widget::onBtnPlayClicked()
{
    if(m_playState == STATE_PLAY)
    {
        pause();
    }
    else if(m_playState == STATE_PAUSE)
    {
        resume();
    }
    else
    {
        play();
    }
}

void Widget::onScreenShotRsp()
{
    QString screenshotPath = getFileName(FILE_PICTURE);
    bool ret = m_video->capture(screenshotPath);
    if(ret)
        Msg().setMsg(QString("Screenshot saved to %1").arg(screenshotPath), true, MSGHELPER_PICTURE);    
    else
        Msg().setMsg("Failed to save screenshot.");
}

void Widget::onRecordRsp()
{
    m_bRecording = !m_bRecording;
    if(m_bRecording)
    {
        QString recordPath = getFileName(FILE_VIDEO);
        bool ret = m_video->startRecord(recordPath, m_recordFormat);
        if(ret)
        {
            ui->m_btnPlay->setEnabled(false);
            m_recordTimer->start();
            m_recordElapsedTimer.restart();
            Msg().setMsg("Recording started.", true);
        }
        else
        {
            m_bRecording = false;
            Msg().setMsg("Failed to start recording.");
        }
    }
    else
    {
        m_video->stopRecord();
        ui->m_btnPlay->setEnabled(true);
        m_recordTimer->stop();
        Msg().setMsg("Recording stopped.", true, MSGHELPER_VIDEO);
    }
}

void Widget::onVolumePopup()
{
    QPoint btnPoint = ui->m_btnVolume->mapToGlobal(QPoint(0, 0));
    int x = btnPoint.x() - (m_volumeControl->sizeHint().width()-ui->m_btnVolume->width())/2;
    int y = btnPoint.y() - m_volumeControl->sizeHint().height() - 10;
    m_volumeControl->move(QPoint(x, y));
    m_volumeControl->show();
}

void Widget::onSpeedCtrlPopup()
{
    if(!m_speedCtrlMenu)
    {
        m_speedCtrlMenu = new DigiBaseMenu(this);
        DigiBaseAction *action1 = new DigiBaseAction(m_speedCtrlMenu, "0.5");
        DigiBaseAction *action2 = new DigiBaseAction(m_speedCtrlMenu, "1.0");
        DigiBaseAction *action3 = new DigiBaseAction(m_speedCtrlMenu, "1.5");
        DigiBaseAction *action4 = new DigiBaseAction(m_speedCtrlMenu, "2.0");
        m_speedCtrlMenu->SetScale(1.0);
        connect(action1, &QAction::triggered, this, [this](){ m_playSpeed = 0.5; });
        connect(action2, &QAction::triggered, this, [this](){ m_playSpeed = 1.0; });
        connect(action3, &QAction::triggered, this, [this](){ m_playSpeed = 1.5; });
        connect(action4, &QAction::triggered, this, [this](){ m_playSpeed = 2.0; });
    }

    QPoint btnPoint = ui->m_btnSpeed->mapToGlobal(QPoint(0, 0));
    int x = btnPoint.x() - (m_speedCtrlMenu->sizeHint().width()-ui->m_btnSpeed->width())/2;
    int y = btnPoint.y() - m_speedCtrlMenu->sizeHint().height() - 10;
    m_speedCtrlMenu->popup(QPoint(x, y));
}

void Widget::onRecordTimeout()
{
    if(!m_bRecording)
    {
        m_recordTimer->stop();
        return;
    }
    qint64 elapsed = m_recordElapsedTimer.elapsed();
    QString recTime = formatTime(elapsed);
    ui->m_labRecTime->setText(recTime);
}

void Widget::onFrameReady(const QImage &image, qint64 duration)
{
    ui->m_playWidget->updateImage(image);
    if(m_bSliderPress)
        return;
    ui->m_slider->setValue(duration);
    if(m_bSliderHover)
        return;
    m_duration = duration;
    QString curTime = formatTime(duration);
    ui->m_labCurTime->setText(curTime);
}

void Widget::onVideoOpened(qint64 duration)
{
    ui->m_slider->setMaximum(duration);
    QString curTime = formatTime(duration);
    ui->m_labTotalTime->setText(curTime);
}

void Widget::onVolumeRsp(int volume)
{
    m_audio->setVolume(volume);
}

void Widget::onVolumeControl(bool b)
{
    m_audio->setMuted(!b);
}

void Widget::captrueFrame()
{
    if(!m_video->isOpen())
        return;

    qDebug() << "video startTime:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    while(true)
    {
        // 暂停
        if(m_playState == STATE_PAUSE)
        {
            QThread::msleep(100);
            continue;
        }
        // 检查是否需要跳转到指定位置
        if(m_jmpToPos >= 0)
        {
            m_video->seek(m_jmpToPos);
            m_audio->seek(m_jmpToPos);
            m_jmpToPos = -1;
        }
        if(m_playState == STATE_STOP)
            break;

        // 音视频同步 ±40ms
        qint64 videoPts = m_video->getCurrentPosition();
        qint64 audioClock = m_audio->getAudioClock();  // 音频当前播放位置
        qint64 diff = videoPts - audioClock;
        int maxFrameSkip = 5;      // 最大连续跳帧数，防止死循环
        int frameSkipCount = 0;
        if (diff > 0)   // 视频比音频快，需要等待
        {
            qDebug() << "videoPts=" << videoPts << " audioClock=" << audioClock << " diff=" << diff;
            int waitMs = qMin(diff, 100);
            QThread::msleep(waitMs);
        }
        else if (diff < -40)   // 视频比音频慢超过40ms，考虑跳帧
        {
            qDebug() << "videoPts=" << videoPts << " audioClock=" << audioClock << " diff=" << diff;
            if (frameSkipCount < maxFrameSkip)
            {
                frameSkipCount++;
                m_video->skipFrame();
                QThread::msleep(10);
                continue; 
            }
        }

        if(!m_video->refreshFrame())
            break;
        int delay = m_video->delayTime();// 默认40ms一帧
        QThread::msleep(delay);
    }

    qDebug() << "video endTime:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    stop();
}

#include "digivolumecontrol.h"
#include "ui_digivolumecontrol.h"
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include "Res/resfun.h"

DigiVolumeControl::DigiVolumeControl(QWidget *parent) :
    DigiBaseWidget(parent),
    ui(new Ui::DigiVolumeControl)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0, 0);// 阴影偏移量
    shadow->setColor(QColor(68, 68, 68));
    shadow->setBlurRadius(10);// 模糊半径，越大越模糊
    ui->m_widget->setGraphicsEffect(shadow);
    ui->m_widget->SetBGColor(ColorManager::GetColor(ColorManager::CN_THEME_BACKGROUND));
    ui->m_widget->SetBorderRadius(8);
    m_bSound = true;
    ui->m_pBtnSound->LoadImagePath(ResFun::GetPngResUrlByName(ICON_PLAYBACK_VOLUME));
    connect(ui->m_pSliderVolume, &QSlider::valueChanged, this, &DigiVolumeControl::onValueChanged);
    connect(ui->m_pBtnSound, &QPushButton::clicked, this, &DigiVolumeControl::onBtnSoundRsp);
    ui->m_pSliderVolume->setRange(0, 100);
    ui->m_pSliderVolume->setValue(25);
    ui->m_line->SetBGColor(ColorManager::GetColor(ColorManager::CN_DIVIDING_LINE_BG));
    ui->m_pSliderVolume->installEventFilter(this);
}

DigiVolumeControl::~DigiVolumeControl()
{
    delete ui;
}

bool DigiVolumeControl::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->m_pSliderVolume)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)	//判断左键
            {
                int dur = ui->m_pSliderVolume->maximum() - ui->m_pSliderVolume->minimum();
                double per = 1.0 - (1.0*mouseEvent->y() / ui->m_pSliderVolume->height());
                int pos = ui->m_pSliderVolume->minimum() + dur * per;
                if(pos != ui->m_pSliderVolume->sliderPosition())
                {
                    ui->m_pSliderVolume->setValue(pos);
                }
            }
            return false;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void DigiVolumeControl::onValueChanged(int value)
{
    ui->m_pLabDigital->setText(QString::number(value)+"%");
    ui->m_pSliderVolume->setValue(value);
    if(value == 0 && m_bSound)
    {
        m_bSound = false;
        ui->m_pBtnSound->LoadImagePath(ResFun::GetPngResUrlByName(ICON_PLAYBACK_VOLUME));
        emit btnVolumeChanged(false);
    }
    else if(value != 0 && !m_bSound)
    {
        m_bSound = true;
        ui->m_pBtnSound->LoadImagePath(ResFun::GetPngResUrlByName(ICON_PLAYBACK_MUTE));
        emit btnVolumeChanged(true);
    }
    emit setVolumeReq(value);
}

void DigiVolumeControl::onBtnSoundRsp()
{
    m_bSound = !m_bSound;
    if(m_bSound)
    {
        ui->m_pBtnSound->LoadImagePath(ResFun::GetPngResUrlByName(ICON_PLAYBACK_VOLUME));
        emit btnVolumeChanged(true);
    }
    else
    {
        ui->m_pBtnSound->LoadImagePath(ResFun::GetPngResUrlByName(ICON_PLAYBACK_MUTE));
        emit btnVolumeChanged(false);
    }
}

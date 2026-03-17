#include "digibaseslider.h"

#include <QDebug>
#include <QStyle>
#include <QMouseEvent>
#include <QHBoxLayout>
#include "Res/resfun.h"
#include "digibaselabel.h"

DigiBaseSlider::DigiBaseSlider(QWidget *parent):
    QSlider(parent),
    m_GrooveBorderColorHex(ColorManager::GetColorHex(ColorManager::CN_SLIDER_GROOVE_BORDER)),
    m_MinSideColorHex(ColorManager::GetColorHex(ColorManager::CN_SLIDER_MINSIDE)),
    m_MaxSideColorHex(ColorManager::GetColorHex(ColorManager::CN_SLIDER_MAXSIDE)),
    m_HandleImgPath(ResFun::GetSvgResUrlByName(ICON_PREVIEW_PTZ_HANDLE)),
    m_ImgSize(14),
    m_GrooveSize(6),
    m_bMoveFlag(false),
    m_disableWheel(false),
    m_LabValue(nullptr),
    m_LabMinValue(nullptr),
    m_LabMaxValue(nullptr)
{
    setRange(0,100);
    setPageStep(1);
    setTracking(false);

    if (orientation() == Qt::Horizontal)
    {
        m_width = 0;
        m_height = 14;
    }
    else //orientation() == Qt::Vertical
    {
        m_width = 14;
        m_height = 0;
    }
}

void DigiBaseSlider::setOrientation(Qt::Orientation orientation)
{
    if (orientation == this->orientation())
        return;

    QSlider::setOrientation(orientation);
    qSwap(m_width,m_height);
}

void DigiBaseSlider::SetMinSideColor(const QColor &color)
{
    m_MinSideColorHex = color.name(QColor::HexArgb);
}

void DigiBaseSlider::SetMaxSideColor(const QColor &color)
{
    m_MaxSideColorHex = color.name(QColor::HexArgb);
}

void DigiBaseSlider::SetGrooveSize(qreal size)
{
    m_GrooveSize = size;
}

void DigiBaseSlider::LoadHandleImgPath(const QString &path, qreal imgSize)
{
    m_HandleImgPath = path;
    m_ImgSize = imgSize;
}

void DigiBaseSlider::setDisableWheel(bool b)
{
    m_disableWheel = b;
}

void DigiBaseSlider::setValueVisible(bool b, const QString &unit)
{
    bool bInit = false;
    if(!m_LabValue)
    {
        bInit = true;
        m_LabValue = new DigiBaseLabel(this);
        m_LabValue->SetTextSize(12);
        connect(this, &QSlider::sliderMoved, this, [this, unit](int value)
        {
            m_LabValue->setText(QString::number(value)+unit);
            MoveValuePos(value);

        });
        connect(this, &QSlider::valueChanged, this, [this, unit](int value)
        {
            m_LabValue->setText(QString::number(value)+unit);
            MoveValuePos(value);
        });
    }

    if(!m_LabMinValue)
    {
        m_LabMinValue = new DigiBaseLabel(this);
        m_LabMinValue->SetTextSize(12);
        m_LabMinValue->setText(QString::number(minimum())+unit);
    }

    if(!m_LabMaxValue)
    {
        m_LabMaxValue = new DigiBaseLabel(this);
        m_LabMaxValue->SetTextSize(12);
        m_LabMaxValue->setText(QString::number(maximum())+unit);
        connect(this, &QSlider::rangeChanged, this, [this, unit](int min, int max)
        {
            m_LabMinValue->setText(QString::number(min)+unit);
            m_LabMaxValue->setText(QString::number(max)+unit);
        });
    }

    if (orientation() == Qt::Horizontal)
    {
        m_width = 360;
        m_height = 50;
        if(bInit)
        {
            QHBoxLayout *hLayout = new QHBoxLayout;
            hLayout->setContentsMargins(0, 0, 0, 0);
            hLayout->addWidget(m_LabMinValue);
            hLayout->addStretch(1);
            hLayout->addWidget(m_LabMaxValue);
            QVBoxLayout *vLayout = new QVBoxLayout(this);
            vLayout->setContentsMargins(0, 0, 0, 0);
            vLayout->addStretch(1);
            vLayout->addLayout(hLayout);
        }
    }
    else //orientation() == Qt::Vertical
    {
        m_width = unit.isEmpty() ? 60 : 85;
        m_height = 360;
        if(bInit)
        {
            QVBoxLayout *vLayout = new QVBoxLayout;
            vLayout->setContentsMargins(0, 0, 0, 0);
            vLayout->addWidget(m_LabMaxValue);
            vLayout->addStretch(1);
            vLayout->addWidget(m_LabMinValue);
            QHBoxLayout *hLayout = new QHBoxLayout(this);
            hLayout->setContentsMargins(0, 0, 0, 0);
            hLayout->addStretch(1);
            hLayout->addLayout(vLayout);
        }
    }
    m_LabValue->setVisible(b);
    m_LabMinValue->setVisible(b);
    m_LabMaxValue->setVisible(b);
}

void DigiBaseSlider::SetScale(qreal scale)
{
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);

    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    if(m_LabValue)
        m_LabValue->SetScale(scale);
    if(m_LabMinValue)
        m_LabMinValue->SetScale(scale);
    if(m_LabMaxValue)
        m_LabMaxValue->SetScale(scale);

    SetCustomStyleSheet(scale);
}

void DigiBaseSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bMoveFlag = false;
    }
    QSlider::mousePressEvent(event);
}

void DigiBaseSlider::mouseMoveEvent(QMouseEvent *event)
{
    m_bMoveFlag = true;
    QSlider::mouseMoveEvent(event);
}

void DigiBaseSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!m_bMoveFlag)//按下鼠标后没有拖动，则根据鼠标位置计算点位
        {
            int pos = 1;
            if (orientation() == Qt::Horizontal)
            {
                pos = QStyle::sliderValueFromPosition(minimum(),maximum(),event->pos().x(),width());
            }
            else
            {
                pos = QStyle::sliderValueFromPosition(minimum(),maximum(),event->pos().y(),height(),true);
                setValue(pos);
            }
            setValue(pos);
        }
        emit sliderClickSig(sliderPosition());
    }

    QSlider::mouseReleaseEvent(event);
}

void DigiBaseSlider::wheelEvent(QWheelEvent *e)
{
    if(m_disableWheel)
        e->ignore();
    else
        QSlider::wheelEvent(e);
}

void DigiBaseSlider::resizeEvent(QResizeEvent *event)
{
    QSlider::resizeEvent(event);
    MoveValuePos(this->value());
}

void DigiBaseSlider::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;

    qss.append(QString("QSlider::groove:horizontal {"
                           "height: %1px;"
                           "border: 0px;"
                           "border-radius: %2px;"
                           "left: 0px; right: 0px;"
                           "margin-left: %3px;"
                           "margin-right: %3px;}")
                       .arg(m_GrooveSize * scale)
                       .arg(m_GrooveSize/2 * scale)
                       .arg((m_ImgSize/2)-1 * scale));

    qss.append(QString("QSlider::handle:horizontal {"
                           "border: 0px ;"
                           "image:  url(%1);"
                           "width: %2px;"
                           "margin: -%3px -%3px -%3px -%3px;}")
                       .arg(m_HandleImgPath)
                       .arg(m_ImgSize * scale)
                       .arg((m_ImgSize/2)-1 * scale));

    qss.append(QString("QSlider::add-page:horizontal{"
                           "background: %1;"
                           "border-radius: %2px;}")
                       .arg(m_MaxSideColorHex)
                       .arg(m_GrooveSize/2 * scale));

    qss.append(QString("QSlider::sub-page:horizontal{"
                           "background: %1;"
                           "border-radius: %2px;}")
                       .arg(m_MinSideColorHex)
                       .arg(m_GrooveSize/2 * scale));

    qss.append(QString("QSlider::groove:vertical{"
                           "width: %1px;"
                           "border: 0px;"
                           "border-radius: %2px;"
                           "top: 0px; bottom: 0px;"
                           "margin-top: %3px;"
                           "margin-bottom: %3px;}")
                       .arg(m_GrooveSize * scale)
                       .arg(m_GrooveSize/2 * scale)
                       .arg((m_ImgSize/2)-1 * scale));

    qss.append(QString("QSlider::handle:vertical{"
                           "border: 0px ;"
                           "image:  url(%1);"
                           "height: %2px;"
                           "margin: -%3px -%3px -%3px -%3px;}")
                       .arg(m_HandleImgPath)
                       .arg(m_ImgSize * scale)
                       .arg((m_ImgSize/2)-1 * scale));

    qss.append(QString("QSlider::add-page:vertical{"
                           "background: %1;"
                           "border-radius: %2px;}")
                       .arg(m_MinSideColorHex)
                       .arg(m_GrooveSize/2 * scale));

    qss.append(QString("QSlider::sub-page:vertical{"
                           "background: %1;"
                           "border-radius: %2px;}")
                       .arg(m_MaxSideColorHex)
                       .arg(m_GrooveSize/2 * scale));

    setStyleSheet(qss.join(""));
}

void DigiBaseSlider::MoveValuePos(int value)
{
    if(!m_LabValue)
        return;

    if (orientation() == Qt::Horizontal)
    {
        m_LabValue->adjustSize();
        m_LabValue->move((width()-m_LabValue->width())*(value-minimum())/(maximum()-minimum()),0);
    }
    else //orientation() == Qt::Vertical
    {
        m_LabValue->adjustSize();
        m_LabValue->move(0,(height()-m_LabValue->height())*(maximum()-value)/(maximum()-minimum()));
    }
}

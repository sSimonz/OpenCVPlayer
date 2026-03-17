#include "digiswitchbtn.h"

#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "Base/colormanager.h"
#include "digibaselabel.h"

DigiSwitchBtn::DigiSwitchBtn(QWidget *parent) :
    QFrame(parent),
    m_index(0),
    m_firstEnter(true),
    m_groupID(0),
    m_Status(false),
    m_DotSize(10),
    m_LeftPos(2,2),
    m_RightPos(16,2),
    m_OnColorHex(ColorManager::GetColorHex(ColorManager::CN_SWITCHBTN_ON)),
    m_OffColorHex(ColorManager::GetColorHex(ColorManager::CN_SWITCHBTN_OFF)),
    m_OnDisableColorHex(ColorManager::GetColorHex(ColorManager::CN_SWITCHBTN_ON_DISABLE)),
    m_OffDisableColorHex(ColorManager::GetColorHex(ColorManager::CN_SWITCHBTN_OFF_DISABLE)),
    m_DotColorHex(ColorManager::GetColorHex(ColorManager::CN_SWITCHBTN_DOT)),
    m_DotDisableColorHex(ColorManager::GetColorHex(ColorManager::CN_SWITCHBTN_DOT_DISABLE))
{
    setAttribute(Qt::WA_LayoutUsesWidgetRect);
    m_width = 26;
    m_height = 14;
    m_DotLab = new DigiBaseLabel(this);

    m_Animation = new QPropertyAnimation(m_DotLab,"pos",this);
    connect(m_Animation, &QPropertyAnimation::finished,
            this, &DigiSwitchBtn::changeStatus);

    SetScale(1);
}

void DigiSwitchBtn::SetOnStatusColor(const QColor &color)
{
    m_OnColorHex = color.name(QColor::HexArgb);
}

void DigiSwitchBtn::SetOffStatusColor(const QColor &color)
{
    m_OffColorHex = color.name(QColor::HexArgb);
}

void DigiSwitchBtn::SetOnDisableStatusColor(const QColor &color)
{
    m_OnDisableColorHex = color.name(QColor::HexArgb);
}

void DigiSwitchBtn::SetOffDisableStatusColor(const QColor &color)
{
    m_OffDisableColorHex = color.name(QColor::HexArgb);
}

void DigiSwitchBtn::SetDotColor(const QColor &color)
{
    m_DotColorHex = color.name(QColor::HexArgb);
}

void DigiSwitchBtn::SetDotSize(qreal size)
{
    if (size > m_height)
        m_DotSize = m_height;
    else
        m_DotSize = size;
}

bool DigiSwitchBtn::getBtnStatus()
{
    return m_Status;
}

void DigiSwitchBtn::setBtnStatus(bool newStatus)
{
    m_Status = newStatus;
    if (newStatus)
        m_DotLab->move(m_RightPos);
    else
        m_DotLab->move(m_LeftPos);

    changeStatus();
}

void DigiSwitchBtn::setEnabled(bool flag)
{   
    QStringList qss;
    qss.append(this->styleSheet().left(this->styleSheet().indexOf("background-color")));

    QString colorHex;
    if (flag)
        colorHex = m_Status ? m_OnColorHex : m_OffColorHex;
    else
        colorHex = m_Status ? m_OnDisableColorHex : m_OffDisableColorHex;

    qss.append(QString("background-color: %1").arg(colorHex));
    this->setStyleSheet(qss.join(""));

    QFrame::setEnabled(flag);
}

void DigiSwitchBtn::setEnable(bool flag)
{
    this->setEnabled(flag);
}

void DigiSwitchBtn::setData(const QString &data)
{
    m_Data = data;
}

QString DigiSwitchBtn::getData()
{
    return m_Data;
}

void DigiSwitchBtn::SetScale(qreal scale)
{
    this->setFixedSize(m_width*scale, m_height*scale);
    m_DotLab->setFixedSize(m_DotSize*scale, m_DotSize*scale);

    //更新、移动点的位置
    qreal y = (this->height() - m_DotLab->height()) / 2;
    m_LeftPos = QPoint(y,y);
    m_RightPos = QPoint(this->width() - m_DotLab->width() - y, y);
    if (m_Status)
        m_DotLab->move(m_RightPos);
    else
        m_DotLab->move(m_LeftPos);

    SetCustomStyleSheet(scale);
}

void DigiSwitchBtn::SetHeight(qreal height)
{
    m_height = height;
    m_DotSize = m_height - 4;
}

void DigiSwitchBtn::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && rect().contains(event->pos()) && isEnabled())
    {
        animationMove();
        emit clicked(m_Status);
    }
    QFrame::mouseReleaseEvent(event);
}

void DigiSwitchBtn::changeStatus()
{
    QStringList qss;

    qss.append(this->styleSheet().left(this->styleSheet().indexOf("background-color")));

    QString colorHex;
    if (this->isEnabled())
        colorHex = m_Status ? m_OnColorHex : m_OffColorHex;
    else
        colorHex = m_Status ? m_OnDisableColorHex : m_OffDisableColorHex;

    qss.append(QString("background-color: %1").arg(colorHex));
    this->setStyleSheet(qss.join(""));

    emit statusChange(m_Status);
}

void DigiSwitchBtn::animationMove()
{
    if(m_Animation->state() == QPropertyAnimation::Running)
    {
        m_Animation->stop();
    }

    if(m_Status == false)
    {
        m_Status = true;
        m_Animation->setStartValue(m_DotLab->pos());
        m_Animation->setEndValue(m_RightPos);
    }
    else
    {
        m_Status = false;
        m_Animation->setStartValue(m_DotLab->pos());
        m_Animation->setEndValue(m_LeftPos);
    }
    m_Animation->setDuration(100);
    m_Animation->start();
}

void DigiSwitchBtn::SetCustomStyleSheet(qreal scale)
{
    QStringList qssLab;
    qssLab.append(QString("QLabel{"
                              "border-radius: %1px;"
                              "border: 0px;"
                              "background-color: %2;}"
                          "QLabel:diabled{"
                              "background-color: %3;}")
                          .arg(m_DotSize/2 * scale)
                          .arg(m_DotColorHex)
                          .arg(m_DotDisableColorHex));

    m_DotLab->setStyleSheet(qssLab.join(""));

    QStringList qss;
    qss.append(QString("border-radius: %1px;"
                       "border: 0px;")
                       .arg(m_height/2 * scale));
    QString colorHex;
    if (this->isEnabled())
        colorHex = m_Status ? m_OnColorHex : m_OffColorHex;
    else
        colorHex = m_Status ? m_OnDisableColorHex : m_OffDisableColorHex;

    qss.append(QString("background-color: %1").arg(colorHex));

    this->setStyleSheet(qss.join(""));
}

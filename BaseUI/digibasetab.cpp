#include "digibasetab.h"
#include <QPainter>
DigiBaseTab::DigiBaseTab(QWidget *parent) : QPushButton(parent)
{
    m_bRead = true;
    m_Font = GetGlobalFont();
    QString family = GetGlobalFamily(QFont::Medium);
    m_Font.setFamily(family);
    SetWidth(134);
    SetHeight(32);
    setCheckable(true);
}

void DigiBaseTab::setTabType(int type)
{
    if(type == Left)
        setObjectName("left");
    else if(type == Right)
        setObjectName("right");
}
void DigiBaseTab::setRead(bool bRead)
{
    m_bRead = bRead;
    update();
}

void DigiBaseTab::SetScale(qreal scale)
{
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize()*scale);
    setFont(aFont);

    SetCustomStyleSheet(scale);
}

void DigiBaseTab::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    // 红点
    if(!m_bRead)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QRect(85, 5, 7, 7));
    }
}

void DigiBaseTab::mouseReleaseEvent(QMouseEvent *event)
{
    setChecked(!isChecked());
    QPushButton::mouseReleaseEvent(event);
}

void DigiBaseTab::SetCustomStyleSheet(qreal scale)
{
    QString qss;

    qss.append(QString("QPushButton{"
                            "background-color: %1;"
                            "border: %3px solid %2;"
                            "color: %2;"
                            "font-size:%5px;}"
                       "QPushButton:checked{"
                            "background-color: %2;"
                            "color: %1;}"
                       "QPushButton#left{"
                            "border-top-left-radius: %4px;"
                            "border-bottom-left-radius: %4px;}"
                       "QPushButton#right{"
                            "border-top-right-radius: %4px;"
                            "border-bottom-right-radius: %4px;}"
                       "QPushButton:disabled{"
                            "border: 0px;"
                            "background-color: %6;"
                            "color: %7;}")
                       .arg(ColorManager::GetColorHex(ColorManager::CN_THEME_BACKGROUND))
                       .arg(ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_CHECK))
                       .arg(1*scale)    // border
                       .arg(16*scale)   // radius
                       .arg(14*scale)   // font-size
                       .arg(ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_DISABLE))
                       .arg(ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT_DISABLE)));

    setStyleSheet(qss);
}

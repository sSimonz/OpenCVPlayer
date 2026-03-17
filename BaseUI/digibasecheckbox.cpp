#include "digibasecheckbox.h"

#include <QDebug>
#include "Res/resfun.h"

DigiBaseCheckBox::DigiBaseCheckBox(QWidget *parent):
    QCheckBox(parent),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_IndicatorSize(20)
{
    m_width = 0;
    m_height = 20;

    m_Font = GetGlobalFont();
    m_iconList << ICON_PLAYBACK_CHECKBOX_UNSELECTED
               << ICON_PLAYBACK_CHECKBOX_SELECTED
               << ICON_PLAYBACK_CHECKBOX_PARTSELECTED
               << ICON_PLAYBACK_CHECKBOX_UNSELECTED_DISABLED
               << ICON_PLAYBACK_CHECKBOX_SELECTED_DISABLED
               << ICON_PLAYBACK_CHECKBOX_PARTSELECTED_DISABLED;
    setAttribute(Qt::WA_LayoutUsesWidgetRect);
}

DigiBaseCheckBox::DigiBaseCheckBox(const QString &text, QWidget *parent):
    QCheckBox(text, parent),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_IndicatorSize(20)
{
    m_width = 0;
    m_height = 20;

    m_Font = GetGlobalFont();
    m_iconList << ICON_PLAYBACK_CHECKBOX_UNSELECTED
               << ICON_PLAYBACK_CHECKBOX_SELECTED
               << ICON_PLAYBACK_CHECKBOX_PARTSELECTED
               << ICON_PLAYBACK_CHECKBOX_UNSELECTED_DISABLED
               << ICON_PLAYBACK_CHECKBOX_SELECTED_DISABLED
               << ICON_PLAYBACK_CHECKBOX_PARTSELECTED_DISABLED;
    setAttribute(Qt::WA_LayoutUsesWidgetRect);
}

void DigiBaseCheckBox::SetIconColor(const QStringList &list)
{
    m_iconList.clear();
    m_iconList = list;
}

void DigiBaseCheckBox::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseCheckBox::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseCheckBox::GetCustomFont()
{
    return m_Font;
}

void DigiBaseCheckBox::SetScale(qreal scale)
{
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    SetCustomStyleSheet(scale);
}

void DigiBaseCheckBox::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;

    //图标
    qss.append(QString("QCheckBox::indicator{image: url(%1);}"
                       "QCheckBox::indicator:checked{image: url(%2);}"
                       "QCheckBox::indicator:indeterminate{image: url(%3);}"
                       "QCheckBox::indicator:disabled{image: url(%4);}"
                       "QCheckBox::indicator:checked:disabled{image: url(%5);}"
                       "QCheckBox::indicator:indeterminate:disabled{image: url(%6);}")
                   .arg(ResFun::GetSvgResUrlByName(m_iconList.at(0)))
                   .arg(ResFun::GetSvgResUrlByName(m_iconList.at(1)))
                   .arg(ResFun::GetSvgResUrlByName(m_iconList.at(2)))
                   .arg(ResFun::GetSvgResUrlByName(m_iconList.at(3)))
                   .arg(ResFun::GetSvgResUrlByName(m_iconList.at(4)))
                   .arg(ResFun::GetSvgResUrlByName(m_iconList.at(5))));

    //图标大小
    qss.append(QString("QCheckBox::indicator{width:%1px; height:%1px;}").arg(m_IndicatorSize * scale));

    //图标和文字间距
    qss.append(QString("QCheckBox{spacing: %1px}").arg(5 * scale));

    //文字
    qss.append(QString("QCheckBox:enabled{color:%1;}").arg(m_TextColorHex));

    setStyleSheet(qss.join(""));
}

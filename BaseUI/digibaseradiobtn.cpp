#include "digibaseradiobtn.h"

#include "Res/resfun.h"

DigiBaseRadioBtn::DigiBaseRadioBtn(QWidget *parent) :
    QRadioButton(parent),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_UnselectedImgPath(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_RADIO_UNSELECTED)),
    m_SelectedImgPath(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_RADIO_SELECTED)),
    m_IndicatorSize(16)
{
    m_width = 0;
    m_height = 20;
    m_Font = GetGlobalFont();
}

void DigiBaseRadioBtn::LoadImgPath(const QString &unsel, const QString &sel, int indicatorSize)
{
    if (!unsel.isEmpty())
        m_UnselectedImgPath = unsel;

    if (!sel.isEmpty())
        m_SelectedImgPath = sel;

    if (indicatorSize > 0)
        m_IndicatorSize = indicatorSize;

    if (m_IndicatorSize>m_height)
        m_height = m_IndicatorSize;
}

void DigiBaseRadioBtn::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseRadioBtn::SetIndicatorSize(qreal size)
{
    if (size > 0)
        m_IndicatorSize = size;

    if (m_IndicatorSize>m_height)
        m_height = m_IndicatorSize;
}

void DigiBaseRadioBtn::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseRadioBtn::GetCustomFont()
{
    return m_Font;
}


void DigiBaseRadioBtn::SetScale(qreal scale)
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

void DigiBaseRadioBtn::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;
    //图标
    qss.append(QString("QRadioButton::indicator{image: url(%1);}"
                       "QRadioButton::indicator:checked{image: url(%2);}")
               .arg(m_UnselectedImgPath).arg(m_SelectedImgPath));

    //图标大小
    qss.append(QString("QRadioButton::indicator {width: %1px;height: %1px;}").arg(m_IndicatorSize * scale));

    //图标和文字间距
    qss.append(QString("QRadioButton{spacing: %1px}").arg(5 * scale));

    //文字
    qss.append(QString("QRadioButton:enabled{color:%1;}").arg(m_TextColorHex));

    setStyleSheet(qss.join(""));
}

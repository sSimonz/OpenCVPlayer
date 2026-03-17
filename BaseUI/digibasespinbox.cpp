#include "digibasespinbox.h"

#include <QLineEdit>
#include "Res/resfun.h"

DigiBaseSpinBox::DigiBaseSpinBox(QWidget *parent) :
    QSpinBox(parent),
    m_TextMargins(5,0,5,0),
    m_Radius(2),
    m_BorderWidth(1),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_BGColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)),
    m_BorderColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUTWIDGET_BORDER)),
    m_UpBtnImgPath(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_TIMEUP)),
    m_DownBtnImgPath(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_TIMEDOWN)),
    m_bBtnVisible(true)
{
    m_width = 110;
    m_height = 25;
    m_Font = GetGlobalFont();
}

void DigiBaseSpinBox::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseSpinBox::SetTextMargins(const QMargins &margin)
{
    m_TextMargins = margin;
}

void DigiBaseSpinBox::SetBorderRadius(qreal radius)
{
    m_Radius = radius;
}

void DigiBaseSpinBox::SetBorderWidth(qreal border)
{
    m_BorderWidth = border;
}

void DigiBaseSpinBox::SetBoederColor(const QColor &color)
{
    m_BorderColorHex = color.name(QColor::HexArgb);
}

void DigiBaseSpinBox::SetBGColor(const QColor &color)
{
    m_BGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseSpinBox::SetBtnVisible(bool visible)
{
    m_bBtnVisible = visible;
}

void DigiBaseSpinBox::LoadBtnImg(const QString &upPath, const QString &downPath)
{
    m_UpBtnImgPath = upPath;
    m_DownBtnImgPath = downPath;
}

void DigiBaseSpinBox::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseSpinBox::GetCustomFont()
{
    return m_Font;
}

void DigiBaseSpinBox::SetScale(qreal scale)
{
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    lineEdit()->setTextMargins(m_TextMargins * scale);

    SetCustomStyleSheet(scale);
}

void DigiBaseSpinBox::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;

    //内阴影样式，暂时弃用
//    qss.append(QString("QSpinBox{"
//                           "border-left:   %1px groove  %2;"
//                           "border-top:    %1px groove  %2;"
//                           "border-right:  %3px solid   %4;"
//                           "border-bottom: %3px solid   %4;"
//                           "border-radius: %5px;"
//                           "background-color: %6;}")
//                       .arg(m_TopLeftBorder * scale)
//                       .arg(m_TopLeftColorHex)
//                       .arg(m_BottomRightBorder * scale)
//                       .arg(m_BottomRightColorHex)
//                       .arg(m_Radius * scale)
//                       .arg(m_BGColorHex));

    //描边
    qss.append(QString("QSpinBox{"
                           "border: %1px solid   %2;"
                           "border-radius: %3px;"
                           "background-color: %4;}")
                       .arg(m_BorderWidth * scale)
                       .arg(m_BorderColorHex)
                       .arg(m_Radius * scale)
                       .arg(m_BGColorHex));

    qss.append(QString("QSpinBox:enabled{"
                           "color: %1;}")
                       .arg(m_TextColorHex));

    qss.append(QString("QSpinBox::up-button{"
                           "image: url(%1);}")
                       .arg(m_UpBtnImgPath));

    qss.append(QString("QSpinBox::down-button{"
                           "image: url(%1);}")
                       .arg(m_DownBtnImgPath));

    if (!m_bBtnVisible)
    {
        qss.append("QSpinBox::up-button{width:0px;}"
                   "QSpinBox::down-button{width:0px;}");
    }

    setStyleSheet(qss.join(""));
}

#include "digibasetimeedit.h"

#include <QLineEdit>
#include "Res/resfun.h"

DigiBaseTimeEdit::DigiBaseTimeEdit(QWidget *parent) :
    QTimeEdit(parent),
    m_TextMargins(5,0,5,0),
    m_Radius(2),
    m_BorderWidth(1),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_BorderColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUTWIDGET_BORDER)),
    m_BGColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)),
    m_UpBtnImgPath(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_TIMEUP)),
    m_DownBtnImgPath(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_TIMEDOWN))
{
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();

    setDisplayFormat("HH:mm");
}

void DigiBaseTimeEdit::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTimeEdit::SetTextMargins(const QMargins &margin)
{
    m_TextMargins = margin;
}

void DigiBaseTimeEdit::SetBorderRadius(qreal radius)
{
    m_Radius = radius;
}

void DigiBaseTimeEdit::SetBorderWidth(qreal width)
{
    m_BorderWidth = width;
}

void DigiBaseTimeEdit::SetBoederColor(const QColor &color)
{
    m_BorderColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTimeEdit::SetBGColor(const QColor &color)
{
    m_BGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTimeEdit::LoadBtnImg(const QString &upPath, const QString &downPath)
{
    m_UpBtnImgPath = upPath;
    m_DownBtnImgPath = downPath;
}

void DigiBaseTimeEdit::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseTimeEdit::GetCustomFont()
{
    return m_Font;
}

void DigiBaseTimeEdit::SetScale(qreal scale)
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

void DigiBaseTimeEdit::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;

    qss.append(QString("QTimeEdit{"
                           "border: %1px solid %2;"
                           "border-radius: %3px;"
                           "background-color: %4;}")
                       .arg(m_BorderWidth * scale)
                       .arg(m_BorderColorHex)
                       .arg(m_Radius * scale)
                       .arg(m_BGColorHex));

    qss.append(QString("QTimeEdit:enabled{"
                           "color: %1;}")
                       .arg(m_TextColorHex));

    qss.append(QString("QTimeEdit::up-button{"
                           "image: url(%1);}")
                       .arg(m_UpBtnImgPath));

    qss.append(QString("QTimeEdit::down-button{"
                           "image: url(%1);}")
                       .arg(m_DownBtnImgPath));

    setStyleSheet(qss.join(""));
}

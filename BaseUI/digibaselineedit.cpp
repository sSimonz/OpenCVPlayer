#include "digibaselineedit.h"
#include <QDebug>
#include "digibytelengthvalidator.h"

DigiBaseLineEdit::DigiBaseLineEdit(QWidget *parent) :
    QLineEdit(parent),
    m_TextMargins(10,0,10,0)
{
    m_width = 150;
    m_height = 32;
    setTextMargins(m_TextMargins);
    m_Font = GetGlobalFont();
    initStyleSheet();

    m_validator = new DigiByteLengthValidator(this);
    this->setValidator(m_validator);
}

void DigiBaseLineEdit::SetScale(qreal scale)
{
    if (!qFuzzyIsNull(m_width))
        setMinimumWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize()*scale);
    setFont(aFont);

    setTextMargins(m_TextMargins * scale);

    SetCustomStyleSheet(scale);
}

void DigiBaseLineEdit::SetStyleTextColor(const QColor &nmClr)
{
    m_StyleColorList[lTextColor] = nmClr.name(QColor::HexArgb);
}

void DigiBaseLineEdit::SetStyleColor(const QColor &nmClr,const QColor &hvClr,const QColor &bgClr)
{
    m_StyleColorList[lNormal]     = nmClr.name(QColor::HexArgb);
    m_StyleColorList[lHover]      = hvClr.name(QColor::HexArgb);
    m_StyleColorList[lBackGround] = bgClr.name(QColor::HexArgb);
}

void DigiBaseLineEdit::SetStyleSize(qreal borderWidth, qreal radius)
{
    m_StyleSizeList[BorderWidth] = borderWidth;
    m_StyleSizeList[Radius]      = radius;
}

QSize DigiBaseLineEdit::sizeHint() const
{
    return QSize(m_width,m_height);
}

void DigiBaseLineEdit::initStyleSheet()
{
    m_StyleColorList << ColorManager::GetColorHex(ColorManager::CN_INPUTWIDGET_BORDER)
                     << ColorManager::GetColorHex(ColorManager::CN_INPUTWIDGET_BORDER_HOVER)
                     << ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)
                     << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT);

//    m_StyleSizeList[BorderWidth] = 1;
//    m_StyleSizeList[Radius]      = 6;
    m_StyleSizeList << 1 << 6;
}

void DigiBaseLineEdit::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;

    qss.append(QString("QLineEdit{"
                           "border:%1px solid %3;"
                           "border-radius:%2px;"
                           "background:%4;}"
                       "QLineEdit:hover,QLineEdit:focus{"
                           "border:%1px solid %5;}")
                       .arg(m_StyleSizeList.at(BorderWidth) * scale)
                       .arg(m_StyleSizeList.at(Radius) * scale)
                       .arg(m_StyleColorList.at(lNormal))
                       .arg(m_StyleColorList.at(lBackGround))
                       .arg(m_StyleColorList.at(lHover)));

    qss.append(QString("QLineEdit:enabled{color:%1;}").arg(m_StyleColorList.at(lTextColor)));

    setStyleSheet(qss.join(""));
}

void DigiBaseLineEdit::SetCustomTextMargins(int nleft,int ntop,int nright,int nbottom)
{
    m_TextMargins = QMargins(nleft,ntop,nright,nbottom);
    setTextMargins(m_TextMargins);
}

void DigiBaseLineEdit::SetCustomTextMargins(const QMargins& nMargins)
{
    m_TextMargins = nMargins;
    setTextMargins(m_TextMargins);
}

void DigiBaseLineEdit::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseLineEdit::GetCustomFont()
{
    return m_Font;
}

void DigiBaseLineEdit::SetMaxByteLength(unsigned int max)
{
    m_validator->setMaxBytes(max);
}

void DigiBaseLineEdit::SetCharLengthLimit(bool blimit)
{
    m_validator->setLimitChar(blimit);
}

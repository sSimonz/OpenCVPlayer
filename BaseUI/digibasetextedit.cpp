#include "digibasetextedit.h"
#include "digibasescrollbar.h"

DigiBaseTextEdit::DigiBaseTextEdit(QWidget *parent):
    QTextEdit(parent)
{
    m_width = 0;
    m_height = 0;

    m_Font = GetGlobalFont();
    setHorizontalScrollBar(new DigiBaseScrollBar(this));
    setVerticalScrollBar(new DigiBaseScrollBar(this));
    initStyleSheet();
}

void DigiBaseTextEdit::SetScale(qreal scale)
{
    //自身大小
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);

    //滚动条
    dynamic_cast<IWidgetBase*>(horizontalScrollBar())->SetScale(scale);
    dynamic_cast<IWidgetBase*>(verticalScrollBar())->SetScale(scale);

    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    SetCustomStyleSheet(scale);
}

void DigiBaseTextEdit::SetStyleSize(qreal radius, qreal padding, qreal border)
{
    m_StyleSizeList[Radius] = radius;
    m_StyleSizeList[Padding] = padding;
    m_StyleSizeList[Border] = border;
}

void DigiBaseTextEdit::SetTextSize(int textSize)
{
    m_Font.setPixelSize(textSize);
}

QFont DigiBaseTextEdit::GetCustomFont()
{
    return m_Font;
}

void DigiBaseTextEdit::initStyleSheet()
{
    m_StyleColorList << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)
                     << ColorManager::GetColorHex(ColorManager::CN_THEME_BACKGROUND)
                     << ColorManager::GetColorHex(ColorManager::CN_TEXTEDIT_BORDER);

    m_StyleSizeList << 6 << 7 << 1;
}

void DigiBaseTextEdit::SetCustomStyleSheet(qreal scale)
{
    QString qss = QString("QTextEdit{"
                              "border-radius: %1px;"
                              "color: %4;"
                              "padding: %2px;"
                              "background-color: %5;"
                              "border:%3px solid %6;}")
                          .arg(m_StyleSizeList.at(Radius) *scale)
                          .arg(m_StyleSizeList.at(Padding) *scale)
                          .arg(m_StyleSizeList.at(Border) *scale)
                          .arg(m_StyleColorList.at(textNormal))
                          .arg(m_StyleColorList.at(BackGround))
                          .arg(m_StyleColorList.at(WidgetBorder));

   setStyleSheet(qss);
}

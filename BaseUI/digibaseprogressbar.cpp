#include "digibaseprogressbar.h"

DigiBaseProgressBar::DigiBaseProgressBar(QWidget *parent):
    QProgressBar(parent),
    m_BorderRadius(4),
    m_BorderWidth(0)
{
    m_Font = GetGlobalFont();

    m_BGColorHex = ColorManager::GetColorHex(ColorManager::CN_PROGRESSBAR_BG);
    m_OccupyColorHex = ColorManager::GetColorHex(ColorManager::CN_PROGRESSBAR_CHUNK);
    m_TextColorHex = ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT);

    if (orientation() == Qt::Horizontal)
    {
        m_width = 0;
        m_height = 10;
    }
    else //orientation() == Qt::Vertical
    {
        m_width = 10;
        m_height = 0;
    }
}

void DigiBaseProgressBar::setOrientation(Qt::Orientation orientation)
{
    if (orientation == this->orientation())
        return;

    QProgressBar::setOrientation(orientation);
    qSwap(m_width, m_height);
}

void DigiBaseProgressBar::SetBorderRadius(int radius)
{
    m_BorderRadius = radius;
}

void DigiBaseProgressBar::SetBorder(int width, const QColor &borderColor)
{
    m_BorderWidth = width;
    m_BorderColorHex = borderColor.name(QColor::HexArgb);
}

void DigiBaseProgressBar::SetColor(const QColor &bgColor, const QColor &occupyColor)
{
    m_BGColorHex = bgColor.name(QColor::HexArgb);
    m_OccupyColorHex = occupyColor.name(QColor::HexArgb);
}

void DigiBaseProgressBar::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseProgressBar::SetFont(const QFont &font)
{
    m_Font = font;
}

void DigiBaseProgressBar::SetScale(qreal scale)
{
    //自身大小
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    //字体
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize()*scale);
    setFont(aFont);

    //样式表
    SetCustomStyleSheet(scale);
}

void DigiBaseProgressBar::SetCustomStyleSheet(qreal scale)
{
    //默认使用Qt原有样式，有设置则使用设置样式
    QString qss;

    qss.append(QString("QProgressBar, QProgressBar::chunk{"
                           "border-radius: %1px;}")
                           .arg(m_BorderRadius * scale));

    qss.append(QString("QProgressBar{"
                           "color: %1;}")
                           .arg(m_TextColorHex));

    if (m_BorderWidth != 0 && !m_BorderColorHex.isEmpty())
        qss.append(QString("QProgressBar{"
                               "border: %1px solid %2;}")
                           .arg(m_BorderWidth * scale)
                           .arg(m_BorderColorHex));

    if (!m_BGColorHex.isEmpty())
        qss.append(QString("QProgressBar{"
                               "background-color: %1;}")
                           .arg(m_BGColorHex));

    if (!m_OccupyColorHex.isEmpty())
        qss.append(QString("QProgressBar::chunk{"
                               "background-color: %1;}")
                           .arg(m_OccupyColorHex));

    if (!qss.isEmpty())
        setStyleSheet(qss);
}

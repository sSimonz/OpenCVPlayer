#include "digiiconpushbtn.h"

#include <QDebug>
#include <QtMath>
#include <QFontMetricsF>

DigiIconPushBtn::DigiIconPushBtn(QWidget *parent) :
    QPushButton(parent),
    m_BackImg(nullptr),
    m_Renderer(nullptr),
    m_IconSize(QSize(16, 16))
{
    m_scale = 0;
    m_width = 0;
    m_height = 28;
    m_Font = GetGlobalFont();
    m_Font.setFamily(GetGlobalFamily(QFont::Medium));
    initStyleSheet();
    setAttribute(Qt::WA_LayoutUsesWidgetRect);
}

DigiIconPushBtn::DigiIconPushBtn(const QString &text, QWidget *parent):
    QPushButton(text, parent),
    m_BackImg(nullptr),
    m_Renderer(nullptr),
    m_IconSize(QSize(16, 16))
{
    m_scale = 0;
    m_width = 0;
    m_height = 32;
    m_Font = GetGlobalFont();
    m_Font.setFamily(GetGlobalFamily(QFont::Medium));
    initStyleSheet();
    setAttribute(Qt::WA_LayoutUsesWidgetRect);
}

void DigiIconPushBtn::SetCustomIcon(const QString &iconpath)
{
    m_iconpath = iconpath;
    if(!m_iconpath.isEmpty())
    {
        QPixmap pixmap(iconpath);
        m_width = pixmap.width();
        m_height = pixmap.height();
        m_IconSize = pixmap.size();
        setIconSize(m_IconSize);
        if(m_iconpath.contains(".svg"))
            setIcon(QIcon(m_iconpath));
        else
            setIcon(QIcon(QPixmap(m_iconpath).scaled(m_IconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }
    else
    {
        setIcon(QIcon());
    }
}

void DigiIconPushBtn::SetCustomIcon(const QString &iconpath, const QSize &size)
{
    m_iconpath = iconpath;
    if(!m_iconpath.isEmpty())
    {
        m_width = size.width();
        m_height = size.height();
        m_IconSize = size;
        setIconSize(m_IconSize);
        if(m_iconpath.contains(".svg"))
            setIcon(QIcon(m_iconpath));
        else
            setIcon(QIcon(QPixmap(m_iconpath).scaled(m_IconSize, Qt::KeepAspectRatio)));
    }
}

void DigiIconPushBtn::AdjustWidthByText(int extraMargin)
{
    QFontMetricsF fm(m_Font);
    m_width = fm.horizontalAdvance(text()) + (m_StyleSizeList.at(Padding) + extraMargin) * 2;
}

void DigiIconPushBtn::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiIconPushBtn::GetCustomFont()
{
    return m_Font;
}

void DigiIconPushBtn::initStyleSheet()
{
    m_StyleColorList << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT)
                     << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT_DISABLE)
                     << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT_CHECK)
                     << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_NORMAL)
                     << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_HOVER)
                     << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_CHECK)
                     << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_DISABLE)
                     << ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BORDER);

//    m_StyleSizeList[Radius]  = 4;
//    m_StyleSizeList[Padding] = 2;
//    m_StyleSizeList[Margin]  = 1;
//    m_StyleSizeList[Border]  = 0;
    qreal margin = 1;
    qreal radius = 4;
    m_StyleSizeList << radius << 2 << margin << 0;
    m_StyleSizeList << -1 << -1 << -1 << -1;
    m_StyleSizeList << -1 << -1 << -1 << -1;
}

void DigiIconPushBtn::SetCustomStyleSheet(qreal scale)
{
    QString radius = "", border = "";
    if(m_StyleSizeList.at(Radius) < 0)
    {
        radius = QString("border-top-left-radius: %1px;"
                         "border-top-right-radius: %2px;"
                         "border-bottom-right-radius: %3px;"
                         "border-bottom-left-radius: %4px;")
                        .arg(m_StyleSizeList.at(RadiusLeftTop)*scale)
                        .arg(m_StyleSizeList.at(RadiusRightTop)*scale)
                        .arg(m_StyleSizeList.at(RadiusRightBtm)*scale)
                        .arg(m_StyleSizeList.at(RadiusLeftBtm)*scale);
    }
    else
    {
        radius = QString("border-radius: %1px;")
                        .arg(m_StyleSizeList.at(Radius)*scale);
    }

    if(m_StyleSizeList.at(Border) < 0)
    {
        border = QString("border-top:%1px solid %2;"
                         "border-left:%3px solid %4;"
                         "border-right:%5px solid %6;"
                         "border-bottom:%7px solid %8;")
                        .arg(m_StyleSizeList.at(BorderTop)*scale)
                        .arg(m_StyleColorList.at(btnBorder))
                        .arg(m_StyleSizeList.at(BorderLeft)*scale)
                        .arg(m_StyleColorList.at(btnBorder))
                        .arg(m_StyleSizeList.at(BorderRight)*scale)
                        .arg(m_StyleColorList.at(btnBorder))
                        .arg(m_StyleSizeList.at(BorderBottom)*scale)
                        .arg(m_StyleColorList.at(btnBorder));
    }
    else
    {
        border = QString("border:%1px solid %2;")
                        .arg(m_StyleSizeList.at(Border)*scale)
                        .arg(m_StyleColorList.at(btnBorder));
    }

     QString qss = QString("QPushButton{"
                               "%1"
                               "padding: %2px;"
                               "color: %8;"
                               "background-color: %4;"
                               "%7}"
                           "QPushButton:hover{"
                               "outline: none;"
                               "color: %8;"
                               "background-color: %5;}"
                           "QPushButton:pressed{"
                               "margin: %3px;}"
                           "QPushButton:disabled{"
                               "background-color: %6;"
                               "color: %9;}"
                           "QPushButton:checked{"
                               "background-color: %10;"
                               "color: %11;}")
                           .arg(radius)
                           .arg(qFloor(m_StyleSizeList.at(Padding) *scale))
                           .arg(qFloor(m_StyleSizeList.at(Margin) *scale))
                           .arg(m_StyleColorList.at(btnNormal))
                           .arg(m_StyleColorList.at(btnHover))
                           .arg(m_StyleColorList.at(btnDisabled))
                           .arg(border)
                           .arg(m_StyleColorList.at(textNormal))
                           .arg(m_StyleColorList.at(textDisable))
                           .arg(m_StyleColorList.at(btnCheck))
                           .arg(m_StyleColorList.at(textCheck));

    setStyleSheet(qss);
}

void DigiIconPushBtn::SetScale(qreal scale)
{
    m_scale = scale;
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    if (m_iconpath.isEmpty())
    {
        QFont aFont = m_Font;
        aFont.setPixelSize(m_Font.pixelSize()*scale);
        setFont(aFont);
    }
    else
    {
        setIconSize(m_IconSize);
        if(m_iconpath.contains(".svg"))
            setIcon(QIcon(m_iconpath));
        else
            setIcon(QIcon(QPixmap(m_iconpath).scaled(m_IconSize)));
    }

    SetCustomStyleSheet(scale);
}

void DigiIconPushBtn::SetHeight(qreal height)
{
    m_height = height;
}

void DigiIconPushBtn::SetCustomIconSize(const QSize &size)
{
    m_IconSize = size;
}

void DigiIconPushBtn::SetStyleTextColor(const QColor &nmClr, const QColor &disClr, const QColor &chkClr)
{
    m_StyleColorList[textNormal]  = nmClr.name(QColor::HexArgb);
    m_StyleColorList[textDisable] = disClr.name(QColor::HexArgb);
    m_StyleColorList[textCheck]   = chkClr.name(QColor::HexArgb);
}

void DigiIconPushBtn::SetStyleBtnColor(const QColor &nmClr, const QColor &hvClr, const QColor &disClr, const QColor &chkClr)
{
    m_StyleColorList[btnNormal]   = nmClr.name(QColor::HexArgb);
    m_StyleColorList[btnHover]    = hvClr.name(QColor::HexArgb);
    m_StyleColorList[btnDisabled] = disClr.name(QColor::HexArgb);
    m_StyleColorList[btnCheck]    = chkClr.name(QColor::HexArgb);
}

void DigiIconPushBtn::SetStyleBtnAllColor(const QColor &color)
{
    SetStyleBtnColor(color, color, color, color);
    SetStyleBorderColor(color);
    QColor disClr = color == QColor(0, 0, 0, 0) ? color : ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_DISABLE);
    m_StyleColorList[btnDisabled] = disClr.name(QColor::HexArgb);
}

void DigiIconPushBtn::SetStyleBorderColor(const QColor &color)
{
    m_StyleColorList[btnBorder] = color.name(QColor::HexArgb);
}

void DigiIconPushBtn::SetStyleSize(qreal radius, qreal padding, qreal margin ,qreal border)
{
    m_StyleSizeList[Radius]  = radius;
    m_StyleSizeList[Padding] = padding;
    m_StyleSizeList[Margin]  = margin;
    m_StyleSizeList[Border]  = border;
}

void DigiIconPushBtn::SetStyleRadius(qreal left_top, qreal right_top, qreal right_btm, qreal left_btm)
{
    m_StyleSizeList[RadiusLeftTop] = left_top;
    m_StyleSizeList[RadiusRightTop] = right_top;
    m_StyleSizeList[RadiusRightBtm] = right_btm;
    m_StyleSizeList[RadiusLeftBtm] = left_btm;
    m_StyleSizeList[Radius] = -1;
}

void DigiIconPushBtn::SetStyleBorder(qreal top, qreal left, qreal right, qreal bottom)
{
    m_StyleSizeList[BorderTop] = top;
    m_StyleSizeList[BorderLeft] = left;
    m_StyleSizeList[BorderRight] = right;
    m_StyleSizeList[BorderBottom] = bottom;
    m_StyleSizeList[Border] = -1;
}

void DigiIconPushBtn::UpdateCustomStyleSheet()
{
    SetCustomStyleSheet(m_scale);
}

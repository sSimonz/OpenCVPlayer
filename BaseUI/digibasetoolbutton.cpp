#include "digibasetoolbutton.h"

#include <QtMath>

DigiBaseToolButton::DigiBaseToolButton(QWidget *parent) :
    QToolButton(parent),
    m_PngIcon(nullptr)
{
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();
    m_IconSize = QSize(16,16);
    setAttribute(Qt::WA_LayoutUsesWidgetRect);
    InitStyle();
}

DigiBaseToolButton::~DigiBaseToolButton()
{
    if (m_PngIcon != nullptr)
        delete m_PngIcon;
}

void DigiBaseToolButton::SetCustomIcon(const QString &path)
{
    if (path.endsWith(".svg"))
    {
        //svg图片直接加载
        setIcon(QIcon(path));

        if (m_PngIcon != nullptr)
        {
            delete m_PngIcon;
            m_PngIcon = nullptr;
        }
    }
    else
    {
        if(m_PngIcon == nullptr)
            m_PngIcon = new QPixmap(path);

        m_PngIcon->load(path);
        m_IconSize = m_PngIcon->size();
        setIconSize(m_IconSize);
        QPixmap pix = m_PngIcon->scaled(m_IconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        setIcon(pix);
    }
}

void DigiBaseToolButton::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

void DigiBaseToolButton::SetCustomIconSize(const QSize &size)
{
    m_IconSize = size;
}

void DigiBaseToolButton::SetTextColor(const QColor &normal, QColor hover, QColor pressed, QColor disable)
{
    m_TextColorList[Normal] = normal.name(QColor::HexArgb);

    if (!hover.isValid())
        hover = normal;
    if (!pressed.isValid())
        pressed = hover;
    if (!disable.isValid())
        disable = ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT_DISABLE);

    m_TextColorList[Hover] = hover.name(QColor::HexArgb);
    m_TextColorList[Pressed] = pressed.name(QColor::HexArgb);
    m_TextColorList[Disabled] = disable.name(QColor::HexArgb);
}

void DigiBaseToolButton::SetBgColor(const QColor &normal, QColor hover, QColor pressed, QColor disable)
{
    m_BgColorList[Normal] = normal.name(QColor::HexArgb);

    if (!hover.isValid())
        hover = normal;
    if (!pressed.isValid())
        pressed = hover;
    if (!disable.isValid())
        disable = ColorManager::GetColorHex(ColorManager::CN_TOOLBTN_BACKGROUND_DISABLE);

    m_BgColorList[Hover] = hover.name(QColor::HexArgb);
    m_BgColorList[Pressed] = pressed.name(QColor::HexArgb);
    m_BgColorList[Disabled] = disable.name(QColor::HexArgb);
}

void DigiBaseToolButton::SetBorderWidth(qreal borderWidth)
{
    m_Border = borderWidth;
}

void DigiBaseToolButton::SetBorderColor(const QColor &color)
{
    m_BorderColor = color.name(QColor::HexArgb);
}

void DigiBaseToolButton::SetRadius(qreal radius)
{
    m_Radius = radius;
}

void DigiBaseToolButton::SetPadding(qreal normal, qreal pressed)
{
    m_PaddingList[0] = normal;
    m_PaddingList[1] = pressed;
}

void DigiBaseToolButton::SetMargin(qreal normal, qreal pressed)
{
    m_MarginList[0] = normal;
    m_MarginList[1] = pressed;
}

QFont DigiBaseToolButton::GetCustomFont()
{
    return m_Font;
}

void DigiBaseToolButton::AdjustNormalStyle()
{
    // DigiBaseIconPushBtn::initStyleSheet
    m_BgColorList[Normal] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_NORMAL);
    m_BgColorList[Hover] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_HOVER);
    m_BgColorList[Pressed] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_HOVER);
    m_BgColorList[Disabled] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_BACKGROUND_DISABLE);

    m_TextColorList[Normal] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT);
    m_TextColorList[Hover] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT);
    m_TextColorList[Pressed] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT);
    m_TextColorList[Disabled] = ColorManager::GetColorHex(ColorManager::CN_ICONBTN_TEXT_DISABLE);

    m_height = 28;
    m_Radius = 4;
    m_Font.setPixelSize(14);
    m_Font.setFamily(GetGlobalFamily(QFont::Medium));
}

void DigiBaseToolButton::SetScale(qreal scale)
{
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    setIconSize(m_IconSize * scale);
    if (m_PngIcon != nullptr)
    {
        QPixmap pix = m_PngIcon->scaled(iconSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        setIcon(pix);
    }

    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    SetCustomStyleSheet(scale);
}

void DigiBaseToolButton::InitStyle()
{
    m_TextColorList << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)
                    << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)
                    << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)
                    << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT_DISABLE);
    m_BgColorList << ColorManager::GetColorHex(ColorManager::CN_TOOLBTN_BACKGROUND_NORMAL)
                  << ColorManager::GetColorHex(ColorManager::CN_TOOLBTN_BACKGROUND_HOVER)
                  << ColorManager::GetColorHex(ColorManager::CN_TOOLBTN_BACKGROUND_HOVER)
                  << ColorManager::GetColorHex(ColorManager::CN_TOOLBTN_BACKGROUND_DISABLE);

    m_Border = 0;
    m_Radius = 0;

    m_PaddingList << 0 << 1;
    m_MarginList << 0 << 1;
}

void DigiBaseToolButton::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;

    qss.append(QString("QToolButton{"
                           "background-color: %1;"
                           "color:%2;"
                           "border:%3px solid %7;"
                           "border-radius: %4px;"
                           "padding: %5px;"
                           "margin: %6px;}")
                       .arg(m_BgColorList.at(Normal))
                       .arg(m_TextColorList.at(Normal))
                       .arg(m_Border * scale)
                       .arg(m_Radius * scale)
                       .arg(qFloor(m_PaddingList.at(0) * scale))
                       .arg(qFloor(m_MarginList.at(0) * scale))
                       .arg(m_BorderColor));

    qss.append(QString("QToolButton:hover{"
                           "background-color:%1;"
                           "color:%2;}")
                       .arg(m_BgColorList.at(Hover))
                       .arg(m_TextColorList.at(Hover)));

    qss.append(QString("QToolButton:pressed{"
                           "background-color:%1;"
                           "color: %2;"
                           "padding: %3px;"
                           "margin: %4px;}")
                       .arg(m_BgColorList.at(Pressed))
                       .arg(m_TextColorList.at(Pressed))
                       .arg(qFloor(m_PaddingList.at(1) * scale))
                       .arg(qFloor(m_MarginList.at(1) * scale)));

    qss.append(QString("QToolButton:disabled{"
                           "border: 0px;"
                           "background-color: %1;"
                           "color: %2;}")
                       .arg(m_BgColorList.at(Disabled))
                       .arg(m_TextColorList.at(Disabled)));

    setStyleSheet(qss.join(""));
}

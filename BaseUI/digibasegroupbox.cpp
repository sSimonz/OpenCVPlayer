#include "digibasegroupbox.h"

#include <QLayout>
#include "Res/resfun.h"

DigiBaseGroupBox::DigiBaseGroupBox(QWidget *parent) :
    QGroupBox(parent),
    m_bFirstShow(true),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_BorderColorHex(ColorManager::GetColorHex(ColorManager::CN_GROUPBOX_BORDER)),
    m_Radius(4),
    m_TopMargins(8),
    m_BorderWidth(2),
    m_IndicatorSize(16)
{
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();
    SetLayoutMargins(QMargins(-1,-1,-1,-1));
    SetLayoutSpacing(-1, -1);
}

DigiBaseGroupBox::DigiBaseGroupBox(const QString &title, QWidget *parent) :
    QGroupBox(title, parent),
    m_bFirstShow(true),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_BorderColorHex(ColorManager::GetColorHex(ColorManager::CN_GROUPBOX_BORDER)),
    m_Radius(4),
    m_TopMargins(8),
    m_BorderWidth(2),
    m_IndicatorSize(16)
{
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();
    SetLayoutMargins(QMargins(-1,-1,-1,-1));
    SetLayoutSpacing(-1, -1);
}

void DigiBaseGroupBox::SetLayoutMargins(const QMargins &margin)
{
    QLayout* layout = this->layout();
    if(!layout)
        return;

    LayoutInfo info = m_layoutMap.value(layout);
    info.margins = margin;
    if(!m_layoutMap.contains(layout))
    {
        if(layout->inherits("QGridLayout"))
        {
            QGridLayout *gLayout = qobject_cast<QGridLayout*>(layout);
            info.hspacing = gLayout->horizontalSpacing();
            info.vspacing = gLayout->verticalSpacing();
        }
        else
        {
            info.spacing = layout->spacing();
        }
    }
    m_layoutMap[layout] = info;
}

void DigiBaseGroupBox::SetLayoutSpacing(int horSpacing, int verSpacing)
{
    QLayout* layout = this->layout();
    if(!layout)
        return;

    LayoutInfo info = m_layoutMap.value(layout);
    info.spacing = horSpacing;
    info.hspacing = horSpacing;
    info.vspacing = verSpacing;
    if(!m_layoutMap.contains(layout))
    {
        info.margins = layout->contentsMargins();
    }
    m_layoutMap[layout] = info;
}

void DigiBaseGroupBox::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseGroupBox::SetBorderColor(const QColor &color)
{
    m_BorderColorHex = color.name(QColor::HexArgb);
}

void DigiBaseGroupBox::SetRadius(qreal radius)
{
    m_Radius = radius;
}

void DigiBaseGroupBox::SetTopMargins(qreal topMargin)
{
    m_TopMargins = topMargin;
}

void DigiBaseGroupBox::SetBorderWidth(qreal border)
{
    m_BorderWidth = border;
}

void DigiBaseGroupBox::SetIndicatorSize(qreal size)
{
    m_IndicatorSize = size;
}

void DigiBaseGroupBox::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseGroupBox::GetCustomFont()
{
    return m_Font;
}

void DigiBaseGroupBox::SetScale(qreal scale)
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

    //子控件
    auto widgetlist = findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
    foreach (QWidget *var, widgetlist)
    {
        if (var->inherits("IWidgetBase"))
        {
            dynamic_cast<IWidgetBase *>(var)->SetScale(scale);
        }
    }

    //布局
    auto layoutlist = findChildrenRecursively<QLayout*>(this);
    foreach (QLayout *var, layoutlist)
    {
        if(!m_layoutMap.contains(var))
        {
            LayoutInfo info;
            info.margins = var->contentsMargins();
            if(var->inherits("QGridLayout"))
            {
                QGridLayout *gLayout = qobject_cast<QGridLayout*>(var);
                info.hspacing = gLayout->horizontalSpacing();
                info.vspacing = gLayout->verticalSpacing();
            }
            else
            {
                info.spacing = var->spacing();
            }
            m_layoutMap.insert(var, info);
        }
    }

    foreach (QLayout *var, m_layoutMap.keys())
    {
        if(!layoutlist.contains(var))
        {
            m_layoutMap.remove(var);
        }
    }

    foreach (QLayout* layout, m_layoutMap.keys()) {
        LayoutInfo info = m_layoutMap.value(layout);
        layout->setContentsMargins(info.margins * scale);
        if(layout->inherits("QGridLayout"))
        {
            QGridLayout *gLayout = qobject_cast<QGridLayout*>(layout);
            gLayout->setHorizontalSpacing(info.hspacing * scale);
            gLayout->setVerticalSpacing(info.vspacing * scale);
        }
        else
        {
            layout->setSpacing(info.spacing * scale);
        }
    }

    //样式
    SetCustomStyleSheet(scale);

}

void DigiBaseGroupBox::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;
    this->setCheckable(true);
    qss.append(QString("QGroupBox{"
                           "border: %1px solid %2;"
                           "border-radius:%3px;"
                           "margin-top:%4px;"
                           "color: %5;}")
                       .arg(m_BorderWidth * scale)
                       .arg(m_BorderColorHex)
                       .arg(m_Radius * scale)
                       .arg(m_TopMargins * scale)
                       .arg(m_TextColorHex));

    //标题
    qss.append(QString("QGroupBox::title{"
                           "subcontrol-origin:margin;"
                           "subcontrol-position:top left;"
                           "left: %1px;"
                           "margin-left:%2px; margin-right:%2px;"
                           "spacing: %3px;}")
                       .arg(8*scale).arg(4*scale).arg(2*scale));

    //勾选框
    qss.append(QString("QGroupBox::indicator{"
                           "width: %1px;"
                           "height: %1px;}")
                       .arg(m_IndicatorSize * scale));
    qss.append(QString("QGroupBox::indicator:unchecked {image: url(%1);}"
                       "QGroupBox::indicator:checked{image: url(%2);}"
                       "QGroupBox::indicator:indeterminate{image: url(%3);}"
                       "QGroupBox::indicator:disabled{image: url(%4);}"
                       "QGroupBox::indicator:checked:disabled{image: url(%5);}"
                       "QGroupBox::indicator:indeterminate:disabled{image: url(%6);}")
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_UNSELECTED))
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_SELECTED))
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_PARTSELECTED))
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_UNSELECTED_DISABLED))
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_SELECTED_DISABLED))
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_PARTSELECTED_DISABLED)));

    setStyleSheet(qss.join(""));
}

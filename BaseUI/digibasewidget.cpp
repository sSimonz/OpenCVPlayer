#include "digibasewidget.h"
#include <QLayout>

DigiBaseWidget::DigiBaseWidget(QWidget *parent):
    QWidget(parent),
    m_bFirstShow(true)
{
    m_scale = 0;
    m_width = 0;
    m_height = 0;

    m_SizeList << 0 << 0;
    m_ColorList << "#00000000" << "#00000000";

    SetLayoutMargins(QMargins(-1,-1,-1,-1));
    SetLayoutSpacing(-1, -1);

    setAttribute(Qt::WA_StyledBackground);
}

void DigiBaseWidget::SetBGColor(const QColor &color)
{
    m_ColorList[BackGroundColor] = color.name(QColor::HexArgb);
}

void DigiBaseWidget::SetBorderRadius(int radius)
{
    m_SizeList[Radius] = radius;
}

void DigiBaseWidget::SetBorderWidth(int border)
{
    m_SizeList[BorderWidth] = border;
}

void DigiBaseWidget::SetBorderColor(const QColor &color)
{
    m_ColorList[BorderColor] = color.name(QColor::HexArgb);
}

void DigiBaseWidget::SetLayoutMargins(const QMargins &margin)
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

void DigiBaseWidget::SetLayoutSpacing(int horSpacing, int verSpacing)
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

void DigiBaseWidget::SetScale(qreal scale)
{
    if(qFuzzyIsNull(scale))
        return;
    if(qFuzzyCompare(m_scale,scale))
        return;
    m_scale = scale;
    //自身
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height*scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width*scale);

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

void DigiBaseWidget::SetCustomStyleSheet(qreal scale)
{
    QString styleSheet = QString("DigiBaseWidget{"
                                     "background-color: %1;"
                                     "border-radius: %2px;"
                                     "border: %3px solid %4;}")
                             .arg(m_ColorList.at(BackGroundColor))
                             .arg(m_SizeList.at(Radius)*scale)
                             .arg(m_SizeList.at(BorderWidth)*scale)
                             .arg(m_ColorList.at(BorderColor));

    setStyleSheet(styleSheet);
}

void DigiBaseWidget::UpdateCustomStyleSheet()
{
    SetCustomStyleSheet(m_scale);
}

#include "digibaseframe.h"
#include <QLayout>
#include <QGraphicsDropShadowEffect>

DigiBaseFrame::DigiBaseFrame(QWidget *parent):
    QFrame(parent),
    m_bFirstShow(true)
{
    m_width = 0;
    m_height = 0;

    m_SizeList << 0 << 0;
    m_ColorList << "#00000000" << "#00000000";

    SetLayoutMargins(QMargins(-1,-1,-1,-1));
    SetLayoutSpacing(-1, -1);

    m_FrameScale = 0;
}

void DigiBaseFrame::SetBGColor(const QColor &color)
{
    m_ColorList[BackGroundColor] = color.name(QColor::HexArgb);
}

void DigiBaseFrame::SetBorderRadius(int radius)
{
    m_SizeList[Radius] = radius;
}

void DigiBaseFrame::SetBorderWidth(int border)
{
    m_SizeList[BorderWidth] = border;
}

void DigiBaseFrame::SetBorderColor(const QColor &color)
{
    m_ColorList[BorderColor] = color.name(QColor::HexArgb);
}

void DigiBaseFrame::SetLayoutMargins(const QMargins &margin)
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

void DigiBaseFrame::SetLayoutSpacing(int horSpacing, int verSpacing)
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

void DigiBaseFrame::SetPopupFlags(bool b)
{
    if(b)
    {
        DigiBaseFrame *widget = new DigiBaseFrame(this);
        widget->setLayout(this->layout());
        QHBoxLayout *hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(6, 6, 6, 6);
        hLayout->addWidget(widget);
        setLayout(hLayout);
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        setAttribute(Qt::WA_TranslucentBackground, true);
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setOffset(0, 0);// 阴影偏移量
        shadow->setColor(QColor(68, 68, 68));
        shadow->setBlurRadius(10);// 模糊半径，越大越模糊
        widget->setGraphicsEffect(shadow);
        widget->SetBGColor(ColorManager::GetColor(ColorManager::CN_THEME_BACKGROUND));
        widget->SetBorderRadius(8);
    }
}

void DigiBaseFrame::SetScale(qreal scale)
{
    if(qFuzzyIsNull(scale))
        return;
    if(qFuzzyCompare(m_FrameScale,scale))
        return;
    m_FrameScale = scale;
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

void DigiBaseFrame::SetCustomStyleSheet(qreal scale)
{
    QString styleSheet = QString("DigiBaseFrame{"
                                     "background-color: %1;"
                                     "border-radius: %2px;"
                                     "border: %3px solid %4;}")
                                .arg(m_ColorList.at(BackGroundColor))
                                .arg(m_SizeList.at(Radius)*scale)
                                .arg(m_SizeList.at(BorderWidth)*scale)
                                .arg(m_ColorList.at(BorderColor));

    setStyleSheet(styleSheet);
}

void DigiBaseFrame::UpdateCustomStyleSheet()
{
    SetCustomStyleSheet(m_FrameScale);
}

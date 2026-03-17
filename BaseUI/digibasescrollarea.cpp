#include "digibasescrollarea.h"
#include "digibasescrollbar.h"

DigiBaseScrollArea::DigiBaseScrollArea(QWidget *parent):
    QScrollArea(parent)
{
    m_width = 0;
    m_height = 0;

    setHorizontalScrollBar(new DigiBaseScrollBar(this));
    setVerticalScrollBar(new DigiBaseScrollBar(this));

    QString qss;
    qss = "QScrollArea, QScrollArea .QWidget{"
              "background-color: #00000000;"
              "border:0px;}";
    setStyleSheet(qss);
}

void DigiBaseScrollArea::SetScale(qreal scale)
{
    //自身大小
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);

    //滚动条
    dynamic_cast<IWidgetBase*>(horizontalScrollBar())->SetScale(scale);
    dynamic_cast<IWidgetBase*>(verticalScrollBar())->SetScale(scale);

    //子控件
    if (widget()->inherits("IWidgetBase"))
    {
        dynamic_cast<IWidgetBase *>(widget())->SetScale(scale);
    }
    else
    {
        auto widgetlist = widget()->findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
        foreach (QWidget *var, widgetlist)
        {
            if (var->inherits("IWidgetBase"))
            {
                dynamic_cast<IWidgetBase *>(var)->SetScale(scale);
            }
        }
    }
}

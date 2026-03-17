#include "digibasestackedwidget.h"

DigiBaseStackedWidget::DigiBaseStackedWidget(QWidget *parent):
    QStackedWidget(parent)
{
    m_width = 0;
    m_height = 0;
}

void DigiBaseStackedWidget::SetBGColor(const QColor &color)
{
    setStyleSheet(QString(".QStackedWidget{background-color:%1;}").arg(color.name(QColor::HexArgb)));
}

void DigiBaseStackedWidget::SetScale(qreal scale)
{
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
}

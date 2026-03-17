#include "digibasesplitter.h"

DigiBaseSplitter::DigiBaseSplitter(QWidget *parent):
    QSplitter(parent),
    m_Scale(0)
{
    m_width = 0;
    m_height = 0;
}

void DigiBaseSplitter::SetScale(qreal scale)
{
    if(qFuzzyIsNull(scale))
        return;
    if(qFuzzyCompare(m_Scale,scale))
        return;

    m_Scale = scale;

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

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseSplitter::SetCustomStyleSheet(qreal scale)
{
    setStyleSheet("QSplitter::handle{background-color: #00000000;}"); //透明，跟随背景色
}

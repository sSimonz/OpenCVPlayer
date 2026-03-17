#include "widget_dialogbase.h"
#include <QStyleOption>

Widget_DialogBase::Widget_DialogBase(QWidget *parent) :
    DigiBaseWidget(parent)
{

}

Widget_DialogBase::Widget_DialogBase(QObject *param, QWidget *parent):
    DigiBaseWidget(parent),
    param(param)
{

}

void Widget_DialogBase::closeResponse()
{

}

void Widget_DialogBase::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#ifndef WIDGET_DIALOGBASE_H
#define WIDGET_DIALOGBASE_H
/**
 * @brief 对话框基类 Widget_DialogBase
 * 1. 提供对话框返回结果的信号 doneRequest(ResultType r,QObject* result)
 * 2. 提供对话框关闭响应函数 closeResponse()，子类可重载该函数
 *
 * version 1.0
 * @date 2024-6-10
 *
 * Copyright (c) 2007-2024, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include "digibasewidget.h"
#include <QPainter>
#include <QFrame>

// 对话框返回结果类型
enum ResultType
{
    CancelOrClose=-1,
    Done=0,
    DoneWithParam=1
};

class Widget_DialogBase : public DigiBaseWidget
{
    Q_OBJECT
public:
    explicit Widget_DialogBase(QWidget *parent = nullptr);
    Widget_DialogBase(QObject* param,QWidget *parent = nullptr);

    virtual void closeResponse();
    QObject* param;

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void doneRequest(ResultType r,QObject* result);
    void doneClose(bool flag);
signals:
    void moveMainWindowRequest(QPoint point);
    void changeSizeRequest(qreal width = 0, qreal height = 0);// 默认0表示不修改

};


#endif // WIDGET_DIALOGBASE_H

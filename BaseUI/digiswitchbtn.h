#ifndef DIGISWITCHBTN_H
#define DIGISWITCHBTN_H

/**
 * @brief 客户端公共库中的开关按钮 SwitchBtn 封装
 * 1. 可进行缩放，大小默认固定值：47*20，圆点直径16
 * 2. 可以设置开关颜色、圆点颜色、圆点大小
 * 3. 修改m_height的值时，圆点大小也会同步变化
 * 4. 开关状态变化会发送信号 statusChange(bool)
 * 5. 可以设置、获取index、QString数据
 *
 * version 1.0
 * @date 2022-3-23
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QFrame>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class QPropertyAnimation;
class DigiBaseLabel;
class DigiSwitchBtn : public QFrame, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiSwitchBtn(QWidget *parent = nullptr);

    void SetOnStatusColor(const QColor &color);         //设置 开 颜色
    void SetOffStatusColor(const QColor &color);        //设置 关 颜色
    void SetOnDisableStatusColor(const QColor &color);  //设置disable下 开 颜色
    void SetOffDisableStatusColor(const QColor &color); //设置disable下 关 颜色
    void SetDotColor(const QColor &color);              //设置圆点颜色
    void SetDotSize(qreal size);                        //设置圆点大小

    bool getBtnStatus();                //获取按钮的开、关状态
    void setBtnStatus(bool newStatus);  //设置按钮的开、关状态
    void setEnabled(bool flag);         //隐藏了父类的setEnabled
    void setEnable(bool flag);          //兼容旧代码

    void setData(const QString &data);   //设置数据
    QString getData();                   //获取数据

    virtual void SetScale(qreal scale);  //缩放
    virtual void SetHeight(qreal height);

signals:
    void statusChange(bool status);  //开、关状态变化时发送此信号
    void clicked(bool status);  // 点击时发送此信号

protected:
    void mouseReleaseEvent(QMouseEvent *event);  //点击，切换开关

private slots:
    void changeStatus();    //白点移动动画结束调用该槽函数

private:
    void animationMove();   //用于移动白点，此函数会反转开关状态
    void SetCustomStyleSheet(qreal scale = 1.0);

public:
    //兼容旧代码
    int m_index;
    bool m_firstEnter;
    int m_groupID;

private:
    bool m_Status;    //当前开、关状态
    QString m_Data;   //QString数据

    DigiBaseLabel *m_DotLab; //圆点
    qreal m_DotSize;  //圆点大小（直径）

    QPoint m_LeftPos;  //圆点左侧坐标
    QPoint m_RightPos; //圆点右侧坐标
    QPropertyAnimation *m_Animation; //动画类

    QString m_OnColorHex;         //开 颜色
    QString m_OffColorHex;        //关 颜色
    QString m_OnDisableColorHex;  //disable 开 颜色
    QString m_OffDisableColorHex; //disable 关 颜色
    QString m_DotColorHex;        //圆点颜色
    QString m_DotDisableColorHex; //disable 圆点颜色
};

#endif // DIGISWITCHBTN_H

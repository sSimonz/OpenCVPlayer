#ifndef DIGICHECKHEADERVIEW_H
#define DIGICHECKHEADERVIEW_H

/**
 * @brief 客户端公共库中的带勾选框表头 CheckHeaderView 封装类
 * 1. 可进行缩放，缩放只改变 CheckBox 的大小，其他大小、样式属性均在表格类中实现，不在此实现
 * 2. CheckBox默认在该列居中显示
 * 3. 表头默认属性：显示排序指示符，响应鼠标单击，尾项自动拉伸
 * 4. 可设置CheckBox的大小（默认16*16）、CheckBox的图标（支持svg/png）、CheckBox位置靠左中右（尾项靠右会显示不全）
 * 5. 实现原理，类中没有 CheckBox 类，由m_CheckState变量记录状态，根据状态绘制相应的图标在表头上
 *
 * version 1.0
 * @date 2022-3-29
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QHeaderView>
#include "Base/iwidgetbase.h"

class QSvgRenderer;

class DigiCheckHeaderView : public QHeaderView, public IWidgetBase
{
    Q_OBJECT

public:
    DigiCheckHeaderView(int checkColumnIndex,
                               Qt::Orientation orientation,
                               QWidget * parent = nullptr);  //checkColumnIndex为checkBox所在的列数下标
    ~DigiCheckHeaderView();

    void LoadCheckBoxImgPath(const QString &unCheckImg,    //未选中图标
                             const QString &paCheckedImg,  //部分选中图标
                             const QString &checkedImg);   //已选中图标

    void SetCheckBoxAlignment(Qt::AlignmentFlag flag);     //设置图标靠左、中、右
    void SetCheckBoxSize(qreal size);                      //设置checkBox大小
    void SetCheckState(Qt::CheckState state);              //设置checkBox状态，调用onCheckStateChangeToTable()
    Qt::CheckState GetCheckState();                        //获取checkBox状态

    void SetScale(qreal scale);                            //缩放

public slots:
    void onCheckStateChangeToTable(Qt::CheckState state);  //设置checkBox状态

signals:
    void CheckStateChangeSig(Qt::CheckState state);        //点击checkBox后发射该信号

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private:
    QRect checkBoxRect(int logicalIndex) const;   //计算出checkBox显示的位置
    void ClearList();                             //清空、初始化svg和pixmap的list，在LoadCheckBoxImgPath()中调用
    void LoadImg(int index, const QString &path); //根据下标初始化list的内容，在LoadCheckBoxImgPath()中调用
    void DrawCheckBox(QPainter *painter, int index, const QRect &rectBox) const;  //画出checkBox，在paintSection()中调用

private:
    Qt::CheckState m_CheckState;    //checkBox状态
    Qt::AlignmentFlag m_Alignment;  //checkBox位置属性
    int m_CheckColumnIndex;         //checkBox所在列
    qreal m_CheckBoxCurSize;        //checkBox当前大小
    qreal m_CheckBoxSize;           //checkBox基值大小

    QList<QSvgRenderer *> m_RendererList; //存放svg
    QList<QPixmap *> m_PixmapList;        //存放png
    QFont m_Font;
};

#endif // DIGICHECKHEADERVIEW_H

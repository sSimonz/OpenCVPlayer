#ifndef DIGIBASETREEVIEW_H
#define DIGIBASETREEVIEW_H

/**
 * @brief 客户端公共库中的基础树表 TreeView 封装类
 * 1. 可进行缩放，组件默认无基础大小(宽高基值均为0)，跟随布局改变窗体大小
 * 2. 缩放改变的内容：行高、表格字体大小、表格branch箭头大小、滚动条样式，若宽高基值不为0，则参与缩放。
 * 3. 默认属性：显示折叠展开的动画、隐藏表头、内容不可编辑、表头尾自动拉伸、列宽根据内容自动变化、无焦点
 * 4. 可设置统一的行高(默认35)、表格背景颜色、表格文本颜色、表格项选中和悬停时颜色
 * 5. 表格内容、Item的图标和checkbox等内容，由自定义的model来实现
 * 6. 暂时只提供了样式设置的接口
 *
 * version 1.0
 * @date 2022-3-17
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QTreeView>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseTreeView : public QTreeView, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseTreeView(QWidget *parent = nullptr);

    void SetRowHeight(qreal height);               //行高，itemHeight
    void SetBGColor(const QColor &color);          //TreeView背景颜色
    void SetTextColor(const QColor &color);        //文本颜色
    void SetHoverAndSelColor(const QColor &color); //item选中、悬停颜色
    void SetCustomIconSize(const QSize &size);     //图标大小
    void SetCustomFont(const QFont &font);         //设置字体
    QFont GetCustomFont();                         //获取字体
    void SetBorderWidth(qreal borderWidth);        //设置描边宽度
    void SetBorderColor(const QColor &color);      //设置描边颜色
    void SetRadius(qreal radius);                  //设置圆角半径

    virtual void SetScale(qreal scale);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    void InitStyle();
    virtual void SetCustomStyleSheet(qreal scale = 1.0);

private:
    QFont m_Font;            //字体
    QSize m_IconSize;        //图标大小
    qreal m_RowHeight;       //行高
    qreal m_branchIconSize;  //branch箭头图标大小
    QString m_BGColorHex;    //背景颜色
    QString m_TextColorHex;  //文本颜色
    QString m_HoverAndSelColorHex; //item的hover和selection背景颜色
    QString m_BorderColor;   //描边颜色
    qreal m_Border;          //描边宽度
    qreal m_Radius;          //圆角大小
};

#endif // DIGIBASETREEVIEW_H

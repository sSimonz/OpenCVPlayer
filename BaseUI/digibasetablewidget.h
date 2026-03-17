#ifndef DIGIBASETABLEWIDGET_H
#define DIGIBASETABLEWIDGET_H

/**
 * @brief 客户端公共库中的基础表格 TableWidget 封装类
 * 1. 可进行缩放，组件默认无基础大小(宽高基值均为0)，跟随布局改变窗体大小
 * 2. 缩放改变的内容：行高列宽、横表头高度及其样式、表格字体大小、滚动条样式，若宽高基值不为0，则参与缩放。
 * 3. 默认属性：内容不可编辑、竖表头隐藏、横表头尾自动拉伸、整行选择、无焦点
 * 4. 可设置统一的行高(默认30)、表格背景颜色、表格项选中时颜色、横表头高度(默认32)及其样式颜色
 * 5. 表头内容、表格内容、表格hover颜色等内容，由自定义的model来实现，列宽等具体表格内容再定
 * 6. SetColumnSizeList可设定每列的宽度，若某列的基值传入0，则该列的属性会被设置为QHeaderView::Stretch，且横表头尾自动拉伸属性会取消
 * 7. 暂时只提供了样式设置的接口
 *
 * version 1.0
 * @date 2022-3-15
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QTableWidget>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseTableWidget : public QTableWidget, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseTableWidget(QWidget *parent = nullptr);

    void SetRowHeight(qreal height);              //每行的高度基值
    void SetHeaderHeight(qreal height);           //横表头高度
    void SetBGColor(const QColor &color);         //tableWidget背景颜色
    void SetAlternateColor(const QColor &color);  //交替颜色
    void SetSelectionColor(const QColor &color);  //选中时颜色
    void SetHeaderTextColor(const QColor &color); //横表头文本颜色
    void SetHeaderBGColor(const QColor &color);   //横表头背景颜色
    void SetHeaderBorderColor(const QColor &color);//横表头边框颜色
    void SetColumnSizeList(const QList<int> &list);//设置列宽
    void SetCustomFont(const QFont &font);   //设置字体
    QFont GetCustomFont();                   //获取字体

    void SetScale(qreal scale) override;
    void setItem(int row, int column, QTableWidgetItem *item);

private:
    void InitStyle();
    void SetCustomStyleSheet(qreal scale = 1.0) override;

private:
    bool m_bFirst;                 //用于判断是否第一次进入缩放函数
    QString m_BGColorHex;          //背景颜色
    QString m_AlternateColorHex;   //背景交替颜色显示时的交替色
    QString m_SelectionColorHex;   //选中时的背景颜色
    QString m_HeaderTextColorHex;  //横表头文本颜色
    QString m_HeaderBGColorHex;    //横表头背景颜色
    QString m_HeaderBorderColorHex;//横表头边框颜色

    QFont m_Font;
    qreal m_RowHeight;      //行高度
    qreal m_HeaderHeight;   //表头高度
    qreal m_HeaderFontSize; //表头文字大小
    QList<int> m_ColumnSizeList; //存放列宽
};

#endif // DIGIBASETABLEWIDGET_H

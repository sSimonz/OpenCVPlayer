#ifndef DIGIBASELISTVIEW_H
#define DIGIBASELISTVIEW_H

/**
 * @brief 客户端公共库中的基础列表 ListView 封装类
 * 1. 可进行缩放，组件默认无基础大小(宽高基值均为0)，跟随布局改变窗体大小
 * 2. 缩放改变的内容：行高、字体大小、滚动条样式，若宽高基值不为0，则参与缩放。
 * 3. 默认属性：横滚动条始终不显示、一次滚动一像素(拖滚动条)
 * 4. 可设置统一的行高(默认20)、列表背景颜色、文本颜色、项选中和悬停时背景、文本颜色、文本字体
 * 5. 列表内容，由自定义的model来实现
 * 6. 暂时只提供了样式设置的接口
 *
 * version 1.0
 * @date 2022-5-13
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QListView>
#include "Base/istylesheetbase.h"
#include "Base/iwidgetbase.h"

class DigiBaseListView : public QListView, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseListView(QWidget *parent = nullptr);

    void SetCustomFont(const QFont &font);         //设置字体
    void SetRowHeight(qreal height);               //行高，itemHeight
    void SetBGColor(const QColor &color);          //ListWidget背景颜色
    void SetTextColor(const QColor &color);        //文本颜色
    void SetHoverAndSelColor(const QColor &color); //item选中、悬停颜色
    void SetTextHoverColor(const QColor &color);   //item选中、悬停时文本颜色

    QFont GetCustomFont();         //获取字体

    virtual void SetScale(qreal scale);

private:
    void InitStyle();
    virtual void SetCustomStyleSheet(qreal scale = 1.0);

private:
    QFont m_Font;                  //字体
    qreal m_RowHeight;             //行高
    QString m_BGColorHex;          //背景颜色
    QString m_TextColorHex;        //文本颜色
    QString m_HoverSelColorHex;    //item的hover和selection背景颜色
    QString m_TextHoverColorHex;   //item的hover和selection文本颜色
};

#endif // DIGIBASELISTVIEW_H

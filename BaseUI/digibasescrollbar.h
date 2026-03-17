#ifndef DIGIBASESCROLLBAR_H
#define DIGIBASESCROLLBAR_H

/**
 * @brief 公共库中的滚动条 ScrollBar 封装类
 * 1. 可进行缩放。只对样式进行缩放。
 * 2. 在有QScrollBar的控件中对原QScrollBar进行替换，比如TableView、TreeView等控件，这些控件缩放时需调用滚动条类的缩放函数。
 * 3. 提供颜色样式设置接口
 *
 * version 1.0
 * @date 2022-6-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QScrollBar>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseScrollBar : public QScrollBar, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseScrollBar(QWidget *parent = nullptr);

    void SetBgColor(const QColor &color);           //设置背景色
    void SetHandleNormalColor(const QColor &color); //设置Handle正常色
    void SetPressNormalColor(const QColor &color);  //设置Handle鼠标点击色

    void SetScale(qreal scale);

private:
    void SetCustomStyleSheet(qreal scale = 1.0);

private:
    QString m_BgColorHex;     //背景色
    QString m_NormalColorHex; //Handle正常色
    QString m_PressColorHex;  //Handle鼠标点击色
};

#endif // DIGIBASESCROLLBAR_H

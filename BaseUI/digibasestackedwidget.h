#ifndef DIGIBASESTACKEDWIDGET_H
#define DIGIBASESTACKEDWIDGET_H

/**
 * @brief 客户端公共库中的分割窗口 DigiBaseStackedWidget 基础类
 *
 * version 1.0
 * @date 2024-5-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include <QStackedWidget>
#include "Base/iwidgetbase.h"

class DigiBaseStackedWidget : public QStackedWidget, public IWidgetBase
{
    Q_OBJECT
public:
    explicit DigiBaseStackedWidget(QWidget *parent = nullptr);

    void SetBGColor(const QColor &color);     //设置背景颜色

    void SetScale(qreal scale) override;

};

#endif // DIGIBASESTACKEDWIDGET_H

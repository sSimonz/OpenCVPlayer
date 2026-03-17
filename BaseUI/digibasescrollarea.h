#ifndef DIGIBASESCROLLAREA_H
#define DIGIBASESCROLLAREA_H

/**
 * @brief 客户端公共库中的滚动区域 QScrollArea 基础类
 * 继承于 DigiBaseLineEdit 类, 可点击按钮打开文件管理器
 *
 * version 1.0
 * @date 2024-5-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include <QScrollArea>
#include "Base/iwidgetbase.h"

class DigiBaseScrollArea : public QScrollArea, public IWidgetBase
{
    Q_OBJECT
public:
    explicit DigiBaseScrollArea(QWidget *parent = nullptr);

    void SetScale(qreal scale) override;

};

#endif // DIGIBASESCROLLAREA_H

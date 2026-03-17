#ifndef DIGIBASESPLITTER_H
#define DIGIBASESPLITTER_H

/**
 * @brief 客户端公共库中的分割窗口 DigiBaseSplitter 基础类
 *
 * version 1.0
 * @date 2024-5-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include <QSplitter>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseSplitter : public QSplitter, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseSplitter(QWidget *parent = nullptr);

    void SetScale(qreal scale) override;

private:
    void SetCustomStyleSheet(qreal scale) override;

protected:
    qreal m_Scale;    //缩放比例
};

#endif // DIGIBASESPLITTER_H

#ifndef DIGIBASETABWIDGET_H
#define DIGIBASETABWIDGET_H

/**
 * @brief 公共库中的 多标签空间 DigiBaseTabWidget 基础类
 * 继承于 DigiBaseLineEdit 类
 *
 * version 1.0
 * @date 2024-5-16
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include <QTabWidget>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseTabWidget : public QTabWidget, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseTabWidget(QWidget *parent = nullptr);
    void SetMinimumTabWidth(qreal width);
    void SetSelectColor(const QColor &color = "#00000000");

    void SetScale(qreal scale) override;

private:
    void SetCustomStyleSheet(qreal scale) override;

private:
    QFont m_Font;
    qreal m_minTabWidth;
    QString m_selectColor;
};

#endif // DIGIBASETABWIDGET_H

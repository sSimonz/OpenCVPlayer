#ifndef DIGIBASETEXTEDIT_H
#define DIGIBASETEXTEDIT_H

/**
  * @brief 客户端公共库中的基础QTextEdit封装
  * 1. 可进行缩放，
  * 2. 可设置字体大小.
  * 3. 可设置边框圆角大小和边框border大小
  *
  * version 1.0
  * @date 2024-4-19
  *
  * Copyright (c) 2007-2022, Co.,Ltd.
  * All Rights Reserved
  * Histrory:
  * Revision
  */

#include <QTextEdit>
#include "Base/iwidgetbase.h"

class DigiBaseTextEdit : public QTextEdit, public IWidgetBase
{
    Q_OBJECT
private:
    enum
    {
        textNormal = 0,
        BackGround,
        WidgetBorder
    };
    enum
    {
        Radius = 0,
        Padding,
        Border
    };
public:
    DigiBaseTextEdit(QWidget *parent = nullptr);

    void SetScale(qreal scale) override;

    /**
     * @brief SetStyleSize 样式数值设置
     * @param radius  圆角半径
     * @param padding 内边距
     * @param border 描边大小
     */
    void SetStyleSize(qreal radius, qreal padding, qreal border = 1);

    void SetTextSize(int textSize);           //设置文本大小
    QFont GetCustomFont();                    //获取字体

private:
    void initStyleSheet();
    virtual void SetCustomStyleSheet(qreal scale);

private:
    QFont m_Font;
    QStringList m_StyleColorList;
    QList<qreal> m_StyleSizeList;
};

#endif // DIGIBASETEXTEDIT_H

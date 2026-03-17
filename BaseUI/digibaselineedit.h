#ifndef DIGIBASELINEEDIT_H
#define DIGIBASELINEEDIT_H

/**
 * @brief 客户端公共库中的文本编辑框LineEdit封装类
 * 1. 可进行缩放，基础大小默认：200*35，宽度只设定最小值
 * 2. 可设置样式，包括背景颜色、描边颜色、描边宽度、圆角半径
 * 3. 可设置文本边距，默认基值 QMargins(10,5,10,5)
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiByteLengthValidator;
class DigiBaseLineEdit : public QLineEdit,public IStyleSheetBase,public IWidgetBase
{
    Q_OBJECT

private:
    enum
    {
        lNormal = 0,
        lHover,
        lBackGround,
        lTextColor
    };
    enum
    {
        BorderWidth = 0,
        Radius
    };
public:
    explicit DigiBaseLineEdit(QWidget *parent = nullptr);

    virtual void SetScale(qreal scale);

    /**
     * @brief SetStyleTextColor 颜色设置
     * @param nmClr 文本颜色
     */
    void SetStyleTextColor(const QColor &nmClr);

    /**
     * @brief SetStyleColor 颜色设置
     * @param nmClr normal描边颜色
     * @param hvClr hover描边颜色
     * @param bgClr 背景颜色
     */
    void SetStyleColor(const QColor &nmClr,const QColor &hvClr,const QColor &bgClr);

    /**
     * @brief SetStyleSize 样式数值设置
     * @param borderWidth 描边宽度
     * @param radius 圆角半径
     */
    void SetStyleSize(qreal borderWidth,qreal radius);

    /**
     * @brief SetCustomTextMargins 设置文本边距基值
     * @param 左上右下边距
     */
    void SetCustomTextMargins(int nleft, int ntop, int nright, int nbottom);
    void SetCustomTextMargins(const QMargins& nMargins);

    void SetCustomFont(const QFont &font);   //设置字体
    QFont GetCustomFont();                   //获取字体

    void SetMaxByteLength(unsigned int max); //最大字节长度
    void SetCharLengthLimit(bool blimit);//设置字符长度限制
signals:

public slots:

protected:
    virtual QSize sizeHint() const;

private:
    void initStyleSheet();
    virtual void SetCustomStyleSheet(qreal scale);

protected:
    QMargins m_TextMargins;

private:
    QFont m_Font;

    QStringList m_StyleColorList;
    QList<qreal> m_StyleSizeList;
    DigiByteLengthValidator *m_validator;
};

#endif // DIGIBASELINEEDIT_H

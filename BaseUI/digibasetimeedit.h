#ifndef DIGIBASETIMEEDIT_H
#define DIGIBASETIMEEDIT_H

/**
 * @brief 客户端公共库中的时间输入框 TimeEdit 封装类
 * 1. 可进行缩放，基础大小默认固定值：110*25
 * 2. 可设置样式，包括背景颜色、描边颜色、描边宽度、圆角半径、按钮图片
 * 3. 可设置文本边距，默认基值 QMargins(5,0,5,0)
 * 4. 时间显示格式默认 HH:mm
 *
 * version 1.0
 * @date 2022-3-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QTimeEdit>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseTimeEdit : public QTimeEdit, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseTimeEdit(QWidget *parent = nullptr);

    void SetTextColor(const QColor &color);      //设置文本颜色
    void SetTextMargins(const QMargins &margin); //设置文本边距
    void SetBorderRadius(qreal radius);          //设置描边圆角半径
    void SetBorderWidth(qreal width);            //设置描边宽度
    void SetBoederColor(const QColor &color);    //设置描边颜色
    void SetBGColor(const QColor &color);        //设置输入框背景颜色
    void LoadBtnImg(const QString &upPath, const QString &downPath); //设置右侧按钮图标
    void SetCustomFont(const QFont &font);   //设置字体
    QFont GetCustomFont();                   //获取字体

    virtual void SetScale(qreal scale);

private:
    virtual void SetCustomStyleSheet(qreal scale = 1.0);

private:
    QFont m_Font;
    QMargins m_TextMargins;    //文本边距

    qreal m_Radius;            //描边圆角半径
    qreal m_BorderWidth;       //描边宽度
    QString m_TextColorHex;    //文本颜色
    QString m_BorderColorHex;  //描边颜色
    QString m_BGColorHex;      //输入框背景颜色

    QString m_UpBtnImgPath;    //增 按钮图片
    QString m_DownBtnImgPath;  //减 按钮图片
};

#endif // DIGIBASETIMEEDIT_H

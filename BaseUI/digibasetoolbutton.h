#ifndef DIGIBASETOOLBUTTON_H
#define DIGIBASETOOLBUTTON_H

/**
 * @brief 客户端公共库中的工具按钮 ToolButton 封装类
 * 1. 主要用于文本和图标都需显示的按钮、有按压效果的无背景色图片按钮
 * 2. 通过SetCustomIcon()加载图标，通过setToolButtonStyle()设置图标和文本相对位置
 * 3. 可进行缩放，基础大小默认固定值：40*30, 图标默认固定值：16*16
 * 4. 可设置样式，包括4中按钮状态的背景颜色、文本颜色、描边宽度、圆角半径、Icon图片、字体、内外边距
 *
 * version 1.0
 * @date 2022-4-02
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QToolButton>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"
#include "Base/iiconbase.h"

class DigiBaseToolButton : public QToolButton, public IWidgetBase, public IStyleSheetBase, public IiconBase
{
    Q_OBJECT

public:
    explicit DigiBaseToolButton(QWidget *parent = nullptr);
    ~DigiBaseToolButton();

    virtual void SetCustomIcon(const QString &path);//设置图标
    void SetCustomFont(const QFont &font);         //设置字体
    void SetCustomIconSize(const QSize &size);     //设置图标大小基值
    void SetTextColor(const QColor &normal,        //设置4种状态下的文本颜色
                      QColor hover = QColor(),         //若不传hover颜色，则与normal颜色一致
                      QColor pressed = QColor(),       //若不传pressed颜色，则与hover颜色一致
                      QColor disable = QColor());      //若不传disable颜色，则normal颜色一致
    void SetBgColor(const QColor &normal,          //设置4种状态下的背景颜色，后三种颜色传参机制同上
                    QColor hover = QColor(),
                    QColor pressed = QColor(),
                    QColor disable = QColor());
    void SetBorderWidth(qreal borderWidth);        //设置描边宽度
    void SetBorderColor(const QColor &color);      //设置描边颜色
    void SetRadius(qreal radius);                  //设置圆角半径
    void SetPadding(qreal normal, qreal pressed);  //设置正常、点击时内边距
    void SetMargin(qreal normal, qreal pressed);   //设置正常、点击时外边距

    QFont GetCustomFont();                         //获取字体
    void AdjustNormalStyle();                      //样式调整和文本按钮一致(参考DigiBaseIconPushBtn)

    virtual void SetScale(qreal scale);

private:
    void InitStyle();
    virtual void SetCustomStyleSheet(qreal scale = 1.0);

private:
    QFont m_Font;
    QSize m_IconSize;
    QStringList m_TextColorList;
    QStringList m_BgColorList;
    QString m_BorderColor;
    qreal m_Border;
    qreal m_Radius;
    QList<qreal> m_PaddingList;
    QList<qreal> m_MarginList;

    QPixmap *m_PngIcon;
};

#endif // DIGIBASETOOLBUTTON_H

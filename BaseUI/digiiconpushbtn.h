#ifndef DIGIICONPUSHBTN_H
#define DIGIICONPUSHBTN_H

/**
 * @brief 公共库中的可设置 图标/文本 和背景色的按钮 IconPushBtn 封装类
 * 1. 可进行缩放，基础大小默认固定值：40*30
 * 2. 按用途分为文本按钮，图标按钮
 * 3. 根据当前是否载入了图片，来判断是文本按钮还是图标按钮
 * 4. 文本按钮，可调用AdjustWidthByText()，让宽度基值自适应当前文本长度
 * 5. 图标按钮，载入按钮图标(svg、png)后，基值会自动设置为30*30
 * 6. 该按钮分正常，悬停和选中，禁用三种状态
 * 7. 可设置按钮在不同状态下的背景颜色、按钮文本颜色、圆角半径等属性
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QWidget>
#include <QPushButton>
#include <QSvgRenderer>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"
#include "Base/iiconbase.h"

class DigiIconPushBtn : public QPushButton, public IWidgetBase,public IStyleSheetBase,public IiconBase/*,public IimageBase*/
{
    Q_OBJECT

protected:
    enum
    {
        textNormal = 0,
        textDisable,
        textCheck,
        btnNormal,
        btnHover,
        btnCheck,
        btnDisabled,
        btnBorder
    };
    enum
    {
        Radius = 0,
        Padding,
        Margin,
        Border,
        RadiusLeftTop,
        RadiusRightTop,
        RadiusRightBtm,
        RadiusLeftBtm,
        BorderTop,
        BorderLeft,
        BorderRight,
        BorderBottom
    };
public:
    explicit DigiIconPushBtn(QWidget *parent = nullptr);
    explicit DigiIconPushBtn(const QString &text, QWidget *parent = nullptr);

    virtual void SetCustomIcon(const QString &iconpath);
    virtual void SetCustomIcon(const QString &iconpath, const QSize &size);
    virtual void SetScale(qreal scale);
    virtual void SetHeight(qreal height);
    void SetCustomIconSize(const QSize &size);     //设置图标大小基值

    /**
     * @brief SetStyleTextColor 按钮文字颜色设置
     * @param nmClr  normal颜色
     * @param disClr disable颜色
     * @param chkClr checked颜色
     */
    void SetStyleTextColor(const QColor &nmClr = ColorManager::GetColor(ColorManager::CN_ICONBTN_TEXT),
                           const QColor &disClr = ColorManager::GetColor(ColorManager::CN_ICONBTN_TEXT_DISABLE),
                           const QColor &chkClr = ColorManager::GetColor(ColorManager::CN_ICONBTN_TEXT_CHECK));

    /**
     * @brief SetStyleBtnColor 按钮背景颜色设置
     * @param nmClr normal颜色
     * @param hvClr hover颜色
     * @param disClr disable颜色
     * @param chkClr checked颜色
     */
    void SetStyleBtnColor(const QColor &nmClr = ColorManager::GetColor(ColorManager::CN_ICONBTN_BACKGROUND_NORMAL),
                          const QColor &hvClr = ColorManager::GetColor(ColorManager::CN_ICONBTN_BACKGROUND_HOVER),
                          const QColor &disClr = ColorManager::GetColor(ColorManager::CN_ICONBTN_BACKGROUND_DISABLE),
                          const QColor &chkClr = ColorManager::GetColor(ColorManager::CN_ICONBTN_BACKGROUND_CHECK));

    void SetStyleBtnAllColor(const QColor &color);// 设置所有按钮背景颜色
    void SetStyleBorderColor(const QColor &color);// 设置描边颜色

    /**
     * @brief SetStyleSize 样式数值设置
     * @param radius  圆角半径
     * @param padding 内边距
     * @param margin 按下按钮时，背景内缩的大小
     * @param border 描边大小
     */
    void SetStyleSize(qreal radius, qreal padding, qreal margin = 1, qreal border = 0);

    void SetStyleRadius(qreal left_top, qreal right_top, qreal right_btm, qreal left_btm);
    void SetStyleBorder(qreal top, qreal left, qreal right, qreal bottom);


    void AdjustWidthByText(int extraMargin = 2);  //根据当前文本设定适合的宽度,extraMargin为每边额外的边距,默认为2
    void SetCustomFont(const QFont &font);        //设置字体
    QFont GetCustomFont();                        //获取字体
    void UpdateCustomStyleSheet();
signals:

public slots:

private:
    void initStyleSheet();
    virtual void SetCustomStyleSheet(qreal scale);

private:
    QPixmap *m_BackImg;
    QSvgRenderer* m_Renderer;
    QRectF m_curViewBox;

    QSize m_IconSize;

    QString m_iconpath;

    QFont m_Font;

    QStringList m_StyleColorList;
protected:
    QList<qreal> m_StyleSizeList;
    qreal m_scale;

};

#endif // DIGIICONPUSHBTN_H

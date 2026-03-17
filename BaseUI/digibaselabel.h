#ifndef DIGIBASELABEL_H
#define DIGIBASELABEL_H

/**
  * @brief 客户端公共库中的基础Label封装
  * 1. 可进行缩放，基础大小默认高度：40，宽度基值为0，不参与缩放。
  * 2. Label用途分为文本Label、图片Label、纯色块Label
  * 3. 载入了图片时，宽高基值会设置为图片的宽高
  * 4. 可设置样式，包括文本的颜色、大小、是否粗体、背景颜色
  * 5. 可设置文本超长时是否显示省略号
  *
  * version 1.0
  * @date 2022-3-14
  *
  * Copyright (c) 2007-2022, Co.,Ltd.
  * All Rights Reserved
  * Histrory:
  * Revision
  */

#include <QLabel>
#include "Base/istylesheetbase.h"
#include "Base/iwidgetbase.h"

class QSvgRenderer;

class DigiBaseLabel : public QLabel,public IStyleSheetBase,public IWidgetBase
{
    Q_OBJECT
private:
    enum
    {
        BorderRadius = 0,
        Padding
    };

public:
    explicit DigiBaseLabel(QWidget *parent = nullptr);
    explicit DigiBaseLabel(const QString &text, QWidget *parent=nullptr);
    ~DigiBaseLabel();


    virtual void SetScale(qreal scale = 1.0);

    /**
     * @brief LoadImagePath 加载图片，图片Label
     * @param path  图片文件路径
     * @param viewBox 图片显示区域，默认显示全图
     */
    void LoadImagePath(const QString &path, const QRect &viewBox = QRect(0,0,0,0));
    QString GetCurrentImagePath();     //获取当前所显示图片的路径

    /**
     * @brief SetElidedText 设置文本超长时是否显示省略号，暂不支持html格式文本
     * @param on  true->显示省略号，false->不显示省略号
     * @param mode  省略号显示的位置，默认传参是右边
     * @param maxWidth 传0时，按照当前的实际宽度来计算显示省略号，否则按传入值缩放后的值来计算
     */
    void SetElidedText(bool on, Qt::TextElideMode mode = Qt::ElideRight, qreal maxWidth = 0);

    void setText(const QString &text);   //设置文本内容，隐藏原函数
    QString text() const;                //获取文本内容，隐藏原函数

    void SetBGColor(const QColor &color);     //设置背景颜色
    void SetTextColor(const QColor &textClr); //设置文本颜色
    void SetTextSize(int textSize);           //设置文本大小
    void SetTextBold(bool bBold);             //设置是否粗体
    void SetTextFamily(const QString &family);//设置字体族
    void SetCustomFont(const QFont &font);    //设置字体
    void SetTextWeight(int weight);           //设置字体粗细
    QFont GetCustomFont();                    //获取字体

    void SetBorderRadius(qreal radius);       //设置描边圆角半径
    void SetPadding(qreal padding);            //设置填充大小

    void UpdateTextColor();                   //立即更新文本颜色
    void UpdateTextFont();                    //立即更新文本字体

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    virtual void SetCustomStyleSheet(qreal scale);
    void LongTextProcessing();  //文本内容超过长度，显示省略号

private:
    QFont m_Font;
    QColor m_BgColor;
    QColor m_TextColor;
    QColor m_HoverColor;
    QList<qreal> m_StyleSizeList;

    QString m_CurImgPath;
    QSvgRenderer *m_SvgImg;
    QPixmap *m_PixImg;

    QString m_text;
    QPair<qreal,qreal> m_MaxWidth;  //设置ElidedText时的最大值，仅当m_width为0时启用，QPair<当前值，基值>
    bool m_bElidedText;             //文本超长显示...标志位
    Qt::TextElideMode m_ElideMode;  //省略号显示模式
protected:
    qreal m_scale;
};

#endif // DIGIBASELABEL_H

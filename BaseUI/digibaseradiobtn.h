#ifndef DIGIBASERADIOBTN_H
#define DIGIBASERADIOBTN_H

/**
 * @brief 客户端公共库中的基础RadioButton封装
 * 1. 可进行缩放，高度默认固定值：20
 * 2. 默认只对高度m_height进行缩放，宽度根据QRadioButton自身默认的SizePolicy，自适应布局
 * 3. 默认m_width=0，若不为0，则宽度也会进行缩放(一般在无文本情况下，需设定m_width的值)
 * 4. 可以设置文本的颜色、粗细体、indicator图标及图标大小
 * 5. 设置indicator图标大小时，若参数超过高度m_height，则m_height会设置成参数的大小
 *
 * version 1.0
 * @date 2022-3-21
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QRadioButton>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"
class DigiBaseRadioBtn : public QRadioButton, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseRadioBtn(QWidget *parent = nullptr);

    /**
     * @brief LoadImgPath 替换默认的图片
     * @param unsel 未选中时的图片路径
     * @param sel   选中时的图片路径
     * @param indicatorSize  图片大小
     */
    void LoadImgPath(const QString &unsel,const QString &sel,int indicatorSize = 16);

    void SetTextColor(const QColor &color); //文本颜色
    void SetIndicatorSize(qreal size);      //图标大小
    void SetCustomFont(const QFont &font);  //设置字体
    QFont GetCustomFont();                  //获取字体

    virtual void SetScale(qreal scale);

signals:

public slots:

private:
    virtual void SetCustomStyleSheet(qreal scale = 1.0);
private:
    QFont m_Font;
    QString m_TextColorHex;      //文本颜色 16进制字符串
    QString m_UnselectedImgPath; //未选中时的图片
    QString m_SelectedImgPath;   //选中时的图片
    qreal m_IndicatorSize;         //图片大小
};

#endif // DIGIBASERADIOBTN_H

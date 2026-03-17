#ifndef DIGIBASECHECKBOX_H
#define DIGIBASECHECKBOX_H

#include <QWidget>

/**
 * @brief 公共库中的基础CheckBox封装
 * 1. 可进行缩放，高度默认固定值：20
 * 2. 默认只对高度m_height进行缩放，宽度根据QCheckBox自身默认的SizePolicy，自适应布局
 * 3. 默认m_width=0，若不为0，则宽度也会进行缩放(一般在无文本情况下，需设定m_width的值)
 * 4. 可以设置文本的颜色、粗细体
 *
 * version 1.0
 * @date 2022-3-18
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QCheckBox>
#include "Base/istylesheetbase.h"
#include "Base/iwidgetbase.h"

class DigiBaseCheckBox : public QCheckBox, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseCheckBox(QWidget *parent = nullptr);
    explicit DigiBaseCheckBox(const QString &text, QWidget *parent = nullptr);

    /**
     * @param 1、正常 2、正常全选 3、正常半选 4、禁用 5、禁用全选 6、禁用半选
     */
    void SetIconColor(const QStringList &list); //设置复选框图片
    void SetTextColor(const QColor &color); //文本颜色
    void SetCustomFont(const QFont &font);  //设置字体
    QFont GetCustomFont();                  //获取字体

    virtual void SetScale(qreal scale);

private:
    virtual void SetCustomStyleSheet(qreal scale = 1.0);

private:
    QFont m_Font;
    QString m_TextColorHex;  //文本颜色 16进制字符串
    qreal m_IndicatorSize;   //勾选框图标大小
    QStringList m_iconList;
};

#endif // DIGIBASECHECKBOX_H

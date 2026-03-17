#ifndef DIGIBASESLIDER_H
#define DIGIBASESLIDER_H

/**
 * @brief 客户端公共库中的滑块条 Slider 封装类
 * 1. 可进行缩放。横滑块默认高度固定22，宽度不固定。竖滑块相反。
 * 2. setOrientation()函数，会将 m_height 和 m_width 的值交换
 * 3. 以横滑块为例说明：宽度不固定，m_width默认设置为0，不参与缩放，自适应布局。若设置了大于0的固定值，则参与缩放。
 * 4. 可设置样式，包括滑块条颜色、滑块条的宽度、滑块图标及大小
 *
 * version 1.0
 * @date 2022-3-22
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QSlider>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseLabel;
class DigiBaseSlider : public QSlider, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseSlider(QWidget *parent = nullptr);

    void setOrientation(Qt::Orientation orientation);   //设置横还是竖，此函数会交换m_height和m_width的值
    void SetMinSideColor(const QColor &color);          //设置滑块左侧/下侧的颜色
    void SetMaxSideColor(const QColor &color);          //设置滑块右侧/上侧的颜色
    void SetGrooveSize(qreal size);                     //设置滑块条宽度
    void LoadHandleImgPath(const QString &path, qreal imgSize = 15); //设置滑块的图标、大小
    void setDisableWheel(bool b);
    void setValueVisible(bool b, const QString &unit = ""); //设置数值标签是否可见, 数值单位

    virtual void SetScale(qreal scale);

signals:
    void sliderClickSig(int value);  //鼠标松开滑块后发送该信号，参数为当前的值

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);
    void resizeEvent(QResizeEvent *event);

private:
    virtual void SetCustomStyleSheet(qreal scale = 1.0);
    void MoveValuePos(int value);

private:
    QString m_GrooveBorderColorHex;
    QString m_MinSideColorHex;   //滑块左侧/下侧的颜色
    QString m_MaxSideColorHex;   //滑块右侧/上侧的颜色
    QString m_HandleImgPath;     //滑块的图标
    qreal m_ImgSize;             //图标大小
    qreal m_GrooveSize;          //滑块条宽度
    bool m_bMoveFlag;
    bool m_disableWheel;
    DigiBaseLabel *m_LabValue;   //显示当前值
    DigiBaseLabel *m_LabMinValue;//显示最小值
    DigiBaseLabel *m_LabMaxValue;//显示最大值
};

#endif // DIGIBASESLIDER_H

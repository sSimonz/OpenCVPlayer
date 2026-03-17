#ifndef DIGIBASEPROGRESSBAR_H
#define DIGIBASEPROGRESSBAR_H

/**
  * @brief DigiBaseProgressBar 进度条基础类
  *
  * 1. 可进行缩放，基础宽度默认：10，宽度只设定最小值
  * 2. 可设置样式，包括背景颜色、描边颜色、描边宽度、圆角半径
  * 3. 可设置进度条方向，横/竖
  * version 1.0
  * @date 2024-2-22
  */

#include <QProgressBar>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseProgressBar : public QProgressBar, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseProgressBar(QWidget *parent = nullptr);

    void setOrientation(Qt::Orientation orientation);   //设置横还是竖，此函数会交换m_height和m_width的值
    void SetBorderRadius(int radius);                                //设置边框圆角
    void SetBorder(int width, const QColor &borderColor);            //设置边框宽度及颜色
    void SetColor(const QColor &bgColor, const QColor &occupyColor); //设置进度条颜色
    void SetTextColor(const QColor &color);                          //设置字体颜色
    void SetFont(const QFont &font);                                 //设置字体
    void SetScale(qreal scale) override;

private:
    void SetCustomStyleSheet(qreal scale) override;

private:
    int m_BorderRadius;        //边框圆角
    int m_BorderWidth;         //边框宽度
    QString m_BorderColorHex;  //边框颜色

    QString m_BGColorHex;      //背景颜色
    QString m_OccupyColorHex;  //占用颜色
    QString m_TextColorHex;    //字体颜色
    QFont m_Font;              //字体
};

#endif // DIGIBASEPROGRESSBAR_H

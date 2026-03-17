#ifndef DIGIRINGPROGRESSBARWIDGET_H
#define DIGIRINGPROGRESSBARWIDGET_H

/**
 * 百分比仪表盘控件
 * 1:可设置范围值,支持负数值
 * 2:可设置精确度,最大支持小数点后3位
 * 3:可设置圆弧宽度
 * 4:可设置开始旋转角度/结束旋转角度
 * 5:可设置背景颜色/进度颜色/值颜色/文字颜色
 * 6:自适应窗体拉伸,文字自动缩放
 * 7:可设置百分比模式,自动计算值换算成百分比
 */

#include <QWidget>
#include <QPainter>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"
class DigiRingProgressBarWidget : public QWidget, public IWidgetBase, public IStyleSheetBase

{
    Q_OBJECT
public:
    explicit DigiRingProgressBarWidget(QWidget *parent = 0);
    ~DigiRingProgressBarWidget();

    virtual void SetScale(qreal scale = 1.0);

    void SetTextSize(int textSize);           //设置百分比数值文本大小
    void SetTextBold(bool bBold);             //设置百分比数值是否粗体
    void SetTextFamily(const QString &family);//设置百分比数值字体族
    void SetCustomFont(const QFont &font);    //设置百分比数值字体
    QFont GetCustomFont();                    //获取百分比数值字体
    void SetTextWeight(int weight);           //设置百分比数值字体粗细


    void SetTitleTextSize(int textSize);//设置标题文本大小
    void SetTitleTextBold(bool bBold);//设置标题是否粗体
    void SetTitleCustomFont(const QFont &font);    //设置标题字体
    QFont GetTitleCustomFont();                    //获取标题字体
    void SetTitleTextWeight(int weight);           //设置标题字体粗细
    //设置范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    //设置最大最小值
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);

    //设置目标值
    void setValue(double value);
    void setValue(int value);

    //设置精确度
    void setPrecision(int precision);

    //设置开始旋转角度
    void setStartAngle(int startAngle);
    //设置结束旋转角度
    void setEndAngle(int endAngle);

    //设置圆弧颜色
    void setArcColor(const QColor &arcColor);
    //设置文本颜色
    void setTextColor(const QColor &textColor);
    //设置基准颜色
    void setBaseColor(const QColor &baseColor);
    //设置背景颜色
    void setBgColor(const QColor &bgColor);
    //设置标题颜色
    void setTitleColor(const QColor &titleColor);

    //设置百分比模式
    void setPercent(bool percent);
    //设置圆弧宽度
    void setArcWidth(int arcWidth);
    //设置标题
    void setTitle(const QString &title);


protected:
    void paintEvent(QPaintEvent *);
    void drawArc(QPainter *painter);
    void drawValue(QPainter *painter);
private:
    virtual void SetCustomStyleSheet(qreal scale);
private:
    double m_minValue;                //最小值
    double m_maxValue;                //最大值
    double m_value;                   //目标值
    int m_precision;                  //精确度,小数点后几位

    int m_startAngle;                 //开始旋转角度
    int m_endAngle;                   //结束旋转角度

    QColor m_arcColor;                //圆弧颜色
    QColor m_textColor;               //文字颜色
    QColor m_baseColor;               //基准颜色
    QColor m_bgColor;                 //背景颜色
    QColor m_titleColor;              //标题颜色

    bool m_percent;                   //百分比模式
    int m_arcWidth;                   //圆弧宽度
    QString m_title;                  //标题
    QFont m_Font;//百分比数值文本字体
    QFont m_TitleFont;//标题文本字体
};

#endif // DIGIRINGPROGRESSBARWIDGET_H

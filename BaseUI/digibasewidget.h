#ifndef DIGIBASEWIDGET_H
#define DIGIBASEWIDGET_H

#include <QWidget>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseWidget : public QWidget, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

private:
    enum
    {
        BorderWidth = 0,
        Radius
    };
    enum
    {
        BorderColor = 0,
        BackGroundColor,
    };

public:
    explicit DigiBaseWidget(QWidget *parent = nullptr);

    void SetBGColor(const QColor &color);         //设置背景颜色
    void SetBorderRadius(int radius);             //设置描边圆角半径
    void SetBorderWidth(int border);              //设置描边宽度
    void SetBorderColor(const QColor &color);     //设置描边颜色
    void SetLayoutMargins(const QMargins &margin);         //设置布局边距基值
    void SetLayoutSpacing(int horSpacing, int verSpacing); //设置布局间距基值，如果是BoxLayout，则只使用第一个参数

    void SetScale(qreal scale) override;
    void UpdateCustomStyleSheet();
protected:
    void SetCustomStyleSheet(qreal scale) override;

private:
    bool m_bFirstShow;
    QList<int> m_SizeList;
    QStringList m_ColorList;
    QMap<QLayout*, LayoutInfo> m_layoutMap;
protected:
    qreal m_scale;
};

#endif // DIGIBASEWIDGET_H

#ifndef DIGIBASEGROUPBOX_H
#define DIGIBASEGROUPBOX_H

#include <QGroupBox>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseGroupBox : public QGroupBox, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseGroupBox(QWidget *parent = nullptr);
    explicit DigiBaseGroupBox(const QString &title, QWidget *parent = nullptr);

    void SetLayoutMargins(const QMargins &margin);        //设置布局边距基值
    void SetLayoutSpacing(int horSpacing, int verSpacing);//设置布局间距基值，如果是BoxLayout，则只使用第一个参数
    void SetTextColor(const QColor &color);   //设置标题文本颜色
    void SetBorderColor(const QColor &color); //设置边框颜色
    void SetRadius(qreal radius);             //设置边框圆角
    void SetTopMargins(qreal topMargin);      //设置标题和内容的距离
    void SetBorderWidth(qreal border);        //设置边框宽度
    void SetIndicatorSize(qreal size);        //设置标题勾选框大小
    void SetCustomFont(const QFont &font);    //设置字体
    QFont GetCustomFont();                    //获取字体

    virtual void SetScale(qreal scale);

private:
    virtual void SetCustomStyleSheet(qreal scale);

private:
    bool m_bFirstShow;
    QFont m_Font;
    QString m_TextColorHex;    //标题文本颜色
    QString m_BorderColorHex;  //边框颜色
    qreal m_Radius;            //边框圆角
    qreal m_TopMargins;        //标题和内容的距离
    qreal m_BorderWidth;       //边框宽度
    qreal m_IndicatorSize;     //标题勾选框图标大小
    QMap<QLayout*, LayoutInfo> m_layoutMap;
};

#endif // DIGIBASEGROUPBOX_H

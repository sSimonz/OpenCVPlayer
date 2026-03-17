#ifndef DIGIBASEFRAME_H
#define DIGIBASEFRAME_H

#include <QFrame>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseFrame : public QFrame, public IWidgetBase, public IStyleSheetBase
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
    explicit DigiBaseFrame(QWidget *parent = nullptr);

    void SetBGColor(const QColor &color);         //设置背景颜色
    void SetBorderRadius(int radius);             //设置描边圆角半径
    void SetBorderWidth(int border);              //设置描边宽度
    void SetBorderColor(const QColor &color);     //设置描边颜色
    void SetLayoutMargins(const QMargins &margin);         //设置布局边距基值
    void SetLayoutSpacing(int horSpacing, int verSpacing); //设置布局间距基值，如果是BoxLayout，则只使用第一个参数
    void SetPopupFlags(bool b);//设置弹出菜单样式

    virtual void SetScale(qreal scale);
    void UpdateCustomStyleSheet();
protected:
    virtual void SetCustomStyleSheet(qreal scale);

private:
    bool m_bFirstShow;
    QList<int> m_SizeList;
    QStringList m_ColorList;
    QMap<QLayout*, LayoutInfo> m_layoutMap;

protected:
    qreal m_FrameScale;        //缩放比例
};

#endif // DIGIBASEFRAME_H

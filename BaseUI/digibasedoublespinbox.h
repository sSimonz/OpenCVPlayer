#ifndef DIGIBASEDOUBLESPINBOX_H
#define DIGIBASEDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseDoubleSpinBox : public QDoubleSpinBox, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseDoubleSpinBox(QWidget *parent = nullptr);
    void SetTextColor(const QColor &color);               //设置文本颜色
    void SetTextMargins(const QMargins &margin);          //设置文本边距
    void SetBorderRadius(qreal radius);                   //设置描边圆角半径
    void SetBorderWidth(qreal border);                    //设置描边宽度
    void SetBoederColor(const QColor &color);             //设置描边颜色
    void SetBGColor(const QColor &color);                 //设置输入框背景颜色
    void SetBtnVisible(bool visible);                     //设置右侧按钮显隐
    void LoadBtnImg(const QString &upPath, const QString &downPath);//设置右侧按钮图标
    void SetCustomFont(const QFont &font);   //设置字体
    QFont GetCustomFont();                   //获取字体

    virtual void SetScale(qreal scale);

private:
    virtual void SetCustomStyleSheet(qreal scale = 1.0);

private:
    QFont m_Font;
    QMargins m_TextMargins;       //文本边距
    qreal m_Radius;               //描边圆角半径
    qreal m_BorderWidth;          //描边宽度
//    qreal m_TopLeftBorder;        //左、上描边宽度
//    qreal m_BottomRightBorder;    //右、下描边宽度
    QString m_TextColorHex;       //文本颜色
    QString m_BGColorHex;         //输入框背景颜色
    QString m_BorderColorHex;     //描边颜色
//    QString m_TopLeftColorHex;    //左、上描边颜色
//    QString m_BottomRightColorHex;//右、下描边颜色
    QString m_UpBtnImgPath;       //增 按钮图片
    QString m_DownBtnImgPath;     //减 按钮图片

    bool m_bBtnVisible;           //按钮显隐标志
};

#endif // DIGIBASEDOUBLESPINBOX_H

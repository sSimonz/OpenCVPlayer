#ifndef DIGIBASECOMBOBOX_H
#define DIGIBASECOMBOBOX_H

/**
  * @brief 公共库中的基础ComboBox封装
  * 1. 可进行缩放，基础大小默认：200*35，宽度只设定最小值
  * 2. ComboBox由 普通/LineEdit文本框 和 下拉框ListView两部分组成
  * 3. 默认设置Editable(true)，LineEdit默认只读不可编辑，可以设置成可编辑
  * 4. 可以设置文本框的样式，包括颜色、描边、右侧下拉框按钮大小
  * 5. 可以设置ListView的样式，包括颜色、描边、单个item高度
  * 6. 可以给LineEdit添加位于左侧或右侧的图标，普通文本框无法添加图标
  *
  * version 1.0
  * @date 2022-3-14
  *
  * Copyright (c) 2007-2022, Co.,Ltd.
  * All Rights Reserved
  * Histrory:
  * Revision
  */

#include <QComboBox>
#include <QLineEdit>
#include "Base/istylesheetbase.h"
#include "Base/iwidgetbase.h"

class DigiBaseLabel;

class DigiBaseComboBox : public QComboBox,public IWidgetBase,public IStyleSheetBase
{
    Q_OBJECT

private:
    enum
    {
        LineNormal = 0,
        LineHover,
        LineBackGround,
        LineTextColor,
        ViewNormal,
        ViewBackGround,
        ViewSelection,
        ViewSelectionBG
    };
    enum
    {
        EditBorderWidth = 0,
        EditRadius,
        EditiconWidth,
        ViewBorderWidth,
        ViewRadius,
        ViewItemHeight
    };

public:
    explicit DigiBaseComboBox(QWidget *parent = nullptr);

    void setTextOptional(bool on); //设置文本是否可选中（true -> edit模式，但不可输入）
    void setReadAndWrite();  // 将combox的lineEdit设置可输入

    /**
     * @brief SetStyleLineEditColor 文本框颜色设置
     * @param nmClr normal描边颜色
     * @param hvClr hover描边颜色
     * @param bgClr 背景颜色
     * @param textClr 文本颜色
     */
    void SetStyleLineEditColor(QColor nmClr,QColor hvClr,QColor bgClr,QColor textClr);

    /**
     * @brief SetStyleLineEditSize 文本框样式数值设置
     * @param borderWidth 描边宽度
     * @param radius 圆角半径
     * @param iconWidth 下拉按钮宽度
     */
    void SetStyleLineEditSize(qreal borderWidth, qreal radius, qreal iconWidth=16);

    /**
     * @brief SetStyleViewItemColor 下拉框颜色设置
     * @param nmClr 下拉框描边颜色
     * @param bgClr 下拉框背景颜色
     * @param selClr 下拉框选中item字体颜色
     * @param selBgClr 下拉框选中item背景颜色
     */
    void SetStyleViewItemColor(const QColor &nmClr, const QColor &bgClr,
                               const QColor &selClr,const QColor &selBgClr);

    /**
     * @brief SetStyleViewItemSize 下拉框样式数值设置
     * @param borderWidth 描边宽度
     * @param radius    圆角半径
     * @param itemHeight 单个item高度
     */
    void SetStyleViewItemSize(qreal borderWidth, qreal radius, qreal itemHeight);

    virtual void SetScale(qreal scale);

    /**
     * @brief GetItemHeight 获取ListView中单个item的高度基值
     * @return ListView中单个item的高度基值
     */
    qreal GetItemHeight() {return m_StyleSizeList[ViewItemHeight];}

    /**
     * @brief AddCustomAction LineEdit添加图标
     * @param path 图标文件路径
     * @param Name 占位符文本PlaceholderText
     * @param position 图标位置
     * @param iconSize 图标大小
     */
    void AddCustomAction(QString path,
                         QString Name = "",
                         QLineEdit::ActionPosition position = QLineEdit::LeadingPosition,
                         QSize iconSize = QSize(16,16));

    /**
     * @brief SetCustomTextMargins 设置LineEdit文本边距的基值
     * @param 左上右下边距
     */
    void SetCustomTextMargins(int nleft, int ntop, int nright, int nbottom);
    void SetCustomTextMargins(const QMargins& nMargins);

    void SetCustomFont(const QFont &font);   //设置字体
    QFont GetCustomFont();                   //获取字体

    void SetItemIconSize(const QSize &size); //addItem时传了QIcon的icon大小

signals:

public slots:


private:
    void initStyleSheet();
    virtual void SetCustomStyleSheet(qreal scale);
    void SetCursorToFirst();

protected:
    QMargins m_TextMargins;

private:
    bool m_bCursorToFirst;     //lineEdit()是否已关联SetCursorToFirst()函数
    QFont m_Font;
    DigiBaseLabel *m_IconLab;
    QStringList m_StyleColorList;
    QList<qreal> m_StyleSizeList;
    QSize m_IconSize;
};

#endif // DIGIBASECOMBOBOX_H

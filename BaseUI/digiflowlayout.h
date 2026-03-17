#ifndef DIGIFLOWLAYOUT_H
#define DIGIFLOWLAYOUT_H

/**
 * @brief 公共库中的流式布局 DigiFlowlayout 封装类
 * 1. 可进行缩放
 * 2. 组件按照加入的先后顺序按照设置的对齐方式从左向右排列，一行排满到下一行开始继续排列
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include <QLayout>
#include <QWidgetItem>
#include <QStyle>
#include <QWidget>

class DigiFlowlayout : public QLayout
{
    Q_OBJECT

public:
    DigiFlowlayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    DigiFlowlayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~DigiFlowlayout();

    void addItem(QLayoutItem *item);

    void keepWidgetSize(bool flag);
    int horizontalSpacing() const;
    int verticalSpacing() const;
    void setHorizontalSpacing(int hSpacing);
    void setVerticalSpacing(int vSpacing);
    Qt::Orientations expandingDirections() const;
    virtual bool hasHeightForWidth() const;
    int heightForWidth(int) const;
    int count() const;
    QLayoutItem *itemAt(int index) const;
    QSize minimumSize() const;
    void setGeometry(const QRect &rect);
    QSize sizeHint() const;
    QLayoutItem *takeAt(int index);

    virtual void SetScale(qreal scale); 

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;

    bool m_keepWsize;
    int m_hSpace;
    int m_vSpace;
    QMargins m_Margin;
    qreal m_Scale;
};

#endif // DIGIFLOWLAYOUT_H

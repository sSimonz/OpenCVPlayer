#ifndef DIGIBASETOOLBOX_H
#define DIGIBASETOOLBOX_H

 /**
 * @brief 公共库中的工具箱 DigiBaseToolBox 封装类
 * 1. 可进行缩放，基础宽度默认固定值：288
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QMap>
#include <QFrame>
#include <QVBoxLayout>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseLabel;
class DigiBaseToolBoxItem;

class DigiBaseToolBox : public QFrame, public IWidgetBase
{
    Q_OBJECT
public:
    explicit DigiBaseToolBox(QWidget *parent = nullptr);
    int AddItem(const QString &text, QWidget *widget);
    void ExpandItem(int index); //展开显示item

    void SetScale(qreal scale) override;

private:
    void InitWidget();

private:
    QMap<int, QPair<DigiBaseToolBoxItem*, QWidget*>> m_items;
    QVBoxLayout *m_Layout;
    int m_index;
};

/*************************
 *  DigiBaseToolBoxItem  *
 *************************/
class DigiBaseToolBoxItem : public QFrame, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseToolBoxItem(const QString &text, int index, QWidget *parent = nullptr);
    void LoadArrowImg(bool bOpen);// true:open ; false: close
    void setArrowVisible(bool bVisible);

    void SetScale(qreal scale) override;

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void itemClicked(int index);

private:
    void InitWidget();
    void SetCustomStyleSheet(qreal scale) override;

private:
    DigiBaseLabel *m_pLabName;
    DigiBaseLabel *m_pLabPic;
    QString m_pName;
    int m_index;
    QHBoxLayout *m_MainLayout;
    QMargins m_Margin;

    friend class DigiBaseToolBox;
};


#endif // DIGIBASETOOLBOX_H

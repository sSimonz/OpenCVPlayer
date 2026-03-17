#ifndef DIGILISTCOMBOBOX_H
#define DIGILISTCOMBOBOX_H

/**
 * @brief 客户端公共库中的ListComboBox封装
 * 1. 继承于 DigiBaseComboBox，可缩放，样式设置相同
 * 2. 有自定义的DigiListComboBoxItem类，该item右侧会有一个删除按钮
 * 3. 通过insertItem插入一个自定义的ListItem，item保存自己的下标、文本信息
 * 4. 文本框默认可编辑输入
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include "digibasecombobox.h"
#include "digibasewidget.h"

class QHBoxLayout;
class DigiBaseLabel;
class DigiSvgPushBtn;
class DigiBaseListWidget;
class DigiListComboBoxItem;

class DigiListComboBox : public DigiBaseComboBox
{
   Q_OBJECT
public:
   explicit DigiListComboBox(QWidget* parent = nullptr);

    /**
     * @brief setCurrentIndex 输入框当前显示的内容设置为item下标为index的内容
     * @param index 下标
     */
    void setCurrentIndex(int index);
    void hidePopup() override;
    void showPopup() override;

    /**
     * @brief insertItem  插入一个item
     * @param id     ID
     * @param text   文本内容
     * @param bShowDelBtn  是否显示删除图标
     */
    void insertItem(QString text, qint64 id, bool bShowDelBtn = true);

    /**
     * @brief removeItem  删除一个item
     * @param item   文本内容
     */
    void removeItem(QString item);

    /**
     * @brief getListWidgetCount
     * @return 当前ListWidget的item数量
     */
    int getListWidgetCount();

    /**
     * @brief itemTextAt 根据id获取item文本内容
     * @param iData  item的data，id
     * @return item的文本内容
     */
    QString itemTextAt(qint64 iData);

    void SetScale(qreal scale) override;
signals:
    void removeItemReq(qint64 data);  //信号-删除item的请求

protected:
    void keyPressEvent(QKeyEvent *event) override;  //回车添加内容
    void wheelEvent(QWheelEvent *event) override;   //滚轮事件

private slots:
    void SlotShowItem(qint64 data);
    void SlotRemoveItem(qint64 data);
private:
    qreal m_Scale;
    QStringList m_ItemText;
    DigiBaseListWidget* m_pListWidget;
};


/************************
 * DigiListComboBoxItem *
 ************************/
class DigiListComboBoxItem : public DigiBaseWidget
{
    Q_OBJECT
public:
    explicit DigiListComboBoxItem(qint64 id = -1, QString item = "", bool bShowDel = true, DigiListComboBox *parent = nullptr);
    ~DigiListComboBoxItem();

    /**
     * @brief getItemData
     * @return item的Data数据，id
     */
    qint64 getItemData() { return m_ItemData;}

    /**
     * @brief getItemStr
     * @return item的文本
     */
    QString getItemStr() { return m_Text;}

signals:
    void SignalRemoveItem(qint64 data);  //信号-删除该item
    void SignalShowItem(qint64 data);    //信号-在文本框显示该item

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    qint64 m_ItemData;
    DigiBaseLabel *m_LabItem;
    DigiSvgPushBtn *m_DelBtn;
    QHBoxLayout *m_MainLayout;
    QString m_Text;
    QMargins m_Margin;
};

#endif // DIGILISTCOMBOBOX_H

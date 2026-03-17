#ifndef DIGIMULTIPLECOMBOBOX_H
#define DIGIMULTIPLECOMBOBOX_H

/**
 * @brief 客户端公共库中的类型多选列表 MultipleComboBox 封装类
 * 1. 本头文件包含 MultipleComboBox 和 MultipleComboBoxItem 两个类
 * 2. 下拉框Item显示checkBox勾选框、可多选选项显示在编辑框中
 *
 * version 1.0
 * @date 2024-5-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include "digibasecombobox.h"

struct MultipleOption
{
    MultipleOption(quint8 _index, QString _text, bool _check = true)
        :index(_index), text(_text), check(_check){}

    quint8 index;
    QString text;
    bool check;
};

class QListWidget;
class QHBoxLayout;
class DigiBaseCheckBox;
class DigiMultipleComboBoxItem;
class DigiMultipleComboBox : public DigiBaseComboBox
{
    Q_OBJECT
public:
    explicit DigiMultipleComboBox(QWidget* parent = nullptr);

    virtual void SetScale(qreal scale); //缩放

    virtual void showPopup();  //显示下拉框
    virtual void hidePopup();  //隐藏下拉框

    void SetOptionText(const QString &All, QList<MultipleOption> list);//总选项、子选项名称（必须设置）
    void SetCheckedOptions(QList<quint8> indexList);//设置选中的选项
    QStringList GetCheckedTexts();//返回选中的选项文本列表
    QList<quint8> GetCheckedOptions();//返回选中的选项索引
    QMap<quint8, bool> GetAllCheckState();// 返回所有选项对应的选中状态

protected:
    void wheelEvent(QWheelEvent *event); //用于忽略鼠标滚轮事件，避免清空输入框

private:
    void SetText(QStringList textList);  //下拉框显示与隐藏时，设置输入框的文本

private slots:
    void CheckClickSlot(); //checkBox点击，状态变化更新

private:
    QListWidget* m_pListWidget;
    QList<DigiMultipleComboBoxItem*> m_ItemList;  //存储item
};

/********************************
 * DigiMultipleComboBoxItem *
 ********************************/
class DigiMultipleComboBoxItem : public QWidget
{
    Q_OBJECT

public:
    DigiMultipleComboBoxItem(QWidget *parent = nullptr,quint8 index = 0, const QString &text = "");

    void SetText(const QString &text);        //设置item文本
    void SetCheckState(Qt::CheckState state); //设置checkBox状态
    void SetIndex(quint8 index);              //设置当前下标

    QString GetText();              //获取当前文本
    Qt::CheckState GetCheckState(); //获取checkBox状态
    bool GetChecked();              //获取checkBox是否是Qt::Checked状态
    quint8 GetIndex();              //获取当前下标

    void SetScale(qreal scale);     //缩放

signals:
    void CheckBoxStateChanged(); //checkBox状态改变发送此信号
public slots:

protected:
    void mouseReleaseEvent(QMouseEvent *event);  //鼠标点击事件，改变checkBox状态

private:
    void CreateLayout();
    void CreateConnect();

private:
    DigiBaseCheckBox *m_CheckBox; //checkBox勾选框
    DigiBaseLabel *m_Label;       //文本

    QHBoxLayout *m_MainLayout;       //主布局

    quint8 m_Index;        //下标
};

#endif // DIGIMULTIPLECOMBOBOX_H

#ifndef DIGIFILECOMBOBOX_H
#define DIGIFILECOMBOBOX_H 

/**
  * @brief 客户端公共库中的FileComboBox封装
  * 1. 继承于 DigiBaseComboBox，可缩放，样式设置相同
  * 2. 点击输入框右侧的下拉框按钮，会弹出FileDialog，选择文件后，路径会显示在输入框中
  * 3. 该ComboBox不会显示下拉框
  * 4. 可设置FileDialog的各种属性
  * 5. 该FileDialog是自定义类，基于QFileDialog
  *
  * version 1.0
  * @date 2022-3-14
  *
  * Copyright (c) 2007-2022, Co.,Ltd.
  * All Rights Reserved
  * Histrory:
  * Revision
  */

#include <QMouseEvent>
#include "digibasecombobox.h"
#include "digibasefiledialog.h"
class DigiFileComboBox : public DigiBaseComboBox
{
    Q_OBJECT
public:
    explicit DigiFileComboBox(QWidget *parent = nullptr);
    void setFileMode(QFileDialog::FileMode); //设置FileDialog的FileMode
    void setNameFilter(const QString&);      //设置文件过滤
    void setCurrentDir(const QString&);      //设置当前打开的路径
    void setFileOption(QFileDialog::Option); //设置QFileDialog的Option
    void setTitle(const QString& title);
    void setLocalFlag(bool bflag);
signals:
    void currentTextChanged();
public slots:

protected:
    void mousePressEvent(QMouseEvent* event);
private:
    QFileDialog::FileMode m_mode;
    QFileDialog::Option m_option;
    QString m_filter;
    QString m_curDir;
    QString m_title;
    bool m_enSetOption;
    bool m_bLocalflag;

    DigiBaseFileDialog* m_fileDialog;//文件对话框
};

#endif // DIGIFILECOMBOBOX_H

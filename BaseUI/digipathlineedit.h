#ifndef DIGIPATHLINEEDIT_H
#define DIGIPATHLINEEDIT_H

/**
 * @brief 客户端公共库中的打开文件编辑框 PathLineEdit 封装类
 * 继承于 DigiBaseLineEdit 类, 可点击按钮打开文件管理器
 *
 * version 1.0
 * @date 2024-5-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include "digibaselineedit.h"
#include <QFileDialog>

class DigiBaseWidget;
class DigiIconPushBtn;

class DigiPathLineEdit : public DigiBaseLineEdit
{
    Q_OBJECT
public:
    explicit DigiPathLineEdit(QWidget *parent = nullptr);
    void SetFileMode(QFileDialog::FileMode mode);
    void SetNameFilter(const QString &filter);
    void SetAcceptMode(QFileDialog::AcceptMode mode);
    void SetIconBtn(QString path);

    void SetScale(qreal scale) override;

    void SetHeight(qreal height) override;

signals:
    void fileAccepted();

private:
    void InitWidget();
    void CreateConnect();

private slots:
    void OpenFileDialog();

private:
    int m_Spacing;
    QMargins m_Margin;
    DigiIconPushBtn *m_Btn;
    QFileDialog::FileMode m_mode;
    QString m_Filter;
    QFileDialog::AcceptMode m_accept;
};

#endif // DIGIPATHLINEEDIT_H

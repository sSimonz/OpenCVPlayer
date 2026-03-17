#ifndef DIGIPASSWORDLINEEDIT_H
#define DIGIPASSWORDLINEEDIT_H

/**
 * @brief 公共库中的密码文本编辑框PasswordLineEdit封装类
 * 继承于 DigiBaseLineEdit 类
 *
 * version 1.0
 * @date 2024-5-16
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include "digibaselineedit.h"

class DigiIconPushBtn;

class DigiPasswordLineEdit : public DigiBaseLineEdit
{
    Q_OBJECT
public:
    explicit DigiPasswordLineEdit(QWidget *parent = nullptr);
    void SetLength(int Length);  //设置最长文本长度
    void SetLimitLength(bool blimit);

    bool getEyeVisible();           //获取密码显隐按钮是否可见
    void setEyeVisible(bool bshow); //设置密码显隐按钮是否可见

    void setLimitFormat(const QString &format);

    void setText(const QString &name);  //覆盖 setText 函数
    void setEchoMode(EchoMode mode);    //覆盖 setEchoMode 函数
    void setCopyPasteEnabled(bool b);   //设置是否可以复制粘贴

    void SetScale(qreal scale);

signals:
    void pwdChanged(QString);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void Init();
    void CreateConnect();

public slots:
    void ShowPassword();

private:
    int m_limitLength;
    bool m_bNolimitLength;
    QString m_limitformat;//现在输入字符类型
	DigiIconPushBtn *m_PwdBtn;
    bool m_bPaste;
};

#endif // DIGIPASSWORDLINEEDIT_H

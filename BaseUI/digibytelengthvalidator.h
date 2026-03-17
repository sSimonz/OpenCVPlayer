#ifndef DIGIBYTELENGTHVALIDATOR_H
#define DIGIBYTELENGTHVALIDATOR_H

/**
  * @brief 
  *
  * version 1.0
  * @date 2024-2-22
  */
/**
 * @brief 客户端公共库中的验证器 DigiByteLengthValidator 封装类
 * 1. 可设置长度限制
 * 2. 可设置字符限制

 *
 * version 1.0
 * @date 2022-3-24
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include <QValidator>

class DigiByteLengthValidator : public QValidator
{
    Q_OBJECT
public:
    explicit DigiByteLengthValidator(QObject *parent = nullptr);

    virtual State validate(QString &input, int &pos) const override;

    void setMaxBytes(unsigned int max);
    void setLimitChar(bool blimit);
private:
    unsigned int m_maxBytes; //最大限制字节数
    QRegularExpression re;
    bool m_limitLength;//是否限制长度

signals:
};

#endif // DIGIBYTELENGTHVALIDATOR_H

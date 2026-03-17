#ifndef DIGIICONLINEEDIT_H
#define DIGIICONLINEEDIT_H

/**
 * @brief 客户端公共库中的文本编辑框ClearLineEdit封装类
 * 1. 继承于 DigiBaseLineEdit 类，可缩放，样式设置相同
 * 2. 输入框尾部有图标
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include "Base/iiconbase.h"
#include "digibaselineedit.h"

class DigiBaseLabel;

class DigiIconLineEdit : public DigiBaseLineEdit, public IiconBase
{
    Q_OBJECT
public:
    explicit DigiIconLineEdit(QWidget *parent = nullptr);

    void SetIconSize(const QSize &size);

    void SetCustomIcon(const QString &iconpath) override;
    void SetScale(qreal scale) override;

private:
    DigiBaseLabel *m_LabIcon;
    QSize m_IconSize;
};

#endif // DIGIICONLINEEDIT_H

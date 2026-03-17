#ifndef DIGIBASETAB_H
#define DIGIBASETAB_H

/**
 * @brief 公共库中的页签 DigiBaseTab 封装类
 * 1. 可进行缩放，基础大小默认固定值：134*32
 * 2. 需要设置页签类型，Left/Right/Middle
 * 3. 可设置页签右上角红点显示，表示未读状态
 *
 * version 1.0
 * @date 2022-3-14
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */
#include <QPushButton>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"

class DigiBaseTab : public QPushButton, public IStyleSheetBase, public IWidgetBase
{
    Q_OBJECT
public:
    enum
    {
        Left = 0,
        Right,
        Middle
    };
    explicit DigiBaseTab(QWidget *parent = nullptr);
    void setTabType(int type);
    void setRead(bool bRead);
    void SetScale(qreal scale = 1.0);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void SetCustomStyleSheet(qreal scale);

private:
    bool m_bRead;
    QFont m_Font;
};

#endif // DIGIBASETAB_H

#ifndef ISTYLESHEETBASE_H
#define ISTYLESHEETBASE_H
#include <QString>
#include "colormanager.h"
#include "ifontconfig.h"
/**
 * @brief The IStyleSheetBase class
 * 设置文本显示和样式
 */

class IStyleSheetBase
{
public:
    virtual ~IStyleSheetBase(){}
    virtual void SetCustomStyleSheet(qreal scale) = 0;
};
#endif // ISTYLESHEETBASE_H



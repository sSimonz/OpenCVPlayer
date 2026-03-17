#ifndef IICONBASE_H
#define IICONBASE_H
#include <QString>
/**
 * @brief The IiconBase class
 * 通过SetIcon加载图标
 */
class IiconBase
{
public:
    virtual ~IiconBase(){}
    virtual void SetCustomIcon(const QString &iconpath) = 0;
};

#endif // IICONBASE_H

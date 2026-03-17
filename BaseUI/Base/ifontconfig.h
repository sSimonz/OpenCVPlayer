#ifndef IFONTCONFIG_H
#define IFONTCONFIG_H

#include <QFont>
#include <QMap>
#include "isingleton.h"

class IFontConfig : public Singleton<IFontConfig>
{
public:
    IFontConfig(token);
    ~IFontConfig();
    IFontConfig(const IFontConfig&) = delete;
    IFontConfig& operator =(const IFontConfig&) = delete;

    QFont GetGlobalFont();
    QString GetGlobalFamily(int);

private:
    QFont m_GlobalFont;
    QMap<int, QString> m_GlobalFamily;
};

QFont GetGlobalFont();
QString GetGlobalFamily(int);

#endif // IFONTCONFIG_H

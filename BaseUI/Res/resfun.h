#ifndef RESFUN_H
#define RESFUN_H
#include <QApplication>
#include <QIcon>
#include "resource.h"

enum{
    RES_THEME_GRAY = 0,
};

enum{
    RES_TYPE_PNG = 0,
    RES_TYPE_ICON,
    RES_TYPE_QSS,
    RES_TYPE_GIF,
    RES_TYPE_BMP,
    RES_TYPE_SVG,
    RES_TYPE_HTML,
    RES_TYPE_FONT
};

enum{
    ICO_GROUP = 0,
    ICO_VEHICLE_ONLINE,
    ICO_VEHICLE_OFFLINE,
    ICO_VEHICLE_ALARM,
    ICO_VEHICLE_UNREGISTER,
    ICO_VEHICLE_UNLOCATION,
    ICO_VEHICLE_ICCARD,
    ICO_VEHICLE_GASSTATION,
    ICO_VEHICLE_USER,
    ICO_PDA_ONLINE,
    ICO_PDA_OFFLINE,
    ICO_PDA_ALARM,
    ICO_PDA_UNREGISTER,
    ICO_PDA_UNLOCATION,
    ICO_COUNT
};

class ResFun
{
public:
    ResFun();
    static QString GetResUrlByName(const QString &name,int type,bool bAddExt=true);
    static QString GetBmpResUrlByName(const QString &name,bool bAddExt = true);
    static QString GetPngResUrlByName(const QString &name, bool bAddExt=true, QString suffix = "svg");
    static QString GetGifResUrlByName(const QString &name,bool bAddExt=true);
    static QString GetIconResUrlByName(const QString &name,bool bAddExt=true);
    static QString GetHtmlImage(const QString &name, bool bAddExt=true);
    static QString GetSvgResUrlByName(const QString &name,bool bAddExt=true);
    static QString GetFontResUrlByName(const QString &name,bool bAddExt=true);
    static QString GetImageFromFile(const QString &fileName = "app.png");
    static QIcon GetWindowIcon(const QString &fileName = "app.png");
public:
    static QString strTheme;
};

#endif // RESFUN_H

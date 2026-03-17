#include "resfun.h"
#include <QDebug>
#include <QFileInfo>

ResFun::ResFun()
{
}

QString ResFun::GetResUrlByName(const QString &name,int type,bool bAddExt)
{
    QString qResUrlPath("");
    QString suf;
    switch (type) {
    case RES_TYPE_PNG:
        qResUrlPath = ":/res/pic/";
        suf = ".png";
        break;
    case RES_TYPE_ICON:
        qResUrlPath = ":/res/icon/";
        suf = ".icon";
        break;
    case RES_TYPE_QSS:
        qResUrlPath = ":/res/qss/";
        suf = ".qss";
        break;
    case RES_TYPE_GIF:
        qResUrlPath = ":/res/gif/";
        suf = ".gif";
        break;
    case RES_TYPE_BMP:
        qResUrlPath = ":/res/bmp/";
        suf = ".bmp";
        break;
    case RES_TYPE_SVG:
        qResUrlPath = ":/res/pic/";
        suf = ".svg";
        break;
    case RES_TYPE_HTML:
        qResUrlPath = ":/res/html/";
        suf = ".html";
        break;
    case RES_TYPE_FONT:
        qResUrlPath = ":/res/font/";
        suf = ".ttf";
        break;
    default:
        return qResUrlPath;
    }
    qResUrlPath += name;
    if(bAddExt)
    {
        qResUrlPath += suf;
    }
  //  qDebug() << name << type << qResUrlPath;
    return qResUrlPath;
}

QString ResFun::GetPngResUrlByName(const QString &name, bool bAddExt, QString suffix)
{
#ifdef QT_USER_VECTOR
    if(suffix.contains("svg"))
        return GetSvgResUrlByName(name,bAddExt);
#endif
    return ResFun::GetResUrlByName(name,RES_TYPE_PNG,bAddExt);
}

QString ResFun::GetSvgResUrlByName(const QString &name,bool bAddExt)
{
    return ResFun::GetResUrlByName(name,RES_TYPE_SVG,bAddExt);
}

QString ResFun::GetBmpResUrlByName(const QString &name,bool bAddExt)
{
    return ResFun::GetResUrlByName(name,RES_TYPE_BMP,bAddExt);
}

QString ResFun::GetGifResUrlByName(const QString &name,bool bAddExt)
{
    return ResFun::GetResUrlByName(name,RES_TYPE_GIF,bAddExt);
}

QString ResFun::GetIconResUrlByName(const QString &name,bool bAddExt)
{
    return ResFun::GetResUrlByName(name,RES_TYPE_ICON,bAddExt);
}

QString ResFun::GetHtmlImage(const QString &name, bool bAddExt)
{
    return ResFun::GetResUrlByName(name,RES_TYPE_HTML,bAddExt);
}

QString ResFun::GetFontResUrlByName(const QString &name,bool bAddExt)
{
    return ResFun::GetResUrlByName(name,RES_TYPE_FONT,bAddExt);
}

QString ResFun::GetImageFromFile(const QString &fileName)
{
    QString strName = qApp->applicationDirPath() + "/skin/"  + fileName;
    QFileInfo file(strName);
    if (!file.exists()) {
        strName = ResFun::GetPngResUrlByName(fileName,false);
    }
    return strName;
}

QIcon ResFun::GetWindowIcon(const QString &fileName)
{
    return QIcon(GetImageFromFile(fileName));
}



#include "ifontconfig.h"
#include <QFontDatabase>
#include "BaseUI/Res/resfun.h"
IFontConfig::IFontConfig(Singleton::token)
{
    // medium
    int fontId = QFontDatabase::addApplicationFont(ResFun::GetFontResUrlByName("SourceHanSansSC-Medium"));
    QStringList fontnames = QFontDatabase::applicationFontFamilies(fontId);
    QString medium = QFontDatabase::applicationFontFamilies(fontId).at(0);
    m_GlobalFamily.insert(QFont::Medium, medium);
    // normal
    fontId = QFontDatabase::addApplicationFont(ResFun::GetFontResUrlByName("SourceHanSansSC-Regular"));
    fontnames = QFontDatabase::applicationFontFamilies(fontId);
    QString normal = QFontDatabase::applicationFontFamilies(fontId).at(0);
    m_GlobalFamily.insert(QFont::Normal, normal);
    // bold
    fontId = QFontDatabase::addApplicationFont(ResFun::GetFontResUrlByName("SourceHanSansSC-Bold"));
    fontnames = QFontDatabase::applicationFontFamilies(fontId);
    QString bold = QFontDatabase::applicationFontFamilies(fontId).at(0);
    m_GlobalFamily.insert(QFont::Bold, bold);

    m_GlobalFont.setFamily(normal);
    m_GlobalFont.setHintingPreference(QFont::PreferNoHinting);
    m_GlobalFont.setPixelSize(14);
}

IFontConfig::~IFontConfig()
{

}

QFont IFontConfig::GetGlobalFont()
{
    return m_GlobalFont;
}

QString IFontConfig::GetGlobalFamily(int weight)
{
    auto it = m_GlobalFamily.find(weight);
    if(it != m_GlobalFamily.end())
        return it.value();
    else
        return m_GlobalFamily.value(QFont::Normal);
}

QFont GetGlobalFont()
{
    IFontConfig& fontConfig = IFontConfig::get_instance();
    return fontConfig.GetGlobalFont();
}

QString GetGlobalFamily(int weight)
{
    IFontConfig& fontConfig = IFontConfig::get_instance();
    return fontConfig.GetGlobalFamily(weight);
}

#include "colormanager.h"
#include <QFile>
#include <QDebug>
#include <QMetaEnum>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

QScopedPointer<ColorManager, ColorManager::ScopedPointerCustomDeleter> ColorManager::s_ColorManager;

ColorManager::ColorManager(QObject *parent) : QObject(parent)
{
    InitColorMap();
}

ColorManager::~ColorManager()
{
    m_ColorMap.clear();
}

QColor ColorManager::GetColor(int colorName)
{
    if (s_ColorManager.isNull())
        s_ColorManager.reset(new ColorManager);  //InitColorMap()

    const QColor &color = s_ColorManager->m_ColorMap.value(colorName, QColor());
    Q_ASSERT(color.isValid());
    return color;
}

QString ColorManager::GetColorHex(int colorName)
{
    return GetColor(colorName).name(QColor::HexArgb);
}

void ColorManager::InitColorMap()
{
    //加载json文件读取颜色
    QFile jsonFile(":/res/ColorInCode.json");
    jsonFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&jsonFile);
    // in.setCodec("UTF-8");

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8(), &error);
    jsonFile.close();
    Q_ASSERT(error.error == QJsonParseError::NoError);

    //解析json
    QJsonObject obj = doc.object();
    QJsonObject colorObj = obj.value("Color").toObject();
    QStringList keyList = colorObj.keys();

    //使用字符串的枚举名来获取枚举值，插入map表中
    QMetaEnum metaEnum = QMetaEnum::fromType<COLOR_NAME>();
    foreach (QString key, keyList)
    {
        QJsonArray array = colorObj.value(key).toArray();
        for (QJsonValue value : array)
        {
            QJsonObject cObj = value.toObject();
            QString coloName = cObj.value("name").toString();
            QString coloHex = cObj.value("hex").toString();

            int enumValue = metaEnum.keyToValue(coloName.toUtf8());  //返回-1代表缺少枚举值
            m_ColorMap.insert(enumValue, coloHex);
        }
    }

    Q_ASSERT(!m_ColorMap.contains(-1));
}

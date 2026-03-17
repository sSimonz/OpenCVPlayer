#include "digibasesplashscreen.h"

DigiBaseSplashScreen::DigiBaseSplashScreen(const QPixmap &pixmap) : QSplashScreen(pixmap)
{

}

DigiBaseSplashScreen::~DigiBaseSplashScreen()
{

}


void DigiBaseSplashScreen::SetScale(qreal scale)
{
    //自身
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height*scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width*scale);

    QPixmap pix = pixmap().scaled(width(), height());
    setPixmap(pix);

    //子控件
    auto widgetlist = findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
    foreach (QWidget *var, widgetlist)
    {
        if (var->inherits("IWidgetBase"))
        {
            dynamic_cast<IWidgetBase *>(var)->SetScale(scale);
        }
    }
    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseSplashScreen::SetCustomStyleSheet(qreal scale)
{

}

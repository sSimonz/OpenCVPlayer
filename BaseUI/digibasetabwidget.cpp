#include "digibasetabwidget.h"
#include <QTabBar>

DigiBaseTabWidget::DigiBaseTabWidget(QWidget *parent):
    QTabWidget(parent)
{
    m_width = 0;
    m_height = 0;
    m_minTabWidth = 200;
    m_Font = GetGlobalFont();
    m_Font.setPixelSize(14);
    m_selectColor = ColorManager::GetColorHex(ColorManager::CN_TABBAR_TEXT_BORDER_BOTTOM_SELECT);
}

void DigiBaseTabWidget::SetMinimumTabWidth(qreal width)
{
    m_minTabWidth = width;
}

void DigiBaseTabWidget::SetSelectColor(const QColor &color)
{
    m_selectColor = color.name(QColor::HexArgb);
}

void DigiBaseTabWidget::SetScale(qreal scale)
{
    //自身大小
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    //字体
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize()*scale);
    setFont(aFont);
    tabBar()->setFont(aFont);

    //子控件
    int widCount = count();
    for (int i=0; i<widCount; i++)
    {
        QWidget *wid = widget(i);
        if (wid->inherits("IWidgetBase"))
        {
            dynamic_cast<IWidgetBase*>(wid)->SetScale(scale);
        }
        else
        {
            auto widgetlist = wid->findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
            foreach (QWidget *var, widgetlist)
            {
                if (var->inherits("IWidgetBase"))
                {
                    dynamic_cast<IWidgetBase*>(var)->SetScale(scale);
                }
            }
        }
    }

    //样式表
    SetCustomStyleSheet(scale);
}

void DigiBaseTabWidget::SetCustomStyleSheet(qreal scale)
{
    QString qss;
    qss.append(QString("QTabWidget::tab-bar{"
                           "subcontrol-position:left;}"
                       "QTabWidget::pane{"
                           "border-top: %2px solid %3;"
                           "position: absolute;"
                           "background-color: %1;}"
                       "QTabWidget::tab-bar{"
                           "alignment: center;}")
               .arg(ColorManager::GetColorHex(ColorManager::CN_THEME_BACKGROUND))
               .arg(2 * scale)
               .arg(ColorManager::GetColorHex(ColorManager::CN_TABBAR_TEXT_BORDER_BOTTOM)));

    qss.append(QString("QTabBar::tab {"
                           "background: %1;"
                           "min-width: %2px;"
                           "padding: %3px;"
                           "height: %4px;"
                           "color:%5;}")
               .arg(ColorManager::GetColorHex(ColorManager::CN_THEME_BACKGROUND))
               .arg(m_minTabWidth * scale)
               .arg(2 * scale)
               .arg(34 * scale)
               .arg(ColorManager::GetColorHex(ColorManager::CN_TABBAR_TEXT_NORMAL)));

    qss.append(QString("QTabBar::tab:selected, QTabBar::tab:hover {"
                           "font-weight: Medium;"
                           "font-size: 16px;"
                           "color: %1;}"
                       "QTabBar::tab:selected{"
                           "border-bottom: %2px solid %3;}")
               .arg(ColorManager::GetColorHex(ColorManager::CN_TABBAR_TEXT_SELECT))
               .arg(3 * scale)
               .arg(m_selectColor));


    setStyleSheet(qss);
}

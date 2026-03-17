#include "digibasescrollbar.h"

#include "Res/resfun.h"

DigiBaseScrollBar::DigiBaseScrollBar(QWidget *parent) :
    QScrollBar(parent),
    m_BgColorHex(ColorManager::GetColorHex(ColorManager::CN_QSCROLLBAR_BACKGROUND)),
    m_NormalColorHex(ColorManager::GetColorHex(ColorManager::CN_QSCROLLBAR_HANDLE_NORMAL)),
    m_PressColorHex(ColorManager::GetColorHex(ColorManager::CN_QSCROLLBAR_HANDLE_PRESS))
{

}

void DigiBaseScrollBar::SetBgColor(const QColor &color)
{
    m_BgColorHex = color.name(QColor::HexArgb);
}

void DigiBaseScrollBar::SetHandleNormalColor(const QColor &color)
{
    m_NormalColorHex = color.name(QColor::HexArgb);
}

void DigiBaseScrollBar::SetPressNormalColor(const QColor &color)
{
    m_PressColorHex = color.name(QColor::HexArgb);
}

void DigiBaseScrollBar::SetScale(qreal scale)
{
    SetCustomStyleSheet(scale);
}

void DigiBaseScrollBar::SetCustomStyleSheet(qreal scale)
{
    QStringList qssScrollBar;

    //vertical
    qssScrollBar.append(QString("QScrollBar::vertical{"
                                    "border-radius: %1px;"
                                    "background: %2;"
                                    "min-width: %3px;"
                                    "max-width: %3px;"
                                    "margin-top:%4px;"
                                    "margin-bottom:%4px;}")
                                .arg(0*scale)
                                .arg(m_BgColorHex)
                                .arg(16*scale)
                                .arg(0*scale));

    qssScrollBar.append(QString("QScrollBar::handle:vertical{"
                                    "border-radius: %1px;"
                                    "min-height: %2px;"
                                    "margin-left:  %3px;"
                                    "margin-right: %3px;"
                                    "margin-top: %4px;"
                                    "margin-bottom: %4px;}")
                                .arg(4*scale)
                                .arg(20*scale)
                                .arg(4*scale)
                                .arg(16*scale));

    qssScrollBar.append(QString("QScrollBar::handle:vertical:enabled{background-color: %1;}"
                                "QScrollBar::handle:vertical:pressed{background-color: %2;}")
                                .arg(m_NormalColorHex)
                                .arg(m_PressColorHex));

    qssScrollBar.append(QString("QScrollBar::add-page:vertical,"
                                "QScrollBar::sub-page:vertical{"
                                    "background: transparent;}"));

    qssScrollBar.append(QString("QScrollBar::add-line:vertical{"
                                    "border-image: url(%1);"
                                    "background: transparent;}"
                                "QScrollBar::sub-line:vertical{"
                                    "border-image: url(%2);"
                                    "background: transparent;}")
                                .arg(ResFun::GetSvgResUrlByName(ICON_COMMON_DOWNARROW))
                                .arg(ResFun::GetSvgResUrlByName(ICON_COMMON_UPARROW)));


    //horizontal
    qssScrollBar.append(QString("QScrollBar::horizontal{"
                                    "border-radius: %1px;"
                                    "background: %2;"
                                    "min-height: %3px;"
                                    "max-height: %3px;"
                                    "margin-left:%4px;"
                                    "margin-right:%4px;}")
                                .arg(0*scale)
                                .arg(m_BgColorHex)
                                .arg(16*scale)
                                .arg(0*scale));

    qssScrollBar.append(QString("QScrollBar::handle:horizontal{"
                                    "border-radius:%1px;"
                                    "min-width: %2px;"
                                    "margin-top: %3px;"
                                    "margin-bottom: %3px;"
                                    "margin-left: %4px;"
                                    "margin-right: %4px;}")
                                .arg(4*scale)
                                .arg(20*scale)
                                .arg(4*scale)
                                .arg(16*scale));

    qssScrollBar.append(QString("QScrollBar::handle:horizontal:enabled{background-color: %1;}"
                                "QScrollBar::handle:horizontal:pressed{background-color: %2;}")
                                .arg(m_NormalColorHex)
                                .arg(m_PressColorHex));

    qssScrollBar.append(QString("QScrollBar::add-page:horizontal,"
                                "QScrollBar::sub-page:horizontal{"
                                    "background: transparent;}"));

    qssScrollBar.append(QString("QScrollBar::add-line:horizontal{"
                                    "border-image: url(%1);"
                                    "background: transparent;}"
                                "QScrollBar::sub-line:horizontal{"
                                    "border-image: url(%2);"
                                    "background: transparent;}")
                                .arg(ResFun::GetSvgResUrlByName(ICON_COMMON_RIGHTARROW))
                                .arg(ResFun::GetSvgResUrlByName(ICON_COMMON_LEFTARROW)));

    this->setStyleSheet(qssScrollBar.join(""));
}

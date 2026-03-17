#include "icolorconfig.h"

IColorConfig::IColorConfig(Singleton::token)
{
    InitColorList();
}

IColorConfig::~IColorConfig()
{
    m_colorList.clear();
}

QString IColorConfig::LoadColorFromID(int colorID)
{
    Q_ASSERT(colorID>-1 && colorID<m_colorList.count());
    return m_colorList.at(colorID);
}

void IColorConfig::InitColorList()
{
    m_colorList << "#000000"    //COLOR_GLOBAL_TEXT_BLACK
                << "#f5f7f8"    //COLOR_GLOBAL_TEXT_WHITE

                << "#b3b3b3"    //COLOR_BORDER_NORMAL
                << "#666666"    //COLOR_BORDER_HOVER
                << "#f5f7f8"    //COLOR_EDIT_BACKGROUND

                << "#f6f7f8"    //COLOR_ICONBTN_TEXT_NORMAL
                << "#a0a0a0"    //COLOR_ICONBTN_TEXT_DISABLE
                << "#336ed3"    //COLOR_ICONBTN_BACKGROUND_NORMAL
                << "#0f4cb2"    //COLOR_ICONBTN_BACKGROUND_HOVER
                << "#d4d7d7"    //COLOR_ICONBTN_BACKGROUND_DISABLE

                << "#000000"    //COLOR_TOOLBTN_TEXT_NORMAL
                << "#a0a0a0"    //COLOR_TOOLBTN_TEXT_DISABLE
                << "#00000000"  //COLOR_TOOLBTN_BACKGROUND_NORMAL
                << "#00000000"  //COLOR_TOOLBTN_BACKGROUND_HOVER
                << "#00000000"  //COLOR_TOOLBTN_BACKGROUND_DISABLE

                << "#000000"    //COLOR_COMBOBOXITEM_SELECTION
                << "#d3eefb"    //COLOR_COMBOBOXITEM_SELECTION_BACKGROUND,

                << "#ffffff"    //COLOR_HOMEBTN_BG_NORMAL
                << "#f1f1f1"    //COLOR_HOMEBTN_BG_HOVER
                << "#dfdfdf"    //COLOR_HOMEBTN_BG_PRESSED
                << "#dfdfdf"    //COLOR_HOMEBTN_BG_CHECKED
                << "#000000"    //COLOR_HOMEBTN_TEXT_NORMAL
                << "#abb2bd"    //COLOR_HOMEBTN_TEXT_HOVER
                << "#40c4ff"    //COLOR_HOMEBTN_TEXT_PRESSED
                << "#40c4ff"    //COLOR_HOMEBTN_TEXT_CHECKED

				<< "#000000"    //COLOR_HOMETOOLBTN_TEXT_TITLE
                << "#7E8690"    //COLOR_HOMETOOLBTN_TEXT_TIPS
                << "#50B6FF"    //COLOR_HOMETOOLBTN_HOVER
                << "#f2f3f4"    //COLOR_HOMEPAGE_BOTTOM_BG

				<< "#8bbbdc"    //COLOR_CALENDAR_WEEKLAB_TEXT
                << "#000000"    //COLOR_CALENDAR_TOOLBTN_TEXT
                << "#00000000"  //COLOR_CALENDAR_TOOLBTN_BG_NORMAL,
                << "#00000000"  //COLOR_CALENDAR_TOOLBTN_BG_HOVER,
                << "#00000000"  //COLOR_CALENDAR_TOOLBTN_BG_DISABLE,
                << "#ff5500"    //COLOR_CALENDAR_CALBTN_RECORD
                << "#000000"    //COLOR_CALENDAR_CALBTN_TEXT_WORKDAY
                << "#ff0000"    //COLOR_CALENDAR_CALBTN_TEXT_WEEKEND
                << "#ffffff"    //COLOR_CALENDAR_CALBTN_TEXT_SELECT
                << "#ffffff"    //COLOR_TITLE_TEXT
                << "#817889"    //COLOR_CALENDAR_CALBTN_TEXT_PREV_OR_NEXT
                << "#accbdb"    //COLOR_CALENDAR_CALBTN_BG_HOVER
                << "#3b92c9"    //COLOR_CALENDAR_CALBTN_BG_SELECT
                << "#878787"    //COLOR_CALENDAR_CALBTN_BORDER
                << "#817889"    //COLOR_CALENDAR_CALBTN_TEXT_DISABLE
                << "#000000"    //COLOR_CALENDAR_CALBTN_TEXT_CURRENT_DISABLE

                << "#ffffff"    //COLOR_ACTION_TEXT
                << "#2458af"    //COLOR_ACTION_BG_NORMAL
                << "#50b7fa"    //COLOR_ACTION_BG_HOVER
                << "#06446a"    //COLOR_RSWINBASE_BORDER
                << "#f5f7f8"    //COLOR_RSWINBASE_BG
                << "#00bbd8"    //COLOR_TRIANGLE_BG
				<< "#ffffff"    //COLOR_DIALOG_TITLTBAR_TEXT
                << "#0f4cb2"    //COLOR_DIALOG_TITLEBAR_BG
                << "#d5e6ef"    //COLOR_DIALOG_MAINWIDGET_BG
                << "#50b7fa"    //COLOR_CPU_NORMAL
                << "#fff212"    //COLOR_CPU_OCCUPY
                << "#2458af"    //COLOR_LOCK_BG
                << "#ffffff"    //COLOR_LOCK_TEXT

                << "#f5f7f8"    //COLOR_TABLEVIEW_BG
                << "#e7eff4"    //COLOR_TABLEVIEW_ALTERNATE_BG
                << "#98b7c4"    //COLOR_TABLEVIEW_SELECTION_BG
                << "#ffffff"    //COLOR_TABLEVIEW_HEADER_TEXT
                << "#97b7c4"    //COLOR_TABLEVIEW_HEADER_BG
                << "#c1c1c1"    //COLOR_TABLEVIEW_HEADER_BORDER
                << "#f5f7f8"    //COLOR_TREEVIEW_BG
                << "#000000"    //COLOR_TREEVIEW_TEXT
                << "#e3e4e6"    //COLOR_TREEVIEW_ITEM_BG_HOVER_AND_SELECTION

                << "#00000000"	//COLOR_QSCROLLBAR_BG
                << "#b2b7c2"	//COLOR_QSCROLLBAR_HANDLE_NORMAL
                << "#7d8a93"	//COLOR_QSCROLLBAR_HANDLE_PRESS

                << "#000000"    //COLOR_BASECHECKBOX_TEXT
                << "#10aa10"	//COLOR_COMBOBOX_RECORD_ALL
                << "#008000"	//COLOR_COMBOBOX_RECORD_NORMAL
                << "#ff7800"	//COLOR_COMBOBOX_RECORD_ALARM
                << "#00b4ff"    //COLOR_COMBOBOX_RECORD_INTELLIGENT

                << "#000000"    //COLOR_BASERADIOBTN_TEXT
                << "#999999"    //COLOR_SLIDER_GROOVE_BORDER
                << "#336ed3"    //COLOR_SLIDER_MINSIDE
                << "#b7d4e6"    //COLOR_SLIDER_MAXSIDE
                << "#3361d3"    //COLOR_SWITCHBTN_ON
                << "#22449d"    //COLOR_SWITCHBTN_ON_DISABLE
                << "#b8b8b8"    //COLOR_SWITCHBTN_OFF
                << "#888888"    //COLOR_SWITCHBTN_OFF_DISABLE
                << "#ffffff"    //COLOR_SWITCHBTN_DOT

                << "#f5f7f8"    //COLOR_SPINBOX_BG
                << "#9eb2c0"	//COLOR_SPINBOX_BORDER
                << "#f5f7f8"    //COLOR_DATETIMEEDIT_BG
                << "#9eb2c0"    //COLOR_DATETIMEEDIT_BORDER
                << "#a9a9a9"    //COLOR_DATETIMEEDIT_RS_DROPDOWN_BORDER
                << "#d5e6ef"    //COLOR_DATETIMEEDIT_RS_POPUP_BG

                << "#f5f7f8"    //COLOR_LISTWIDGET_BG
                << "#000000"    //COLOR_LISTWIDGET_TEXT
                << "#d3eefb"    //COLOR_LISTWIDGET_ITEM_BG_HOVER_AND_SELECTION
                << "#000000"    //COLOR_LISTWIDGET_ITEM_TEXT_HOVER_AND_SELECTION

                << "#00000000"  //COLOR_VIDEOSCREENLAYOUTMENU_TOOLBTN_BG_NORMAL
                << "#3278a5"    //COLOR_VIDEOSCREENLAYOUTMENU_TOOLBTN_BG_HOVER
                << "#ffffff"    //COLOR_VIDEOSCREENLAYOUTMENU_TOOLBTN_TEXT_NORMAL
                << "#000000"    //COLOR_VIDEOSCREENLAYOUTMENU_TOOLBTN_TEXT_HOVER

                << "#dcdcdc"    //COLOR_GROUPBOX_BORDER

                << "#387EE3"    //COLOR_WAITING_DIALOG_BG1
                << "#3890E3"    //COLOR_WAITING_DIALOG_BG2

                << "#f5f7f8"    //COLOR_MSG_POPUP_BG
                << "#0f4cb2"    //COLOR_MSG_POPUP_TEXT
                << "#e43333"    //COLOR_MSG_POPUP_TEXT_FAILED
                << "#0f4cb2"    //COLOR_MSG_POPUP_TITLE_BG
                << "#e43333"    //COLOR_MSG_POPUP_TITLE_BG_FAILED
                << "#ffffff";   //COLOR_MSG_POPUP_TITLE_TEXT
}

QString LoadColorFromID(int colorID)
{
    IColorConfig& colorConfig = IColorConfig::get_instance();
    return colorConfig.LoadColorFromID(colorID);
}

QString getHexFromQColor(const QColor &color)
{
    QString strAlpha = QString("%1").arg(color.alpha() & 0xFF, 2, 16, QChar('0'));
    QString strRed   = QString("%1").arg(color.red()   & 0xFF, 2, 16, QChar('0'));
    QString strGreen = QString("%1").arg(color.green() & 0xFF, 2, 16, QChar('0'));
    QString strBlue  = QString("%1").arg(color.blue()  & 0xFF, 2, 16, QChar('0'));

    return QString("#%1%2%3%4").arg(strAlpha).arg(strRed).arg(strGreen).arg(strBlue);
}

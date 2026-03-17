#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QObject>
#include <QColor>
#include <QMap>

class ColorManager : public QObject
{
    Q_OBJECT

private:
    explicit ColorManager(QObject *parent = nullptr);
    ~ColorManager();
    Q_DISABLE_COPY(ColorManager)

    struct ScopedPointerCustomDeleter
    {
        static void cleanup(ColorManager *pointer)
        {
            delete pointer;
        }
    };

public:
    static QColor GetColor(int colorName);
    static QString GetColorHex(int colorName);

private:
    void InitColorMap();

private:
    static QScopedPointer<ColorManager, ScopedPointerCustomDeleter> s_ColorManager;
    QMap<int, QColor> m_ColorMap;

public:
    enum COLOR_NAME
    {
        /* Global */
        CN_GLOBAL_TEXT = 0,          //全局文本颜色
        CN_GLOBAL_TEXT_DISABLE,      //全局文本颜色-disabled状态
        CN_TOOLTIP_TEXT,             //QToolTip文本颜色
        CN_THEME_BACKGROUND,         //主题背景色
        CN_INPUT_BACKGROUND,         //表格类、输入类等显示数据的控件背景
        CN_FIELD_BACKGROUND,         //一些子容器控件内部背景，比如QWidget、QFrame、QTabWidget

        CN_INPUTWIDGET_BORDER,       //输入框、combobox边框颜色
        CN_INPUTWIDGET_BORDER_HOVER, //输入框、combobox边框颜色-悬停

        CN_CALENDAR_WEEKLAB_TEXT,             //日历星期表头文本
        CN_CALENDAR_CALBTN_RECORD,            //日历日期按钮-录像标记颜色
        CN_CALENDAR_CALBTN_TEXT_WORKDAY,      //日历日期按钮文本-工作日
        CN_CALENDAR_CALBTN_TEXT_WEEKEND,      //日历日期按钮文本-周末
        CN_CALENDAR_CALBTN_TEXT_SELECT,       //日历日期按钮文本-选中
        CN_CALENDAR_CALBTN_TEXT_PREV_OR_NEXT, //日历日期按钮文本-其他月份
        CN_CALENDAR_CALBTN_BG_HOVER,          //日历日期按钮背景-悬停
        CN_CALENDAR_CALBTN_BG_SELECT,         //日历日期按钮背景-选中

        CN_DIALOG_BACKGROUND,             //弹窗背景
        CN_DIALOG_TITLT_TEXT,             //弹窗标题栏文本
        CN_DIALOG_SUB_TITLT_TEXT,         //弹窗副标题文本
        CN_DIALOG_TITLE_BACKGROUND,       //弹窗标题栏背景-默认
        CN_DIALOG_TITLE_BACKGROUND_WARN,  //弹窗标题栏背景-警告

        CN_TABLEVIEWITEM_BACKGROUND_ALTERNATE, //表格视图交替颜色显示时的交替色
        CN_TABLEVIEWITEM_BACKGROUND_SELECTION, //表格视图选中时的背景
        CN_TABLEVIEWITEM_NODATA_TEXT,          //表格视图无数据时显示的NO DATA文本颜色
        CN_HEADERVIEW_TEXT,                    //表头文本
        CN_HEADERVIEW_BACKGROUND,              //表头背景
        CN_TABLEVIEW_HEADER_BORDER,            //表头分割线
        CN_TABLEVIEW_ITEM_HOVER,               //表格项背景-悬停
        CN_LISTWIDGETITEM_BACKGROUND_HOVER,    //列表视图项背景-悬停
        CN_TREEVIEWITEM_BACKGROUND_HOVER,      //树表item背景-悬停

        CN_QSCROLLBAR_BACKGROUND,    //滚动条背景
        CN_QSCROLLBAR_HANDLE_NORMAL, //滚动条handle背景
        CN_QSCROLLBAR_HANDLE_PRESS,  //滚动条handle背景-点击

        CN_SLIDER_GROOVE_BORDER, //滑块边框,此颜色不会显示出来
        CN_SLIDER_MINSIDE,       //滑块左侧/下侧的颜色
        CN_SLIDER_MAXSIDE,       //滑块右侧/上侧的颜色

        CN_SWITCHBTN_DOT,         //开关控件圆点
        CN_SWITCHBTN_DOT_DISABLE, //开关控件圆点-disabled
        CN_SWITCHBTN_ON,          //开关控件背景 开
        CN_SWITCHBTN_ON_DISABLE,  //开关控件背景 开-disabled
        CN_SWITCHBTN_OFF,         //开关控件背景 关
        CN_SWITCHBTN_OFF_DISABLE, //开关控件背景 关-disabled

        CN_GROUPBOX_BORDER, //groupbox边框

        CN_MSGPOPUP_TEXT_SUCCESS, //右下角弹窗文本-成功
        CN_MSGPOPUP_TEXT_FAILED,  //右下角弹窗文本-失败

        CN_TOOLBTN_BACKGROUND_NORMAL,  //tool按钮背景
        CN_TOOLBTN_BACKGROUND_HOVER,   //tool按钮背景-悬停
        CN_TOOLBTN_BACKGROUND_DISABLE, //tool按钮背景-diasbled

        CN_ICONBTN_TEXT,               //按钮文本
        CN_ICONBTN_TEXT_DISABLE,       //按钮文本-disabled
        CN_ICONBTN_TEXT_CHECK,         //按钮文本-checked
        CN_ICONBTN_BACKGROUND_NORMAL,  //按钮背景
        CN_ICONBTN_BACKGROUND_HOVER,   //按钮背景-悬停
        CN_ICONBTN_BACKGROUND_CHECK,   //按钮背景-点击
        CN_ICONBTN_BACKGROUND_DISABLE, //按钮背景-disabled
        CN_ICONBTN_BORDER,             //按钮边框
        CN_DIVIDING_LINE_BG,           //分割线背景
        CN_TOOLBOX_BG,                 //左侧工具箱背景
        CN_MENU_BG,                      //菜单背景
        CN_MENU_ACTION_TEXT_SELECTED,    //菜单可选ACTION的文本-选中状态
        CN_MENU_ACTION_BACKGROUND_HOVER, //菜单ACTION背景-悬停
        CN_MENU_ACTION_SEPARATOR,        //菜单ACTION分隔线
        CN_RADIOBTN_TEXT_CHECKED,      //单选按钮选中文本颜色
        CN_TEXTEDIT_BORDER,            //文本编辑框边框
        CN_PROGRESSBAR_CHUNK,          //进度条左侧的颜色
        CN_PROGRESSBAR_BG,             //进度条右侧的颜色
        CN_TIPS_COLOR,                 //提示标签文本颜色
        CN_TIPS_BG,                    //提示标签背景颜色
        CN_TABBAR_TEXT_NORMAL,         //标签栏文本颜色
        CN_TABBAR_TEXT_SELECT,         //标签栏文本颜色-选中状态
        CN_TABBAR_TEXT_BORDER_BOTTOM,  //标签栏边框底部颜色
        CN_TABBAR_TEXT_BORDER_BOTTOM_SELECT,//标签栏边框底部颜色-选中状态
        CN_BLACK_POPUP_BG,             //透明黑浮窗背景，透明度通过QColor自行调整
        CN_BLACK_POPUP_TEXT,           //透明黑浮窗背景中的文本颜色


        /* Home Page */
        CN_CPU_BACKGROUND_NORMAL,  //CPU条背景-未占用部分
        CN_CPU_BACKGROUND_OCCUPY,  //CPU条背景-已占用部分

        CN_TITLEBAR_BACKGROUND,           //程序标题栏背景
        CN_TITLEBARTAB_BACKGROUND_HOVER,  //程序标题栏Tab背景-悬停
        CN_TITLEBARTAB_SPLITLINE,         //程序标题栏Tab分割线
        CN_TITLEBARTAB_CLOSEBTN_BG_HOVER, //程序标题栏Tab关闭按钮背景-悬停

        CN_HOMEPAGE_DEVINFO_TEXT_TOTAL,     //主页右侧设备在线信息文本-总数
        CN_HOMEPAGE_DEVINFO_BG_TOTAL,       //主页右侧设备在线信息背景-总数
        CN_HOMEPAGE_DEVINFO_BORDER_TOTAL,   //主页右侧设备在线信息边框-总数
        CN_HOMEPAGE_DEVINFO_TEXT_ONLINE,    //主页右侧设备在线信息文本-在线
        CN_HOMEPAGE_DEVINFO_BG_ONLINE,      //主页右侧设备在线信息背景-在线
        CN_HOMEPAGE_DEVINFO_BORDER_ONLINE,  //主页右侧设备在线信息边框-在线
        CN_HOMEPAGE_DEVINFO_TEXT_OFFLINE,   //主页右侧设备在线信息文本-离线
        CN_HOMEPAGE_DEVINFO_BG_OFFLINE,     //主页右侧设备在线信息背景-离线
        CN_HOMEPAGE_DEVINFO_BORDER_OFFLINE, //主页右侧设备在线信息边框-离线

        CN_HOMEPAGE_TIMELABEL_TEXT,     //主页右侧时间信息文本
        CN_HOMEPAGE_HINTLABEL_BG,       //主页右侧概述/管理图标颜色
        CN_HOMEPAGE_USERLABEL_TEXT,     //主页右登录用户文本
        CN_HOMEPAGE_TOOLBTN_BG_NORMAL,  //主页按钮常规背景色
        CN_HOMEPAGE_TOOLBTN_BG_HOVER,   //主页按钮悬浮背景色
        CN_HOMEPAGE_TOOLBTN_BORDER,     //主页按钮边框颜色
        CN_ALARM_BAR_BACKGROUND,        //告警栏背景
        CN_USERLOGIN_TITLE_TEXT,        //登录页面标题文本
        CN_USERLOGIN_TIPS_TEXT,         //登录页面提示文本
        CN_USERLOGIN_LINK_TEXT,         //登录页面超链接文本
        CN_USERLOGIN_SUBTITLE_TEXT,     //登录页面副标题文本

        /* MainView */
        CN_VIEWSCREEN_BACKGROUND,    //视频窗体背景
        CN_VIEWSCREEN_BORDER_NORMAL, //视频窗体边框-未选中
        CN_VIEWSCREEN_BORDER_SELECT, //视频窗体边框-选中

        CN_HIDEWIDGETBTN_BAR_BACKGROUND_1,  //预览、回放页，隐藏左侧控件的按钮-分隔条渐变背景1
        CN_HIDEWIDGETBTN_BAR_BACKGROUND_2,  //预览、回放页，隐藏左侧控件的按钮-分隔条渐变背景2
        CN_HIDEWIDGETBTN_BAR_BACKGROUND_3,  //预览、回放页，隐藏左侧控件的按钮-分隔条渐变背景3
        CN_HIDEWIDGETBTN_BTN_BACKGROUND_1,  //预览、回放页，隐藏左侧控件的按钮-按钮渐变背景1
        CN_HIDEWIDGETBTN_BTN_BACKGROUND_2,  //预览、回放页，隐藏左侧控件的按钮-按钮渐变背景2
        CN_HIDEWIDGETBTN_BTN_BACKGROUND_3,  //预览、回放页，隐藏左侧控件的按钮-按钮渐变背景3

        CN_VIDEOSCREENLAYOUTMENU_TOOLBTN_BG,          //视频屏幕布局菜单按钮背景
        CN_VIDEOSCREENLAYOUTMENU_TOOLBTN_BG_HOVER,    //视频屏幕布局菜单按钮背景-悬停
        CN_VIDEOSCREENLAYOUTMENU_TOOLBTN_TEXT_NORMAL, //视频屏幕布局菜单按钮文本
        CN_VIDEOSCREENLAYOUTMENU_TOOLBTN_TEXT_HOVER,  //视频屏幕布局菜单按钮文本-悬停

        CN_PREVIEWFACELISTITEM_TITLE_TEXT,   //预览页右侧告警缩略图标题文本
        CN_PREVIEWFACELISTITEM_TITLE_BG,     //预览页右侧告警缩略图标题背景
        CN_PREVIEWFACELISTITEM_WHITELIST,    //预览页右侧告警缩略图-白名单颜色
        CN_PREVIEWFACELISTITEM_BLACKLIST,    //预览页右侧告警缩略图-黑名单颜色
        CN_DEVICEBASEINFO_TITLE,             //预览页设备信息弹窗标题颜色
        CN_VIEWSCREEN_CHNAME_TIME,           //视频预览通道名、时间文本颜色
        CN_DEVICE_TREE_BG,                   //设备树背景
        CN_PREVIEWFACELISTITEM_SUBTITLE,     //预览页右侧告警缩略图-副标题颜色

        CN_360_PROGRESSBAR_BG,   //预览页全景进度条背景
        CN_360_PROGRESSBAR_TEXT, //预览页全景进度条文本

        /* Playback */
        CN_PLAYBACK_TIMECTRL_BACKGROUND_EMPTY,  //回放时间轴背景-空状态
        CN_PLAYBACK_TIMECTRL_TIMELINE_TEXT,     //回放时间轴刻度-文本颜色
        CN_PLAYBACK_TIMECTRL_TIMELINE_SCALE,    //回放时间轴刻度-线颜色
        CN_PLAYBACK_TIMECTRL_CLIP_BACKGROUND,   //回放时间轴-裁剪下载选中区域左右刻度颜色
        CN_PLAYBACK_TIMECTRL_CLIP_TEXT,         //回放时间轴-裁剪下载选中区域左右时间文本颜色

        CN_PLAYBACK_TYPE_COMMON,  //回放类型-Common
        CN_PLAYBACK_TYPE_MOTION,  //回放类型-Motion
        CN_PLAYBACK_TYPE_SMART,   //回放类型-Smart

        CN_PLAYBACK_VIDEO_INFOBAR_BG,    //回放窗体信息条背景
        CN_PLAYBACK_VIDEO_INFOBAR_TEXT,  //回放窗体信息条文本

        CN_BG_DOWNLOAD_BG,               //后台下载页面背景颜色
        CN_BG_DOWNLOAD_LISTWIDGET_BG,    //后台下载页列表窗口背景颜色
        CN_BG_DOWNLOAD_ITEM_BG,          //后台下载页下载项背景颜色
        CN_BG_DOWNLOAD_ITEM_BORDER,      //后台下载页下载项边框颜色
        CN_BG_DOWNLOAD_ITEM_SELECT,      //后台下载页下载项悬停选中颜色
        CN_BG_DOWNLOAD_ITEM_TIP_TEXT,    //后台下载页下载项下载异常文本颜色
        CN_BG_DOWNLOAD_ITEM_TIP_COM_TEXT,//后台下载页下载项下载完成文本颜色
        CN_BG_DOWNLOAD_ITEM_TIME_TEXT,   //后台下载页下载项时间文本颜色
        CN_PLAYBACK_BUTTON_CONTROL_BG,   //回放窗体按钮控制条背景
        CN_PLAYBACK_FISHEYE_TABBAR_BG,   //鱼眼窗体页签栏背景

        /* Log Search */

        /* Topology Management */

        /* AI Search */
        CN_AISEARCH_SIDE_TAB_BAR_BG,                    //人脸搜索左侧页标签栏背景
        CN_AISEARCH_DEV_BTN_BG_SELECTED,                //设备按钮选中背景
        CN_AISEARCH_DEV_BTN_BG_UNSELECT,                //设备按钮未选中背景
        CN_AISEARCH_IMAGEWIDGET_TITLE_TEXT_COLOR,       //图片控件标题栏标题文本
        CN_AISEARCH_IMAGEWIDGET_TITLE_BG_SELECTED,      //图片控件标题栏选中背景颜色
        CN_AISEARCH_IMAGEWIDGET_TITLE_BG_UNSELECT,      //图片控件标题栏未选中背景颜色
        CN_AISEARCH_IMAGEWIDGET_BORDER_COLOR_SELECTED,  //图片控件选中边框颜色
        CN_AISEARCH_IMAGEWIDGET_BORDER_COLOR_UNSELECT,  //图片控件未选中边框颜色
        CN_AISEARCH_IMAGEWIDGET_BG_SELECTED,            //图片控件选中背景颜色
        CN_AISEARCH_IMAGEWIDGET_BG_UNSELECT,            //图片控件未选中背景颜色
        CN_AISEARCH_IMAGEWIDGET_GROUPTEXT_COLOR,        //图片控件人脸组文本颜色
        CN_AISEARCH_IMAGEWIDGET_GROUPTEXT_BG_ALLOW,     //图片控件白名单文本背景
        CN_AISEARCH_IMAGEWIDGET_GROUPTEXT_BG_BLOCK,     //图片控件黑名单文本背景
        CN_AISEARCH_IMAGEWIDGET_GROUPTEXT_BG_STRANGER,  //图片控件陌生人组文本背景
        CN_AISEARCH_IMAGEWIDGET_SUBTEXT_COLOR,          //图片控件副文本颜色
        CN_AISEARCH_INFOWIDGET_PIC_BG,                  //抓拍人脸信息界面抓拍图片所在控件背景,
        CN_AISEARCH_INFOWIDGET_LINE_COLOR,              //抓拍人脸信息界面分界线颜色
        CN_AISEARCH_ADDPICWIDGET_BG,                    //添加设备图片翻页操作栏背景
        CN_AISEARCH_INFOWIDGET_FRAME_BORDER_COLOR,      //抓拍人脸信息界面Frame边框颜色
        CN_AISEARCH_INFOWIDGET_LABEL_BG_COLOR,          //抓拍人脸信息界面标题图标颜色

        /* Data Analysis */
        CN_ANALYSIS_BARCHART_STRIPE_COLOR,              //柱状图纵向条纹颜色
        CN_ANALYSIS_BARCHART_GRID_COLOR,                //柱状图栅格线颜色

        /* AI Scene */
        CN_SCENE_CC_ENTER_COLOR,        //过线统计进入颜色
        CN_SCENE_CC_EXIT_COLOR,         //过线统计退出颜色
        CN_SCENE_CC_INSIDE_COLOR,       //过线统计内部颜色
        CN_SUB_MODULES_TITLE_BG,        //子模块标题栏背景
        CN_SCENE_ENTER_GREEN,           //过线统计进入-绿色
        CN_SCENE_ENTER_RED,             //过线统计进入-红色

        /* Cloud Alarm */
        CN_MSG_INFO_BORDER,             //云告警页信息框边框
        CN_MSG_INFO_BORDER_SELECT,      //云告警页信息框边框-选中
        CN_MSG_INFO_BG,                 //云告警页信息框背景
        CN_MSG_INFO_BG_SELECT,          //云告警页信息框背景-选中
        CN_MSG_INFO_TIME_TEXT,          //云告警页信息框时间、消息文本
        CN_MSG_TITLE_TIME_TEXT,         //云告警页标题时间文本

        /* Device Management */

        /* Alarm Config */
        CN_SIDE_TAB_BAR_BG,              //告警配置左侧页签栏背景
        CN_ALARM_SCHEDULE_UNSELECT,      //告警配置计划表未选中颜色
        CN_ALARM_SCHEDULE_SELECTED,      //告警配置计划表已选中颜色
        CN_ALARM_SCHEDULE_DIVIDE_LINE,   //告警配置计划表分割线
        CN_ALARM_SCHEDULE_COVER_BG,      //告警配置计划表拖动覆盖颜色
        CN_ALARM_EVENT_VIDEO_DEFAULT_BG, //告警事件中心录像、图片默认颜色

        /* Cruise Setting */
        CN_CRUISE_TASK_BG,               //轮询页面任务控件背景
        CN_CRUISE_TASK_BORDER,           //轮询页面任务控件边框

        /* Account Management */

        /* Local Record */

        /* System config */

        /* AI Config */

        /* Emap Config */

        /* Emap Monitoring */
    };
    Q_ENUM(COLOR_NAME)
};

#endif // COLORMANAGER_H

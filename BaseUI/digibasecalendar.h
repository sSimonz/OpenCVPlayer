#ifndef DigiBaseCalendar_H
#define DigiBaseCalendar_H

/**
  * @brief 日历基类
  * 1. 日历分为三个界面，年、月、日
  * 2. 显示日的界面里面代表日的按钮，分为四种状态，分别是前后月的天，当前月的工作日，当前月的周末，当前选中的日
  * 3. 有函数可以设置相关样式，不同客户的颜色样式在 Init() 函数中进行修改
  * 4. 该类是虚基类，有两个子类继承了该类：普通日历类DigiNormalCalendar、波斯日历类DigiPersianCalendar
  * 5. 在 DigiCalendar 类的构造函数中，会根据当前的语言来创建对应的日历类。
  * 6. 相比原日历，删去了多选功能、时间功能，保留了部分旧的接口、信号以兼容旧代码。
  *
  * version 1.0
  * @date 2023-4-19
  *
  * Copyright (c) 2007-2023, Co.,Ltd.
  * All Rights Reserved
  * Histrory:
  * Revision
  */

#include <QFrame>
#include <QDate>
#include <QMap>
#include <QPushButton>
#include "persiandate.h"
#include "digibaseframe.h"
class QVBoxLayout;
class DigiCalendarButton;
class DigiBaseToolButton;

namespace Ui {
class DigiBaseCalendar;
}

enum ButtonStatus
{
    Else=-1,
    Current,            //current month day           工作日
    Current67,          //current month weekend       周末
    CurrentSelect,      //current month select day    选中状态
    PreviousOrNext      //previous,next month day     邻月日期
};

enum ButtonTextType
{
    DAY,           //button text show day    日 按钮
    MONTH,         //button text show month  月 按钮
    YEAR           //button text show year   年 按钮
};

/**
 * @brief The CalendarButtonStyle struct
 * fontColor：按钮在四种状态下的文本颜色
 * hoverColor 和 selectColor：分别是选中和悬停的背景色
 * textType：按钮类型，年、月、日按钮
 * hasRecordMark：有录像标记的形状，目前三种，右下角三角形，底部横线，底部原点
 * hasRecordColor：标记颜色
 * fontSize: 字体大小
 */
struct CalendarButtonStyle
{
    QColor fontColor[4]{ColorManager::GetColor(ColorManager::CN_CALENDAR_CALBTN_TEXT_WORKDAY),       //工作日
                        ColorManager::GetColor(ColorManager::CN_CALENDAR_CALBTN_TEXT_WEEKEND),       //周末
                        ColorManager::GetColor(ColorManager::CN_CALENDAR_CALBTN_TEXT_SELECT),        //选中时
                        ColorManager::GetColor(ColorManager::CN_CALENDAR_CALBTN_TEXT_PREV_OR_NEXT)}; //邻月
    QColor hoverColor = ColorManager::GetColor(ColorManager::CN_CALENDAR_CALBTN_BG_HOVER);
    QColor selectColor = ColorManager::GetColor(ColorManager::CN_CALENDAR_CALBTN_BG_SELECT);
    QColor hasRecordColor = ColorManager::GetColor(ColorManager::CN_CALENDAR_CALBTN_RECORD);
    ButtonTextType textType = DAY;
};

class DigiBaseCalendar : public DigiBaseFrame
{
    Q_OBJECT

public:
    explicit DigiBaseCalendar(QWidget *parent = nullptr);
    explicit DigiBaseCalendar(const QDate& currentDate = QDate::currentDate(), QWidget *parent = nullptr);
    ~DigiBaseCalendar();

    //日期相关函数
    QList<QDate*> getSelectDates() const;         //获取已选日期List（多选）
    QDate* getSelectDate()const;                  //获取已选日期List的第一项
    QDate GetCurrentDate();                       //获取当前日历所显示的月份的日期，day()的值为1
    virtual void setSelectDate(const QDate& date);        //设置选中日期
    virtual void setSelectDates(QList<QDate> dateList);   //设置选中日期（多选）

    //获取日历显示的这个月的月初和月尾的公历日期（波斯日历会获取波斯月的首尾所对应的公历日期）
    virtual void GetFirstAndLastOfMonth(QDate &firstDate, QDate &lastDate) = 0;

    //标记相关函数
    virtual void setRecordFlag(QDate date);   //给指定日期设置标记
    void clearRecordFlag(QDate date); //给指定日期清除标记
    void clearAllRecordFlag();        //清除所有标记
    bool curDateHasRecord();          //已选中日期是否有标记
    bool curMonthHasRecord();         //当月是否有标记

    //其它函数
    void SetDateRange(const QDate &min, const QDate &max);  //设置日期范围，范围外的日期按钮不可点击
    QFont GetCustomFont();           //获取字体

    void SetScale(qreal scale);

signals:
    void dateChanged(QDate);         //日期改变信号，兼容旧代码
    void monthChanged(QDate);        //月份改变信号

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    virtual void calenderShow(const QDate& date,bool bclear = true);  //根据所给的日期，显示该日期所在月的日历

protected slots:
    void ToPrevMonth();        //跳转上个月
    void ToNextMonth();        //跳转下个月
    void ToPrevYear();         //跳转上一年
    void ToNextYear();         //跳转下一年
    void ToToday();            //跳转当天
    virtual void onMonthBtnClicked();  //点击月份按钮，打开月份选择页面
    void onYearBtnClicked();           //点击年份按钮，打开年份选择页面
    void changeYearPage(const QDate& date);  //初始化年份页面文本

    void currentDateChanged(); //点击日按钮 槽函数
    void onMonthChanged();     //点击月按钮 槽函数
    void onYearChanged();      //点击年按钮 槽函数

private:
    void Init();                  //初始化控件，包括不同客户的颜色样式
    void initCalender();          //创建日历
    virtual void InitText() = 0;  //初始化显示星期的那7个Label
    virtual void CreateConnect() = 0;

    //改变日历背景颜色、字体颜色、字体大小、hover颜色、选中颜色、有录像颜色
    void setBackGroundColor(QColor);
    void setColor(QColor cur, QColor cur67,QColor curSelect, QColor prevOrNext);
    void setHasRecordColor(QColor);
    void setHoverColor(QColor);
    void setSelectColor(QColor);
    void setTitleColor(QColor weekColor, QColor ButtonTextColor);
    void setFontSize(int size);  //设置字体基值大小
    void SetCustomFont(const QFont &font);         //设置字体

protected:
    Ui::DigiBaseCalendar *ui;
    QStringList m_MonthName;     //月份名
    QDate m_CurrentDate;         //日历当前显示的月份，day()的值为1

    QList<QDate*> m_SelectDates;                 //选中的日期
    QVector<DigiCalendarButton*> m_DayBtnVector;   //日期页面按钮数组
    QVector<DigiCalendarButton*> m_MonthBtnVector; //月份页面按钮数组
    QVector<DigiCalendarButton*> m_YearBtnVector;  //年份页面按钮数组

    DigiBaseToolButton *m_PrevYearPageBtn;   //年份翻页按钮 上一页
    DigiBaseToolButton *m_NextYearPageBtn;   //年份翻页按钮 下一页

    QDate m_YearPageCurVal;   //年份页面的当前年份

    QDate m_MinDate;   //日期范围-小
    QDate m_MaxDate;   //日期范围-大

private:
    QFont m_Font;  //字体
};

/**************
 * 日历日期按钮 *
 **************/
class DigiCalendarButton : public DigiBaseFrame
{
    Q_OBJECT

public:
    DigiCalendarButton(int index, DigiBaseCalendar *parent = nullptr, ButtonTextType type = DAY);

    //设置、获取按钮的日期
    void SetDate(int y,int m,int d);
    void SetDate(const QDate &date);
    QDate GetDate();

    //标记
    void SetHasRecord(bool h);   //设置标记
    bool GetHasRecord();         //获取是否有标记

    //状态
    void select(ButtonStatus status);          //设置按钮状态，并更新样式
    void SetButtonStatus(ButtonStatus status); //设置按钮状态
    ButtonStatus GetBtnStatus() const;         //获取按钮状态

    void SetPersianDateMode(bool on);          //设置显示波斯日期

    void SetScale(qreal scale) override;//缩放

signals:
    void BtnClicked();

protected:
    void paintEvent(QPaintEvent * event) override; //画对应类型的录像标志

private:
    void Init();
    void UpdateStyleSheet(); //更新样式

public:
    int m_Index;    //按钮下标位置
    bool setted;    //用于判断此按钮是否需要刷新样式
    CalendarButtonStyle m_style; //按钮样式结构体

private:
    DigiBaseCalendar* m_Parent;   //父窗口
    bool m_hasRecord;           //是否有标记
    QDate m_Date;               //日期
    QScopedPointer<PersianDate> m_PersianDate; //波斯日期
    ButtonStatus m_Status;      //按钮状态属性

    qreal m_Scale;           //缩放因子

    QPushButton *m_Btn;
    QVBoxLayout *m_Layout;
};

#endif // DigiBaseCalendar_H

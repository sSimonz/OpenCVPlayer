#ifndef PERSIANDATE_H
#define PERSIANDATE_H

/**
 * @brief 波斯日期类
 * 1.提供了多个公历日期转波斯日期的静态函数
 * 2.可存储波斯日期
 *
 * @version 1.0
 * @date 2023-04-14
 *
 * Copyright (c) 2007-2023, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
*/

#include <QDate>
#include <QString>

class PersianDate
{

public:
    explicit PersianDate(int Year = 0, int month = 0, int day = 0);

    //公历转成波斯历，参数传入公历时间
    static PersianDate GregorianToJalali(const QDate &date);
    static QString GregorianToJalali(const QString &dateStr, const QString &format);

    //波斯日历转公历，参数传入波斯时间
    static QString JalaliToGregorian(const QString &dateStr, const QString &format);
    static QDate JalaliToGregorian(const PersianDate &date);
    static QDate JalaliToGregorian(int j_year, int j_month, int j_day);

    //给出波斯年、月、星期几、第几个星期，返回对应的波斯日
    static int GetPersianDay(int year, int month, int week, int weekIndex);

    int Year() const;
    int Month() const;
    int Day() const;
    void SetYear(int Year);
    void SetMonth(int Month);
    void SetDay(int Day);

    bool isValid();  //判断当前的波斯历是否有效

    int DaysInMonth() const; //获取该月有多少天
    int DayIndexOfPersianWeek() const; //返回该天在波斯日历的第几列（从右往左数）
    PersianDate AddMonths(int nmonths) const;  //类似 QDate 类的 addMonths()

private:
    //公历年月日 转成 波斯历年月日
    static void gregorian_to_jalali(/* output */ int *j_y, int *j_m, int *j_d,
                                    /*  input */ int  g_y, int  g_m, int  g_d);
    //波斯历年月日转公历年月日
    static void jalali_to_gregorian(/* output */ int *g_y, int *g_m, int *g_d,
                                    /*  input */ int  j_y, int  j_m, int  j_d);

    //判断传入的波斯年是否为闰年
    static bool jalaliIsLeap(int jalaliYear);

private:
    int m_Year;  //波斯日期-年
    int m_Month; //波斯日期-月
    int m_Day;   //波斯日期-日
};

#endif // PERSIANDATE_H

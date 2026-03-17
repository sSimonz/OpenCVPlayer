#include "persiandate.h"
#include <QVector>

const int g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29}; //此处12月数值保存的是29天，闰年时是30天，使用需 +1 天

PersianDate::PersianDate(int Year, int month, int day) :
    m_Year(Year),
    m_Month(month),
    m_Day(day)
{

}

PersianDate PersianDate::GregorianToJalali(const QDate &date)
{
    int gYear = date.year();
    int gMonth = date.month();
    int gDay = date.day();

    int year, month, day;

    gregorian_to_jalali(&year, &month, &day,
                        gYear, gMonth, gDay);

    return PersianDate(year, month, day);
}

QString PersianDate::GregorianToJalali(const QString &dateStr, const QString &format)
{
    //目前字符串处理只支持部分时间格式的组合：yy、yyyy、M、MM、d、dd
    QString jDateStr = dateStr;

    //转换得到波斯时间
    QDateTime dateTime = QDateTime::fromString(dateStr, format);

    if (format.count("y") == 2) //临时处理，当年份格式是yy时，年份默认变为19XX年，需手动+100年
        dateTime = dateTime.addYears(100);

    PersianDate jDate = GregorianToJalali(dateTime.date());

    //将原字符串的日期修改成波斯日期
    //修改年
    int yIndex = format.indexOf("yy", 0);
    if (yIndex != -1)
    {
        QString yearStr = QString::number(jDate.m_Year);

        //格式为 yy 时，只显示后两位数字
        int yCount = format.count("y");
        if (yCount == 2)
            yearStr = yearStr.right(2);

        jDateStr.replace(yIndex, yCount, yearStr);
    }

    //修改月
    bool bIndexChange = false; //月份格式为 M 时，是否显示了两位数字
    int mIndex = format.indexOf("M", 0);
    if (mIndex != -1)
    {
        QString monthStr = QString::number(jDate.m_Month);
        int mCount = format.count("M");

        //格式为 MM 时且波斯月小于10时，左侧需补0
        if (mCount == 2 && jDate.m_Month<10)
            monthStr = monthStr.rightJustified(2, '0');
        else if (mCount == 1 && dateTime.date().month()>=10) //格式为 M 且公历月大于10时，mCount需要+1
            mCount = 2; //mCount++

        jDateStr.replace(mIndex, mCount, monthStr);

        if (format.count("M")==1 && monthStr.count()>1)
            bIndexChange = true;
    }

    //修改日

    int dIndex = format.indexOf("d", 0);
    if (dIndex != -1)
    {
        if (bIndexChange)
            dIndex++;

        QString dayStr = QString::number(jDate.m_Day);
        int dCount = format.count("d");

        //格式为 dd 且波斯日小于10时，左侧需补0
        if (dCount == 2 && jDate.m_Day<10)
            dayStr = dayStr.rightJustified(2, '0');
        else if (dCount == 1 && dateTime.date().day()>=10) //格式为 d 且公历日大于10时，dCount需要+1
            dCount = 2;

        jDateStr.replace(dIndex, dCount, dayStr);
    }

    return jDateStr;
}

QString PersianDate::JalaliToGregorian(const QString &dateStr, const QString &format)
{
    //目前字符串处理只支持部分时间格式的组合：yyyy、MM、dd
    QString gDateStr = dateStr;

    //取字符串，转换成公历时间
    int jYear = -1, jMonth = -1, jDay = -1;
    int year, month, day;
    int yIndex, mIndex, dIndex;

    yIndex = format.indexOf("yyyy", 0);
    if (yIndex != -1)
        jYear = dateStr.mid(yIndex, 4).toInt();

    mIndex = format.indexOf("MM", 0);
    if (mIndex != -1)
        jMonth = dateStr.mid(mIndex, 2).toInt();

    dIndex = format.indexOf("dd", 0);
    if (dIndex != -1)
        jDay = dateStr.mid(dIndex, 2).toInt();

    if (jYear == -1 || jMonth == -1 || jDay == -1)
        return QString();

    jalali_to_gregorian(&year, &month, &day,
                        jYear, jMonth, jDay);

    //修改字符串为公历时间
    if (yIndex != -1)
        gDateStr.replace(yIndex, 4, QString::number(year));
    if (mIndex != -1)
        gDateStr.replace(mIndex, 2, QString::number(month).rightJustified(2, '0'));
    if (dIndex != -1)
        gDateStr.replace(dIndex, 2, QString::number(day).rightJustified(2, '0'));

    return gDateStr;
}

QDate PersianDate::JalaliToGregorian(const PersianDate &date)
{
    int year, month, day;
    jalali_to_gregorian(&year, &month, &day,
                        date.Year(), date.Month(), date.Day());

    return QDate(year, month, day);
}

QDate PersianDate::JalaliToGregorian(int j_year, int j_month, int j_day)
{
    return JalaliToGregorian(PersianDate(j_year, j_month, j_day));
}

int PersianDate::GetPersianDay(int year, int month, int week, int weekIndex)
{
    int day;
    PersianDate firstDate(year, month, 1);
    int firstWeek = firstDate.DayIndexOfPersianWeek()+1;
    if(firstWeek > week)
        day = (weekIndex)*7 + week - (firstWeek-1);
    else
        day = (weekIndex-1)*7 + week - (firstWeek-1);

    PersianDate perDate(year, month, day);
    if (perDate.isValid() && perDate.m_Month == month)
        return day;
    else
        return -1;
}

int PersianDate::Year() const
{
    return m_Year;
}

int PersianDate::Month() const
{
    return m_Month;
}

int PersianDate::Day() const
{
    return m_Day;
}

void PersianDate::SetYear(int Year)
{
    m_Year = Year;
}

void PersianDate::SetMonth(int Month)
{
    m_Month = Month;
}

void PersianDate::SetDay(int Day)
{
    m_Day = Day;
}

bool PersianDate::isValid()
{
    if (m_Day<=0 || m_Day>DaysInMonth() || m_Month<=0 || m_Month>12 || m_Year<=0)
        return false;

    return true;
}

int PersianDate::DaysInMonth() const
{
    if (m_Month<1 || m_Month>12)
        return 0;

    int day = j_days_in_month[m_Month-1];

    //是否是闰年
    if (jalaliIsLeap(m_Year) && m_Month==12)
        day+=1;

    return day;
}

int PersianDate::DayIndexOfPersianWeek() const
{
    QDate date = PersianDate::JalaliToGregorian(*this);

    if (!date.isValid())
        return -1;

    return (date.dayOfWeek()+1) % 7; //周六在第0列，周五在第6列，右往左
}

PersianDate PersianDate::AddMonths(int nmonths) const
{
    PersianDate date;

    int addYear  = nmonths / 12;
    int addMonth = nmonths % 12;

    if (m_Month + addMonth <= 0) //-10 ~ 0
    {
        addYear -= 1;
        addMonth += 12;
    }
    else if (m_Month + addMonth > 12) //13 ~ 23
    {
        addYear += 1;
        addMonth -= 12;
    }

    date.SetYear(m_Year + addYear);
    date.SetMonth(m_Month + addMonth);

    //日 需要判断，存不存在
    int day = m_Day;
    switch (date.Month())
    {
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            if (m_Day >= 31)
                day = 30;
            break;
        case 12:
            if (jalaliIsLeap(m_Year) && m_Day>=31)
                day = 30;
            else if (!jalaliIsLeap(m_Year) && m_Day>=30)
                day = 29;
            break;
        default:
            break;
    }
    date.SetDay(day);

    return date;
}

void PersianDate::gregorian_to_jalali(int *j_y, int *j_m, int *j_d, int g_y, int g_m, int g_d)
{
    int gy, gm, gd;
    int jy, jm, jd;
    long g_day_no, j_day_no;
    int j_np;

    int i;

    gy = g_y-1600;
    gm = g_m-1;
    gd = g_d-1;

    g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
    for (i=0;i<gm;++i)
       g_day_no += g_days_in_month[i];
    if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
       /* leap and after Feb */
       ++g_day_no;
    g_day_no += gd;

    j_day_no = g_day_no-79;

    j_np = j_day_no / 12053;
    j_day_no %= 12053;

    jy = 979+33*j_np+4*(j_day_no/1461);
    j_day_no %= 1461;

    if (j_day_no >= 366) {
       jy += (j_day_no-1)/365;
       j_day_no = (j_day_no-1)%365;
    }

    for (i = 0; i < 11 && j_day_no >= j_days_in_month[i]; ++i) {
       j_day_no -= j_days_in_month[i];
    }
    jm = i+1;
    jd = j_day_no+1;
    *j_y = jy;
    *j_m = jm;
    *j_d = jd;
}

void PersianDate::jalali_to_gregorian(int *g_y, int *g_m, int *g_d, int j_y, int j_m, int j_d)
{
    int gy, gm, gd;
    int jy, jm, jd;
    long g_day_no, j_day_no;
    int leap;

    int i;

    jy = j_y-979;
    jm = j_m-1;
    jd = j_d-1;

    j_day_no = 365*jy + (jy/33)*8 + (jy%33+3)/4;
    for (i=0; i < jm; ++i)
       j_day_no += j_days_in_month[i];

    j_day_no += jd;

    g_day_no = j_day_no+79;

    gy = 1600 + 400*(g_day_no/146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */
    g_day_no = g_day_no % 146097;

    leap = 1;
    if (g_day_no >= 36525) /* 36525 = 365*100 + 100/4 */
    {
       g_day_no--;
       gy += 100*(g_day_no/36524); /* 36524 = 365*100 + 100/4 - 100/100 */
       g_day_no = g_day_no % 36524;

       if (g_day_no >= 365)
          g_day_no++;
       else
          leap = 0;
    }

    gy += 4*(g_day_no/1461); /* 1461 = 365*4 + 4/4 */
    g_day_no %= 1461;

    if (g_day_no >= 366) {
       leap = 0;

       g_day_no--;
       gy += g_day_no/365;
       g_day_no = g_day_no % 365;
    }

    for (i = 0; g_day_no >= g_days_in_month[i] + (i == 1 && leap); i++)
       g_day_no -= g_days_in_month[i] + (i == 1 && leap);
    gm = i+1;
    gd = g_day_no+1;

    *g_y = gy;
    *g_m = gm;
    *g_d = gd;
}

bool PersianDate::jalaliIsLeap(int perYear)
{
    int remainder = perYear % 33;
    QList<int> leap_years_remainders = {1, 5, 9, 13, 17, 22, 26, 30};

    for (int i : leap_years_remainders)
    {
        if (remainder == i)
            return true;
    }
    return false;
}

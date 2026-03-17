#include "digipersiancalendar.h"
#include "ui_digibasecalendar.h"

#include "persiandate.h"

DigiPersianCalendar::DigiPersianCalendar(QWidget *parent) :
    DigiBaseCalendar(parent)
{
    InitText();
    CreateConnect();
    ReSetBtnIndex();
    AdjustFirstShowMonth();
    calenderShow(m_CurrentDate);
}

DigiPersianCalendar::DigiPersianCalendar(const QDate &currentDate, QWidget *parent) :
    DigiBaseCalendar(currentDate, parent)
{
    InitText();
    CreateConnect();
    ReSetBtnIndex();
    AdjustFirstShowMonth();
    calenderShow(m_CurrentDate);
}

void DigiPersianCalendar::setSelectDate(const QDate &date)
{
    for(int i=0;i<m_SelectDates.count();++i)
    {
        delete m_SelectDates[i];
    }
    m_SelectDates.clear();
    m_SelectDates.append(new QDate(date));
    m_CurrentDate.setDate(date.year(), date.month(), 1);
    AdjustFirstShowMonth();     //重写多执行了此函数
    calenderShow(date, false);

    emit dateChanged(date);             //兼容旧代码
}

void DigiPersianCalendar::setSelectDates(QList<QDate> dateList)
{
    for(int i = 0;i<m_SelectDates.count();++i)
    {
        delete m_SelectDates[i];
    }
    m_SelectDates.clear();

    for(QDate date: dateList)
        m_SelectDates.append(new QDate(date));

    if(dateList.count() != 0)
    {
        QDate date = dateList.first();
        m_CurrentDate.setDate(date.year(), date.month(), 1);
        AdjustFirstShowMonth();     //重写多执行了此函数
        calenderShow(date, false);

        emit dateChanged(date);             //兼容旧代码
    }
}

void DigiPersianCalendar::setRecordFlag(QDate date)
{
    //如果要设置标志的日期没有显示在当前日历上，直接返回
    if (date < m_DayBtnVector.first()->GetDate()
        || date > m_DayBtnVector.last()->GetDate())
        return;

    for(int i=0; i<m_DayBtnVector.count(); i++)
    {
        auto btn = m_DayBtnVector.at(i);
        if(date==btn->GetDate() && btn->GetBtnStatus()!=PreviousOrNext)  //邻月日期不设置标志
        {
            if (btn->GetBtnStatus() == CurrentSelect)  //判断当前选中的日期是不是邻月
            {
                QDate firstDate, lastDate;
                GetFirstAndLastOfMonth(firstDate, lastDate); //获取月初和月尾
                if (date<firstDate || date>lastDate)
                    continue;
            }

            btn->SetHasRecord(true);
            break;
        }
    }
}

void DigiPersianCalendar::GetFirstAndLastOfMonth(QDate &firstDate, QDate &lastDate)
{
    PersianDate perDate = PersianDate::GregorianToJalali(m_CurrentDate);

    firstDate = PersianDate::JalaliToGregorian(perDate.Year(), perDate.Month(), 1);
    lastDate  = PersianDate::JalaliToGregorian(perDate.Year(), perDate.Month(), perDate.DaysInMonth());
}

void DigiPersianCalendar::InitText()
{
    ui->label_1->setText("F");
    ui->label_2->setText("T");
    ui->label_3->setText("W");
    ui->label_4->setText("T");
    ui->label_5->setText("M");
    ui->label_6->setText("S");
    ui->label_7->setText("S");
}

void DigiPersianCalendar::CreateConnect()
{
    //波斯日历，翻页，左边是下一页，右边是上一页
    connect(ui->m_pMonPrevBtn,  &QPushButton::clicked, this, &DigiPersianCalendar::ToNextMonth);
    connect(ui->m_pMonNextBtn,  &QPushButton::clicked, this, &DigiPersianCalendar::ToPrevMonth);
    connect(ui->m_pYearPrevBtn, &QPushButton::clicked, this, &DigiPersianCalendar::ToNextYear);
    connect(ui->m_pYearNextBtn, &QPushButton::clicked, this, &DigiPersianCalendar::ToPrevYear);
    connect(ui->m_pMonthButton, &QPushButton::clicked, this, &DigiPersianCalendar::onMonthBtnClicked);
    connect(ui->m_pYearButton,  &QPushButton::clicked, this, &DigiPersianCalendar::onYearBtnClicked);

    //年份页，跳转下一页（波斯历）
    connect(m_PrevYearPageBtn, &QToolButton::clicked, this, [this]()
    {
        m_YearPageCurVal = m_YearPageCurVal.addYears(12);
        changeYearPage(m_YearPageCurVal);
    });

    //年份页，跳转上一页（波斯历）
    connect(m_NextYearPageBtn, &QToolButton::clicked, this, [this]()
    {
        m_YearPageCurVal = m_YearPageCurVal.addYears(-12);
        changeYearPage(m_YearPageCurVal);
    });

    connect(ui->stackedWidget,&QStackedWidget::currentChanged,this,[this](int index)
    {
        if(index == 1) //年份页
        {
            m_YearPageCurVal = m_CurrentDate;
        }
    });

    //日期按钮
    qint8 i = 0;
    for(i=0; i<m_YearBtnVector.size(); i++)
    {
        connect(m_YearBtnVector.at(i), &DigiCalendarButton::BtnClicked, this, &DigiPersianCalendar::onYearChanged);
    }
    for(i=0; i<m_MonthBtnVector.size(); i++)
    {
        connect(m_MonthBtnVector.at(i), &DigiCalendarButton::BtnClicked, this, &DigiPersianCalendar::onMonthChanged);
    }
    for(i=0; i<m_DayBtnVector.size(); i++)
    {
        connect(m_DayBtnVector.at(i), &DigiCalendarButton::BtnClicked, this, &DigiPersianCalendar::currentDateChanged);
    }
}

void DigiPersianCalendar::calenderShow(const QDate &date, bool bclear)
{
    //清空录像标志
    if(bclear)
        clearAllRecordFlag();

    PersianDate perDate = PersianDate::GregorianToJalali(date);

    //设置月份和年份的文本
    ui->m_pYearButton->setText(QString::number(perDate.Year()));
    ui->m_pMonthButton->setText(m_MonthName.at(perDate.Month()-1));

    //给所有 日按钮 设置成 需刷新 的状态
    for(int i = 0; i < m_DayBtnVector.count(); ++i)
    {
        m_DayBtnVector[i]->setted=false;
    }

    //获取选中按钮第一天的日期
    QDate selectDate;
    PersianDate selectPerDate;
    int selectCount = m_SelectDates.count();
    if(!m_SelectDates.isEmpty())
    {
        selectDate = *m_SelectDates.first();
        selectPerDate = PersianDate::GregorianToJalali(selectDate);
    }

    //更新当前月日期按钮
    PersianDate currentPerdate(perDate.Year(), perDate.Month(), 1);
    int weekFirst = currentPerdate.DayIndexOfPersianWeek();  //获取本月第一天在哪一列（右往左数）
    int currentDays = perDate.DaysInMonth(); //当前月的天数
    int currentYear  = perDate.Year();
    int currentMonth = perDate.Month();

    //更新当前月选中日期样式
    if(!selectDate.isNull() && selectPerDate.Year()==currentYear && selectPerDate.Month()==currentMonth)
    {
        QList<PersianDate> perDateSelectList = GetPerDateListFromQDateList(m_SelectDates);

        for(int i=0; i<selectCount; ++i)
        {
            DigiCalendarButton* day = m_DayBtnVector[weekFirst + perDateSelectList[i].Day() - 1];
            day->SetDate(*m_SelectDates[i]);
            day->select(CurrentSelect);
            day->setted=true;
        }
    }
    //更新当前月其他日期的样式
    for(int i=0; i<currentDays; ++i)
    {
        DigiCalendarButton* day = m_DayBtnVector[weekFirst + i];
        if(!day->setted)
        {
            QDate date(PersianDate::JalaliToGregorian(currentYear, currentMonth, i+1));
            day->SetDate(date);

            int weekNum = date.dayOfWeek(); //该天是否是周末
            if(weekNum == 5)
            {
                day->select(Current67);
                day->setted=true;
            }
            else
            {
                day->select(Current);
                day->setted=true;
            }
        }
    }


    //更新上个月日期
    PersianDate prveious = currentPerdate.AddMonths(-1);
    int prveiousDays  = prveious.DaysInMonth();
    int prveiousYear  = prveious.Year();
    int prveiousMonth = prveious.Month();

    //更新上个月选中日期样式
    if(!selectDate.isNull() && selectPerDate.Year()==prveiousYear && selectPerDate.Month()==prveiousMonth)
    {
        QList<PersianDate> perDateSelectList = GetPerDateListFromQDateList(m_SelectDates);
        for(int i=0; i<selectCount; ++i)
        {
            int n = prveiousDays - perDateSelectList[i].Day();
            if(n <= weekFirst)
            {
                DigiCalendarButton* day = m_DayBtnVector[weekFirst - n - 1];
                day->SetDate(*m_SelectDates[i]);
                day->select(CurrentSelect);
                day->setted=true;
            }
        }
    }
    //更新上个月其他日期的样式
    for(int i=0; i<weekFirst; ++i)
    {
        DigiCalendarButton* day = m_DayBtnVector[weekFirst - 1 - i];
        QDate date(PersianDate::JalaliToGregorian(prveiousYear, prveiousMonth, prveiousDays-i));
        day->SetDate(date);

        if(!day->setted)
        {
            day->select(PreviousOrNext);
            day->setted=true;
        }
    }

    //更新下个月日期
    PersianDate next = currentPerdate.AddMonths(1);
    int nextYear  = next.Year();
    int nextMonth = next.Month();
    int pastDays  = currentDays + weekFirst;   //上个月显示的天数 + 当月天数
    int nextDays  = m_DayBtnVector.count() - pastDays; //下个月显示的天数
    //更新下个月选中日期样式
    if(!selectDate.isNull() && selectPerDate.Year()==nextYear && selectPerDate.Month()==nextMonth)
    {
        QList<PersianDate> perDateSelectList = GetPerDateListFromQDateList(m_SelectDates);
        for(int i=0; i<selectCount; ++i)
        {
            if(perDateSelectList[i].Day() <= nextDays)
            {
                DigiCalendarButton* day = m_DayBtnVector[pastDays + perDateSelectList[i].Day() - 1];
                day->SetDate(*m_SelectDates[i]);
                day->select(CurrentSelect);
                day->setted = true;
            }
        }
    }
    //更新下个月其他日期的样式
    for(int i = 0; i< nextDays; ++i)
    {
        DigiCalendarButton* day = m_DayBtnVector[pastDays+i];
        QDate date(PersianDate::JalaliToGregorian(nextYear, nextMonth, i+1));
        day->SetDate(date);

        if(!day->setted)
        {
            day->select(PreviousOrNext);
            day->setted=true;
        }
    }

    //年、月按钮刷新
    for(DigiCalendarButton *btn : m_YearBtnVector)
    {
        btn->select(Current);
        btn->setted = true;
    }
    for(DigiCalendarButton *btn : m_MonthBtnVector)
    {
        btn->select(Current);
        btn->setted = true;
    }

    //日期范围设定
    if(!m_MinDate.isNull() && !m_MaxDate.isNull())
    {
        if (m_MaxDate >= m_MinDate)
        {
            foreach(DigiCalendarButton *btn, m_DayBtnVector)
            {
                QDate date = btn->GetDate();
                bool unEnable = (date<m_MinDate || date>m_MaxDate);
                btn->setEnabled(!unEnable);
            }
        }
    }
}

void DigiPersianCalendar::AdjustFirstShowMonth()
{
    PersianDate perDate = PersianDate::GregorianToJalali(*m_SelectDates.first());

    int monthDiff = m_CurrentDate.month() - perDate.Month();

    if (monthDiff < 0) //公历 1~3 月，波斯日历 10~12月，相减小于0
        monthDiff+=12;

    if (monthDiff != 3) //差值不等于3，需要调整
        m_CurrentDate = m_CurrentDate.addMonths(1);
}

void DigiPersianCalendar::ReSetBtnIndex()
{
    //波斯日历日期是从右往左变大，按钮下标需重新排序
    QVector<DigiCalendarButton*> temp;
    int row, column;

    //日期 6行7列
    row = 6;
    column = 7;
    temp = m_DayBtnVector;
    m_DayBtnVector.clear();
    for(int i=0; i<row; i++)
    {
        for(int j=column-1; j>=0; j--)
        {
            auto btn = temp.at(i*column + j);
            btn->m_Index = m_DayBtnVector.count();
            btn->SetPersianDateMode(true); //开启显示波斯日期
            m_DayBtnVector.append(btn);
        }
    }

    //月份页 3行4列
    row = 3;
    column = 4;
    temp = m_MonthBtnVector;
    m_MonthBtnVector.clear();
    for(int i=0; i<row; i++)
    {
        for(int j=column-1; j>=0; j--)
        {
            auto btn = temp.at(i*column + j);
            btn->m_Index = m_MonthBtnVector.count();
            btn->SetPersianDateMode(true); //开启显示波斯日期
            m_MonthBtnVector.append(btn);
        }
    }

    //年份页 4行3列
    row = 4;
    column = 3;
    temp = m_YearBtnVector;
    m_YearBtnVector.clear();
    for(int i=0; i<row; i++)
    {
        for(int j=column-1; j>=0; j--)
        {
            auto btn = temp.at(i*column + j);
            btn->m_Index = m_YearBtnVector.count();
            btn->SetPersianDateMode(true); //开启显示波斯日期
            m_YearBtnVector.append(btn);
        }
    }
}

QList<PersianDate> DigiPersianCalendar::GetPerDateListFromQDateList(const QList<QDate *> datelist)
{
    QList<PersianDate> perDateList;
    for (int i=0; i<datelist.count(); i++)
    {
        perDateList << PersianDate::GregorianToJalali(*datelist.at(i));
    }
    return perDateList;
}

void DigiPersianCalendar::onMonthBtnClicked()
{
    ui->m_pMonPrevBtn->setVisible(false);
    ui->m_pMonNextBtn->setVisible(false);

    //获取当年波斯年对应的公历年，存入date中
    PersianDate currPerDate(1, 1, 1);
    currPerDate.SetYear(PersianDate::GregorianToJalali(m_CurrentDate).Year());
    QDate date(PersianDate::JalaliToGregorian(currPerDate).year(), 1, 1);

    //波斯日历1月 约等于公历4月，所以月数需要+3
    int startMonth = 3;
    for(int i=0; i<m_MonthBtnVector.count(); i++)
    {
        m_MonthBtnVector[i]->SetDate(date.addMonths(startMonth + i));
        m_MonthBtnVector[i]->setted = true;
        m_MonthBtnVector[i]->select(Current);
    }

    ui->stackedWidget->setCurrentIndex(2);
}

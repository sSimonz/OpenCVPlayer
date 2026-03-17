#include "digibasecalendar.h"
#include "ui_digibasecalendar.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include "Res/resfun.h"
#include "digibasetoolbutton.h"
#include "digibaselabel.h"

DigiBaseCalendar::DigiBaseCalendar(QWidget *parent) :
    DigiBaseFrame(parent),
    ui(new Ui::DigiBaseCalendar),
    m_PrevYearPageBtn(nullptr),
    m_NextYearPageBtn(nullptr)
{
    ui->setupUi(this);

    QDate date = QDate::currentDate();
    m_SelectDates.append(new QDate(date));
    m_CurrentDate.setDate(date.year(), date.month(), 1);

    Init();
}

DigiBaseCalendar::DigiBaseCalendar(const QDate& currentDate, QWidget *parent) :
    DigiBaseFrame(parent),
    ui(new Ui::DigiBaseCalendar),
    m_PrevYearPageBtn(nullptr),
    m_NextYearPageBtn(nullptr)
{
    ui->setupUi(this);

    m_SelectDates.append(new QDate(currentDate));
    m_CurrentDate.setDate(currentDate.year(), currentDate.month(), 1);

    Init();
}

DigiBaseCalendar::~DigiBaseCalendar()
{
    for(int i=0;i<m_SelectDates.count();++i)
    {
        delete m_SelectDates[i];
    }
    delete ui;
}

QList<QDate*> DigiBaseCalendar::getSelectDates() const
{
    return m_SelectDates;
}

QDate* DigiBaseCalendar::getSelectDate()const
{
    return m_SelectDates.first();
}

QDate DigiBaseCalendar::GetCurrentDate()
{
    return m_CurrentDate;
}

void DigiBaseCalendar::setSelectDate(const QDate& date)
{
    for(int i=0;i<m_SelectDates.count();++i)
    {
        delete m_SelectDates[i];
    }
    m_SelectDates.clear();
    m_SelectDates.append(new QDate(date));
    m_CurrentDate.setDate(date.year(), date.month(), 1);
    calenderShow(date, false);

    emit dateChanged(date);             //兼容旧代码
}

void DigiBaseCalendar::setSelectDates(QList<QDate> dateList)
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
        calenderShow(date, false);

        emit dateChanged(date);             //兼容旧代码
    }
}

void DigiBaseCalendar::setRecordFlag(QDate date)
{
    //如果要设置标志的日期没有显示在当前日历上，直接返回
    if (date < m_DayBtnVector.first()->GetDate()
        || date > m_DayBtnVector.last()->GetDate())
        return;

    for(int i=0; i<m_DayBtnVector.count(); i++)
    {
        if(date == m_DayBtnVector[i]->GetDate())
        {
            m_DayBtnVector[i]->SetHasRecord(true);
            break;
        }
    }
}

void DigiBaseCalendar::clearRecordFlag(QDate date)
{
    //如果要清除标志的日期没有显示在当前日历上，直接返回
    if (date < m_DayBtnVector.first()->GetDate()
        || date > m_DayBtnVector.last()->GetDate())
        return;

    for(int i = 0; i<m_DayBtnVector.count(); i++)
    {
        if(date == m_DayBtnVector[i]->GetDate())
        {
            m_DayBtnVector[i]->SetHasRecord(false);
            break;
        }
    }
}

void DigiBaseCalendar::clearAllRecordFlag()
{
    for(int i = 0; i<m_DayBtnVector.count(); i++)
    {
        m_DayBtnVector[i]->SetHasRecord(false);
    }
}

bool DigiBaseCalendar::curDateHasRecord()
{
    if (m_SelectDates.isEmpty())
        return false;

    QDate *selelct = nullptr;
    selelct = m_SelectDates.first();
    if(selelct == nullptr)
        return false;

    foreach (DigiCalendarButton* var, m_DayBtnVector)
    {
       if(var->GetHasRecord())
       {
           QDate recDate = var->GetDate();
           if(recDate == *selelct)
               return true;
       }
    }
    return false;
}

bool DigiBaseCalendar::curMonthHasRecord()
{
    foreach (DigiCalendarButton* var, m_DayBtnVector)
    {
       if(var->GetHasRecord())
       {
            return true;
       }
    }
    return false;
}

void DigiBaseCalendar::SetDateRange(const QDate &min, const QDate &max)
{
    m_MinDate = min;
    m_MaxDate = max;
    calenderShow(m_CurrentDate);
}

QFont DigiBaseCalendar::GetCustomFont()
{
    return m_Font;
}

void DigiBaseCalendar::SetScale(qreal scale)
{
    DigiBaseFrame::SetScale(scale);
}

bool DigiBaseCalendar::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->stackedWidget && event->type() == QEvent::Leave)
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->m_pMonPrevBtn->setVisible(true);
        ui->m_pMonNextBtn->setVisible(true);
        ui->m_pYearPrevBtn->setVisible(true);
        ui->m_pYearNextBtn->setVisible(true);
//        ui->m_pTodayBtn->setVisible(true);
        return true;
    }

    return false;
}

void DigiBaseCalendar::Init()
{
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();
    m_Font.setPixelSize(14);

    //星期名称Label
    auto labelList = ui->frame->findChildren<DigiBaseLabel*>();
    foreach(auto label, labelList)
    {
        label->SetWidth(20);
        label->SetHeight(20);
        label->SetTextColor(ColorManager::GetColor(ColorManager::CN_CALENDAR_WEEKLAB_TEXT));
        label->SetCustomFont(m_Font);
    }

    //初始化各类控件
    m_PrevYearPageBtn = new DigiBaseToolButton(this);
    m_PrevYearPageBtn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_PLAYBACK_LASTYEAR));
    m_PrevYearPageBtn->SetWidth(20);
    m_PrevYearPageBtn->SetHeight(20);
    m_PrevYearPageBtn->SetCustomIconSize(QSize(20,20));
    m_PrevYearPageBtn->SetPadding(0,2);
    m_PrevYearPageBtn->SetMargin(0,0);

    m_NextYearPageBtn = new DigiBaseToolButton(this);
    m_NextYearPageBtn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_PLAYBACK_NEXTYEAR));
    m_NextYearPageBtn->SetWidth(20);
    m_NextYearPageBtn->SetHeight(20);
    m_NextYearPageBtn->SetCustomIconSize(QSize(20,20));
    m_NextYearPageBtn->SetPadding(0,2);
    m_NextYearPageBtn->SetMargin(0,0);

    ui->m_pMonPrevBtn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_PLAYBACK_LASTMONTH));
    ui->m_pMonPrevBtn->SetWidth(20);
    ui->m_pMonPrevBtn->SetHeight(20);
    ui->m_pMonPrevBtn->SetCustomIconSize(QSize(20,20));
    ui->m_pMonPrevBtn->SetPadding(0,2);
    ui->m_pMonPrevBtn->SetMargin(0,0);

    ui->m_pMonNextBtn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_PLAYBACK_NEXTMONTH));
    ui->m_pMonNextBtn->SetWidth(20);
    ui->m_pMonNextBtn->SetHeight(20);
    ui->m_pMonNextBtn->SetCustomIconSize(QSize(20,20));
    ui->m_pMonNextBtn->SetPadding(0,2);
    ui->m_pMonNextBtn->SetMargin(0,0);

    ui->m_pYearPrevBtn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_PLAYBACK_LASTYEAR));
    ui->m_pYearPrevBtn->SetWidth(20);
    ui->m_pYearPrevBtn->SetHeight(20);
    ui->m_pYearPrevBtn->SetCustomIconSize(QSize(20,20));
    ui->m_pYearPrevBtn->SetPadding(0,2);
    ui->m_pYearPrevBtn->SetMargin(0,0);

    ui->m_pYearNextBtn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_PLAYBACK_NEXTYEAR));
    ui->m_pYearNextBtn->SetWidth(20);
    ui->m_pYearNextBtn->SetHeight(20);
    ui->m_pYearNextBtn->SetCustomIconSize(QSize(20,20));
    ui->m_pYearNextBtn->SetPadding(0,2);
    ui->m_pYearNextBtn->SetMargin(0,0);

    ui->m_pMonthButton->SetHeight(20);
    // ui->m_pMonthButton->SetWidth(32);
    ui->m_pMonthButton->SetPadding(0,0);
    ui->m_pMonthButton->SetMargin(0,0);
    QFont font = GetGlobalFont();
    font.setPixelSize(14);
    font.setFamily(GetGlobalFamily(QFont::Medium));
    ui->m_pMonthButton->SetCustomFont(font);

    ui->m_pYearButton->SetHeight(20);
    ui->m_pYearButton->SetWidth(32);
    ui->m_pYearButton->SetPadding(0,0);
    ui->m_pYearButton->SetMargin(0,0);
    ui->m_pYearButton->SetCustomFont(font);

    ui->m_pTodayBtn->SetHeight(20);
    ui->m_pTodayBtn->SetWidth(50);
    ui->m_pTodayBtn->setText("Today");
    ui->m_pTodayBtn->SetStyleSize(10, 0, 0, 1);
    ui->m_pTodayBtn->SetCustomFont(m_Font);
    ui->m_pTodayBtn->setVisible(false);

    ui->frame->SetHeight(20);

    //创建日历
    initCalender();

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->installEventFilter(this);
}

void DigiBaseCalendar::initCalender()
{
    m_MonthName << "Jan" << "Feb"
                << "Mar" << "Apr"
                << "May" << "Jun"
                << "Jul" << "Aug"
                << "Sep" << "Oct"
                << "Nov" << "Dec";

    //日期按钮添加至布局
    int row = 0;
    int column = 0;

    //日期 6行7列
    row = 6;
    column = 7;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<column; j++)
        {
            DigiCalendarButton* button = new DigiCalendarButton(i*column+j, this, DAY);
            button->setAttribute(Qt::WA_LayoutUsesWidgetRect);
            button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
            m_DayBtnVector.append(button);
            ui->gridLayout->addWidget(button,i,j);
        }
    }

    //月份页 3行4列
    row = 3;
    column = 4;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<column; j++)
        {
            DigiCalendarButton *button = new DigiCalendarButton(i*column+j, this, MONTH);
            button->setAttribute(Qt::WA_LayoutUsesWidgetRect);
            button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
            m_MonthBtnVector.append(button);
            ui->gridLayout_3->addWidget(button,i,j);
        }
    }

    //年份页 4行3列
    row = 4;
    column = 3;
    for(int i=0; i<row; i++)
    {
        for(int j=1; j<column+1; j++) //j=1是因为要考虑年份翻页按钮的位置
        {
            DigiCalendarButton *button = new DigiCalendarButton(i*column+j-1, this, YEAR);
            button->setAttribute(Qt::WA_LayoutUsesWidgetRect);
            button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
            m_YearBtnVector.append(button);
            ui->gridLayout_2->addWidget(button,i,j);
        }
    }
    ui->gridLayout_2->addWidget(m_PrevYearPageBtn,1,0,2,1);
    ui->gridLayout_2->addWidget(m_NextYearPageBtn,1,4,2,1);
}

void DigiBaseCalendar::calenderShow(const QDate& date,bool bclear)
{
    //清空录像标志
    if(bclear)
        clearAllRecordFlag();

    //设置月份和年份的文本
    ui->m_pYearButton->setText(QString::number(date.year()));
    ui->m_pMonthButton->setText(m_MonthName.at(date.month()-1));

    //给所有日按钮设置成 需刷新 的状态
    for(int i = 0; i < m_DayBtnVector.count(); ++i)
    {
        m_DayBtnVector[i]->setted=false;
    }

    //获取选中按钮第一天的日期
    QDate selectDate;
    int selectCount = m_SelectDates.count();
    if(!m_SelectDates.isEmpty())
        selectDate = *m_SelectDates.first();

    //更新当前月日期按钮
    QDate current(date.year(), date.month(), 1);
    int weekFirst = current.dayOfWeek();   //获取本月第一天是星期几，用于定位第一天的按钮位置
    int currentDays  = date.daysInMonth(); //当前月的天数
    int currentYear  = date.year();
    int currentMonth = date.month();
    //更新当前月选中日期样式
    if(!selectDate.isNull() && selectDate.year()==currentYear && selectDate.month()==currentMonth)
    {
        for(int i = 0; i<selectCount; ++i)
        {
            //找到对应的按钮进行设置
            DigiCalendarButton* day = m_DayBtnVector[weekFirst - 1 + m_SelectDates[i]->day()];
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
            QDate date(currentYear, currentMonth, i+1);
            day->SetDate(date);
            int weekNum = date.dayOfWeek(); //该天是否是周末

            if(weekNum==7 || weekNum==6)
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
    QDate prveious = current.addMonths(-1);
    int prveiousDays  = prveious.daysInMonth();
    int prveiousYear  = prveious.year();
    int prveiousMonth = prveious.month();
    //更新上个月选中日期样式
    if(!selectDate.isNull() && selectDate.year()==prveiousYear && selectDate.month()==prveiousMonth)
    {
        for(int i=0; i<selectCount; ++i)
        {
            int n = prveiousDays-m_SelectDates[i]->day();
            if(n < weekFirst)
            {
                DigiCalendarButton* day=m_DayBtnVector[weekFirst - 1 - n];
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
        day->SetDate(prveiousYear, prveiousMonth, prveiousDays-i);
        if(!day->setted)
        {
            day->select(PreviousOrNext);
            day->setted=true;
        }
    }

    //更新下个月日期
    QDate next = current.addMonths(1);
    int nextYear  = next.year();
    int nextMonth = next.month();
    int pastDays  = currentDays + weekFirst;   //上个月显示的天数 + 当月天数
    int nextDays  = m_DayBtnVector.count() - pastDays; //下个月显示的天数
    //更新下个月选中日期样式
    if(!selectDate.isNull() && selectDate.year()==nextYear && selectDate.month()==nextMonth)
    {
        for(int i=0; i<selectCount; ++i)
        {
            if(m_SelectDates[i]->day() <= nextDays)
            {
                DigiCalendarButton* day = m_DayBtnVector[pastDays + m_SelectDates[i]->day() - 1];
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
        day->SetDate(nextYear, nextMonth, i+1);
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

void DigiBaseCalendar::changeYearPage(const QDate &date)
{
    int curYear = date.year();

    for(int i=0;i<m_YearBtnVector.count();i++)
    {
        int year = curYear-6+i;
        m_YearBtnVector[i]->SetButtonStatus(Else);
        m_YearBtnVector[i]->SetDate(year, date.month(), date.day()); //第七个年份按钮显示当前年
        m_YearBtnVector[i]->setted = true;
        m_YearBtnVector[i]->select(Current);
    }
}

void DigiBaseCalendar::ToPrevMonth()
{
    m_CurrentDate =  m_CurrentDate.addMonths(-1);
    calenderShow(m_CurrentDate);
    emit monthChanged(m_CurrentDate);

    emit dateChanged(m_CurrentDate);                      //兼容旧代码
}

void DigiBaseCalendar::ToNextMonth()
{
   m_CurrentDate =  m_CurrentDate.addMonths(1);
   calenderShow(m_CurrentDate);
   emit monthChanged(m_CurrentDate);

   emit dateChanged(m_CurrentDate);                      //兼容旧代码
}

void DigiBaseCalendar::ToPrevYear()
{
    m_CurrentDate =  m_CurrentDate.addYears(-1);
    calenderShow(m_CurrentDate);
    emit monthChanged(m_CurrentDate);

    emit dateChanged(m_CurrentDate);                      //兼容旧代码
}

void DigiBaseCalendar::ToNextYear()
{
    m_CurrentDate =  m_CurrentDate.addYears(1);
    calenderShow(m_CurrentDate);
    emit monthChanged(m_CurrentDate);

    emit dateChanged(m_CurrentDate);                      //兼容旧代码
}

void DigiBaseCalendar::ToToday()
{
    //波斯历暂未验证
    QDate today = QDate::currentDate();
    QDate currentSelectDate = *m_SelectDates.first();

    //如果选中日期没变，且当前显示的月份也不需改变，则直接返回
    if (today == currentSelectDate
        && today.month() == m_CurrentDate.month()
        && today.year() == m_CurrentDate.year())
        return;

    //将今天的日期选中
    qDeleteAll(m_SelectDates);
    m_SelectDates.clear();
    m_SelectDates.append(new QDate(today));

    //月份是否有变
    if (today.month()!=m_CurrentDate.month() || today.year()!=m_CurrentDate.year())
    {
        m_CurrentDate.setDate(today.year(), today.month(), 1);
        calenderShow(today);
        emit monthChanged(m_CurrentDate);
    }
    else
    {
        calenderShow(today, false);
    }

    emit dateChanged(today);  //兼容旧代码
}

void DigiBaseCalendar::onYearBtnClicked()
{
    ui->m_pMonPrevBtn->setVisible(false);
    ui->m_pMonNextBtn->setVisible(false);
    ui->m_pYearPrevBtn->setVisible(false);
    ui->m_pYearNextBtn->setVisible(false);
    ui->m_pTodayBtn->setVisible(false);

    changeYearPage(m_CurrentDate);

    ui->stackedWidget->setCurrentIndex(1);
}

void DigiBaseCalendar::onMonthBtnClicked()
{
    ui->m_pMonPrevBtn->setVisible(false);
    ui->m_pMonNextBtn->setVisible(false);
    ui->m_pYearPrevBtn->setVisible(false);
    ui->m_pYearNextBtn->setVisible(false);
    ui->m_pTodayBtn->setVisible(false);

    for(int i=0; i<m_MonthBtnVector.count(); i++)
    {
        m_MonthBtnVector[i]->SetDate(m_CurrentDate.year(), i+1, 1);
        m_MonthBtnVector[i]->setted = true;
        m_MonthBtnVector[i]->select(Current);
    }

    ui->stackedWidget->setCurrentIndex(2);
}

void DigiBaseCalendar::currentDateChanged()
{
    qDeleteAll(m_SelectDates);
    m_SelectDates.clear();

    DigiCalendarButton* pButton = qobject_cast<DigiCalendarButton*>(sender());
    QDate date = pButton->GetDate();
    m_SelectDates.append(new QDate(date));

    //点击了邻月日期按钮
    if (pButton->GetBtnStatus() == PreviousOrNext)
    {
        if (pButton->m_Index < 21)  //按钮位置在上半区域
            m_CurrentDate = m_CurrentDate.addMonths(-1);
        else
            m_CurrentDate = m_CurrentDate.addMonths(1);

        calenderShow(date);
        emit monthChanged(m_CurrentDate);
    }
    else
    {
        calenderShow(date, false);
    }

    emit dateChanged(date);             //兼容旧代码
}

void DigiBaseCalendar::onMonthChanged()
{
    ui->m_pMonPrevBtn->setVisible(true);
    ui->m_pMonNextBtn->setVisible(true);
    ui->m_pYearPrevBtn->setVisible(true);
    ui->m_pYearNextBtn->setVisible(true);
//    ui->m_pTodayBtn->setVisible(true);
    DigiCalendarButton *button = qobject_cast<DigiCalendarButton*>(sender());

    if(button != nullptr)
    {
        int year = button->GetDate().year();
        int month = button->GetDate().month();
        m_CurrentDate.setDate(year, month, 1);
        calenderShow(m_CurrentDate);
        ui->stackedWidget->setCurrentIndex(0);
        emit monthChanged(m_CurrentDate);

        emit dateChanged(m_CurrentDate);                      //兼容旧代码
    }
}

void DigiBaseCalendar::onYearChanged()
{
    ui->m_pMonPrevBtn->setVisible(true);
    ui->m_pMonNextBtn->setVisible(true);
    ui->m_pYearPrevBtn->setVisible(true);
    ui->m_pYearNextBtn->setVisible(true);
//    ui->m_pTodayBtn->setVisible(true);
    DigiCalendarButton *button = static_cast<DigiCalendarButton*>(sender());

    if(button)
    {
        int year = button->GetDate().year();
        m_CurrentDate.setDate(year, m_CurrentDate.month(), 1);
        calenderShow(m_CurrentDate);
        ui->stackedWidget->setCurrentIndex(0);
        emit monthChanged(m_CurrentDate);

        emit dateChanged(m_CurrentDate);                      //兼容旧代码
    }

}

void DigiBaseCalendar::setBackGroundColor(QColor color)
{
    QString styleStr = "background-color: %1;";
    setStyleSheet(styleStr.arg(color.name()));
}

void DigiBaseCalendar::setColor(QColor cur, QColor cur67,QColor curSelect, QColor prevOrNext)
{
    for(DigiCalendarButton*btn : m_DayBtnVector)
    {
        btn->m_style.fontColor[0] = cur;
        btn->m_style.fontColor[1] = cur67;
        btn->m_style.fontColor[2] = curSelect;
        btn->m_style.fontColor[3] = prevOrNext;
    }

    for(DigiCalendarButton*btn : m_YearBtnVector)
    {
        btn->m_style.fontColor[0] = cur;
    }

    for(DigiCalendarButton*btn : m_MonthBtnVector)
    {
        btn->m_style.fontColor[0] = cur;
    }
}

void DigiBaseCalendar::setHasRecordColor(QColor color)
{
    for(DigiCalendarButton*btn : m_DayBtnVector)
        btn->m_style.hasRecordColor = color;
}

void DigiBaseCalendar::setHoverColor(QColor color)
{
    for(DigiCalendarButton*btn : m_DayBtnVector)
    {
        btn->m_style.hoverColor = color;
    }

    for(DigiCalendarButton*btn : m_YearBtnVector)
    {
        btn->m_style.hoverColor = color;
    }

    for(DigiCalendarButton*btn : m_MonthBtnVector)
    {
        btn->m_style.hoverColor = color;
    }
}

void DigiBaseCalendar::setSelectColor(QColor color)
{
    for(DigiCalendarButton*btn : m_DayBtnVector)
    {
        btn->m_style.selectColor = color;
    }
}

void DigiBaseCalendar::setTitleColor(QColor weekColor, QColor ButtonTextColor)
{
    auto labelList = ui->frame->findChildren<DigiBaseLabel*>();
    foreach(auto label, labelList)
    {
        label->SetTextColor(weekColor);
    }

    ui->m_pMonthButton->SetTextColor(ButtonTextColor);
    ui->m_pYearButton->SetTextColor(ButtonTextColor);
}

void DigiBaseCalendar::setFontSize(int size)
{
    m_Font.setPixelSize(size);
    auto labelList = ui->frame->findChildren<DigiBaseLabel*>();
    {
        foreach(auto lab, labelList)
        {
            QFont aFont = lab->GetCustomFont();
            aFont.setPixelSize(size);
            lab->SetCustomFont(aFont);
        }
    }
}

void DigiBaseCalendar::SetCustomFont(const QFont &font)
{
    m_Font = font;
    auto labelList = ui->frame->findChildren<DigiBaseLabel*>();
    {
        foreach(auto lab, labelList)
        {
            lab->SetCustomFont(m_Font);
        }
    }
}

/**************
 * 日历日期按钮 *
 **************/
DigiCalendarButton::DigiCalendarButton(int index, DigiBaseCalendar *parent, ButtonTextType textType):
    DigiBaseFrame(parent),
    m_Index(index),
    setted(false),
    m_Parent(parent),
    m_hasRecord(false),
    m_PersianDate(nullptr),
    m_Status(Else),
    m_Scale(1.0),
    m_Btn(nullptr),
    m_Layout(nullptr)
{
    m_style.textType = textType;
    Init();
}

void DigiCalendarButton::SetDate(int y, int m, int d)
{
    m_Date.setDate(y,m,d);

    if (!m_PersianDate.isNull()) //需显示波斯日期
    {
        *m_PersianDate = PersianDate::GregorianToJalali(m_Date);
        y = m_PersianDate->Year();
        m = m_PersianDate->Month();
        d = m_PersianDate->Day();
    }

    //设置显示的文本
    switch (m_style.textType)
    {
        case DAY:
            m_Btn->setText(QString::number(d));
            break;
        case MONTH:
            m_Btn->setText(QString::number(m));
            break;
        case YEAR:
            m_Btn->setText(QString::number(y));
            break;
    }
}

void DigiCalendarButton::SetDate(const QDate &date)
{
    SetDate(date.year(), date.month(), date.day());
}

QDate DigiCalendarButton::GetDate()
{
    return m_Date;
}

void DigiCalendarButton::SetHasRecord(bool h)
{
    m_hasRecord = h;
    update();
}

bool DigiCalendarButton::GetHasRecord()
{
    return m_hasRecord;
}

void DigiCalendarButton::select(ButtonStatus status)
{
    if(m_Status != status)
    {
        m_Status = status;

        if (m_Status != Else)
            UpdateStyleSheet();
    }
}

void DigiCalendarButton::SetButtonStatus(ButtonStatus stat)
{
    m_Status = stat;
}

ButtonStatus DigiCalendarButton::GetBtnStatus() const
{
    return m_Status;
}

void DigiCalendarButton::SetPersianDateMode(bool on)
{
    if (on && m_PersianDate.isNull()) //显示波斯日期
    {
        m_PersianDate.reset(new PersianDate);

        if (m_Date.isValid())
            *m_PersianDate = PersianDate::GregorianToJalali(m_Date);
    }
    else if (!on && !m_PersianDate.isNull())  //显示正常日期
    {
        m_PersianDate.reset();
    }
}

void DigiCalendarButton::SetScale(qreal scale)
{
    DigiBaseFrame::SetScale(scale);

    m_Scale = scale;

    if(m_style.textType == DAY)
        m_Btn->setFixedSize(m_width*scale, m_width*scale);

    QFont font = m_Parent->GetCustomFont();
    font.setPixelSize(font.pixelSize() * scale);
    m_Btn->setFont(font);

    //样式
    if (m_Status != Else)
        UpdateStyleSheet();
}

void DigiCalendarButton::paintEvent(QPaintEvent *event)
{
    DigiBaseFrame::paintEvent(event);

    if (!m_hasRecord)
        return;

    //画录像标志
    QPainter painter(this);

    QColor color = "#FF2A2A";
    if(m_style.hasRecordColor.isValid())
        color = m_style.hasRecordColor;

    QPen pen;
    pen.setColor(color);
    QBrush brush(color);
    painter.setPen(pen);
    painter.setBrush(brush);

    qreal scale = m_Scale;
    qreal x = 8 * scale;
    qreal y = 20 * scale;
    QRectF rect(x, y, 3*scale, 4*scale);
    painter.drawEllipse(rect);
}

void DigiCalendarButton::Init()
{
    m_Btn = new QPushButton(this);
    m_Layout = new QVBoxLayout;


    m_Layout->setSpacing(0);
    m_Layout->addWidget(m_Btn);

    setLayout(m_Layout);
    connect(m_Btn, &QPushButton::clicked, this, &DigiCalendarButton::BtnClicked);

    if(m_style.textType == DAY)
    {
        m_width = 20;
        m_height = 26;
        m_Layout->setContentsMargins(0,0,0,6);
    }
    else
    {
        m_width = 0;
        m_height = 0;
        m_Layout->setContentsMargins(0,0,0,0);
        m_Btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

void DigiCalendarButton::UpdateStyleSheet()
{
    QString qss;

    qss.append(QString("QPushButton{"
                           "border-radius: %1px; "
                           "color: %2;}"
                       "QPushButton:disabled{"
                            "color: %3}")
               .arg(4 * m_Scale)
               .arg(m_style.fontColor[m_Status].name())
               .arg(m_style.fontColor[3].name()));

    switch (m_Status)
    {
        case Current:
        case Current67:
            qss.append(QString("QPushButton:hover{background-color: %1;}")
                       .arg(m_style.hoverColor.name()));
            break;
        case CurrentSelect:
            qss.append(QString("QPushButton{background-color: %1;}")
                       .arg((m_style.selectColor.name())));
            break;
        default:
            break;
    }

    m_Btn->setStyleSheet(qss);
}

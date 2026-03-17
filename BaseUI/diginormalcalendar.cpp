#include "diginormalcalendar.h"
#include "ui_digibasecalendar.h"

DigiNormalCalendar::DigiNormalCalendar(QWidget *parent) :
    DigiBaseCalendar(parent)
{
    InitText();
    CreateConnect();
    calenderShow(m_CurrentDate);
}

DigiNormalCalendar::DigiNormalCalendar(const QDate &currentDate, QWidget *parent) :
    DigiBaseCalendar(currentDate, parent)
{
    InitText();
    CreateConnect();
    calenderShow(m_CurrentDate);
}

void DigiNormalCalendar::GetFirstAndLastOfMonth(QDate &firstDate, QDate &lastDate)
{
    firstDate.setDate(m_CurrentDate.year(), m_CurrentDate.month(), 1);
    lastDate.setDate(m_CurrentDate.year(), m_CurrentDate.month(), m_CurrentDate.daysInMonth());
}

void DigiNormalCalendar::InitText()
{
    ui->label_1->setText("Su");
    ui->label_2->setText("Mo");
    ui->label_3->setText("Tu");
    ui->label_4->setText("We");
    ui->label_5->setText("Th");
    ui->label_6->setText("Fr");
    ui->label_7->setText("Sa");
}


void DigiNormalCalendar::CreateConnect()
{
    connect(ui->m_pMonPrevBtn,  &QPushButton::clicked, this, &DigiNormalCalendar::ToPrevMonth);
    connect(ui->m_pMonNextBtn,  &QPushButton::clicked, this, &DigiNormalCalendar::ToNextMonth);
    connect(ui->m_pYearPrevBtn, &QPushButton::clicked, this, &DigiNormalCalendar::ToPrevYear);
    connect(ui->m_pYearNextBtn, &QPushButton::clicked, this, &DigiNormalCalendar::ToNextYear);
    connect(ui->m_pMonthButton, &QPushButton::clicked, this, &DigiNormalCalendar::onMonthBtnClicked);
    connect(ui->m_pYearButton,  &QPushButton::clicked, this, &DigiNormalCalendar::onYearBtnClicked);

    //年份页，跳转上一页
    connect(m_PrevYearPageBtn, &QToolButton::clicked, this, [this]()
    {
        m_YearPageCurVal = m_YearPageCurVal.addYears(-12);
        changeYearPage(m_YearPageCurVal);
    });

    //年份页，跳转下一页
    connect(m_NextYearPageBtn, &QToolButton::clicked, this, [this]()
    {
        m_YearPageCurVal = m_YearPageCurVal.addYears(12);
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
        connect(m_YearBtnVector.at(i), &DigiCalendarButton::BtnClicked, this, &DigiNormalCalendar::onYearChanged);
    }
    for(i=0; i<m_MonthBtnVector.size(); i++)
    {
        connect(m_MonthBtnVector.at(i), &DigiCalendarButton::BtnClicked, this, &DigiNormalCalendar::onMonthChanged);
    }
    for(i=0; i<m_DayBtnVector.size(); i++)
    {
        connect(m_DayBtnVector.at(i), &DigiCalendarButton::BtnClicked, this, &DigiNormalCalendar::currentDateChanged);
    }
}

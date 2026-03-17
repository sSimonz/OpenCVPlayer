#ifndef DIGIPERSIANCALENDAR_H
#define DIGIPERSIANCALENDAR_H

#include "digibasecalendar.h"

class DigiPersianCalendar : public DigiBaseCalendar
{
    Q_OBJECT

public:
    explicit DigiPersianCalendar(QWidget *parent = nullptr);
    explicit DigiPersianCalendar(const QDate& currentDate = QDate::currentDate(), QWidget *parent = nullptr);

    void setSelectDate(const QDate& date) override;        //设置选中日期
    void setSelectDates(QList<QDate> dateList) override;   //设置选中日期（多选）

    void setRecordFlag(QDate date) override;   //给指定日期设置标记

    void GetFirstAndLastOfMonth(QDate &firstDate, QDate &lastDate) override;

private:
    void InitText() override;
    void CreateConnect() override;
    void calenderShow(const QDate& date,bool bclear = true) override;
    void AdjustFirstShowMonth();  //调整日历初次显示的月份
    void ReSetBtnIndex();    //给按钮重新排序
    QList<PersianDate> GetPerDateListFromQDateList(const QList<QDate*> datelist);

private slots:
    void onMonthBtnClicked() override;   //点击月份按钮，打开月份选择页面
};

#endif // DIGIPERSIANCALENDAR_H

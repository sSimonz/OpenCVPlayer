#ifndef DIGINORMALCALENDAR_H
#define DIGINORMALCALENDAR_H

#include "digibasecalendar.h"

class DigiNormalCalendar : public DigiBaseCalendar
{
    Q_OBJECT

public:
    explicit DigiNormalCalendar(QWidget *parent = nullptr);
    explicit DigiNormalCalendar(const QDate& currentDate = QDate::currentDate(), QWidget *parent = nullptr);

    void GetFirstAndLastOfMonth(QDate &firstDate, QDate &lastDate) override;

private:
    void InitText() override;
    void CreateConnect() override;
};

#endif // DIGINORMALCALENDAR_H

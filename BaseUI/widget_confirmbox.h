#ifndef WIDGET_CONFIRMBOX_H
#define WIDGET_CONFIRMBOX_H

#include "widget_dialogbase.h"
#include <QTimer>
#include <QKeyEvent>

namespace Ui {
class Widget_ConfirmBox;
}

class Widget_ConfirmBox : public Widget_DialogBase
{
    Q_OBJECT
    
public:
    explicit Widget_ConfirmBox(QString msg,QWidget *parent = nullptr, float delayMsec = -1);
    ~Widget_ConfirmBox();

	void SetTranConfirm(QString text);
    void SetTranCancel(QString text);
    void SetClickRes(ResultType ConfirmType, ResultType CancelType);

protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::Widget_ConfirmBox *ui;
    
    QTimer *m_timer;
    ResultType m_ConfirmType;
    ResultType m_CancelType;

private slots:
    void yesRequest();
    void okRequest();
    void cancelRequest();
};

#endif // WIDGET_CONFIRMBOX_H

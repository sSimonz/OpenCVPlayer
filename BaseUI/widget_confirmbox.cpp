#include "widget_confirmbox.h"
#include "ui_widget_confirmbox.h"
Widget_ConfirmBox::Widget_ConfirmBox(QString msg, QWidget *parent, float delayMsec) :
    Widget_DialogBase(parent),
    m_ConfirmType(Done),
    m_CancelType(CancelOrClose),
    ui(new Ui::Widget_ConfirmBox)
{
    ui->setupUi(this);
    ui->m_pMsg->setText(msg);
    ui->m_pBtnYes->SetSize(96, 28);
    ui->m_pBtnCancel->SetSize(96, 28);
    ui->m_pBtnOk->SetSize(96, 28);
    ui->m_pBtnYes->setText("Yes");
    ui->m_pBtnOk->setVisible(false);
    ui->m_pBtnCancel->setText("Cancel");
    ui->m_pBtnOk->setText("OK");
#ifdef Q_OS_MAC
    ui->horizontalLayout->setSpacing(20);
#endif

    m_timer = new QTimer(this);
    if (delayMsec != -1)
    {
        ui->m_pBtnCancel->setVisible(false);
        ui->m_pBtnYes->setVisible(false);
        ui->m_pBtnOk->setVisible(true);
        if(delayMsec > 0)
            m_timer->start(delayMsec);
    }

    connect(ui->m_pBtnYes,SIGNAL(clicked()),this,SLOT(yesRequest()));
    connect(ui->m_pBtnCancel,SIGNAL(clicked()),this,SLOT(cancelRequest()));
    connect(ui->m_pBtnOk,SIGNAL(clicked()),this,SLOT(okRequest()));
    connect(m_timer, SIGNAL(timeout()), ui->m_pBtnCancel, SLOT(click()));
}

Widget_ConfirmBox::~Widget_ConfirmBox()
{
    delete ui;
}

void Widget_ConfirmBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        yesRequest();
        event->accept();
    }

    event->ignore();
}

void Widget_ConfirmBox::yesRequest()
{
    emit doneRequest(m_ConfirmType, nullptr);
}

void Widget_ConfirmBox::okRequest()
{
    emit doneRequest(m_ConfirmType, nullptr);
}

void Widget_ConfirmBox::cancelRequest()
{
    emit doneRequest(m_CancelType, nullptr);
}

void Widget_ConfirmBox::SetTranConfirm(QString text)
{
    ui->m_pBtnYes->setText(text);
    ui->m_pBtnOk->setText(text);
}

void Widget_ConfirmBox::SetTranCancel(QString text)
{
    ui->m_pBtnCancel->setText(text);
}

void Widget_ConfirmBox::SetClickRes(ResultType ConfirmType, ResultType CancelType)
{
    m_ConfirmType = ConfirmType;
    m_CancelType = CancelType;
}

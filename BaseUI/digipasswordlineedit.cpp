#include "digipasswordlineedit.h"

#include <QKeyEvent>
#include <QHBoxLayout>
#include <QRegularExpression>
#include "Res/resfun.h"
#include "digiiconpushbtn.h"

DigiPasswordLineEdit::DigiPasswordLineEdit(QWidget *parent) :
    DigiBaseLineEdit(parent),
    m_limitLength(32),
    m_bNolimitLength(true),
    m_PwdBtn(nullptr),
    m_bPaste(false)
{
    Init();
    CreateConnect();
}

void DigiPasswordLineEdit::SetLength(int Length)
{
    m_limitLength = Length;
    SetMaxByteLength(Length);
}

void DigiPasswordLineEdit::SetLimitLength(bool blimit)
{
    m_bNolimitLength = blimit;
    SetCharLengthLimit(blimit);
}

bool DigiPasswordLineEdit::getEyeVisible()
{
    return m_PwdBtn->isVisible();
}

void DigiPasswordLineEdit::setEyeVisible(bool bshow)
{
    m_PwdBtn->setVisible(bshow);
}

void DigiPasswordLineEdit::setLimitFormat(const QString &format)
{
    m_limitformat = format;
}

void DigiPasswordLineEdit::setText(const QString &name)
{
    QString result = name;
    if(m_bNolimitLength)
        result = name.mid(0, m_limitLength);
    DigiBaseLineEdit::setText(result);
}

void DigiPasswordLineEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    QString iconPath = (mode==Password) ? ResFun::GetPngResUrlByName(ICON_LOGIN_INVISIBLE)
                                        : ResFun::GetPngResUrlByName(ICON_LOGIN_VISIBLE);
    m_PwdBtn->SetCustomIcon(iconPath);
    DigiBaseLineEdit::setEchoMode(mode);
}

void DigiPasswordLineEdit::setCopyPasteEnabled(bool b)
{
    m_bPaste = b;
}

void DigiPasswordLineEdit::SetScale(qreal scale)
{
    DigiBaseLineEdit::SetScale(scale);

    QMarginsF margin(0,0,4*scale,0);
    layout()->setContentsMargins(margin.toMargins());
    m_PwdBtn->SetScale(scale);
}

void DigiPasswordLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(m_bPaste)
        return DigiBaseLineEdit::keyPressEvent(event);

    if(event->matches(QKeySequence::SelectAll))
        return;
    else if(event->matches(QKeySequence::Copy))
        return;
//    else if(event->matches(QKeySequence::Paste))
//        return;
    else if(event->matches(QKeySequence::Cut))
        return;
    else if(event->key() == Qt::Key_Home)
    {
        setCursorPosition(0);
        return;
    }
    else if(event->key() == Qt::Key_End)
    {
        setCursorPosition(text().length());
        return;
    }
    else if(event->key() == Qt::Key_Left)
    {
        int curPos = cursorPosition();
        setCursorPosition(curPos-1);
        return;
    }
    else if(event->key() == Qt::Key_Right)
    {
        int curPos = cursorPosition();
        setCursorPosition(curPos+1);
        return;
    }

    return DigiBaseLineEdit::keyPressEvent(event);
}

void DigiPasswordLineEdit::Init()
{
    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_PwdBtn = new DigiIconPushBtn(this);
    m_PwdBtn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_LOGIN_INVISIBLE));
    m_PwdBtn->SetStyleBtnAllColor("#00000000");

    setEchoMode(QLineEdit::Password);
    m_PwdBtn->setVisible(true);
    m_PwdBtn->setCursor(Qt::PointingHandCursor);
    m_PwdBtn->setFocusPolicy(Qt::NoFocus);

    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(m_PwdBtn);
    btnLayout->setAlignment(Qt::AlignRight);
    btnLayout->setContentsMargins(0, 0, 4, 0);
    setLayout(btnLayout);

    m_TextMargins.setRight(2);
    QMarginsF nMargins = m_TextMargins;
    nMargins.setRight(m_TextMargins.right() + m_PwdBtn->GetWidth() + 4);
    SetCustomTextMargins(nMargins.toMargins());
}

void DigiPasswordLineEdit::CreateConnect()
{
    connect(m_PwdBtn, &DigiIconPushBtn::clicked, this, &DigiPasswordLineEdit::ShowPassword);

    connect(this, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        QString str = text;

        int length = str.length();
        QRegularExpression regexp(m_limitformat);
        if ((!m_limitformat.isEmpty() && !regexp.match(str).hasMatch())|| (m_bNolimitLength && length > m_limitLength))
        {
            str.chop(1);
            setText(str);
        }
        emit pwdChanged(str);
    });
}

void DigiPasswordLineEdit::ShowPassword()
{
    if(echoMode() == QLineEdit::Password)
        setEchoMode(QLineEdit::Normal);
    else if(echoMode() == QLineEdit::Normal)
        setEchoMode(QLineEdit::Password);
}

#include "digipathlineedit.h"
#include <QHBoxLayout>
#include "digibasewidget.h"
#include "digiiconpushbtn.h"
#include "digibasefiledialog.h"
#include "Res/resfun.h"

QString s_bcurPath;
DigiPathLineEdit::DigiPathLineEdit(QWidget *parent) :
    DigiBaseLineEdit(parent),
    m_Spacing(12),
    m_Margin(QMargins(0,0,12,0)),
    m_Btn(nullptr),
    m_mode(QFileDialog::Directory),
    m_accept(QFileDialog::AcceptOpen)
{
    InitWidget();
    CreateConnect();
}

void DigiPathLineEdit::SetFileMode(QFileDialog::FileMode mode)
{
    m_mode = mode;
}

void DigiPathLineEdit::SetNameFilter(const QString &filter)
{
    m_Filter = filter;
}

void DigiPathLineEdit::SetAcceptMode(QFileDialog::AcceptMode mode)
{
    m_accept = mode;
}

void DigiPathLineEdit::SetIconBtn(QString path)
{
    m_Btn->SetCustomIcon(path);
}

void DigiPathLineEdit::SetScale(qreal scale)
{
    DigiBaseLineEdit::SetScale(scale);

    //控件
    m_Btn->SetScale(scale);

    //布局
    layout()->setContentsMargins(m_Margin * scale);
    layout()->setSpacing(static_cast<int>(m_Spacing * scale));
}

void DigiPathLineEdit::SetHeight(qreal height)
{
    m_height = height;
    m_Btn->SetHeight(height);
}

void DigiPathLineEdit::InitWidget()
{
    setReadOnly(true);
    setFocusPolicy(Qt::NoFocus);

    m_Btn = new DigiIconPushBtn(this);
    m_Btn->SetStyleSize(0,0,0,0);
    m_Btn->SetStyleBtnAllColor("#00000000");
    m_Btn->setCursor(Qt::PointingHandCursor);
    m_Btn->setToolTip("Change");
    m_Btn->SetCustomIcon(ResFun::GetPngResUrlByName(ICON_COMMON_ELLIPSES));
    m_Btn->SetHeight(GetHeight());
    m_Btn->AdjustWidthByText(); //会有4像素的空白
    m_Btn->SetWidth(m_Btn->GetWidth()-4);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(m_Margin);
    hLayout->setSpacing(m_Spacing);
    hLayout->addStretch(1);
    hLayout->addWidget(m_Btn);

    setLayout(hLayout);

    m_TextMargins.setRight(static_cast<int>(m_Btn->GetWidth()
                                            + m_Spacing*2
                                            + m_Margin.right()));
}

void DigiPathLineEdit::CreateConnect()
{
    connect(m_Btn, &DigiIconPushBtn::clicked, this, &DigiPathLineEdit::OpenFileDialog);
}

void DigiPathLineEdit::OpenFileDialog()
{
    DigiBaseFileDialog fileDialog;
    if(!s_bcurPath.isEmpty())
        fileDialog.setDirectory(s_bcurPath);
    fileDialog.setFileMode(m_mode);
    fileDialog.setAcceptMode(m_accept);
    if(!m_Filter.isEmpty())
        fileDialog.setNameFilter(m_Filter);

    fileDialog.showDialog();

    bool bDir = m_mode == QFileDialog::Directory;
    QString path = fileDialog.getSelectFile();
    if (!path.isEmpty())
    {
        setText(path);
        if(bDir)
        {
            s_bcurPath = path;
        }
        else
        {
            QFileInfo fileinfo(path);
            s_bcurPath = fileinfo.path();
        }
        emit fileAccepted();
    }
}

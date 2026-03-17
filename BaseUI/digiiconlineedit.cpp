#include "digiiconlineedit.h"
#include <QHBoxLayout>
#include "digibaselabel.h"

DigiIconLineEdit::DigiIconLineEdit(QWidget *parent):
    DigiBaseLineEdit(parent),
    m_LabIcon(nullptr),
    m_IconSize(20,20)
{

}

void DigiIconLineEdit::SetIconSize(const QSize &size)
{
    m_IconSize = size;
    if (m_LabIcon != nullptr)
    {
        m_LabIcon->SetWidth(m_IconSize.width());
        m_LabIcon->SetHeight(m_IconSize.height());
    }
}

void DigiIconLineEdit::SetCustomIcon(const QString &iconpath)
{
    if (iconpath.isEmpty())
        return;

    if (m_LabIcon == nullptr)
    {
        m_LabIcon = new DigiBaseLabel(this);
        auto iconLayout = new QHBoxLayout;
        iconLayout->addStretch();
        iconLayout->addWidget(m_LabIcon);
        iconLayout->setAlignment(Qt::AlignRight);
        iconLayout->setContentsMargins(0, 0, 4, 0);
        setLayout(iconLayout);
    }

    m_LabIcon->LoadImagePath(iconpath);
    m_LabIcon->SetWidth(m_IconSize.width());
    m_LabIcon->SetHeight(m_IconSize.height());

    QMargins nMargins = m_TextMargins;
    nMargins.setRight(m_TextMargins.right()/4 + m_IconSize.width());
    SetCustomTextMargins(nMargins);
}

void DigiIconLineEdit::SetScale(qreal scale)
{
    DigiBaseLineEdit::SetScale(scale);

    if (m_LabIcon != nullptr)
    {
        m_LabIcon->SetScale(scale);
        this->layout()->setContentsMargins(0, 0, 4 * scale, 0);
    }
}

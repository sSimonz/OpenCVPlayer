#include "digibasetoolbox.h"
#include "digibaselabel.h"

#include <QMouseEvent>
#include "Res/resfun.h"

DigiBaseToolBox::DigiBaseToolBox(QWidget *parent):
    QFrame(parent),
    m_index(0)
{
    InitWidget();
}

int DigiBaseToolBox::AddItem(const QString &text, QWidget *widget)
{
    if(!widget)
        return -1;

    DigiBaseToolBoxItem *item = new DigiBaseToolBoxItem(text, m_index);
    m_Layout->addWidget(item);
    m_Layout->addWidget(widget);
    m_items.insert(m_index, qMakePair(item, widget));

    if(m_index != 0)
    {
        widget->setVisible(false);
        item->LoadArrowImg(false);
    }
    else
    {
        item->LoadArrowImg(true);
    }
    connect(item, &DigiBaseToolBoxItem::itemClicked, [this](int index)
    {
        for(auto it = m_items.begin(); it != m_items.end(); it++)
        {
            auto pair = it.value();
            DigiBaseToolBoxItem *item = pair.first;
            QWidget *widget = pair.second;
            bool visible = it.key() == index;
            widget->setVisible(visible);
            item->LoadArrowImg(visible);
        }
    });

    return m_index++;
}

void DigiBaseToolBox::ExpandItem(int index)
{
    auto it = m_items.find(index);
    if (it != m_items.end())
    {
        auto pair = it.value();
        QWidget *widget = pair.second;

        if(widget->isHidden())
        {
            emit pair.first->itemClicked(index);
        }
    }
}

void DigiBaseToolBox::SetScale(qreal scale)
{
    //自身
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height*scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width*scale);


    //子控件
    auto widgetlist = findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
    foreach (QWidget *var, widgetlist)
    {
        if (var->inherits("IWidgetBase"))
        {
            dynamic_cast<IWidgetBase *>(var)->SetScale(scale);
        }
    }

    //布局
    m_Layout->setSpacing(6 * scale);
}

void DigiBaseToolBox::InitWidget()
{
    m_width = 288;
    m_height = 0;

    m_Layout = new QVBoxLayout;
    m_Layout->setContentsMargins(0, 0, 0, 0);
    m_Layout->setSpacing(0);
    m_Layout->setAlignment(Qt::AlignTop);
    setLayout(m_Layout);
}

/*************************
 *  DigiBaseToolBoxItem  *
 *************************/
DigiBaseToolBoxItem::DigiBaseToolBoxItem(const QString &text, int index, QWidget *parent) :
    QFrame(parent)
{
    InitWidget();

    m_pName = text;
    m_index = index;
    m_pLabName->setText(text);
    LoadArrowImg(false);
}

void DigiBaseToolBoxItem::LoadArrowImg(bool bOpen)
{
    if(bOpen)
        m_pLabPic->LoadImagePath(ResFun::GetPngResUrlByName(ICON_COMMON_CLOSE));
    else
        m_pLabPic->LoadImagePath(ResFun::GetPngResUrlByName(ICON_COMMON_EXPAND));

    m_pLabPic->SetWidth(12);
    m_pLabPic->SetHeight(12);
    m_pLabPic->update();
}

void DigiBaseToolBoxItem::setArrowVisible(bool bVisible)
{
    m_pLabPic->setVisible(bVisible);
}

void DigiBaseToolBoxItem::SetScale(qreal scale)
{
    //自身
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height*scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width*scale);

    //子控件
    m_pLabName->SetScale(scale);
    m_pLabPic->SetScale(scale);

    //布局
    m_MainLayout->setContentsMargins(m_Margin * scale);

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseToolBoxItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit itemClicked(m_index);

    QFrame::mouseReleaseEvent(event);
}

void DigiBaseToolBoxItem::InitWidget()
{
    m_width = 0;
    m_height = 40;

    m_pLabName = new DigiBaseLabel(this);
    m_pLabPic = new DigiBaseLabel(this);
    m_pLabName->SetTextWeight(QFont::Medium);

    m_pLabName->SetHeight(0);

    m_MainLayout = new QHBoxLayout;
    m_Margin = QMargins(12, 5, 12, 7);
    m_MainLayout->setContentsMargins(m_Margin);
    m_MainLayout->setSpacing(0);

    m_MainLayout->addWidget(m_pLabName);
    m_MainLayout->addStretch(1);
    m_MainLayout->addWidget(m_pLabPic);

    setLayout(m_MainLayout);
}

void DigiBaseToolBoxItem::SetCustomStyleSheet(qreal scale)
{
    Q_UNUSED(scale)

    QString qss;

    qss.append(QString("DigiBaseToolBoxItem{"
                           "background-color: %1;}")
                       .arg(ColorManager::GetColorHex(ColorManager::CN_TOOLBOX_BG)));

    setStyleSheet(qss);
}

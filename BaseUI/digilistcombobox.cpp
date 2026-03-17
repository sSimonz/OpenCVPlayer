#include "digilistcombobox.h"

#include <QDebug>
#include <QKeyEvent>
#include <QHBoxLayout>
#include "Res/resfun.h"
#include "digibaselabel.h"
#include "digibaselistwidget.h"
#include "digisvgpushbtn.h"

DigiListComboBox::DigiListComboBox(QWidget* parent):
    DigiBaseComboBox(parent),
    m_Scale(1)
{
    m_pListWidget = new DigiBaseListWidget(this);
    m_pListWidget->setResizeMode(QListView::Adjust);
    setModel(m_pListWidget->model());
    setView(m_pListWidget);
    setReadAndWrite();
}

void DigiListComboBox::setCurrentIndex(int index)
{
    DigiBaseComboBox::setCurrentIndex(index);
    if(index == -1)
        return;

    setEditText(m_ItemText.at(index));
    hidePopup();
}

void DigiListComboBox::hidePopup()
{
    m_pListWidget->scrollToTop();
    DigiBaseComboBox::hidePopup();
}

void DigiListComboBox::showPopup()
{
    DigiBaseComboBox::showPopup();
    m_pListWidget->scrollToTop();
    int row = currentIndex();
    m_pListWidget->setCurrentRow(row);
}

void DigiListComboBox::insertItem(QString text,qint64 id,bool bShowDelBtn)
{
    DigiListComboBoxItem* item = new DigiListComboBoxItem(id,text,bShowDelBtn,this);
    connect(item, &DigiListComboBoxItem::SignalRemoveItem, this, &DigiListComboBox::SlotRemoveItem);
    connect(item, &DigiListComboBoxItem::SignalShowItem,   this, &DigiListComboBox::SlotShowItem);
    QListWidgetItem* listItem = new QListWidgetItem(m_pListWidget);
    m_pListWidget->setItemWidget(listItem,item);
    m_ItemText.append(text);

    item->SetScale(m_Scale);
}

void DigiListComboBox::removeItem(QString item)
{
    int list_count = getListWidgetCount();
    for (int i = 0; i < list_count; i++)
    {
        QListWidgetItem *listItem = m_pListWidget->item(i);
        DigiListComboBoxItem *pItem = qobject_cast<DigiListComboBoxItem*>(m_pListWidget->itemWidget(listItem));
        QString itemText = pItem->getItemStr();
        if (item == itemText)
        {
            emit removeItemReq(pItem->getItemData());
            m_pListWidget->removeItemWidget(listItem);
            m_pListWidget->takeItem(i);
            m_ItemText.removeAt(i);
            delete listItem;
            break;
        }
    }
}

int DigiListComboBox::getListWidgetCount()
{
    return m_pListWidget->count();
}

QString DigiListComboBox::itemTextAt(qint64 iData)
{
    QString str;
    for(int i = 0; i < m_pListWidget->count(); i++)
    {
        QListWidgetItem* ListItem = m_pListWidget->item(i);
        DigiListComboBoxItem* item = static_cast<DigiListComboBoxItem*>(m_pListWidget->itemWidget(ListItem));
        if(item)
        {
            qint64 itemData = item->getItemData();
            if(itemData == iData)
            {
                str = item->getItemStr();
                break;
            }
        }
    }
    return str;
}

void DigiListComboBox::SetScale(qreal scale)
{
    m_Scale = scale;

    DigiBaseComboBox::SetScale(scale);

    //对列表项进行缩放
    for(int i=0; i<m_pListWidget->count(); i++)
    {
        QListWidgetItem* ListItem = m_pListWidget->item(i);
        DigiListComboBoxItem* item = static_cast<DigiListComboBoxItem*>(m_pListWidget->itemWidget(ListItem));
        item->SetScale(scale);
    }
}

void DigiListComboBox::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
    {
        QString text = currentText();
        if (m_ItemText.contains(text))
        {
            event->ignore();
        }
        else
        {
            DigiListComboBoxItem* item = qobject_cast<DigiListComboBoxItem*>(m_pListWidget->itemWidget(m_pListWidget->item(m_pListWidget->count()-1)));
            auto id = item->getItemData();
            insertItem(text,id+1);
        }

        return;
    }
    DigiBaseComboBox::keyPressEvent(event);
}

void DigiListComboBox::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void DigiListComboBox::SlotShowItem(qint64 data)
{
    QString text;
    int index = 0;
    for (int i = 0; i < m_pListWidget->count(); ++i)
    {
        QListWidgetItem *listItem = m_pListWidget->item(i);
        DigiListComboBoxItem *item = static_cast<DigiListComboBoxItem*>(m_pListWidget->itemWidget(listItem));
        qint64 itemData = item->getItemData();
        if(itemData == data)
        {
            index = i;
            text = m_ItemText.at(i);
            break;
        }
    }
    m_pListWidget->setCurrentRow(index);
    DigiBaseComboBox::setCurrentIndex(index);
    qInfo() << "SlotShowItem" << data << text;
    setEditText(text);
    hidePopup();
}

void DigiListComboBox::SlotRemoveItem(qint64 data)
{
    removeItem(itemTextAt(data));
    hidePopup();
    showPopup();
}


/************************
 * DigiListComboBoxItem *
 ************************/
DigiListComboBoxItem::DigiListComboBoxItem(qint64 id,QString item,bool bShowDel,DigiListComboBox *parent) :
    DigiBaseWidget(parent),
    m_ItemData(id)
{
    m_LabItem = new DigiBaseLabel(this);
    m_LabItem->setText(item);
    m_Text = item;

    m_DelBtn = new DigiSvgPushBtn(this);
    m_DelBtn->LoadImagePath(ResFun::GetPngResUrlByName(PNG_COMMON_TABCLOSE));
    m_DelBtn->SetWidth(16);
    m_DelBtn->SetHeight(16);

    connect(m_DelBtn,&QPushButton::clicked,this,[this]
    {
        emit SignalRemoveItem(m_ItemData);
    });

    m_MainLayout = new QHBoxLayout(this);
    m_MainLayout->addWidget(m_LabItem);
    m_MainLayout->addStretch(0);
    m_MainLayout->addWidget(m_DelBtn);
    m_Margin = QMargins(10,0,6,0);
    m_MainLayout->setContentsMargins(m_Margin);
    m_MainLayout->setSpacing(2);
    setLayout(m_MainLayout);

    m_DelBtn->setVisible(bShowDel);
    m_LabItem->SetElidedText(true);
}

DigiListComboBoxItem::~DigiListComboBoxItem()
{
    qInfo() << "DigiListComboBoxItem::~DigiListComboBoxItem";
}

void DigiListComboBoxItem::mousePressEvent(QMouseEvent* /*event*/)
{
    emit SignalShowItem(m_ItemData);
}

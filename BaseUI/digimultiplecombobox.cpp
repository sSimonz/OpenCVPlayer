#include "digimultiplecombobox.h"

#include <QListWidget>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "digibasecheckbox.h"
#include "digibaselabel.h"

DigiMultipleComboBox::DigiMultipleComboBox(QWidget *parent):
    DigiBaseComboBox(parent)
{
    m_width = 0;
    m_height = 35;
}

void DigiMultipleComboBox::SetScale(qreal scale)
{
    DigiBaseComboBox::SetScale(scale);

    //对列表项进行缩放
    for(int i=0; i<m_pListWidget->count(); i++)
    {
        QListWidgetItem* ListItem = m_pListWidget->item(i);
        DigiMultipleComboBoxItem* item = qobject_cast<DigiMultipleComboBoxItem*>(m_pListWidget->itemWidget(ListItem));
        item->SetScale(scale);
    }
}

void DigiMultipleComboBox::showPopup()
{
    SetText(QStringList());
    QComboBox::showPopup();
}

void DigiMultipleComboBox::hidePopup()
{
    QStringList textList;
    if (m_ItemList.first()->GetCheckState() == Qt::Checked)//All
    {
        textList.append(m_ItemList.first()->GetText());
    }
    else
    {
        for(int i = 1; i < m_ItemList.count(); i++)
        {
            DigiMultipleComboBoxItem* item = m_ItemList.at(i);
            if(item->GetCheckState() == Qt::Checked)
                textList.append(item->GetText());
        }
    }

    SetText(textList);

    //点击选项时不隐藏
    int width = this->view()->width();
    int height = this->view()->height();
    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
    int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();

    QRect rectView;
    if (this->mapToGlobal(QPoint(0,0)).y() < view()->mapToGlobal(QPoint(0,0)).y())
        rectView = QRect(0, this->height(), width, height);  //向下的弹框
    else
        rectView = QRect(0, -height, width, height);  //向上的弹框
    if (!rectView.contains(x, y))
    {
        QComboBox::hidePopup();
    }
}

void DigiMultipleComboBox::SetOptionText(const QString &All, QList<MultipleOption> list)
{
    QStringList textList;
    int nCheckCount = 0;
    MultipleOption allOption(0, All);
    list.prepend(allOption);
    m_pListWidget = new QListWidget(this);
    for(quint8 i = 0; i < list.count(); i++)
    {
        MultipleOption option = list.at(i);
        DigiMultipleComboBoxItem *item;
        item = new DigiMultipleComboBoxItem(this, option.index, option.text);
        item->SetCheckState(option.check ? Qt::Checked : Qt::Unchecked);  //默认勾选
        if(i != 0 && option.check)
        {
            textList << option.text;
            nCheckCount++;
        }
        QListWidgetItem *listItem = new QListWidgetItem(m_pListWidget);
        m_pListWidget->setItemWidget(listItem,item);
        m_ItemList.append(item);
    }

    setModel(m_pListWidget->model());
    setView(m_pListWidget);
    foreach (DigiMultipleComboBoxItem *item, m_ItemList)
    {
        connect(item, &DigiMultipleComboBoxItem::CheckBoxStateChanged,
                this, &DigiMultipleComboBox::CheckClickSlot);
    }

    DigiMultipleComboBoxItem* allItem = m_ItemList.first();
    if(nCheckCount == 0)
        allItem->SetCheckState(Qt::Unchecked);
    else if(nCheckCount == m_ItemList.count() - 1)
    {
        allItem->SetCheckState(Qt::Checked);
        textList.clear();
        textList << allItem->GetText();
    }
    else
        allItem->SetCheckState(Qt::PartiallyChecked);

    SetText(textList);
}

void DigiMultipleComboBox::SetCheckedOptions(QList<quint8> indexList)
{
    m_ItemList.first()->SetCheckState(Qt::Unchecked);
    emit m_ItemList.first()->CheckBoxStateChanged();
    if(indexList.isEmpty())
        return;

    QStringList textList;
    int nCheckCount = 0;
    foreach(quint8 index, indexList)
    {
        for(int i = 1; i < m_ItemList.count(); i++)
        {
            DigiMultipleComboBoxItem* item = m_ItemList.at(i);
            if(item->GetIndex() == index)
            {
                item->SetCheckState(Qt::Checked);
                textList.append(item->GetText());
                nCheckCount++;
            }
        }
    }

    DigiMultipleComboBoxItem* allItem = m_ItemList.first();
    if(nCheckCount == 0)
        allItem->SetCheckState(Qt::Unchecked);
    else if(nCheckCount == m_ItemList.count() - 1)
    {
        allItem->SetCheckState(Qt::Checked);
        textList.clear();
        textList << allItem->GetText();
    }
    else
        allItem->SetCheckState(Qt::PartiallyChecked);

    SetText(textList);
    m_pListWidget->setCurrentRow(-1);
}

QStringList DigiMultipleComboBox::GetCheckedTexts()
{
    QStringList textList;
    for(int i = 1; i < m_ItemList.count(); i++)
    {
        DigiMultipleComboBoxItem* item = m_ItemList.at(i);
        if(item->GetCheckState() == Qt::Checked)
            textList.append(item->GetText());
    }
    return textList;
}

QList<quint8> DigiMultipleComboBox::GetCheckedOptions()
{
    QList<quint8> indexList;
    for(int i = 1; i < m_ItemList.count(); i++)
    {
        DigiMultipleComboBoxItem* item = m_ItemList.at(i);
        if(item->GetCheckState() == Qt::Checked)
            indexList.append(item->GetIndex());
    }
    return indexList;
}

QMap<quint8, bool> DigiMultipleComboBox::GetAllCheckState()
{
    QMap<quint8, bool> map;
    for(int i = 1; i < m_ItemList.count(); i++)
    {
        DigiMultipleComboBoxItem* item = m_ItemList.at(i);
        map.insert(item->GetIndex(), item->GetCheckState());
    }
    return map;
}

void DigiMultipleComboBox::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void DigiMultipleComboBox::SetText(QStringList textList)
{
    this->lineEdit()->setText(textList.join(";"));

    // tips
    if (!textList.isEmpty() && textList.at(0)==m_ItemList.first()->GetText())
    {
        textList.clear();
        for(int i = 1; i < m_ItemList.count(); i++)
        {
            DigiMultipleComboBoxItem* item = m_ItemList.at(i);
            textList.append(item->GetText());
        }
    }
    this->lineEdit()->setToolTip(textList.join("\n"));
}

void DigiMultipleComboBox::CheckClickSlot()
{
    //更新当前选中项
    m_pListWidget->setCurrentRow(qobject_cast<DigiMultipleComboBoxItem*>(sender())->GetIndex());

    //更新按钮状态
    DigiMultipleComboBoxItem* allItem = m_ItemList.first();
    if (sender() == allItem)  //点击了ALL
    {
        if (allItem->GetCheckState() == Qt::Unchecked)
        {
            allItem->SetCheckState(Qt::Unchecked);
            for(int i = 1; i < m_ItemList.count(); i++)
            {
                DigiMultipleComboBoxItem* item = m_ItemList.at(i);
                item->SetCheckState(Qt::Unchecked);
            }
        }
        else
        {
            allItem->SetCheckState(Qt::Checked);
            for(int i = 1; i < m_ItemList.count(); i++)
            {
                DigiMultipleComboBoxItem* item = m_ItemList.at(i);
                item->SetCheckState(Qt::Checked);
            }
        }
    }
    else  //点击了子按钮
    {
        int nCheckCount = 0;
        int nListCount = m_ItemList.count() - 1;
        for(int i = 1; i < m_ItemList.count(); i++)
        {
            DigiMultipleComboBoxItem* item = m_ItemList.at(i);
            if(item->GetCheckState() == Qt::Checked)
                nCheckCount++;
        }

        Qt::CheckState state;
        if(nCheckCount == 0)
            state = Qt::Unchecked;
        else if(nCheckCount == nListCount)
            state = Qt::Checked;
        else
            state = Qt::PartiallyChecked;

        allItem->SetCheckState(state);
    }
    update();
}


/********************************
 * DigiMultipleComboBoxItem *
 ********************************/
DigiMultipleComboBoxItem::DigiMultipleComboBoxItem(QWidget *parent,quint8 index, const QString &text):
    QWidget(parent)
{
    m_Label = new DigiBaseLabel(this);
    m_CheckBox = new DigiBaseCheckBox(this);

    m_Label->SetHeight(qobject_cast<DigiMultipleComboBox*>(parent)->GetItemHeight());
    m_CheckBox->SetWidth(24);
    m_CheckBox->SetHeight(24);
    SetText(text);
    m_Index = index;

    CreateLayout();
    CreateConnect();
}

void DigiMultipleComboBoxItem::SetText(const QString &text)
{
    m_Label->setText(text);
}

void DigiMultipleComboBoxItem::SetCheckState(Qt::CheckState state)
{
    m_CheckBox->setCheckState(state);
}

void DigiMultipleComboBoxItem::SetIndex(quint8 index)
{
    m_Index = index;
}

QString DigiMultipleComboBoxItem::GetText()
{
    return m_Label->text().trimmed();
}

Qt::CheckState DigiMultipleComboBoxItem::GetCheckState()
{
    return m_CheckBox->checkState();
}

bool DigiMultipleComboBoxItem::GetChecked()
{
    return m_CheckBox->isChecked();
}

quint8 DigiMultipleComboBoxItem::GetIndex()
{
    return m_Index;
}

void DigiMultipleComboBoxItem::SetScale(qreal scale)
{
    m_CheckBox->SetScale(scale);
    m_Label->SetScale(scale);
    m_Label->setFont(GetGlobalFont());

    m_MainLayout->setContentsMargins(4*scale,0,0,0);
    m_MainLayout->setSpacing(6*scale);
}

void DigiMultipleComboBoxItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_CheckBox->checkState() != Qt::Checked)
        m_CheckBox->setChecked(true);
    else
        m_CheckBox->setChecked(false);

    update();
    emit CheckBoxStateChanged();
    QWidget::mouseReleaseEvent(event);
}

void DigiMultipleComboBoxItem::CreateLayout()
{
    m_MainLayout = new QHBoxLayout;

    m_MainLayout->addWidget(m_CheckBox);
    m_MainLayout->addWidget(m_Label);
    m_MainLayout->addStretch(1);
    m_MainLayout->setContentsMargins(4,0,0,0);
    m_MainLayout->setSpacing(6);
    setLayout(m_MainLayout);
}

void DigiMultipleComboBoxItem::CreateConnect()
{
    connect(m_CheckBox, &QCheckBox::clicked,
            this, &DigiMultipleComboBoxItem::CheckBoxStateChanged);
}

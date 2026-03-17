#include "digibasetablewidget.h"
#include "digibasescrollbar.h"
#include "Res/resfun.h"
#include <QVBoxLayout>
#include <QHeaderView>

DigiBaseTableWidget::DigiBaseTableWidget(QWidget *parent) :
    QTableWidget(parent),
    m_bFirst(true),
    m_BGColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)),
    m_AlternateColorHex(ColorManager::GetColorHex(ColorManager::CN_TABLEVIEWITEM_BACKGROUND_ALTERNATE)),
    m_SelectionColorHex(ColorManager::GetColorHex(ColorManager::CN_TABLEVIEWITEM_BACKGROUND_SELECTION)),
    m_HeaderTextColorHex(ColorManager::GetColorHex(ColorManager::CN_HEADERVIEW_TEXT)),
    m_HeaderBGColorHex(ColorManager::GetColorHex(ColorManager::CN_HEADERVIEW_BACKGROUND)),
    m_HeaderBorderColorHex(ColorManager::GetColorHex(ColorManager::CN_TABLEVIEW_HEADER_BORDER))
{
    m_width = 0;
    m_height = 0;
    m_HeaderHeight = 32;
    m_HeaderFontSize = 14;
    m_RowHeight = 34;

    m_Font = GetGlobalFont();

    setHorizontalScrollBar(new DigiBaseScrollBar(this));
    setVerticalScrollBar(new DigiBaseScrollBar(this));

    verticalHeader()->setMinimumSectionSize(m_RowHeight/2);

    InitStyle();
}

void DigiBaseTableWidget::SetRowHeight(qreal height)
{
    m_RowHeight = height;
    verticalHeader()->setMinimumSectionSize(m_RowHeight/2);
}

void DigiBaseTableWidget::SetHeaderHeight(qreal height)
{
    m_HeaderHeight = height;
}

void DigiBaseTableWidget::SetBGColor(const QColor &color)
{
    m_BGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableWidget::SetAlternateColor(const QColor &color)
{
    m_AlternateColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableWidget::SetSelectionColor(const QColor &color)
{
    m_SelectionColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableWidget::SetHeaderTextColor(const QColor &color)
{
    m_HeaderTextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableWidget::SetHeaderBGColor(const QColor &color)
{
    m_HeaderBGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableWidget::SetHeaderBorderColor(const QColor &color)
{
    m_HeaderBorderColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableWidget::SetColumnSizeList(const QList<int> &list)
{
    m_ColumnSizeList = list;
    if (!list.contains(0))
        return;

    int index = 0;
    while(index < list.count())
    {
        index = list.indexOf(0,index);

        if(index == -1)
            break;

        horizontalHeader()->setSectionResizeMode(index,QHeaderView::Stretch);
        index++;
    }
    horizontalHeader()->setStretchLastSection(false);
}

void DigiBaseTableWidget::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseTableWidget::GetCustomFont()
{
    return m_Font;
}

void DigiBaseTableWidget::SetScale(qreal scale)
{
    //自身大小
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);

    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    //行高
    verticalHeader()->setDefaultSectionSize(m_RowHeight*scale);

    //缩放横表头中的特殊控件
    if (horizontalHeader()->inherits("IWidgetBase"))
        dynamic_cast<IWidgetBase*>(horizontalHeader())->SetScale(scale);

    //列宽
    if(m_bFirst && m_ColumnSizeList.isEmpty()) //第一次且m_ColumnSizeList为空，获取初始的列宽基值
    {
        int count = model()->columnCount();

        if (horizontalHeader()->stretchLastSection())
            count-=1;

        for (int i=0; i<count; i++)
        {
            m_ColumnSizeList.append(columnWidth(i));
        }

        if (count < model()->columnCount())
            m_ColumnSizeList << 0;

        m_bFirst = false;
    }
    for(int i=0; i<m_ColumnSizeList.count(); i++)
    {
        if (m_ColumnSizeList.at(i) == 0) //为0时不缩放，根据设定的ResizeMode自适应
            continue;
        setColumnWidth(i,m_ColumnSizeList.at(i)*scale);
    }

    //字体
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    //滚动条
    dynamic_cast<IWidgetBase*>(horizontalScrollBar())->SetScale(scale);
    dynamic_cast<IWidgetBase*>(verticalScrollBar())->SetScale(scale);

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseTableWidget::setItem(int row, int column, QTableWidgetItem *item)
{
    item->setTextAlignment(Qt::AlignCenter);//文本居中
    QTableWidget::setItem(row, column, item);
}

void DigiBaseTableWidget::InitStyle()
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);    //不可编辑
    verticalHeader()->setVisible(false);                   //竖表头隐藏
    horizontalHeader()->setStretchLastSection(true);       //横表头尾项自动拉伸
    setSelectionBehavior(QAbstractItemView::SelectRows);   //行选择
    setFocusPolicy(Qt::NoFocus);                           //无焦点
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);  //一次滚动一像素(拖滚动条)
    verticalHeader()->setDefaultSectionSize(34);           //单元格高度

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //横滚动条始终不显示
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::SingleSelection); //单选
    setAlternatingRowColors(true);                        //行交替颜色
}

void DigiBaseTableWidget::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;
    //表头
    qss.append("QHeaderView::section{");
    qss.append(QString("height: %1px;").arg(m_HeaderHeight*scale));
    qss.append(QString("background-color: %1;").arg(m_HeaderBGColorHex));
    qss.append(QString("color: %1;"
                       "font: Medium;"
                       "font-size: %2px;").arg(m_HeaderTextColorHex).arg(m_HeaderFontSize*scale));
    qss.append(QString("border-left:0px;"
                       "border-bottom:0px;"
                       "border-top:0px;"
                       "border-right:%1px solid %2;}").arg(1*scale).arg(m_HeaderBorderColorHex));

    this->horizontalHeader()->setStyleSheet(qss.join(""));

    //表格
    qss.clear();
    qss.append("QTableWidget{");
    qss.append(QString("border: %1px solid %2;").arg(1*scale).arg(m_HeaderBorderColorHex));
    qss.append(QString("background: %1;").arg(m_BGColorHex));
    qss.append(QString("alternate-background-color: %1;").arg(m_AlternateColorHex));
    qss.append(QString("gridline-color: %1;").arg(m_HeaderBorderColorHex));
    qss.append(QString("border-radius: %1px;}").arg(6*scale));
    qss.append(QString("QTableWidget::item:selected {background-color: %1;}").arg(m_SelectionColorHex));

    this->setStyleSheet(qss.join(""));
}

#include "digibasetableview.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include "Res/resfun.h"
#include "digibasescrollbar.h"
#include "digibaselabel.h"

DigiBaseTableView::DigiBaseTableView(QWidget *parent):
    QTableView(parent),
    m_bFirst(true),
    m_BGColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)),
    m_AlternateColorHex(ColorManager::GetColorHex(ColorManager::CN_TABLEVIEWITEM_BACKGROUND_ALTERNATE)),
    m_SelectionColorHex(ColorManager::GetColorHex(ColorManager::CN_TABLEVIEWITEM_BACKGROUND_SELECTION)),
    m_HeaderTextColorHex(ColorManager::GetColorHex(ColorManager::CN_HEADERVIEW_TEXT)),
    m_HeaderBGColorHex(ColorManager::GetColorHex(ColorManager::CN_HEADERVIEW_BACKGROUND)),
    m_HeaderBorderColorHex(ColorManager::GetColorHex(ColorManager::CN_TABLEVIEW_HEADER_BORDER)),
    m_HoverColorHex("#00000000")
{
    m_width = 0;
    m_height = 0;
    m_HeaderHeight = 32;
    m_HeaderFontSize = 14;
    m_RowHeight = 34;
    m_BorderRadius = 6;

    m_Font = GetGlobalFont();

    setHorizontalScrollBar(new DigiBaseScrollBar(this));
    setVerticalScrollBar(new DigiBaseScrollBar(this));

    verticalHeader()->setMinimumSectionSize(m_RowHeight/2);

    InitStyle();
    InitNoDataWidget();
    SetNoDataWidgetVisible(false);
}

void DigiBaseTableView::SetRowHeight(qreal height)
{
    m_RowHeight = height;
    verticalHeader()->setMinimumSectionSize(m_RowHeight/2);
}

void DigiBaseTableView::SetHeaderHeight(qreal height)
{
    m_HeaderHeight = height;
}

void DigiBaseTableView::SetBGColor(const QColor &color)
{
    m_BGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableView::SetAlternateColor(const QColor &color)
{
    m_AlternateColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableView::SetSelectionColor(const QColor &color)
{
    m_SelectionColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableView::SetHeaderTextColor(const QColor &color)
{
    m_HeaderTextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableView::SetHeaderBGColor(const QColor &color)
{
    m_HeaderBGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableView::SetHeaderBorderColor(const QColor &color)
{
    m_HeaderBorderColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTableView::SetColumnSizeList(const QList<int> &list)
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

void DigiBaseTableView::SetBorderRadius(int radius)
{
    m_BorderRadius = radius;
}

void DigiBaseTableView::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseTableView::GetCustomFont()
{
    return m_Font;
}

void DigiBaseTableView::setModel(QAbstractItemModel *model)
{
    QAbstractItemModel *oldModel = this->model();
    QTableView::setModel(model);
    return;

    // 暂时不显示No data
    if (oldModel != nullptr)
    {
        disconnect(oldModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &DigiBaseTableView::rowsAboutToBeInsertedSlot);
        disconnect(oldModel, &QAbstractItemModel::rowsRemoved, this, &DigiBaseTableView::rowsRemovedSlot);
    }

    if (model == nullptr)
        return;

    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, this, &DigiBaseTableView::rowsAboutToBeInsertedSlot, Qt::UniqueConnection);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &DigiBaseTableView::rowsRemovedSlot, Qt::UniqueConnection);

    if (model->rowCount() == 0)
        SetNoDataWidgetVisible(true);
    else
        SetNoDataWidgetVisible(false);
}

void DigiBaseTableView::SetScale(qreal scale)
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
        int count = model() ? model()->columnCount() : horizontalHeader()->count();
        int allCount = count;
        if (horizontalHeader()->stretchLastSection())
            count-=1;

        for (int i=0; i<count; i++)
        {
            m_ColumnSizeList.append(columnWidth(i));
        }

        if (count < allCount)
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

    //无数据提示内容
    m_NoDataLayout->setContentsMargins(0, m_HeaderHeight*scale, 0, 0);
    m_NoDataTextLab->SetScale(scale);
    m_NoDataPictureLab->SetScale(scale);

    //代理
    if (itemDelegate() && itemDelegate()->inherits("IWidgetBase"))
        dynamic_cast<IWidgetBase*>(itemDelegate())->SetScale(scale);

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseTableView::InitStyle()
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);    //不可编辑
    verticalHeader()->setVisible(false);                   //竖表头隐藏
    horizontalHeader()->setStretchLastSection(true);       //横表头尾项自动拉伸
    setSelectionBehavior(QAbstractItemView::SelectRows);   //行选择
    setFocusPolicy(Qt::NoFocus);                           //无焦点
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);  //一次滚动一像素(拖滚动条)
    verticalHeader()->setDefaultSectionSize(34);           //单元格高度
    setMouseTracking(true);

//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //横滚动条始终不显示
//    setSelectionMode(QAbstractItemView::SingleSelection); //单选
    setAlternatingRowColors(true);                        //行交替颜色
    setSortingEnabled(true);                              //允许排序
//    setShowGrid(false);                                   //是否显示网格

}

void DigiBaseTableView::InitNoDataWidget()
{
    m_NoDataTextLab = new DigiBaseLabel(this);
    m_NoDataTextLab->SetHeight(0);
    m_NoDataTextLab->setAlignment(Qt::AlignCenter);
    m_NoDataTextLab->setText("No data");
    m_NoDataTextLab->SetTextBold(true);
    m_NoDataTextLab->SetTextColor(ColorManager::GetColor(ColorManager::CN_TABLEVIEWITEM_NODATA_TEXT));
    m_NoDataTextLab->SetTextSize(16);
    m_NoDataPictureLab = new DigiBaseLabel(this);
    m_NoDataPictureLab->LoadImagePath(ResFun::GetPngResUrlByName(ICON_ALARM_NORECORD));

    m_NoDataLayout = new QVBoxLayout;
    m_NoDataLayout->setContentsMargins(0, m_HeaderHeight, 0, 0);
    m_NoDataLayout->setSpacing(0);
    m_NoDataLayout->addStretch();
    m_NoDataLayout->addWidget(m_NoDataPictureLab);
    m_NoDataLayout->addWidget(m_NoDataTextLab);
    m_NoDataLayout->addStretch();
    m_NoDataLayout->setAlignment(m_NoDataPictureLab, Qt::AlignHCenter);
    m_NoDataLayout->setAlignment(m_NoDataTextLab, Qt::AlignHCenter);

    setLayout(m_NoDataLayout);
}

void DigiBaseTableView::SetNoDataWidgetVisible(bool visible)
{
    m_NoDataPictureLab->setVisible(visible);
    m_NoDataTextLab->setVisible(visible);
}

void DigiBaseTableView::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;
    //表头
    qss.append("QHeaderView,QHeaderView::section{");
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
    qss.append("QTableView{");
    qss.append(QString("border: %1px solid %2;").arg(1*scale).arg(m_HeaderBorderColorHex));
    qss.append(QString("background: %1;").arg(m_BGColorHex));
    qss.append(QString("alternate-background-color: %1;").arg(m_AlternateColorHex));
    qss.append(QString("selection-background-color: transparent;").arg(m_SelectionColorHex));
    qss.append(QString("gridline-color: %1;").arg(m_HeaderBorderColorHex));
    qss.append(QString("border-radius: %1px;}").arg(m_BorderRadius*scale));

    qss.append(QString("QTableView::item:hover{background:%1;}").arg(m_HoverColorHex));

    this->setStyleSheet(qss.join(""));
}

void DigiBaseTableView::rowsAboutToBeInsertedSlot()
{
    if (model() && model()->rowCount() == 0)
        SetNoDataWidgetVisible(false);
}

void DigiBaseTableView::rowsRemovedSlot()
{
    if (model() && model()->rowCount() == 0)
        SetNoDataWidgetVisible(true);
}

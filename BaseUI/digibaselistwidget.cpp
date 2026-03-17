#include "digibaselistwidget.h"

#include <QHeaderView>
#include "Res/resfun.h"
#include "digibasescrollbar.h"

DigiBaseListWidget::DigiBaseListWidget(QWidget *parent) :
    QListWidget(parent),
    m_RowHeight(20),
    m_BGColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_HoverSelColorHex(ColorManager::GetColorHex(ColorManager::CN_LISTWIDGETITEM_BACKGROUND_HOVER)),
    m_TextHoverColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_AlternateColorHex(ColorManager::GetColorHex(ColorManager::CN_TABLEVIEWITEM_BACKGROUND_ALTERNATE)),
    m_Checkable(false)
{
    m_scale = 0;
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();

    setHorizontalScrollBar(new DigiBaseScrollBar(this));
    setVerticalScrollBar(new DigiBaseScrollBar(this));

    InitStyle();
}

DigiBaseListWidget::~DigiBaseListWidget()
{
    
}

void DigiBaseListWidget::SetRowHeight(qreal height)
{
    m_RowHeight = height;
}

void DigiBaseListWidget::SetBGColor(const QColor &color)
{
    m_BGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseListWidget::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseListWidget::SetHoverAndSelColor(const QColor &color)
{
    m_HoverSelColorHex = color.name(QColor::HexArgb);
}

void DigiBaseListWidget::SetTextHoverColor(const QColor &color)
{
    m_TextHoverColorHex = color.name(QColor::HexArgb);
}

void DigiBaseListWidget::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseListWidget::GetCustomFont()
{
    return m_Font;
}

void DigiBaseListWidget::setCheckable(bool b)
{
    m_Checkable = b;
}

void DigiBaseListWidget::SetScale(qreal scale)
{
    if(qFuzzyIsNull(scale))
        return;
    if(qFuzzyCompare(m_scale,scale))
        return;
    m_scale = scale;
    //自身大小
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);

    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    //字体
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    //滚动条
    dynamic_cast<IWidgetBase*>(horizontalScrollBar())->SetScale(scale);
    dynamic_cast<IWidgetBase*>(verticalScrollBar())->SetScale(scale);
    int ncount = count();
    for(int i = 0; i < ncount; ++i)
    {
        QListWidgetItem* baseitem = item(i);
        if(baseitem)
        {
            QWidget* var = itemWidget(baseitem);
            if (var && var->inherits("IWidgetBase"))
            {
                dynamic_cast<IWidgetBase *>(var)->SetScale(scale);
            }
        }

    }
    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseListWidget::InitStyle()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //横滚动条始终不显示
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);  //一次滚动一像素(拖滚动条)
}

void DigiBaseListWidget::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;
    qss.append(QString("QListWidget{"
                           "background-color: %1;"
                           "border:0px;"
                           "outline: 0px;"
                           "color: %2}")
                       .arg(m_BGColorHex)
                       .arg(m_TextColorHex));

    qss.append(QString("QListWidget::item{"
                           "height:%1px;}")
                       .arg(m_RowHeight * scale));

    qss.append(QString("QListWidget::item:hover,"
                       "QListWidget::item:selected{"
                           "background-color: %1;"
                           "color: %2;}")
                       .arg(m_HoverSelColorHex)
                       .arg(m_TextHoverColorHex));

    if(alternatingRowColors())
    {
        qss.append(QString("QListWidget{"
                               "alternate-background-color: %1;}")
                           .arg(m_AlternateColorHex));
    }

    if(m_Checkable)
    {
        qss.append("QListWidget:indicator{image: url(:/png/res/darkgreen/png/playback/Checkbox_unselected.png);}"
                   "QListWidget:indicator:checked{image: url(:/png/res/darkgreen/png/playback/Checkbox_selected.png);}"
                   "QListWidget:indicator:indeterminate{image: url(:/png/res/darkgreen/png/playback/Checkbox_partialselected.png);}");
    }

    this->setStyleSheet(qss.join(""));
}

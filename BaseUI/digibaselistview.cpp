#include "digibaselistview.h"

#include <QHeaderView>
#include "digibasescrollbar.h"

DigiBaseListView::DigiBaseListView(QWidget *parent) :
    QListView(parent),
    m_RowHeight(20),
    m_BGColorHex(ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_HoverSelColorHex(ColorManager::GetColorHex(ColorManager::CN_LISTWIDGETITEM_BACKGROUND_HOVER)),
    m_TextHoverColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT))
{
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();

    setHorizontalScrollBar(new DigiBaseScrollBar(this));
    setVerticalScrollBar(new DigiBaseScrollBar(this));

    InitStyle();
}

void DigiBaseListView::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

void DigiBaseListView::SetRowHeight(qreal height)
{
    m_RowHeight = height;
}

void DigiBaseListView::SetBGColor(const QColor &color)
{
    m_BGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseListView::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseListView::SetHoverAndSelColor(const QColor &color)
{
    m_HoverSelColorHex = color.name(QColor::HexArgb);
}

void DigiBaseListView::SetTextHoverColor(const QColor &color)
{
    m_TextHoverColorHex = color.name(QColor::HexArgb);
}

QFont DigiBaseListView::GetCustomFont()
{
    return m_Font;
}

void DigiBaseListView::SetScale(qreal scale)
{
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

    //model
    if (model()!=nullptr && model()->inherits("IWidgetBase"))
        dynamic_cast<IWidgetBase*>(model())->SetScale(scale);

    //delegate
    if (itemDelegate() != nullptr && itemDelegate()->inherits("IWidgetBase"))
        dynamic_cast<IWidgetBase*>(itemDelegate())->SetScale(scale);

    //delegate中的编辑框
    QWidget* editor = this->findChild<QWidget*>(QString("Editor")); //"Editor"为编辑框的ObejctName，在代理类中设置
    if (editor != nullptr)
        dynamic_cast<IWidgetBase*>(editor)->SetScale(scale);

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseListView::InitStyle()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //横滚动条始终不显示
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);  //一次滚动一像素(拖滚动条)

}

void DigiBaseListView::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;

    qss.append(QString("QListView{"
                           "background-color: %1;"
                           "border:0px;"
                           "outline: 0px;"
                           "color: %2}")
                       .arg(m_BGColorHex)
                       .arg(m_TextColorHex));

    qss.append(QString("QListView::item{"
                           "height:%1px;}")
                       .arg(m_RowHeight * scale));

    qss.append(QString("QListView::item:hover,"
                       "QListView::item:selected{"
                           "background-color: %1;"
                           "color: %2;}")
                       .arg(m_HoverSelColorHex)
                       .arg(m_TextHoverColorHex));

    this->setStyleSheet(qss.join(""));
}

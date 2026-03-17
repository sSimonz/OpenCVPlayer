#include "digibasetreeview.h"

#include <QDebug>
#include <QHeaderView>
#include "Res/resfun.h"
#include "digibasescrollbar.h"

DigiBaseTreeView::DigiBaseTreeView(QWidget *parent) :
    QTreeView(parent),
    m_IconSize(QSize(20,20)),
    m_RowHeight(35),
    m_branchIconSize(16),
    m_BGColorHex(ColorManager::GetColorHex(ColorManager::CN_DEVICE_TREE_BG)),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)),
    m_HoverAndSelColorHex(ColorManager::GetColorHex(ColorManager::CN_TREEVIEWITEM_BACKGROUND_HOVER)),
    m_BorderColor(ColorManager::GetColorHex(ColorManager::CN_DIALOG_TITLE_BACKGROUND)),
    m_Border(0),
    m_Radius(0)
{
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();

    setHorizontalScrollBar(new DigiBaseScrollBar(this));
    setVerticalScrollBar(new DigiBaseScrollBar(this));

    InitStyle();
}

void DigiBaseTreeView::SetRowHeight(qreal height)
{
    m_RowHeight = height;
}

void DigiBaseTreeView::SetBGColor(const QColor &color)
{
    m_BGColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTreeView::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTreeView::SetHoverAndSelColor(const QColor &color)
{
    m_HoverAndSelColorHex = color.name(QColor::HexArgb);
}

void DigiBaseTreeView::SetCustomIconSize(const QSize &size)
{
    m_IconSize = size;
}

void DigiBaseTreeView::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseTreeView::GetCustomFont()
{
    return m_Font;
}

void DigiBaseTreeView::SetBorderWidth(qreal borderWidth)
{
    m_Border = borderWidth;
}

void DigiBaseTreeView::SetBorderColor(const QColor &color)
{
    m_BorderColor = color.name(QColor::HexArgb);
}

void DigiBaseTreeView::SetRadius(qreal radius)
{
    m_Radius = radius;
}

void DigiBaseTreeView::SetScale(qreal scale)
{
    //自身大小
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);

    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    //字体
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize()*scale);
    setFont(aFont);

    //图标
    setIconSize(m_IconSize * scale);

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

    //菜单
    QWidget* menu = this->findChild<QWidget*>(QString("Menu")); //"Menu"为菜单的ObejctName，在Treeview中设置
    if (menu != nullptr)
        dynamic_cast<IWidgetBase*>(menu)->SetScale(scale);

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseTreeView::mousePressEvent(QMouseEvent *event)
{
    //解决拖拽节点后，第一次点击item无反应的问题
    if (state()==DragSelectingState || state()==DraggingState)
        setState(NoState);

    QTreeView::mousePressEvent(event);
}

void DigiBaseTreeView::InitStyle()
{
    setAnimated(true);                              //折叠展开动画
    setHeaderHidden(true);                          //隐藏表头
    setEditTriggers(QTreeView::NoEditTriggers);	    //不可编辑
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);     //一次滚动一像素(拖滚动条)
    header()->setStretchLastSection(true);                        //表头尾项自动拉伸
    header()->setSectionResizeMode(QHeaderView::ResizeToContents); //列宽根据内容自动变化

    setFocusPolicy(Qt::NoFocus);                    //无焦点
//    setFocusPolicy(Qt::ClickFocus);              //点击获得焦点
}

void DigiBaseTreeView::SetCustomStyleSheet(qreal scale)
{
    QStringList qss;
    //TreeView和item
    qss.append(QString("QTreeView {"
                           "border:none;"
                           "background: %1;"
                           "show-decoration-selected: 1;"
                           "border:%2px solid %3;"
                           "border-radius: %4px;}")
                       .arg(m_BGColorHex)
                       .arg(m_Border)
                       .arg(m_BorderColor)
                       .arg(m_Radius));
    qss.append(QString("QTreeView::item {"
                           "height: %1px;"
                           "border: none;"
                           "color: %2;"
                           "background: %3;}")
                       .arg(m_RowHeight*scale).arg(m_TextColorHex).arg(m_BGColorHex));
    qss.append(QString("QTreeView::item:hover,"
                       "QTreeView::item:selected,"
                       "QTreeView::branch:hover,"
                       "QTreeView::branch:selected{"
                           "color: %1;"
                           "background: %2;}")
                       .arg(m_TextColorHex).arg(m_HoverAndSelColorHex));

    //展开图标(箭头)
    qss.append(QString("QTreeView::branch:closed:has-children{"
                           "image: url(%1);}")
                       .arg(ResFun::GetSvgResUrlByName(PNG_EQUIPMENTMANAGEMENT_SCROLLBAR_RIGHT)));
    qss.append(QString("QTreeView::branch:open:has-children{"
                           "image: url(%1);}")
                       .arg(ResFun::GetSvgResUrlByName(PNG_EQUIPMENTMANAGEMENT_SCROLLBAR_DOWN)));
    //分支描述图标
//    qss.append(QString("QTreeView::branch:has-siblings:adjoins-item{"
//                       "border-image: url(%1) 0;}")
//                   .arg(ResFun::GetSvgResUrlByName(PNG_EQUIPMENTMANAGEMENT_BRANCH_MORE)));
//    qss.append(QString("QTreeView::branch:!has-children:!has-siblings:adjoins-item{"
//                       "border-image: url(%1) 0;}")
//                   .arg(ResFun::GetSvgResUrlByName(PNG_EQUIPMENTMANAGEMENT_BRANCH_END)));

    //checkbox
    qss.append(QString("QTreeView::indicator{"
                           "width: %1px; height: %1px;}")
                       .arg(m_branchIconSize * scale));
    qss.append(QString("QTreeView::indicator:unchecked{"
                           "image: url(%1);}")
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_UNSELECTED)));
    qss.append(QString("QTreeView::indicator:checked{"
                           "image: url(%1);}")
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_SELECTED)));
    qss.append(QString("QTreeView::indicator:indeterminate{"
                           "image: url(%1);}")
                       .arg(ResFun::GetSvgResUrlByName(ICON_PLAYBACK_CHECKBOX_PARTSELECTED)));

    setStyleSheet(qss.join(""));
    setIndentation(m_branchIconSize * scale); //调整branch箭头图标大小
}

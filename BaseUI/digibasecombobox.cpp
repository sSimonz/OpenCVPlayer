#include "digibasecombobox.h"
#include <QDebug>
#include <QLineEdit>
#include <QListView>
#include <QHBoxLayout>

#include "Res/resfun.h"
#include "digibaselabel.h"
#include "digibaselistview.h"

DigiBaseComboBox::DigiBaseComboBox(QWidget *parent) :
    QComboBox(parent),
    m_TextMargins(10,0,0,0),
    m_bCursorToFirst(false),
    m_IconLab(nullptr),
    m_IconSize(QSize(24,24))
{
    m_width = 0;
    m_height = 32;
    initStyleSheet();

    m_Font = GetGlobalFont();
    setTextOptional(true);
    setView(new DigiBaseListView);
}

void DigiBaseComboBox::setTextOptional(bool on)
{
    setEditable(on);
    if (on)
    {
        QLineEdit *CurlineEdit = lineEdit();
        CurlineEdit->setReadOnly(true);
        setLineEdit(CurlineEdit);

        if (!m_bCursorToFirst)
        {
            connect(CurlineEdit, &QLineEdit::textChanged, this, &DigiBaseComboBox::SetCursorToFirst);
            m_bCursorToFirst = true;
        }
    }
    else
    {
        QLineEdit *CurlineEdit = lineEdit();
        if (CurlineEdit != nullptr)
        {
            if (m_bCursorToFirst)
            {
                disconnect(CurlineEdit, &QLineEdit::textChanged, this, &DigiBaseComboBox::SetCursorToFirst);
                m_bCursorToFirst = false;
            }
        }
    }
}

void DigiBaseComboBox::setReadAndWrite()
{
    QLineEdit * CurlineEdit = lineEdit();
    if(CurlineEdit == nullptr)
    {
        setEditable(true);
    }
    CurlineEdit->setReadOnly(false);

    if (m_bCursorToFirst)
    {
        disconnect(CurlineEdit, &QLineEdit::textChanged, this, &DigiBaseComboBox::SetCursorToFirst);
        m_bCursorToFirst = false;
    }
}

void DigiBaseComboBox::SetStyleLineEditColor(QColor nmClr, QColor hvClr, QColor bgClr, QColor textClr)
{
    m_StyleColorList[LineNormal]     = nmClr.name(QColor::HexArgb);
    m_StyleColorList[LineHover]      = hvClr.name(QColor::HexArgb);
    m_StyleColorList[LineBackGround] = bgClr.name(QColor::HexArgb);
    m_StyleColorList[LineTextColor]  = textClr.name(QColor::HexArgb);
}

void DigiBaseComboBox::SetStyleLineEditSize(qreal borderWidth, qreal radius, qreal iconWidth)
{
    m_StyleSizeList[EditBorderWidth] = borderWidth;
    m_StyleSizeList[EditRadius]      = radius;
    m_StyleSizeList[EditiconWidth]   = iconWidth;
}

void DigiBaseComboBox::SetStyleViewItemColor(const QColor &nmClr,const QColor &bgClr,const QColor &selClr,const QColor &selBgClr)
{
    m_StyleColorList[ViewNormal]      = nmClr.name(QColor::HexArgb);
    m_StyleColorList[ViewBackGround]  = bgClr.name(QColor::HexArgb);
    m_StyleColorList[ViewSelection]   = selClr.name(QColor::HexArgb);
    m_StyleColorList[ViewSelectionBG] = selBgClr.name(QColor::HexArgb);
}

void DigiBaseComboBox::SetStyleViewItemSize(qreal borderWidth, qreal radius, qreal itemHeight)
{
    m_StyleSizeList[ViewBorderWidth] = borderWidth;
    m_StyleSizeList[ViewRadius]      = radius;
    m_StyleSizeList[ViewItemHeight]  = itemHeight;
}

void DigiBaseComboBox::SetScale(qreal scale)
{
    //自身大小
    if (!qFuzzyIsNull(m_width))
        setMinimumWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    //字体
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize()*scale);
    setFont(aFont);

    //item的icon图标大小
    setIconSize(m_IconSize * scale);

    //输入框
    QLineEdit *CurlineEdit = lineEdit();
    if (CurlineEdit != nullptr)
    {
        CurlineEdit->setTextMargins(m_TextMargins*scale);
        CurlineEdit->setFont(aFont);
        if (m_IconLab != nullptr)
        {
            m_IconLab->SetScale(scale);
            CurlineEdit->layout()->setContentsMargins(6*scale, 0, 6*scale, 0);
        }
        setLineEdit(CurlineEdit);
    }

    //样式
    SetCustomStyleSheet(scale);

    //子控件
    auto childlist = findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
    foreach(auto child,childlist)
    {
        if(child->inherits("IWidgetBase"))
        {
            IWidgetBase* basewid = dynamic_cast<IWidgetBase *>(child);
            if(basewid)
            {
                basewid->SetScale(scale);
            }
        }
    }

    if (view()->inherits("DigiBaseListView"))
        dynamic_cast<IWidgetBase*>(view())->SetScale(scale);

    view()->setStyleSheet("");
    view()->setFont(aFont);
}

void DigiBaseComboBox::AddCustomAction(QString path,QString Name,QLineEdit::ActionPosition position,QSize iconSize)
{
    if (path.isEmpty())
        return;

    QLineEdit* CurlineEdit = lineEdit();
    if (CurlineEdit == nullptr)
        return;

    CurlineEdit->setPlaceholderText(Name);

    m_IconLab = new DigiBaseLabel(this);
    m_IconLab->LoadImagePath(path);
    m_IconLab->SetWidth(iconSize.width());
    m_IconLab->SetHeight(iconSize.height());
    QHBoxLayout *iconLayout = new QHBoxLayout;

    if (position == QLineEdit::LeadingPosition) //左
    {
        m_TextMargins.setLeft(iconSize.width()+10);
        iconLayout->addWidget(m_IconLab);
        iconLayout->addStretch();
        iconLayout->setAlignment(Qt::AlignLeft);
        iconLayout->setContentsMargins(6, 0, 6, 0);
        CurlineEdit->setLayout(iconLayout);
    }
    else
    {
        m_TextMargins.setRight(iconSize.width()+10);
        iconLayout->addStretch();
        iconLayout->addWidget(m_IconLab);
        iconLayout->setAlignment(Qt::AlignRight);
        iconLayout->setContentsMargins(6, 0, 6, 0);
        CurlineEdit->setLayout(iconLayout);
    }
    setLineEdit(CurlineEdit);
}

void DigiBaseComboBox::SetCustomTextMargins(int nleft, int ntop, int nright, int nbottom)
{
    m_TextMargins = QMargins(nleft,ntop,nright,nbottom);
    QLineEdit *CurlineEdit = lineEdit();
    if (CurlineEdit != nullptr)
    {
        CurlineEdit->setTextMargins(m_TextMargins);
        setLineEdit(CurlineEdit);
    }
}

void DigiBaseComboBox::SetCustomTextMargins(const QMargins &nMargins)
{
    m_TextMargins = nMargins;
    QLineEdit *CurlineEdit = lineEdit();
    if (CurlineEdit != nullptr)
    {
        CurlineEdit->setTextMargins(m_TextMargins);
        setLineEdit(CurlineEdit);
    }
}

void DigiBaseComboBox::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiBaseComboBox::GetCustomFont()
{
    return m_Font;
}

void DigiBaseComboBox::SetItemIconSize(const QSize &size)
{
    m_IconSize = size;
}

void DigiBaseComboBox::initStyleSheet()
{
    m_StyleColorList << ColorManager::GetColorHex(ColorManager::CN_INPUTWIDGET_BORDER)
                     << ColorManager::GetColorHex(ColorManager::CN_INPUTWIDGET_BORDER_HOVER)
                     << ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)
                     << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)
                     << ColorManager::GetColorHex(ColorManager::CN_INPUTWIDGET_BORDER)
                     << ColorManager::GetColorHex(ColorManager::CN_INPUT_BACKGROUND)
                     << ColorManager::GetColorHex(ColorManager::CN_GLOBAL_TEXT)
                     << ColorManager::GetColorHex(ColorManager::CN_LISTWIDGETITEM_BACKGROUND_HOVER);

//    m_StyleSizeList[EditBorderWidth] = 1;
//    m_StyleSizeList[EditRadius]      = 6;
//    m_StyleSizeList[EditiconWidth]   = 16;
//    m_StyleSizeList[ViewBorderWidth] = 1;
//    m_StyleSizeList[ViewRadius]      = 6;
//    m_StyleSizeList[ViewItemHeight]  = m_height;
    m_StyleSizeList << 1 << 6 << 24 << 1 << 6 << m_height;
}

void DigiBaseComboBox::SetCustomStyleSheet(qreal scale)
{
    QString qss;
    qss.append(QString("QComboBox{"
                           "border:%1px solid %3;"
                           "border-radius:%2px;"
                           "background:%4;}")
               .arg(m_StyleSizeList.at(EditBorderWidth) *scale)
               .arg(m_StyleSizeList.at(EditRadius) *scale)
               .arg(m_StyleColorList.at(LineNormal))
               .arg(m_StyleColorList.at(LineBackGround)));

    int itemPadding = m_TextMargins.left();
    if(!isEditable())
    {
        qss.append(QString("QComboBox{padding: %1px %2px %3px %4px;}")
                   .arg(m_TextMargins.top() *scale)
                   .arg(m_TextMargins.right() *scale)
                   .arg(m_TextMargins.bottom() *scale)
                   .arg(m_TextMargins.left() *scale));
        itemPadding = m_TextMargins.left() - 4;
    }
    else
    {
        qss.append(QString("QComboBox{padding: %1px 0px %1px 0px;}")
                    .arg(2 * scale));
        itemPadding = m_TextMargins.left();
    }

    if (inherits("DigiListComboBox"))
        itemPadding = 0;

    qss.append(QString("QComboBox:hover,QComboBox:focus{border:%1px solid %2;}")
               .arg(m_StyleSizeList.at(EditBorderWidth) *scale)
               .arg(m_StyleColorList.at(LineHover)));

    qss.append(QString("QComboBox::drop-down{"
                           "subcontrol-origin: padding;"
                           "subcontrol-position: top right;"
                           "outline: 0px;"
                           "border:0px;"
                           "width: %1px;}")
               .arg(m_StyleSizeList.at(EditiconWidth) *scale));

    qss.append(QString("QComboBox::down-arrow{"
                           "width: %1px;height: %1px;"
                           "image: url(%2);"
                           "border: 0px;}")
               .arg(m_StyleSizeList.at(EditiconWidth) *scale)
               .arg(ResFun::GetSvgResUrlByName(ICON_LOGIN_DOWNINPUT)));

    qss.append(QString("QComboBox QAbstractItemView{"
                           "background-color:%1;"
                           "outline: 0px;"
                           "border: %2px solid %3;"
                           "border-radius: %4px;}")
               .arg(m_StyleColorList.at(ViewBackGround))
               .arg(m_StyleSizeList.at(ViewBorderWidth) *scale)
               .arg(m_StyleColorList.at(ViewNormal))
               .arg(m_StyleSizeList.at(ViewRadius) *scale));

    qss.append(QString("QComboBox QAbstractItemView:item{"
                       "height:%1px;"
                       "outline: 0px;"
                       "border-radius: %2px;"
                       "padding-left: %3px;}")
               .arg(m_StyleSizeList.at(ViewItemHeight) *scale)
               .arg(m_StyleSizeList.at(ViewRadius) *scale)
               .arg(itemPadding * scale));

    qss.append(QString("QComboBox QAbstractItemView::item:selected,"
                       "QComboBox QAbstractItemView::item:hover{"
                           "color:%1;"
                           "background:%2}")
               .arg(m_StyleColorList.at(ViewSelection))
               .arg(m_StyleColorList.at(ViewSelectionBG)));

    qss.append(QString("QComboBox:enabled{color:%1;}").arg(m_StyleColorList.at(LineTextColor)));
    setStyleSheet(qss);

    QLineEdit *CurlineEdit = lineEdit();
    if (CurlineEdit != nullptr)
    {
        CurlineEdit->setStyleSheet(QString("border-radius:%1px;background: %2")
                                  .arg(m_StyleSizeList.at(EditBorderWidth)*scale)
                                  .arg(m_StyleColorList.at(LineBackGround)));
    }
}

void DigiBaseComboBox::SetCursorToFirst()
{
    QLineEdit *CurlineEdit = lineEdit();
    if (CurlineEdit != nullptr)
        CurlineEdit->home(false);
}

#include "digibasemenu.h"

#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QtCore/QtMath>
#include <QPainterPath>

#include "Res/resfun.h"
#include "digibaselabel.h"
#include "digiiconpushbtn.h"

namespace
{
    const int SHADOW_WIDTH = 4;
    const int RADIUS = 6;
}

/****************
 * DigiBaseMenu *
 ****************/
DigiBaseMenu::DigiBaseMenu(QWidget *parent) :
    QMenu(parent),
    m_bInit(false),
    m_topFrame(nullptr),
    m_bottomFrame(nullptr),
    m_scale(1.0)
{
    m_width = 250;
    m_height = 0;

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setObjectName("Menu");

    // 头
    m_topFrame = new DigiBaseLabel;
    m_topFrame->SetHeight(6);
    QWidgetAction *topAction = new QWidgetAction(this);
    topAction->setDefaultWidget(m_topFrame);
    addAction(topAction);
}

void DigiBaseMenu::SetScale(qreal scale)
{
    m_scale = scale;

    //自身
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height*scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth((m_width + SHADOW_WIDTH*2) * scale); //加上阴影的左右宽度

    //子控件
    auto widgetlist = findChildren<QWidgetAction*>(QString(),Qt::FindDirectChildrenOnly);
    foreach (QWidgetAction *var, widgetlist)
    {
        if (var->inherits("IWidgetBase"))
        {
            dynamic_cast<IWidgetBase*>(var)->SetScale(scale);
        }
        else
        {
            QWidget *widget = var->defaultWidget();
            if (widget->inherits("IWidgetBase"))
                dynamic_cast<IWidgetBase*>(widget)->SetScale(scale);
        }
    }

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseMenu::SetWidth(qreal width)
{
    m_width = width;

    //子控件
    auto widgetlist = findChildren<QWidgetAction*>(QString(),Qt::FindDirectChildrenOnly);
    foreach (QWidgetAction *var, widgetlist)
    {
        if (var->inherits("IWidgetBase"))
        {
            dynamic_cast<IWidgetBase*>(var)->SetWidth(width);
        }
    }
}

QAction *DigiBaseMenu::addAction(const QString &text)
{
    DigiBaseAction *action = new DigiBaseAction(this, text);
    action->SetScale(m_scale);
    return action;
}

void DigiBaseMenu::addAction(QAction *action)
{
    DigiBaseAction *baseaction = qobject_cast<DigiBaseAction *>(action);
    if(baseaction)
        baseaction->SetScale(m_scale);
    QMenu::addAction(action);
}

QAction *DigiBaseMenu::addSeparator()
{
    return DigiBaseAction::CreateSeparatorAction(this);
}

void DigiBaseMenu::clear()
{
    QList<QAction *> actions = this->actions();
    if(actions.isEmpty())
        return;

    actions.removeFirst();
    for(int i = 0; i < actions.count(); i++)
    {
        QAction *action = actions.at(i);
        QWidget *parent = qobject_cast<QWidget *>(action->parent());
        removeAction(action);
        if(parent == this)
        {
            m_bInit = false;
            m_bottomFrame = nullptr;
            action->deleteLater();
        }
    }
}

void DigiBaseMenu::setMarginsForAllAction(int left, int top, int right, int bottom)
{
    setMarginsForAllAction(QMargins(left, top, right, bottom));
}

void DigiBaseMenu::setMarginsForAllAction(const QMargins &margins)
{
    auto actionList = findChildren<DigiBaseAction*>();
    foreach (DigiBaseAction *var, actionList)
    {
        if (!var->isSeparator())
            var->setContentsMargins(margins);
    }
}

void DigiBaseMenu::setSpacingForAllAction(int spacing)
{
    auto actionList = findChildren<DigiBaseAction*>();
    foreach (DigiBaseAction *var, actionList)
    {
        if (!var->isSeparator())
            var->setSpacing(spacing);
    }
}

void DigiBaseMenu::actionEvent(QActionEvent *event)
{
    if(event->type() == QEvent::ActionAdded)
    {
        QWidgetAction *action = qobject_cast<QWidgetAction*>(event->action());
        if(!action)
            return;

        if (action->defaultWidget()->objectName() == "m_bottomFrame")
        {
            QWidget *widget = action->defaultWidget();
            if (widget->inherits("IWidgetBase"))
                dynamic_cast<IWidgetBase*>(widget)->SetScale(m_scale);
        }
    }

    QMenu::actionEvent(event);
}

void DigiBaseMenu::paintEvent(QPaintEvent *event)
{
    //绘制阴影
    qreal shadowWidth = SHADOW_WIDTH * m_scale;
    int radius = static_cast<int>(RADIUS * m_scale);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(QRect(shadowWidth, shadowWidth, width() - shadowWidth * 2, height() - shadowWidth * 2), radius, radius);

    painter.fillPath(path, QBrush(ColorManager::GetColor(ColorManager::CN_MENU_BG)));

    qreal minAlpha = 0;
    qreal maxAlpha = 255;
    QColor color(150, 150, 150, 55);
    for (int i=0; i<shadowWidth; ++i)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(QRect(shadowWidth - i,
                          shadowWidth - i,
                          width() - (shadowWidth - i) * 2,
                          height() - (shadowWidth - i) * 2),
                          radius,
                          radius);

        qreal alpha = 180*m_scale - (qSqrt(i)*80*m_scale);
        alpha = qBound(minAlpha, alpha, maxAlpha);
        color.setAlpha(qFloor(alpha));
        painter.setPen(color);
        painter.drawPath(path);
    }

    QMenu::paintEvent(event);
}

void DigiBaseMenu::showEvent(QShowEvent *event)
{
    // 尾
    if(!m_bInit)
    {
        m_bInit = true;
        m_bottomFrame = new DigiBaseLabel;
        m_bottomFrame->SetHeight(6);
        m_bottomFrame->setObjectName("m_bottomFrame");
        QWidgetAction *bottomAction = new QWidgetAction(this);
        bottomAction->setDefaultWidget(m_bottomFrame);
        addAction(bottomAction);
    }
    QMenu::showEvent(event);
}

void DigiBaseMenu::SetCustomStyleSheet(qreal scale)
{
    QString qss;

    qss += QString("QMenu{"
                       "margin: %1px;"
                       "border-radius: %2px;"
                       "background-color: #00000000;}")
                    .arg(RADIUS*scale)
                    .arg(RADIUS*scale);

    setStyleSheet(qss);

    if (m_topFrame)
    {
        qss = QString("QFrame{"
                          "border-top-left-radius: %1px;"
                          "border-top-right-radius: %1px;"
                          "background-color: %2;"
                          "margin: %1px %1px 0px %1px;}")
                    .arg(RADIUS*scale)
                    .arg(ColorManager::GetColorHex(ColorManager::CN_MENU_BG));
        m_topFrame->setStyleSheet(qss);
    }

    if (m_bottomFrame)
    {
        qss = QString("QFrame{"
                          "border-bottom-left-radius: %1px;"
                          "border-bottom-right-radius: %1px;"
                          "background-color: %2;"
                          "margin: 0px %1px %1px %1px;}")
                       .arg(RADIUS*scale)
                       .arg(ColorManager::GetColorHex(ColorManager::CN_MENU_BG));
        m_bottomFrame->setStyleSheet(qss);
    }
}

void DigiBaseMenu::mousePressEvent(QMouseEvent *event)
{
    QAction *action = actionAt(event->pos());
    if (action && !action->isEnabled())
    {
        return; // 阻止事件继续传递
    }
    QMenu::mousePressEvent(event);
}

#ifdef Q_OS_MAC
void DigiBaseMenu::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
#endif

/******************
 * DigiBaseAction *
 ******************/
DigiBaseAction::DigiBaseAction(QWidget *parent):
    QWidgetAction(parent),
    m_pWidget(nullptr)
{
    //分隔线构造函数
    m_pWidget = new ActionWidget("", "", true, this);
    setDefaultWidget(m_pWidget);

    QMenu *menu = qobject_cast<QMenu *>(parent);
    if (parent != nullptr)
        menu->addAction(this);

    m_width = m_pWidget->GetWidth();
    m_height = m_pWidget->GetHeight();

    setSeparator(true);
}

DigiBaseAction::DigiBaseAction(QWidget *parent, const QString &text, const QString &icon) :
    QWidgetAction(parent),
    m_pWidget(nullptr),
    m_text(text)
{
    //普通构造函数
    m_pWidget = new ActionWidget(text, icon, false, this);
    setDefaultWidget(m_pWidget);

    QMenu *menu = qobject_cast<QMenu *>(parent);
    if (menu != nullptr)
        menu->addAction(this);

    m_width = m_pWidget->GetWidth();
    m_height = m_pWidget->GetHeight();
    connect(m_pWidget, &ActionWidget::triggered, [this, menu]()
    {
        emit triggered(true);
        if(menu != nullptr && !menu->isHidden())
            menu->hide();
    });
}

DigiBaseAction *DigiBaseAction::CreateSeparatorAction(QWidget *parent)
{
    return new DigiBaseAction(parent);
}

void DigiBaseAction::SetScale(qreal scale)
{
    if (m_pWidget != nullptr)
        m_pWidget->SetScale(scale);
}

void DigiBaseAction::SetWidth(qreal width)
{
    m_width = width;
    if (m_pWidget != nullptr)
        m_pWidget->SetWidth(width);
}

void DigiBaseAction::SetHeight(qreal height)
{
    m_height = height;
    if (m_pWidget != nullptr)
        m_pWidget->SetHeight(height);
}

void DigiBaseAction::setMenu(QMenu *menu)
{
    if (m_pWidget != nullptr)
        m_pWidget->setSubMenu(menu);

    QWidgetAction::setMenu(menu);
}

void DigiBaseAction::setIcon(const QString &icon)
{
    if (m_pWidget != nullptr)
        m_pWidget->setIcon(icon);
}

void DigiBaseAction::setText(const QString &text)
{
    if (m_pWidget != nullptr)
        m_pWidget->setText(text);
}

QString DigiBaseAction::text()
{
    return m_text;
}

void DigiBaseAction::setVisible(bool b)
{
    if (m_pWidget != nullptr)
        m_pWidget->setVisible(b);

    QWidgetAction::setVisible(b);
}

void DigiBaseAction::setContentsMargins(int left, int top, int right, int bottom)
{
    if (m_pWidget != nullptr)
        m_pWidget->setContentsMargins(QMargins(left, top, right, bottom));
}

void DigiBaseAction::setContentsMargins(const QMargins &margins)
{
    if (m_pWidget != nullptr)
        m_pWidget->setContentsMargins(margins);
}

void DigiBaseAction::setSpacing(int spacing)
{
    if (m_pWidget != nullptr)
        m_pWidget->setSpacing(spacing);
}

/****************
 * ActionWidget *
 ****************/
ActionWidget::ActionWidget(const QString &text, const QString &icon, bool bSeparator, QAction *action, QFrame *parent) :
    DigiBaseFrame(parent),
    m_subMenu(nullptr),
    m_checkMouseTimer(nullptr),
    m_action(action),
    m_pBtnIcon(nullptr),
    m_pLabText(nullptr),
    m_pBtnArrow(nullptr),
    m_hLayout(nullptr)
{
    if (bSeparator)
        InitSeparatorAction();
    else
        InitNormalAction(text, icon);

    setProperty("Separator", bSeparator);
}

ActionWidget::~ActionWidget()
{

}

void ActionWidget::setSubMenu(QMenu *menu)
{
     m_subMenu = menu;
     m_pBtnArrow->setVisible(true);

     if (m_checkMouseTimer == nullptr)
     {
         m_checkMouseTimer = new QTimer(this);
         m_checkMouseTimer->setInterval(100);

         connect(m_checkMouseTimer, &QTimer::timeout, this, [this]()
         {
             // 鼠标不处于选项或子菜单上，手动发送离开事件，将子菜单隐藏
             if(!rect().contains(mapFromGlobal(QCursor::pos())) && m_subMenu &&
                !m_subMenu->rect().contains(m_subMenu->mapFromGlobal(QCursor::pos())))
             {
                 QEvent *leaveEvent = new QEvent(QEvent::Leave);
                 QApplication::postEvent(this, leaveEvent);
             }
         });
     }
}

void ActionWidget::setIcon(const QString &icon)
{
    m_IconPath = icon;
    m_pBtnIcon->SetCustomIcon(icon);
    m_pBtnIcon->SetWidth(20);
    m_pBtnIcon->SetHeight(20);
    m_pBtnIcon->SetScale(m_FrameScale);
}

void ActionWidget::setText(const QString &text)
{
    m_pLabText->setText(text);
}

void ActionWidget::SetCheckState(bool bChecked)
{
    if (bChecked)
        m_pLabText->SetTextColor(ColorManager::GetColor(ColorManager::CN_MENU_ACTION_TEXT_SELECTED));
    else
        m_pLabText->SetTextColor(ColorManager::GetColor(ColorManager::CN_GLOBAL_TEXT));

    m_pLabText->UpdateTextColor();

    if (m_IconPath.isEmpty()) //传入图标为空时，选中状态使用默认的√图片，未选中显示空图片
    {
        m_pBtnIcon->SetCustomIcon(bChecked ? ResFun::GetPngResUrlByName(ICON_COMM_MENUSELECTED) : "");
        m_pBtnIcon->SetWidth(24);
        m_pBtnIcon->SetHeight(24);
        m_pBtnIcon->SetScale(m_FrameScale);
    }

    update();
}

void ActionWidget::startTimer()
{
    if(m_checkMouseTimer && !m_checkMouseTimer->isActive())
        m_checkMouseTimer->start();
}

void ActionWidget::stopTimer()
{
    if(m_checkMouseTimer && m_checkMouseTimer->isActive())
        m_checkMouseTimer->stop();
}

void ActionWidget::setContentsMargins(const QMargins &margins)
{
    DigiBaseFrame::SetLayoutMargins(margins);
}

void ActionWidget::setSpacing(int spacing)
{
    DigiBaseFrame::SetLayoutSpacing(spacing, spacing);
}

void ActionWidget::enterEvent(QEnterEvent *event)
{
    startTimer();

    if (m_action->menu() != nullptr && m_action->isEnabled()) //有子菜单
    {
        QTimer::singleShot(200, this, [this]() //停留200ms,显示子菜单
        {
            if (rect().contains(mapFromGlobal(QCursor::pos())) && m_checkMouseTimer->isActive())
                m_action->hover();
        });
    }

    DigiBaseFrame::enterEvent(event);
}

void ActionWidget::leaveEvent(QEvent *event)
{
    stopTimer();

    QMenu *menu = m_action->menu();
    if (menu!=nullptr && menu->isVisible()) //有正在显示的子菜单
    {
        menu->hide();
        update();
    }

    DigiBaseFrame::leaveEvent(event);
}

void ActionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_subMenu != nullptr)
    {
        event->ignore();
        return;
    }

    stopTimer();

    QMenu *menu = qobject_cast<QMenu*>(m_action->parent());
    if(menu)
    {
        QAction *action = menu->menuAction();
        DigiBaseAction *menuAction = qobject_cast<DigiBaseAction*>(action);
        if(menuAction != nullptr) //点击了子菜单中的Action
        {
            ActionWidget *actionWidget = qobject_cast<ActionWidget*>(menuAction->defaultWidget());

            QEvent *leaveEvent = new QEvent(QEvent::Leave);
            QApplication::postEvent(actionWidget, leaveEvent);

            QMenu *mainMenu = qobject_cast<QMenu*>(action->parent());
            mainMenu->hide();
        }
    }
    DigiBaseFrame::mouseReleaseEvent(event);
}

void ActionWidget::SetCustomStyleSheet(qreal scale)
{
    DigiBaseFrame::SetCustomStyleSheet(scale);
    QString qss = styleSheet();
    qss += QString("ActionWidget:hover[Separator=false]{background: %1;}")
                       .arg(ColorManager::GetColorHex(ColorManager::CN_MENU_ACTION_BACKGROUND_HOVER));
    setStyleSheet(qss);
}

void ActionWidget::InitNormalAction(const QString &text, const QString &icon)
{
    m_height = 40;

    m_IconPath = icon;
    m_pBtnIcon = new DigiIconPushBtn(this);
    m_pBtnIcon->SetCustomIcon(icon);
    m_pBtnIcon->SetWidth(20);
    m_pBtnIcon->SetHeight(20);
    m_pBtnIcon->SetStyleSize(0, 0, 0, 0);
    m_pBtnIcon->SetStyleBtnAllColor("#00000000");
    m_pBtnIcon->setAttribute(Qt::WA_TransparentForMouseEvents); //鼠标事件穿透，忽略点击

    m_pLabText = new DigiBaseLabel(this);
    m_pLabText->setText(text);

    m_pBtnArrow = new DigiIconPushBtn(this);
    m_pBtnArrow->SetCustomIcon(ResFun::GetPngResUrlByName(PNG_EQUIPMENTMANAGEMENT_SCROLLBAR_RIGHT));
    m_pBtnArrow->SetWidth(12);
    m_pBtnArrow->SetHeight(12);
    m_pBtnArrow->setVisible(false);
    m_pBtnArrow->SetStyleSize(0, 0, 0, 0);
    m_pBtnArrow->SetStyleBtnAllColor("#00000000");
    m_pBtnArrow->setAttribute(Qt::WA_TransparentForMouseEvents); //鼠标事件穿透，忽略点击

    m_hLayout = new QHBoxLayout;
    m_hLayout->setContentsMargins(QMargins(12, 0, 12, 0));
    m_hLayout->setSpacing(6);
    m_hLayout->addWidget(m_pBtnIcon);
    m_hLayout->addWidget(m_pLabText);
    m_hLayout->addStretch(1);
    m_hLayout->addWidget(m_pBtnArrow);
    setLayout(m_hLayout);

    connect(m_action, &DigiBaseAction::hovered, this, &ActionWidget::OnHovered);
    connect(m_action, &DigiBaseAction::toggled, this, &ActionWidget::OnToggled);
}

void ActionWidget::InitSeparatorAction()
{
    m_height = 13;

    m_pLabText = new DigiBaseLabel(this);
    m_pLabText->SetHeight(1);
    m_pLabText->SetBGColor(ColorManager::GetColor(ColorManager::CN_MENU_ACTION_SEPARATOR));

    m_hLayout = new QHBoxLayout;
    m_hLayout->setContentsMargins(QMargins(12, 0, 12, 0));
    m_hLayout->setSpacing(0);
    m_hLayout->addWidget(m_pLabText);
    setLayout(m_hLayout);
}

void ActionWidget::OnHovered()
{
    QMenu *menu = m_action->menu();
    if (menu!=nullptr && m_action->isEnabled()) //有子菜单
    {
        //显示子菜单
        QPointF pos = mapToGlobal(rect().topRight());
        qreal yOffect = SHADOW_WIDTH * m_FrameScale; //阴影宽度矫正
        pos.setY(pos.y() - yOffect);
        menu->popup(pos.toPoint());
    }
}

void ActionWidget::OnToggled(bool checked)
{
    SetCheckState(checked);
}

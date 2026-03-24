#include "digibasedialog.h"

#include <QtMath>
#include <QDebug>
#include <QScreen>
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <qglobal.h>
#include "Res/resfun.h"
#include "digibaselabel.h"
#include "digisvgpushbtn.h"
#include "digibasewidget.h"

DigiBaseDialog::DigiBaseDialog(QWidget *MainWidget, const QString& title, QWidget *parent) :
    QDialog(parent),
    m_Result(nullptr),
    m_TitleBar(new DigiBaseDialogTitleBar),
    m_DialogWidget(nullptr),
    m_MainWidget(MainWidget),
    m_ShadowLayout(nullptr),
    m_MainLayout(nullptr),
    m_ContentLayout(nullptr),
    m_ShadowMargin(SHADOW_WIDTH,SHADOW_WIDTH,SHADOW_WIDTH,SHADOW_WIDTH),
    m_scale(-1),
    m_Radius(0),
    m_BGColor(ColorManager::GetColorHex(ColorManager::CN_DIALOG_BACKGROUND)),
    m_TitleBarBGColor(ColorManager::GetColorHex(ColorManager::CN_DIALOG_TITLE_BACKGROUND)),
    m_Border(0),
    m_bNeedPreventCloseDialog(false),
    m_bShadowVisibel(true),
    m_TitleHeight(40)
{
    if(title.isEmpty())
    {
        m_TitleBar->setVisible(false);
        m_TitleHeight = 0;
    }
    else
        m_TitleBar->SetTitleText(title);

    InitWidget();
    InitStyle();
    CreateLayout();
    CreateConnect();
}

void DigiBaseDialog::SetWindowStaysOnTopHint(bool onTop)
{
    Qt::WindowFlags flag;
    flag = Qt::FramelessWindowHint /*| Qt::Tool*/ |Qt::Dialog |Qt::X11BypassWindowManagerHint;

    if(onTop)
        flag |= Qt::WindowStaysOnTopHint;
    else
        flag &= ~Qt::WindowStaysOnTopHint;

    setWindowFlags(flag);
}

void DigiBaseDialog::SetBGColor(const QColor &color)
{
    m_BGColor = color.name(QColor::HexArgb);
}

void DigiBaseDialog::SetTitleBGColor(const QColor &color)
{
    m_TitleBarBGColor = color.name(QColor::HexArgb);
}

void DigiBaseDialog::SetTitleIcon(const QString &path)
{
    m_TitleBar->SetIconPath(path);
}

void DigiBaseDialog::SetTitleText(const QString &text)
{
    m_TitleBar->SetTitleText(text);
}

void DigiBaseDialog::SetSubTitleText(const QString &text)
{
    if(m_TitleBar->GetSubTitleText().isEmpty())
    {
        m_TitleHeight += 24;
        m_height += 24;
        m_TitleBar->SetSubTitleText(text);
        if(!qFuzzyCompare(-1,m_scale))
            SetScale(m_scale);
    }
}

void DigiBaseDialog::SetTitleTextColor(const QColor &color)
{
    m_TitleBar->SetTitleTextColor(color);
}

void DigiBaseDialog::SetTitleIconVisible(bool visible)
{
    m_TitleBar->SetIconVisible(visible);
}

void DigiBaseDialog::SetCloseBtnVisible(bool visible)
{
    m_TitleBar->SetCloseBtnVisible(visible);
}

void DigiBaseDialog::SetNeedPreventCloseDialog(bool needToClose)
{
    m_bNeedPreventCloseDialog = needToClose;
}

void DigiBaseDialog::SetRoundedRadius(qreal radius)
{
    m_Radius = radius;
}

void DigiBaseDialog::SetShadowVisibel(bool visible)
{
    m_bShadowVisibel = visible;
    if(!visible)
    {
        this->setAttribute(Qt::WA_TranslucentBackground, false);
        m_ShadowMargin = QMargins(0, 0, 0, 0);
        m_ShadowLayout->setContentsMargins(m_ShadowMargin);
        m_Radius = 0;
        m_Border = 1;
    }
}

void DigiBaseDialog::SetNullParent()
{
    setParent(nullptr);
    Qt::WindowFlags flag = windowFlags();
    flag &= ~Qt::Dialog;
    flag |= Qt::Tool;
    setWindowFlags(flag);
}

void DigiBaseDialog::setBorderSize(int border)
{
    m_Border = border;
}

void DigiBaseDialog::SetScale(qreal scale)
{
    //布局
    m_ShadowLayout->setContentsMargins(m_ShadowMargin * scale);

    //主窗口
    int shadow = m_bShadowVisibel ? SHADOW_WIDTH*2 : 0;
    this->setFixedSize((m_width+shadow)*scale, (m_height+shadow)*scale);
    m_DialogWidget->setFixedSize(m_width*scale, m_height*scale);

    //内容窗口
    if (m_MainWidget!=nullptr && m_MainWidget->inherits("IWidgetBase"))
    {
        dynamic_cast<IWidgetBase *>(m_MainWidget)->SetScale(scale);
    }

    //标题栏
    m_TitleBar->SetScale(scale);

    //样式
    SetCustomStyleSheet(scale);
}

void DigiBaseDialog::SetHeight(qreal height)
{
    if (height <= m_TitleHeight)
    {
        m_height = m_TitleHeight + 1;
    }
    else
    {
        m_height = height;
    }
}

void DigiBaseDialog::setMainWidget(QWidget *widget)
{
    m_MainWidget = widget;
    m_ContentLayout->takeAt(0);
    m_ContentLayout->addWidget(widget);
    widget->setObjectName("main");

    if (widget!=nullptr && widget->inherits("IWidgetBase"))
    {
        dynamic_cast<IWidgetBase*>(widget)->SetScale(m_scale);
    }
}

QWidget *DigiBaseDialog::GetMainWidget()
{
    return m_MainWidget;
}

QWidget *DigiBaseDialog::GetDialogTitle()
{
    return m_TitleBar;
}

void DigiBaseDialog::reject()
{
    if (m_bNeedPreventCloseDialog)
        return;

    QDialog::reject();
}

void DigiBaseDialog::closeDialog()
{
    if (m_bNeedPreventCloseDialog)
        return;

    emit SendCloseReq();
    this->done(-1);
}

void DigiBaseDialog::moveEvent(QMoveEvent *event)
{
    if(!m_DialogWidget || !m_MainLayout)
        return;

    qreal scale = GetScale();
    if(!qFuzzyIsNull(scale) && !qFuzzyCompare(scale,m_scale))
    {
        m_scale = scale;
        SetScale(m_scale);
    }
    QDialog::moveEvent(event);
}

void DigiBaseDialog::closeEvent(QCloseEvent *event)
{
    if (m_bNeedPreventCloseDialog)
        event->ignore();
    else
        QDialog::closeEvent(event);
}

void DigiBaseDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);

    if (!m_bShadowVisibel)
        return;

    int shadow = SHADOW_WIDTH*m_scale;
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    // 圆角阴影边框;
    QColor color(150, 150, 150, 55);
    for (int i = 0; i < shadow; i++)
    {
        qreal alpha =  120*m_scale - qSqrt(i) * 50*m_scale;
        alpha = (alpha>0) ? alpha:0;
        color.setAlpha(alpha);
        painter.setPen(color);
        painter.drawRoundedRect(shadow - i,
                                shadow - i,
                                this->width() - (shadow - i) * 2,
                                this->height() - (shadow - i) * 2,
                                m_Radius*m_scale, m_Radius*m_scale);
    }
}

void DigiBaseDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return || event->key()==Qt::Key_Escape)
        event->ignore();
    else
        QDialog::keyPressEvent(event);
}

void DigiBaseDialog::showEvent(QShowEvent *event)
{
    setAttribute(Qt::WA_Mapped);
    activateWindow();
    raise();
    setFocus();

    QDialog::showEvent(event);
}

void DigiBaseDialog::SetCustomStyleSheet(qreal scale)
{
    QString qss;
    qss = QString("#dialog{border-radius: %1px;background: %2}")
                  .arg(m_Radius*scale).arg(m_BGColor);
    m_DialogWidget->setStyleSheet(qss);

    qss = QString("#titleBar{"
                       "border-top-left-radius: %1px;"
                       "border-top-right-radius: %1px;"
                       "background: %2;}")
                  .arg(m_Radius*scale)
                  .arg(m_TitleBarBGColor);
    m_TitleBar->setStyleSheet(qss);

    qss = QString("#main{border: %1px solid %2;}")
                  .arg(m_Border*scale)
                  .arg(ColorManager::GetColorHex(ColorManager::CN_GROUPBOX_BORDER));
    m_MainWidget->setStyleSheet(qss);
}

qreal DigiBaseDialog::GetScale()
{
    QScreen* screen = this->screen();
    if(screen)
    {
        qreal physicalDpi = screen->physicalDotsPerInch();
        qreal logicalDpi = screen->logicalDotsPerInch();
        qreal scale;

        if(qFuzzyCompare(logicalDpi,96.0))
            scale = physicalDpi/96.0;
        else
            scale = logicalDpi/96.0;

        if (scale>0 && scale<=0.75)
            return 0.5;
        else if (scale>0.75 && scale<=1.25)
            return 1.0;
        else if (scale>1.25 && scale<=1.75)
            return 1.5;
        else if (scale>1.75 && scale<=2.25)
            return 2.0;
        else if (scale>2.25 && scale<=2.5)
            return 2.5;
        else
            return 3.0;
    }
    return 1.0;
}

void DigiBaseDialog::InitWidget()
{
    m_TitleBar->setParent(this);
    m_DialogWidget = new QWidget(this);

    m_TitleBar->setObjectName("titleBar");
    m_DialogWidget->setObjectName("dialog");
    m_MainWidget->setObjectName("main");

    if(m_MainWidget != nullptr)
    {
        m_MainWidget->setParent(m_DialogWidget);
        //dialog大小基值初始化
        if (m_MainWidget->inherits("IWidgetBase"))
        {
            m_width = dynamic_cast<IWidgetBase *>(m_MainWidget)->GetWidth();
            m_height = dynamic_cast<IWidgetBase *>(m_MainWidget)->GetHeight();
        }
    }

    if(qFuzzyIsNull(m_width))
        m_width = m_MainWidget->width();
    if(qFuzzyIsNull(m_height))
        m_height = m_MainWidget->height();

    m_height += m_TitleHeight;
}

void DigiBaseDialog::InitStyle()
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->SetWindowStaysOnTopHint(false);
}

void DigiBaseDialog::CreateLayout()
{
    //dialog内容布局，添加m_MainWidget
    m_ContentLayout = new QVBoxLayout;
    m_ContentLayout->addWidget(m_MainWidget);
    m_ContentLayout->setSpacing(0);
    m_ContentLayout->setContentsMargins(0,0,0,0);

    //主布局，添加标题栏、内容布局
    m_MainLayout = new QVBoxLayout;
    m_MainLayout->addWidget(m_TitleBar);
    m_MainLayout->addLayout(m_ContentLayout);
    m_MainLayout->setStretch(1,1);
    m_MainLayout->setSpacing(0);
    m_MainLayout->setContentsMargins(0,0,0,0);

    //主窗口，布局设置为主布局
    m_DialogWidget->setLayout(m_MainLayout);

    //阴影布局，包裹着主窗口
    m_ShadowLayout = new QVBoxLayout;
    m_ShadowLayout->addWidget(m_DialogWidget);
    m_ShadowLayout->setSpacing(0);
    m_ShadowLayout->setContentsMargins(m_ShadowMargin);
    this->setLayout(m_ShadowLayout);
}

void DigiBaseDialog::CreateConnect()
{
    connect(m_TitleBar, SIGNAL(closeRequest()), this, SLOT(closeDialog()));
    connect(m_TitleBar, SIGNAL(moveWindowRequest(QPoint)), this, SLOT(MoveWindow(QPoint)));

    Widget_DialogBase *widget = qobject_cast<Widget_DialogBase*>(m_MainWidget);
    if (widget != nullptr)
    {
        connect(widget, SIGNAL(moveMainWindowRequest(QPoint)), this, SLOT(MoveWindow(QPoint)));
        connect(widget, SIGNAL(doneRequest(ResultType,QObject*)), this, SLOT(doneResponse(ResultType,QObject*)));
        connect(widget, SIGNAL(changeSizeRequest(qreal, qreal)), this, SLOT(changeSize(qreal, qreal)));
    }
}

void DigiBaseDialog::MoveWindow(QPoint point)
{
    this->move(this->pos()+point);
}

void DigiBaseDialog::doneResponse(ResultType r, QObject *out)
{
    if (r == DoneWithParam)
    {
        this->m_Result = out;
    }
    emit SendCloseReq();
    done(static_cast<int>(r));
}

void DigiBaseDialog::changeSize(qreal width, qreal height)
{
    if(!qFuzzyIsNull(width))
    {
        m_width = width;
        m_MainWidget->setFixedWidth(width*m_scale);
    }
    if(!qFuzzyIsNull(height))
    {
        m_height = height + m_TitleHeight;
        m_MainWidget->setFixedHeight(height*m_scale);
    }

    SetScale(m_scale);
}


/****************************************
 *  以下为 DigiBaseDialogTitleBar 标题栏类
 ****************************************/

DigiBaseDialogTitleBar::DigiBaseDialogTitleBar(QWidget *parent) :
    QFrame(parent),
    m_IconLab(nullptr),
    m_TitleLab(nullptr),
    m_SubTitleLab(nullptr),
    m_CloseBtn(nullptr),
    m_MainLayout(nullptr),
    m_LabelLayout(nullptr),
    m_LabelWidget(nullptr),
    m_bLeftButtonPress(false),
    m_TitleHeight(40)
{
    InitWidget();
    InitStyle();
    CreateLayout();
    CreateConnect();
}

void DigiBaseDialogTitleBar::SetTitleText(const QString &title)
{
    m_TitleLab->setText(title);
}

void DigiBaseDialogTitleBar::SetSubTitleText(const QString &title)
{
    if(m_SubTitleLab->text().isEmpty())
        m_TitleHeight += 24;
    m_SubTitleLab->setText(title);
    m_SubTitleLab->setVisible(true);
}

QString DigiBaseDialogTitleBar::GetSubTitleText()
{
    return m_SubTitleLab->text();
}

void DigiBaseDialogTitleBar::SetIconPath(const QString &path)
{
    m_IconLab->LoadImagePath(path);

    //判断图标有无超出高度
    if (m_IconLab->GetWidth() >= m_TitleHeight)
    {
        m_IconLab->SetWidth(m_TitleHeight-1);
        m_IconLab->SetHeight(m_TitleHeight-1);
    }
}

void DigiBaseDialogTitleBar::SetTitleTextColor(const QColor &color)
{
    m_TitleLab->SetTextColor(color);
    m_TitleLab->UpdateTextColor();
}

void DigiBaseDialogTitleBar::SetIconVisible(bool visible)
{
    m_IconLab->setVisible(visible);
}

void DigiBaseDialogTitleBar::SetCloseBtnVisible(bool visible)
{
    m_CloseBtn->setVisible(visible);
}

int DigiBaseDialogTitleBar::GetTitleHeight()
{
    return m_TitleHeight;
}

void DigiBaseDialogTitleBar::SetScale(qreal scale)
{
    //高度缩放，宽度自动拉伸无需缩放
    setFixedHeight(m_TitleHeight * scale);

    //子控件
    auto childlist = findChildren<QWidget*>();
    foreach(auto child,childlist)
    {
        if(child->inherits("IWidgetBase"))
        {
            dynamic_cast<IWidgetBase *>(child)->SetScale(scale);
        }
    }

    //布局
    m_MainLayout->setContentsMargins(6*scale, 0, 12*scale, 0);
}

void DigiBaseDialogTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_ptPress = event->globalPos();
        m_bLeftButtonPress = true;
    }
    event->ignore();
}

void DigiBaseDialogTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bLeftButtonPress)
    {
        m_ptMove = event->globalPos();
        emit moveWindowRequest(m_ptMove-m_ptPress);
        m_ptPress = m_ptMove;
    }
    event->ignore();
}

void DigiBaseDialogTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bLeftButtonPress = false;
    }
    event->ignore();
}

void DigiBaseDialogTitleBar::InitWidget()
{
    m_IconLab = new DigiBaseLabel(this);
    m_TitleLab = new DigiBaseLabel(this);
    m_SubTitleLab = new DigiBaseLabel(this);
    m_CloseBtn = new DigiSvgPushBtn(this);
    m_SubTitleLab->setVisible(false);

    m_IconLab->LoadImagePath(ResFun::GetPngResUrlByName(PNG_COMMON_DIALOG_INFO));
    m_CloseBtn->LoadImagePath(ResFun::GetPngResUrlByName(ICON_LOGIN_CLOSE));
    SetIconVisible(false);

    m_CloseBtn->SetWidth(24);
    m_CloseBtn->SetHeight(24);
    m_CloseBtn->setFocusPolicy(Qt::NoFocus);

    m_TitleLab->SetHeight(24);
    m_TitleLab->SetTextSize(16);
//    m_TitleLab->SetTextWeight(QFont::Medium);
    m_TitleLab->setAlignment(Qt::AlignCenter);
    m_SubTitleLab->SetHeight(20);
    m_SubTitleLab->SetTextColor(ColorManager::GetColorHex(ColorManager::CN_DIALOG_SUB_TITLT_TEXT));
    m_SubTitleLab->setAlignment(Qt::AlignCenter);
}

void DigiBaseDialogTitleBar::InitStyle()
{
    SetTitleTextColor(ColorManager::GetColorHex(ColorManager::CN_DIALOG_TITLT_TEXT));
}

void DigiBaseDialogTitleBar::CreateLayout()
{
    m_LabelWidget = new DigiBaseWidget;
    m_LabelLayout = new QVBoxLayout(m_LabelWidget);
    m_LabelLayout->setContentsMargins(0, 0, 0, 0);
    m_LabelLayout->setSpacing(0);
    m_LabelLayout->addStretch(1);
    m_LabelLayout->addWidget(m_TitleLab);
    m_LabelLayout->addWidget(m_SubTitleLab);
    m_LabelLayout->addStretch(1);

    m_MainLayout = new QHBoxLayout;
    QHBoxLayout *iconLayout = new QHBoxLayout;
    iconLayout->addWidget(m_IconLab);
    iconLayout->addStretch(1);
    QHBoxLayout *closeLayout = new QHBoxLayout;
    closeLayout->addStretch(1);
    closeLayout->addWidget(m_CloseBtn);
    m_MainLayout->addWidget(m_IconLab);
    m_MainLayout->addLayout(iconLayout);
    m_MainLayout->addWidget(m_LabelWidget);
    m_MainLayout->addLayout(closeLayout);
    m_MainLayout->setContentsMargins(6,0,12,0);
    this->setLayout(m_MainLayout);
}

void DigiBaseDialogTitleBar::CreateConnect()
{
    connect(m_CloseBtn, SIGNAL(clicked()), this, SIGNAL(closeRequest()));
}

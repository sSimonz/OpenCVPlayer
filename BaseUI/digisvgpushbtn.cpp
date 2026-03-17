#include "digisvgpushbtn.h"
#include <QPainter>
#include <QPaintEvent>
#include <QSvgRenderer>
#include <QPointF>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include "Base/ifontconfig.h"

DigiSvgPushBtn::DigiSvgPushBtn(QWidget *Parent) :
    QPushButton(Parent),
    m_BackImg(nullptr),
    m_BtnStatus(Normal),
    m_picWidth(48.0),
    m_picHigh(48.0),
    m_DirectFlag(Left2Right),
    m_slice(1),
    m_Renderer(nullptr),
    m_bEnter(false),
    m_checkTimer(nullptr)
{
    m_scale = 0;
    m_width = 48.0;
    m_height = 48.0;
    setAttribute(Qt::WA_LayoutUsesWidgetRect);
}

DigiSvgPushBtn::~DigiSvgPushBtn()
{
    if(m_BackImg)
    {
        delete m_BackImg;
        m_BackImg = nullptr;
    }
}

/**
 * @brief 将图片加载到SVG渲染类成员m_Renderer，并计算出小图片的长宽
 */
void DigiSvgPushBtn::LoadImagePath(QString path, QString text, int slice, int directFlag)
{
    m_slice = slice;
    m_DirectFlag = directFlag;
    m_text = text;
    m_Font = GetGlobalFont();

    if(path.contains(".svg"))
       loadSvgImg(path,directFlag);
    else
       loadNormalImg(path,directFlag);

    m_width = m_picWidth;
    m_height = m_picHigh;
}

void DigiSvgPushBtn::SetScale(qreal scale)
{
    m_scale = scale;
    //改变字体大小
    if(!m_text.isEmpty())
    {
        QFont aFont = m_Font;
        aFont.setPixelSize(m_Font.pixelSize()*scale);
        setFont(aFont);
    }

    //改变窗体大小，触发resize 事件，重绘窗口
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);
}

/**
 * @brief 根据当前图片可视位置m_curViewBox，绘制按钮图像
 */
void DigiSvgPushBtn::paintEvent (QPaintEvent * Event)
{
    Q_UNUSED(Event)
    if(!isEnabled() && m_BtnStatus != Disabled)
    {
        m_BtnStatus = Disabled;
        updateViewBox();
        return;
    }
    else if(isEnabled() && m_BtnStatus == Disabled)
    {
        m_BtnStatus = Normal;
        updateViewBox();
        return;
    }

    QPainter painter(this);
    if(m_Renderer && m_Renderer->isValid())
    {
        m_Renderer->setViewBox(m_curViewBox);
        m_Renderer->render(&painter);
    }
    else if(m_BackImg && !m_BackImg->isNull())
    {
        QRect pos(0,0,width(),height());
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter.drawPixmap(pos,*m_BackImg,m_curViewBox);
    }

    if(!m_text.isEmpty())
    {
        QRect pos(0,0,width(),height());
        painter.setPen(Qt::red);
        painter.drawText(pos,Qt::AlignHCenter | Qt::AlignVCenter,m_text);
    }

    if(!m_Renderer && !m_BackImg && m_text.isEmpty())
    {
        return QPushButton::paintEvent(Event);
    }
}

/**
 * @brief 更新当前按钮状态记录m_BtnStatus，并将当前图片可视位置m_ViewBox移动到悬停图像下
 */
void DigiSvgPushBtn::enterEvent(QEnterEvent *Event)
{
    m_bEnter = true;
    if(m_checkTimer && !m_checkTimer->isActive())
        m_checkTimer->start();
    if(isCheckable() && isChecked())
    {
        m_BtnStatus = Pressed;
        updateViewBox();
    }
    else if(isEnabled() && m_BtnStatus!=Pressed)
    {
        m_BtnStatus = Hover;
        updateViewBox();
    }
    QPushButton::enterEvent(Event);
}

/**
 * @brief 更新当前按钮状态记录m_BtnStatus，并将当前图片可视位置m_ViewBox移动到正常图像下
 */
void DigiSvgPushBtn::leaveEvent ( QEvent * Event )
{
    m_bEnter = false;
    if(m_checkTimer && m_checkTimer->isActive())
        m_checkTimer->stop();
    if(isCheckable() && isChecked())
    {
        m_BtnStatus = Pressed;
        updateViewBox();
    }
    else if(isEnabled())
    {
       m_BtnStatus = Normal;
       updateViewBox();
    }
    QPushButton::leaveEvent(Event);
}

/**
 * @brief 更新当前按钮状态记录m_BtnStatus，并将当前图片可视位置m_ViewBox移动到选中图像下
 */
void DigiSvgPushBtn::mousePressEvent (QMouseEvent * Event )
{
    if(isEnabled())
    {
        m_BtnStatus = Pressed;
        updateViewBox();
    }

    QPushButton::mousePressEvent(Event);
}

/**
 * @brief 更新当前按钮状态记录m_BtnStatus，并将当前图片可视位置m_ViewBox移动到悬停图像下
 */
void DigiSvgPushBtn::mouseReleaseEvent(QMouseEvent *Event)
{
    if(isCheckable() && isChecked())
    {
        m_BtnStatus = Pressed;
        updateViewBox();
    }
    else if(isEnabled())
    {
        if (m_bEnter)
            m_BtnStatus = Hover;
        else
            m_BtnStatus = Normal;
        updateViewBox();
    }

    QPushButton::mouseReleaseEvent(Event);
}

void DigiSvgPushBtn::changeEvent(QEvent *Event)
{
    if (Event->type()==QEvent::EnabledChange && isEnabled())
        updatePressStatus();

    QPushButton::changeEvent(Event);
}

QSize DigiSvgPushBtn::sizeHint () const
{
    return QSize(m_picWidth, m_picHigh);
}

void DigiSvgPushBtn::loadSvgImg(QString path, int DirectFlag)
{
    if(m_Renderer == nullptr)
        m_Renderer = new QSvgRenderer(this);

    if(m_Renderer->load(path))
    {
        m_ViewBox = m_Renderer->viewBoxF();
        if(DirectFlag == Left2Right || DirectFlag == Right2Left)
        {
            m_picWidth = m_ViewBox.width()/m_slice;
            m_picHigh = m_ViewBox.height();
        }
        else
        {
            m_picWidth = m_ViewBox.width();
            m_picHigh = m_ViewBox.height()/m_slice;
        }
        if(isCheckable() && isChecked())
        {
            m_BtnStatus = Pressed;
        }
        else if(isEnabled())
        {
            m_BtnStatus = Normal;
        }
        else
        {
            m_BtnStatus = Disabled;
        }
        updateViewBox();
    }
}

void DigiSvgPushBtn::loadNormalImg(QString path, int DirectFlag)
{
    if(m_BackImg == nullptr)
        m_BackImg = new QPixmap;

    if(m_BackImg->load(path))
    {
        m_ViewBox = m_BackImg->rect();
        if(DirectFlag == Left2Right || DirectFlag == Right2Left)
        {
            m_picWidth = m_ViewBox.width()/m_slice;
            m_picHigh = m_ViewBox.height();
        }
        else
        {
            m_picWidth = m_ViewBox.width();
            m_picHigh = m_ViewBox.height()/m_slice;
        }
        if(isCheckable() && isChecked())
        {
            m_BtnStatus = Pressed;
        }
        else if(isEnabled())
        {
            m_BtnStatus = Normal;
        }
        else
        {
            m_BtnStatus = Disabled;
        }
        updateViewBox();
    }
}

void DigiSvgPushBtn::setCheckable(bool bCheck)
{
    if (bCheck == isCheckable()) //防止重复设置
        return;

    QPushButton::setCheckable(bCheck);

    if (bCheck)
        connect(this, &DigiSvgPushBtn::toggled, this, &DigiSvgPushBtn::updatePressStatus);
    else
        disconnect(this, &DigiSvgPushBtn::toggled, this, &DigiSvgPushBtn::updatePressStatus);
}

void DigiSvgPushBtn::setChecked(bool status)
{
    QPushButton::setChecked(status);
    updatePressStatus();
}

void DigiSvgPushBtn::updatePressStatus()
{
    if(isCheckable() && isChecked())
    {
        m_BtnStatus = Pressed;
        updateViewBox();
    }
    else if(isEnabled())
    {
       m_BtnStatus = Normal;
       updateViewBox();
    }
}

void DigiSvgPushBtn::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiSvgPushBtn::GetCustomFont()
{
    return m_Font;
}

void DigiSvgPushBtn::updateViewBox()
{
    int index = m_BtnStatus == Normal? 0:m_BtnStatus == Hover?1:m_BtnStatus == Pressed?2:3;

    if(index >= m_slice)
       index = m_slice - 1;

    if(m_DirectFlag == Left2Right)
    {
        m_curViewBox = QRectF(index*m_picWidth,0.0,m_picWidth,m_picHigh);
    }
    else if(m_DirectFlag == Right2Left)
    {
        m_curViewBox = QRectF(m_ViewBox.width()-(index+1)*m_picWidth,0.0,m_picWidth,m_picHigh);
    }
    else if(m_DirectFlag == Up2Down)
    {
        m_curViewBox = QRectF(0.0,index*m_picHigh,m_picWidth,m_picHigh);
    }
    else if(m_DirectFlag == Down2Up)
    {
        m_curViewBox = QRectF(0.0,m_ViewBox.height()-(index+1)*m_picHigh,m_picWidth,m_picHigh);
    }
    update();
}

void DigiSvgPushBtn::initCheckTimer()
{
    m_checkTimer = new QTimer(this);
    m_checkTimer->setInterval(1000);
    m_checkTimer->setSingleShot(true);
    connect(m_checkTimer, &QTimer::timeout, this, [this]()
    {
        if(!rect().contains(mapFromGlobal(QCursor::pos())))
        {
            QEvent *leaveEvent = new QEvent(QEvent::Leave);
            QApplication::sendEvent(this, leaveEvent);
        }
    },Qt::UniqueConnection);
}

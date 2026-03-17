#include "digidelegatebtn.h"

#include <QPainter>
#include <QPainterPath>

DigiDelegateBtn::DigiDelegateBtn(QObject *parent):
    QObject(parent),
    m_slice(1),
    m_DirectFlag(Left2Right),
    m_picWidth(48.0),
    m_picHigh(48.0),
    m_Renderer(nullptr),
    m_BackImg(nullptr)
{

}

void DigiDelegateBtn::LoadImagePath(QString path, int slice, int directFlag)
{
    m_slice = slice;
    m_DirectFlag = directFlag;

    if(path.contains(".svg"))
        loadSvgImg(path, directFlag);
    else
        loadNormalImg(path, directFlag);
}

void DigiDelegateBtn::Draw(QPainter *painter, const QRect &rect, int btnStatus)
{
    painter->save();
    m_BtnStatus = btnStatus;
    updateViewBox(btnStatus);

    if(m_Renderer && m_Renderer->isValid())
    {
        QPainterPath path;
        path.addRect(rect);
        painter->setClipPath(path);
        m_Renderer->setViewBox(m_curViewBox);
        m_Renderer->render(painter, rect);
    }
    else if(m_BackImg && !m_BackImg->isNull())
    {
        painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter->drawPixmap(rect, *m_BackImg, m_curViewBox);
    }
    painter->restore();
}

void DigiDelegateBtn::DrawColor(QPainter *painter, const QRect &rect, QColor color, qreal radius)
{
    painter->save();

    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter->setPen(color);
    painter->setBrush(color);
    painter->drawRoundedRect(rect,radius,radius);

    painter->restore();
}

int DigiDelegateBtn::getBtnStatus()
{
    return m_BtnStatus;
}

void DigiDelegateBtn::loadSvgImg(QString path, int DirectFlag)
{
    if(m_Renderer.isNull())
        m_Renderer.reset(new QSvgRenderer);

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
    }
}

void DigiDelegateBtn::loadNormalImg(QString path, int DirectFlag)
{
    if(m_BackImg.isNull())
        m_BackImg.reset(new QPixmap);

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
    }
}

void DigiDelegateBtn::updateViewBox(int status)
{
    int index = 0;
    switch (status)
    {
        case Normal:
            index = 0;
            break;
        case Hover:
            index = 1;
            break;
        case Pressed:
            index = 2;
            break;
        case Disabled:
            index = 3;
            break;
        default:
            break;
    }

    if(m_DirectFlag == Left2Right)
        m_curViewBox = QRectF(index*m_picWidth,0.0,m_picWidth,m_picHigh);
    else if(m_DirectFlag == Right2Left)
        m_curViewBox = QRectF(m_ViewBox.width()-(index+1)*m_picWidth,0.0,m_picWidth,m_picHigh);
    else if(m_DirectFlag == Up2Down)
        m_curViewBox = QRectF(0.0,index*m_picHigh,m_picWidth,m_picHigh);
    else if(m_DirectFlag == Down2Up)
        m_curViewBox = QRectF(0.0,m_ViewBox.height()-(index+1)*m_picHigh,m_picWidth,m_picHigh);
}

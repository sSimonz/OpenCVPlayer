#include "digibaselabel.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QFontMetricsF>
#include <QDebug>
DigiBaseLabel::DigiBaseLabel(QWidget *parent) :
    QLabel(parent),
    m_BgColor("#00000000"),
    m_TextColor(ColorManager::GetColor(ColorManager::CN_GLOBAL_TEXT)),
    m_SvgImg(nullptr),
    m_PixImg(nullptr),
    m_bElidedText(false),
    m_ElideMode(Qt::ElideRight)
{
    m_scale = 0;
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();
    m_MaxWidth = QPair<qreal,qreal>(0,0);
    m_StyleSizeList<<0<<0;
}

DigiBaseLabel::DigiBaseLabel(const QString &text, QWidget *parent):
    QLabel(text, parent),
    m_BgColor("#00000000"),
    m_TextColor(ColorManager::GetColor(ColorManager::CN_GLOBAL_TEXT)),
    m_SvgImg(nullptr),
    m_PixImg(nullptr),
    m_bElidedText(false),
    m_ElideMode(Qt::ElideRight)
{
    m_scale = 0;
    m_width = 0;
    m_height = 0;
    m_Font = GetGlobalFont();
    m_MaxWidth = QPair<qreal,qreal>(0,0);
    m_StyleSizeList<<0<<0;
}

DigiBaseLabel::~DigiBaseLabel()
{
    if(m_PixImg != nullptr)
    {
        delete m_PixImg;
        m_PixImg = nullptr;
    }
}

void DigiBaseLabel::LoadImagePath(const QString &path, const QRect &viewBox)
{
    //viewBox默认参数是QRect(0,0,0,0),不传viewBox即使用图片全图

    if(path.contains(".svg"))
    {
        if(m_SvgImg == nullptr)
            m_SvgImg = new QSvgRenderer(this);

        if(m_SvgImg->load(path))
        {
            if (viewBox.isValid())
            {
                m_SvgImg->setViewBox(viewBox);
                m_width = viewBox.width();
                m_height =viewBox.height();
            }
            else
            {
                m_width = m_SvgImg->viewBoxF().width();
                m_height = m_SvgImg->viewBoxF().height();
            }
        }
    }
    else
    {
        if(m_PixImg == nullptr)
            m_PixImg = new QPixmap;

        if(m_PixImg->load(path))
        {
            if (viewBox.isValid())
            {
                *m_PixImg = m_PixImg->copy(viewBox);
                m_width = viewBox.width();
                m_height =viewBox.height();
            }
            else
            {
                m_width = m_PixImg->width();
                m_height = m_PixImg->height();
            }
        }
    }

    m_CurImgPath = path;
    update();
}

QString DigiBaseLabel::GetCurrentImagePath()
{
    return m_CurImgPath;
}

void DigiBaseLabel::SetElidedText(bool on, Qt::TextElideMode mode, qreal maxWidth)
{
    m_MaxWidth.second = maxWidth;
    m_bElidedText = on;
    m_ElideMode = mode;
    if (on)
        setSizePolicy(QSizePolicy::Expanding, sizePolicy().verticalPolicy());

}

void DigiBaseLabel::setText(const QString &text)
{
    m_text = text;
    QLabel::setText(m_text);

    if (!wordWrap() && m_bElidedText && !m_text.isEmpty() && isVisible())  //显示省略号
    {
        QMetaObject::invokeMethod(this, &DigiBaseLabel::LongTextProcessing, Qt::QueuedConnection);
    }
}

QString DigiBaseLabel::text() const
{
    return m_text;
}

void DigiBaseLabel::SetBGColor(const QColor &color)
{
    m_BgColor = color;
}

void DigiBaseLabel::SetTextColor(const QColor &textClr)
{
    m_TextColor = textClr;
}

void DigiBaseLabel::SetTextSize(int textSize)
{
    m_Font.setPixelSize(textSize);
}

void DigiBaseLabel::SetTextBold(bool bBold)
{
    m_Font.setBold(bBold);
}

void DigiBaseLabel::SetTextFamily(const QString &family)
{
    m_Font.setFamily(family);
}

void DigiBaseLabel::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

void DigiBaseLabel::SetTextWeight(int weight)
{
    QString family = GetGlobalFamily(weight);
    m_Font.setFamily(family);
}

QFont DigiBaseLabel::GetCustomFont()
{
    return m_Font;
}

void DigiBaseLabel::SetBorderRadius(qreal radius)
{
    m_StyleSizeList[BorderRadius] = radius;
}

void DigiBaseLabel::SetPadding(qreal padding)
{
    m_StyleSizeList[Padding] = padding;
}

void DigiBaseLabel::UpdateTextColor()
{
    qreal scale = m_Font.pixelSize() / font().pixelSize();
    SetCustomStyleSheet(scale);
}

void DigiBaseLabel::UpdateTextFont()
{
    qreal scale = m_Font.pixelSize() / font().pixelSize();
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);
}

void DigiBaseLabel::SetScale(qreal scale)
{
    m_scale = scale;
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);

    if (!qFuzzyIsNull(m_MaxWidth.second))
        m_MaxWidth.first = m_MaxWidth.second * scale;

    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height*scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width*scale);

    SetCustomStyleSheet(scale);
}

void DigiBaseLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if(m_SvgImg!=nullptr && m_SvgImg->isValid())
    {
        QPainter painter(this);
        m_SvgImg->render(&painter);
    }
    else if(m_PixImg!=nullptr && !m_PixImg->isNull())
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter.drawPixmap(rect(), *m_PixImg);
    }
}

void DigiBaseLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);

    if (!wordWrap() && m_bElidedText && !m_text.isEmpty())
        LongTextProcessing();
}

void DigiBaseLabel::SetCustomStyleSheet(qreal scale)
{
    Q_UNUSED(scale);

    QString qss;
    qss.append(QString("QLabel{"
                       "background: %1;"
                       "border-radius:%2px;"
                       "padding: %3px;"
                       "}")
               .arg(m_BgColor.name(QColor::HexArgb))
               .arg(m_StyleSizeList.at(BorderRadius) * scale)
               .arg(m_StyleSizeList.at(Padding) * scale));
    qss.append(QString("QLabel::enabled{"
                       "color: %1;"
                       "padding: 2px;"
                       "border-radius:%2px;"
                       "padding: %3px;"
                       "}")
               .arg(m_TextColor.name(QColor::HexArgb))
               .arg(m_StyleSizeList.at(BorderRadius) * scale)
               .arg(m_StyleSizeList.at(Padding) * scale));

    setStyleSheet(qss);
}

void DigiBaseLabel::LongTextProcessing()
{
    QFont aFont = font();
    QFontMetrics fm(aFont);
    int width = fm.horizontalAdvance(m_text);
    int maxWidth = qFuzzyIsNull(m_MaxWidth.second) ? this->width()
                                                   : static_cast<int>(m_MaxWidth.first);
    if (width > maxWidth)
    {
        QString str = fm.elidedText(m_text, m_ElideMode, maxWidth);
        QLabel::setText(str);
        this->setToolTip(m_text);
    }
    else
    {
        QLabel::setText(m_text);
        this->setToolTip("");
    }
}


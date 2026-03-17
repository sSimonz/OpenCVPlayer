#include "digiringprogressbarwidget.h"
#include <QDebug>
DigiRingProgressBarWidget::DigiRingProgressBarWidget(QWidget *parent):
    QWidget(parent)
{
    m_width = 0;
    m_height = 0;
    QFont CurFont = GetGlobalFont();
    m_Font = CurFont;
    m_TitleFont = CurFont;

    m_minValue = 0;
    m_maxValue = 100;
    m_value = 0;
    m_precision = 0;

    m_arcColor = "#EB6100";
    m_textColor = "#000000";
    m_titleColor = "#000000";
    m_bgColor ="#ffffff";
    m_baseColor = "#DDDFE1";

    m_percent = true;
    m_arcWidth = 16;
    m_startAngle = 180;
    m_endAngle = -180;
    SetTextSize(30);
    SetTitleTextSize(20);
}

DigiRingProgressBarWidget::~DigiRingProgressBarWidget()
{

}

void DigiRingProgressBarWidget::SetTextSize(int textSize)
{
    m_Font.setPixelSize(textSize);
}

void DigiRingProgressBarWidget::SetTextBold(bool bBold)
{
    m_Font.setBold(bBold);
}

void DigiRingProgressBarWidget::SetTextFamily(const QString &family)
{
    m_Font.setFamily(family);
}

void DigiRingProgressBarWidget::SetCustomFont(const QFont &font)
{
    m_Font = font;
}

QFont DigiRingProgressBarWidget::GetCustomFont()
{
    return m_Font;
}

void DigiRingProgressBarWidget::SetTextWeight(int weight)
{
    QString family = GetGlobalFamily(weight);
    m_Font.setFamily(family);
}

void DigiRingProgressBarWidget::SetScale(qreal scale)
{
    QFont aFont = m_Font;
    aFont.setPixelSize(m_Font.pixelSize() * scale);
    setFont(aFont);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height*scale);
    if (!qFuzzyIsNull(m_width))
        setFixedWidth(m_width*scale);

    SetCustomStyleSheet(scale);
}

void DigiRingProgressBarWidget::SetCustomStyleSheet(qreal scale)
{

}

void DigiRingProgressBarWidget::paintEvent(QPaintEvent *)
{
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制背景
    if (m_bgColor != Qt::transparent) {
        painter.setPen(Qt::NoPen);
        painter.fillRect(this->rect(), m_bgColor);
    }

    int width = this->width();
    int height = this->height();
    painter.translate(width / 2, height / 2);
    painter.scale(1/2.0, 1/2.0);

    //绘制圆弧
    drawArc(&painter);
    //绘制当前值
    drawValue(&painter);
}

void DigiRingProgressBarWidget::drawArc(QPainter *painter)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);
    int radius = side - m_arcWidth;
    painter->save();
    painter->setBrush(Qt::NoBrush);

    QPen pen;
    pen.setWidthF(m_arcWidth);
    pen.setCapStyle(Qt::RoundCap);

    //计算总范围角度,当前值范围角度,剩余值范围角度
    double angleAll = 360.0 - m_startAngle - m_endAngle;
    double angleCurrent = angleAll * ((m_value - m_minValue) / (m_maxValue - m_minValue));
    double angleOther = angleAll - angleCurrent;
    QRectF rect = QRectF(-radius, -radius, radius * 2, radius * 2);

    //绘制圆弧背景
    pen.setColor(m_baseColor);
    painter->setPen(pen);
    painter->drawArc(rect, (270 - m_startAngle - angleCurrent - angleOther) * 16, angleOther * 16);

    //绘制圆弧进度
    pen.setColor(m_arcColor);
    painter->setPen(pen);
    painter->drawArc(rect, (270 - m_startAngle - angleCurrent) * 16, angleCurrent * 16);

    painter->restore();
}

void DigiRingProgressBarWidget::drawValue(QPainter *painter)
{
    painter->save();

    QString strValue;
    if (m_percent) {
        double temp = m_value / (m_maxValue - m_minValue) * 100;
        strValue = QString("%1%").arg(temp, 0, 'f', m_precision);
    } else {
        strValue = QString("%1").arg((double)m_value, 0, 'f', m_precision);
    }

    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);
    int radius = side/2;
    QRectF textRect;
    if(!m_title.isEmpty())
    {
        textRect.setRect(-radius, -radius, radius * 2, radius*5/6);
        painter->setFont(m_TitleFont);
        painter->setPen(m_titleColor);
        QTextOption textOption(Qt::AlignCenter);
        textOption.setWrapMode(QTextOption::WordWrap);
        painter->drawText(QRectF(-radius, -radius/6, radius * 2, radius), m_title,textOption);
    }
    else {
        textRect.setRect(-radius, -radius, radius * 2, radius * 2);
    }
    painter->setFont(m_Font);
    painter->setPen(m_textColor);
    painter->drawText(textRect, Qt::AlignCenter, strValue);

    painter->restore();
}

void DigiRingProgressBarWidget::setRange(double minValue, double maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
}
void DigiRingProgressBarWidget::setRange(int minValue, int maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void DigiRingProgressBarWidget::setMinValue(double minValue)
{
    m_minValue = minValue;
}
void DigiRingProgressBarWidget::setMaxValue(double maxValue)
{
    m_maxValue = maxValue;
}

void DigiRingProgressBarWidget::setValue(double value)
{
    m_value = value;
    update();
}
void DigiRingProgressBarWidget::setValue(int value)
{
    m_value = value;
    update();
}

void DigiRingProgressBarWidget::setPrecision(int precision)
{
    m_precision = precision;
}
void DigiRingProgressBarWidget::setStartAngle(int startAngle)
{
    m_startAngle = startAngle;
}
void DigiRingProgressBarWidget::setEndAngle(int endAngle)
{
    m_endAngle = endAngle;
}
void DigiRingProgressBarWidget::setArcColor(const QColor &arcColor)
{
    m_arcColor = arcColor;
}

void DigiRingProgressBarWidget::setTextColor(const QColor &textColor)
{
    m_textColor = textColor;
}

void DigiRingProgressBarWidget::setBaseColor(const QColor &baseColor)
{
    m_baseColor = baseColor;
}

void DigiRingProgressBarWidget::setBgColor(const QColor &bgColor)
{
    m_bgColor = bgColor;
}


void DigiRingProgressBarWidget::setPercent(bool percent)
{
    m_percent = percent;
}

void DigiRingProgressBarWidget::setArcWidth(int arcWidth)
{
    m_arcWidth = arcWidth;
}


void DigiRingProgressBarWidget::setTitle(const QString &title)
{
    m_title = title;
}


void DigiRingProgressBarWidget::setTitleColor(const QColor &titleColor)
{
    m_titleColor = titleColor;
}

void DigiRingProgressBarWidget::SetTitleTextSize(int textSize)
{
    m_TitleFont.setPixelSize(textSize);
}

void DigiRingProgressBarWidget::SetTitleTextBold(bool bBold)
{
    m_TitleFont.setBold(bBold);
}

void DigiRingProgressBarWidget::SetTitleCustomFont(const QFont &font)
{
    m_TitleFont = font;
}

QFont DigiRingProgressBarWidget::GetTitleCustomFont()
{
    return m_TitleFont;
}

void DigiRingProgressBarWidget::SetTitleTextWeight(int weight)
{
    QString family = GetGlobalFamily(weight);
    m_TitleFont.setFamily(family);
}

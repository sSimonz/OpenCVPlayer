#include "playwidget.h"
#include <QPainter>
#include <QMimeData>
#include <QFileInfo>

PlayWidget::PlayWidget(QWidget *parent)
    : DigiBaseWidget{parent}
{
    setAcceptDrops(true);
}

void PlayWidget::updateImage(const QImage &image)
{
    m_pixmap = QPixmap::fromImage(image);

    double srcAspect = static_cast<double>(image.width()) / image.height();
    double targetAspect = static_cast<double>(width()) / height();
    int newWidth, newHeight;
    int xOffset = 0, yOffset = 0;
    if (srcAspect > targetAspect) 
    {
        // 原始图像比目标区域更宽，宽度填满，高度按比例缩放
        newWidth = width();
        newHeight = static_cast<int>(width() / srcAspect);
        yOffset = (height() - newHeight) / 2;
    } 
    else 
    {
        // 原始图像比目标区域更高，高度填满，宽度按比例缩放
        newHeight = height();
        newWidth = static_cast<int>(height() * srcAspect);
        xOffset = (height() - newWidth) / 2;
    }
    m_srcRect = QRect(xOffset, yOffset, newWidth, newHeight);
    update();
}

void PlayWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->accept();
    else
        event->ignore();
}

void PlayWidget::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        const QMimeData* data = event->mimeData();
        QList<QUrl> lists = data->urls();
        for(int i=0;i<lists.count();i++)
        {
            QString file = lists.at(i).path();
            QFileInfo info(file);
            QString str = info.suffix();
            if(str == "mp4" || str == "avi")
            {
                emit fileDropped(info.absoluteFilePath());
                event->accept();
                return;
            }
        }
    }
    event->ignore();
}


void PlayWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0));
    if(!m_pixmap.isNull())
    {
        qreal pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真
        m_pixmap = m_pixmap.scaled(QSize(width() * pixelRatio, height() *pixelRatio), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(m_srcRect, m_pixmap);
    }
}
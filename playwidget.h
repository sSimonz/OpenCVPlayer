#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include "BaseUI/digibasewidget.h"
#include <QDragEnterEvent>
#include <QDropEvent>

class PlayWidget : public DigiBaseWidget
{
    Q_OBJECT
public:
    explicit PlayWidget(QWidget *parent = nullptr);
    void updateImage(const QImage &image);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void fileDropped(const QString &filePath);

private:
    void adaptiveResize();

private:
    QPixmap m_pixmap;
    QRect m_srcRect;
};

#endif // PLAYWIDGET_H

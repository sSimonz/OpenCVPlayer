#ifndef FINDDIFFERMANAGER_H
#define FINDDIFFERMANAGER_H

#include <QObject>

class FindDifferManager : public QObject
{
    Q_OBJECT
public:
    explicit FindDifferManager(QObject *parent = nullptr);
    QList<QRect> findDifference(const QImage &image1, const QImage &image2);

signals:
};

#endif // FINDDIFFERMANAGER_H

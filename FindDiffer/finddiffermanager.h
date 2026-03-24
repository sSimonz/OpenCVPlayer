#ifndef FINDDIFFERMANAGER_H
#define FINDDIFFERMANAGER_H

#include <QObject>
#include <QDebug>
#include "opencvmanager.h"

class FindDifferManager : public QObject
{
    Q_OBJECT
public:
    explicit FindDifferManager(QObject *parent = nullptr);
    QList<QRect> findDifference(const QImage &image1, const QImage &image2);
    cv::Point2d computeTranslation(const cv::Mat &mat1, const cv::Mat &mat2);

signals:
};

#endif // FINDDIFFERMANAGER_H

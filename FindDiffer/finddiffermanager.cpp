#include "finddiffermanager.h"
#include "opencvmanager.h"

FindDifferManager::FindDifferManager(QObject *parent)
    : QObject{parent}
{

}

QList<QRect> FindDifferManager::findDifference(const QImage &image1, const QImage &image2)
{
    QList<QRect> listRect;

    // 将QImage转换为cv::Mat
    cv::Mat srcMat1 = OpenCVManager::image2cvMat(image1);
    cv::Mat srcMat2 = OpenCVManager::image2cvMat(image2);

    // 转灰度图，查看差异 (0-255)
    cv::Mat srcMatGray1;
    cv::Mat srcMatGray2;
    cv::cvtColor(srcMat1, srcMatGray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(srcMat2, srcMatGray2, cv::COLOR_BGR2GRAY);

    cv::Mat diffMatGray;
    cv::subtract(srcMatGray1, srcMatGray2, diffMatGray, cv::Mat(), CV_16SC1);
//    cv::imshow("3", diffMatGray);
    cv::Mat diffAbsMatGray = cv::abs(diffMatGray);
    diffAbsMatGray.convertTo(diffAbsMatGray, CV_8UC1, 1, 0);// 改变位深

    //阈值处理
    int thresh = 20; // 阈值
    cv::Mat threshMat;
    cv::threshold(diffAbsMatGray, threshMat, thresh, 255, cv::THRESH_BINARY);
//    cv::imshow("5", threshMat);
    cv::Mat mdianMat = threshMat.clone();
    cv::Mat closeMat = mdianMat.clone();

    // 寻找边界
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(closeMat, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    std::vector<std::vector<cv::Point>> contoursPoly(contours.size());
    for(int index = 0; index < contours.size(); index++)
    {
        cv::approxPolyDP(cv::Mat(contours[index]), contoursPoly[index], 5, true);// 多边形逼近
        cv::Rect rect = cv::boundingRect(cv::Mat(contoursPoly[index]));// 获取边界矩形
        listRect.append(QRect(rect.x, rect.y, rect.width, rect.height));
    }

    return listRect;
}

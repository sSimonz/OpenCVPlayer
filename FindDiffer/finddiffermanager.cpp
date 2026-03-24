#include "finddiffermanager.h"

FindDifferManager::FindDifferManager(QObject *parent)
    : QObject{parent}
{

}

QList<QRect> FindDifferManager::findDifference(const QImage &image1, const QImage &image2)
{
    QList<QRect> result;
    cv::Mat mat1 = OpenCVManager::image2cvMat(image1);
    cv::Mat mat2 = OpenCVManager::image2cvMat(image2);
    cv::Point2d shift = computeTranslation(mat1, mat2);
    
    // 构造 2*3 仿射变换矩阵
    // 1, 0, -shift.x
    // 0, 1, -shift.y
    cv::Mat transMat = (cv::Mat_<double>(2,3) << 1, 0, -shift.x, 0, 1, -shift.y);

    // 应用仿射变换 (输入图像，输出图像，变换矩阵，输出图像大小，插值方法，边界模式)
    cv::Mat alignMat;
    cv::warpAffine(mat2, alignMat, transMat, mat2.size(), cv::INTER_LINEAR);

    // 计算有效重叠区域
    int dx = cvRound(-shift.x); // mat2 实际移动的像素
    int dy = cvRound(-shift.y);
    int x1 = std::max(0, dx);
    int y1 = std::max(0, dy);
    int x2 = std::min(mat1.cols, mat1.cols + dx);
    int y2 = std::min(mat1.rows, mat1.rows + dy);
    if (x1 >= x2 || y1 >= y2) return result;

    cv::Rect overlapRect(x1, y1, x2 - x1, y2 - y1);
    cv::Mat mat1_roi = mat1(overlapRect);
    cv::Mat mat2_roi = alignMat(overlapRect);

    // 转灰度图，查看差异 (0-255)
    cv::Mat gray1, gray2;
    cv::cvtColor(mat1_roi, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(mat2_roi, gray2, cv::COLOR_BGR2GRAY);
    
    // 计算绝对差异
    cv::Mat diff;
    cv::absdiff(gray1, gray2, diff);
    
    // 二值化
    int threshold = 50;
    cv::Mat binary;
    cv::threshold(diff, binary, threshold, 255, cv::THRESH_BINARY);
    
    // 可选形态学操作：闭运算连接邻近区域
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
    cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);

    cv::imwrite("/Users/mac/Desktop/image1.png", mat1_roi);
    cv::imwrite("/Users/mac/Desktop/image2.png", mat2_roi);
    cv::imwrite("/Users/mac/Desktop/diff.png", diff);
    cv::imwrite("/Users/mac/Desktop/binary.png", binary);
    
    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    int minArea = 5;
    for (const auto &contour : contours) 
    {
        cv::Rect rect = cv::boundingRect(contour);
        if (rect.area() >= minArea)
        {
            // 将坐标转换回原图坐标系
            rect.x += overlapRect.x;
            rect.y += overlapRect.y;
            result.append(QRect(rect.x, rect.y, rect.width, rect.height));
        }
    }
    
    return result;
}

cv::Point2d FindDifferManager::computeTranslation(const cv::Mat &mat1, const cv::Mat &mat2)
{
    // 转为灰度图
    cv::Mat gray1, gray2;
    cv::cvtColor(mat1, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(mat2, gray2, cv::COLOR_BGR2GRAY);

    // 将图像转换为浮点型
    cv::Mat float1, float2;
    gray1.convertTo(float1, CV_32F);
    gray2.convertTo(float2, CV_32F);

    // 计算平移量（相位相关）
    cv::Point2d shift = cv::phaseCorrelate(float1, float2);
    qDebug() << "offset:" << shift.x << shift.y;
    return shift;
}

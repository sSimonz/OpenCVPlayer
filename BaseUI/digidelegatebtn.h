#ifndef DIGIDELEGATEBTN_H
#define DIGIDELEGATEBTN_H

/**
 * @brief 客户端公共库中的代理按钮 DigiDelegateBtn 封装类
 * 1.主要用于代理中4态按钮的绘制
 *
 * version 1.0
 * @date 2024-3-1
 *
 * History:
 * Revision
 */

#include <QObject>
#include <QPixmap>
#include <QSvgRenderer>
// #include "Base/ibaseui.h"

enum DIRECT_FLAG
{
    Up2Down = 0,
    Down2Up,
    Left2Right,
    Right2Left
};

enum STATE_FLAG
{
    Normal = 0,
    Hover,
    Pressed,
    Disabled,
    Checked
};

enum STRUCT_FLAG
{
    LeftIcon = 0,
    RightIcon,
    UnderIcon,
    Upicon,
};
class QPainter;
class QSvgRenderer;
class DigiDelegateBtn : public QObject/*, public IimageBase*/
{
    Q_OBJECT
public:
    explicit DigiDelegateBtn(QObject *parent = nullptr);

    /**
     * @brief 设置按钮正常，悬停，选中，禁止状态下的边框图像
     *
     * @param[in] path 来源图片的路径,svg图片用QSvgRenderer渲染，普通图片QPixmap渲染
     * @param[in] text 按钮显示文本，居中显示
     * @param[in] slice 图片切割的个数
     * @param[in] directFlag 图片切割方向
     * @note 来源图片由四个大小相同小图片拼接而成(图片拼接方向可由上到下，由下到上，由左到右，
     *       由右到左)，按顺序分别为正常，悬停，选中，禁用状态图片，图片为svg图
     */
    void LoadImagePath(QString path, int slice = 4, int directFlag = Left2Right);

    void Draw(QPainter *painter, const QRect &rect, int btnStatus);  //代理类的绘画函数中调用
	int getBtnStatus();
    void DrawColor(QPainter *painter, const QRect &rect, QColor color, qreal radius = 4);  //绘画颜色

private:
    void loadSvgImg(QString path, int DirectFlag);
    void loadNormalImg(QString path, int DirectFlag);

    void updateViewBox(int status);

private:
    int m_slice;
    int m_DirectFlag;
    int m_BtnStatus;

    qreal m_picWidth;
    qreal m_picHigh;

    QRectF m_ViewBox;
    QRectF m_curViewBox;

    QScopedPointer<QSvgRenderer> m_Renderer;
    QScopedPointer<QPixmap> m_BackImg;
};

#endif // DIGIDELEGATEBTN_H

#ifndef DIGISVGPUSHBTN_H
#define DIGISVGPUSHBTN_H
/**
 * @brief 客户端公共库中的按钮封装类
 * 1.按钮分正常，选中，悬停，禁用四种状态
 * 2.加载svg的图片通过改变QSvgRenderer 的viewbox 位置实现切分图片,如果图片小于按钮大小，图片会被拉伸会不会失真
 * 3.加载png,bmp,jpg的图片通过改变QPixmap 的viewbox 位置实现切分图片,如果图片小于按钮大小，图片会被拉伸会失真
 * @version 1.0
 * @date 2021-12-20
 *
 * Copyright (c) 2007-2021 Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
*/

#include "Base/iwidgetbase.h"
#include <QPushButton>
class QPaintEvent;
class QSvgRenderer;

class DigiSvgPushBtn : public QPushButton ,public IWidgetBase
{
    Q_OBJECT
public:
    explicit DigiSvgPushBtn(QWidget *Parent = nullptr);
    ~DigiSvgPushBtn();
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
    void LoadImagePath(QString path,QString text = "",int slice = 4,int directFlag = Left2Right);
    virtual void SetScale(qreal scale);
    void setCheckable(bool bCheck);
    void setChecked(bool status);
    void updatePressStatus();
    void SetCustomFont(const QFont &font);  //设置字体
    QFont GetCustomFont();                  //获取字体
    void initCheckTimer();  // 按钮状态无法刷新时调用

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent *Event);
    virtual void enterEvent(QEnterEvent *Event);
    virtual void leaveEvent(QEvent *Event);
    virtual void mousePressEvent(QMouseEvent *Event);
    virtual void mouseReleaseEvent(QMouseEvent *Event);
    virtual void changeEvent(QEvent *Event);
    virtual QSize sizeHint () const;
private:
    void loadSvgImg(QString path,int DirectFlag);
    void loadNormalImg(QString path,int DirectFlag);
    void updateViewBox();

private:
    QPixmap *m_BackImg;
    int  m_BtnStatus;
    qreal  m_picWidth;
    qreal  m_picHigh;
    int  m_DirectFlag;
    int  m_slice;
    QString m_text;
    QFont m_Font;
    QRectF m_ViewBox;
    QRectF m_curViewBox;
    QSvgRenderer* m_Renderer;
    bool m_bEnter;
    QTimer *m_checkTimer;// 刷新按钮状态(目前只有控制条按钮使用)
protected:
    qreal m_scale;
};

#endif // DIGISVGPUSHBTN_H

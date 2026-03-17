#ifndef DIGIMSGPOPUP_H
#define DIGIMSGPOPUP_H

/**
 * @brief 客户端公共库中的屏幕右下角提示弹窗 DigiMsgPopup 封装类
 * 1. 弹窗基础宽度固定值：320*240
 * 2. 有公有静态变量s_MsgPopup，可通过全局函数 Msg() 获取该变量。
 * 3. ★可通过setMsg()函数设置弹窗内容、并显示弹窗。
 * 4. ★可通过调用相关函数设置颜色、文本、图标的接口，设置弹窗样式，再调用show()显示弹窗。
 * 5. 文本最多显示40行。非MSGHELPER_TEXT文本(链接)只显示一行，有tooltip显示。
 * 6. 用继承Singleton<DigiMsgPopup>的方法写成单例类时，弹窗显示时关闭程序，执行析构函数会有异常，暂未找到解决方法。暂时先用静态变量的方法实现全局调用。
 *
 * version 1.0
 * @date 2022-6-30
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <QDialog>
#include "Base/iwidgetbase.h"
#include "Base/isingleton.h"

#define ANIMATION_DURATION 1 //消失动画持续时间

class QPropertyAnimation;

namespace Ui {
class DigiMsgPopup;
}

enum MSG_TYPE
{
    MSGHELPER_TEXT,
    MSGHELPER_PICTURE,
    MSGHELPER_VIDEO,
    MSGHELPER_FILE,
    MSGHELPER_FILEPICLIST,
    MSGHELPER_PICDOWNLOAD
};

class DigiMsgPopup : public QDialog, public IWidgetBase
{
    Q_OBJECT

public:
    DigiMsgPopup(QWidget *parent = nullptr);
    ~DigiMsgPopup();

    // 使用这个弹出消息框，默认弹出红色告警框
    void setMsg(const QString &text, bool bsuccess = false, int msgType = MSGHELPER_TEXT , bool bVideo = false, quint8 sec = 6);

private:
    DigiMsgPopup& operator<<(const QString& text);  // Msg() << QString("..."); 调用popup函数，使用默认参数

    /**
     * @brief Popup   弹窗弹出显示函数
     * @param text    内容文本
     * @param msgType 弹窗信息类型
     * @param bState  弹窗提示状态， true->成功提示，显示'√'图标、成功主题色;  false->失败提示，显示'!'图标、失败主题色;
     * @param sec     显示秒数，到时间后弹窗消失，值小于 ANIMATION_DURATION 时，弹窗不自动消失
     */
    void Popup(const QString& text, MSG_TYPE msgType = MSGHELPER_TEXT, bool bState = true, quint8 sec = 6);
    bool SetText(const QString &text, MSG_TYPE msgType = MSGHELPER_TEXT);  //设置提示文本
    void SetTextColor(const QColor &color);          //设置提示文本颜色
    void SetTitleText(const QString &text);          //设置标题栏文本
    void SetTitleColor(const QColor &bgColor);       //设置标题栏背景颜色
    void SetBGColor(const QColor &color);            //设置背景颜色
    void SetPicture(const QString &path);            //设置图片
    void SetMaxShowTime(quint8 sec);                 //设置最大显示秒数
    void SetCloseBtnVisible(bool visible);           //设置关闭按钮是否可见
    void SetOpenFileBtnVisible(bool visible);        //设置打开文件按钮是否可见
    void SetRoundedRadius(qreal radius);             //边框圆角半径
    void SetShadowVisibel(bool visible);             //边框阴影是否可见

public:
    QString GetText();   //获取提示文本内容
    virtual void SetScale(qreal scale);

public slots:
    void show();    //覆盖原函数，显示时初始化相关设置
    void close();   //覆盖原函数，关闭弹窗

protected:
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
    void SetCustomStyleSheet(qreal scale = 1.0);

private:
    qreal GetScale(QScreen *screen);
    void InitWidget();
    void CreateConnect();
    void SetCustomStyle(bool bSuccess);    //设置弹窗默认主题颜色 true:成功主题色  false:失败主题色

private slots:
    void OpenFolder(); //打开文件目录链接
    void OpenPlayWidget();// 打开播放器

public:
    static DigiMsgPopup* s_MsgPopup;

private:
    Ui::DigiMsgPopup *ui;

    qreal m_scale;           //缩放因子
    MSG_TYPE m_Type;         //消息类型
    quint8 m_CurSeconds;     //当前已显示的秒数
    quint8 m_MaxSeconds;     //最大显示秒数
    QTimer *m_Timer;         //定时器，定时关闭
    QString m_Text;          //文本内容
    QString m_TextColorHex;  //文本颜色
    QPropertyAnimation *m_PopupAnimation;     //弹出动画
    QPropertyAnimation *m_DisappearAnimation; //消失动画

    QMargins m_ShadowMargin;    //阴影宽度，m_ShadowLayout的边距
    qreal m_Radius;             //边框圆角半径,0则没有圆角
    QString m_BGColor;          //主窗口背景颜色，16进制字符串
    QString m_TitleBarBGColor;  //标题栏背景颜色，16进制字符串
    bool m_bShadowVisibel;      //边框阴影是否可见
};

DigiMsgPopup& Msg();

#endif // DIGIMSGPOPUP_H

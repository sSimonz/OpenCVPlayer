#ifndef DIGIBASEDIALOG_H
#define DIGIBASEDIALOG_H

/**
 * @brief Dialog对话框封装
 * 1. 对话框分为两个部分：对话框标题栏、对话框内容
 * 2. DigiBaseDialogTitleBar 对话框标题栏类，可设置标题栏左侧图标、标题文字、标题栏颜色等属性
 * 3. DigiBaseDialog 对话框主体，可设置标题栏的样式、对话框内容背景颜色、窗口圆角大小、边框阴影可见等属性
 * 4. moveEvent()检测屏幕DPI变化情况，可进行缩放
 * 5. 对话框主体窗体需要继承widget_dialogbase.h中的Widget_DialogBase类，方便传递参数和返回结果
 */


#include <QDialog>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"
#include "widget_dialogbase.h"

class DigiBaseWidget;
class QHBoxLayout;
class QVBoxLayout;
class DigiBaseLabel;
class DigiSvgPushBtn;
class DigiBaseDialogTitleBar;

#define SHADOW_WIDTH 9  //边框阴影布局宽度

class DigiBaseDialog : public QDialog, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT

public:
    explicit DigiBaseDialog(QWidget *MainWidget, const QString& title, QWidget *parent = nullptr);

    void SetWindowStaysOnTopHint(bool onTop); //窗口是否始终置顶
    void SetBGColor(const QColor &color);    //dialog窗体背景颜色
    void SetTitleBGColor(const QColor &color); //标题栏背景颜色
    void SetTitleIcon(const QString &path); //标题栏左侧图标
    void SetTitleText(const QString &text); //标题栏文本
    void SetSubTitleText(const QString &text); //副标题文本，默认不可见
    void SetTitleTextColor(const QColor &color); //标题栏文本颜色
    void SetTitleIconVisible(bool visible);//标题栏左侧图标显示
    void SetCloseBtnVisible(bool visible); //标题栏右侧按钮显示
    void SetNeedPreventCloseDialog(bool needToClose); //dialog能否关闭
    void SetRoundedRadius(qreal radius); //边框圆角半径
    void SetShadowVisibel(bool visible); //边框阴影是否可见
    void SetNullParent();           // 对话框父窗口必须置空的场景使用
    void setBorderSize(int border);// 边框宽度

    void SetScale(qreal scale) override;
    void SetHeight(qreal height) override;

    void setMainWidget(QWidget *widget);
    QWidget* GetMainWidget();
    QWidget* GetDialogTitle();

signals:
    void SendCloseReq();

public slots:
    void reject() override;
    void closeDialog();

protected:
    void moveEvent(QMoveEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void SetCustomStyleSheet(qreal scale = 1.0) override;
    qreal GetScale();


private:
    void InitWidget();
    void InitStyle();
    void CreateLayout();
    void CreateConnect();

private slots:
    void MoveWindow(QPoint point);
    void doneResponse(ResultType r,QObject* out);
    void changeSize(qreal width, qreal height);

public:
    QObject *m_Result;

private:
    DigiBaseDialogTitleBar *m_TitleBar; //标题栏
    QWidget *m_DialogWidget;      //主窗口
    QWidget *m_MainWidget;        //传入自定义内容的widget
    QVBoxLayout *m_ShadowLayout;  //用于隔开阴影和可见窗口的布局
    QVBoxLayout *m_MainLayout;    //主布局，包含标题栏和内容布局
    QVBoxLayout *m_ContentLayout; //内容布局，存放MainWidget
    QMargins m_ShadowMargin;      //阴影宽度，m_ShadowLayout的边距

    qreal m_scale;        //当前缩放值
    qreal m_Radius;       //边框圆角半径,0则没有圆角
    QString m_BGColor;    //主窗口背景颜色，16进制字符串
    QString m_TitleBarBGColor;//标题栏背景颜色，16进制字符串
    int m_Border;         // 边框宽度

    bool m_bNeedPreventCloseDialog; //是否阻止关闭对话框
    bool m_bShadowVisibel; //边框阴影是否可见
    int m_TitleHeight;     //标题栏高度
};
typedef DigiBaseDialog Dialog;

/****************************************
 *  以下为 DigiBaseDialogTitleBar 标题栏类
 ****************************************/

class DigiBaseDialogTitleBar : public QFrame
{
    Q_OBJECT

public:
    explicit DigiBaseDialogTitleBar(QWidget *parent = nullptr);

    void SetTitleText(const QString &title);     //设置标题文本
    void SetSubTitleText(const QString &title);  //设置副标题文本，默认不可见
    QString GetSubTitleText();  //获取副标题文本
    void SetIconPath(const QString &path);       //设置左侧图标
    void SetTitleTextColor(const QColor &color); //设置标题栏文本颜色
    void SetIconVisible(bool visible);           //设置左侧图标是否可见
    void SetCloseBtnVisible(bool visible);       //设置关闭按钮是否可见

    void SetScale(qreal scale);                  //缩放，在DigiBaseDialog的缩放函数中调用

signals:
    void closeRequest();
    void moveWindowRequest(QPoint point);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    void InitWidget();
    void InitStyle();
    void CreateLayout();
    void CreateConnect();

private:
    DigiBaseLabel *m_IconLab;       //标题栏左侧图标
    DigiBaseLabel *m_TitleLab;      //标题栏文字
    DigiBaseLabel *m_SubTitleLab;   //副标题文字
    DigiSvgPushBtn *m_CloseBtn;     //标题栏右侧关闭按钮
    QHBoxLayout* m_MainLayout;          //主布局
    QVBoxLayout* m_LabelLayout;         //标题文字Label中的布局
    DigiBaseWidget *m_LabelWidget;  //标题文字Label中的布局窗体

    bool m_bLeftButtonPress; //是否已按住左键
    QPoint m_ptPress;        //左键按下时的坐标
    QPoint m_ptMove;         //左键按住时，移动后的坐标
    int m_TitleHeight;       //标题栏高度
};

#endif // DIGIBASEDIALOG_H

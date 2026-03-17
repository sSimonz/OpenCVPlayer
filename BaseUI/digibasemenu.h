#ifndef DIGIBASEMENU_H
#define DIGIBASEMENU_H

#include <QMenu>
#include <QWidget>
#include <QWidgetAction>
#include "digibaseframe.h"

class QHBoxLayout;
class DigiBaseLabel;

/****************
 * DigiBaseMenu *
 ****************/
class DigiBaseMenu : public QMenu, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseMenu(QWidget *parent = nullptr);
    void SetScale(qreal scale) override;
    void SetWidth(qreal width) override;

    QAction *addAction(const QString &text);// 覆盖原函数
    void addAction(QAction *action);
    QAction *addSeparator(); //添加分隔线
    void clear();

    void setMarginsForAllAction(int left, int top, int right, int bottom);
    void setMarginsForAllAction(const QMargins &margins);
    void setSpacingForAllAction(int spacing);

protected:
    void actionEvent(QActionEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void SetCustomStyleSheet(qreal scale) override;
    void mousePressEvent(QMouseEvent *event) override;
#ifdef Q_OS_MAC
protected:
    void closeEvent(QCloseEvent *event) override;
#endif

private:
    bool m_bInit;
    DigiBaseLabel *m_topFrame;    //上半部分圆角
    DigiBaseLabel *m_bottomFrame; //下半部分圆角
    qreal m_scale;
};

/******************
 * DigiBaseAction *
*******************/
class ActionWidget;
class DigiBaseAction : public QWidgetAction, public IWidgetBase
{
    Q_OBJECT

private:
    explicit DigiBaseAction(QWidget *parent); //分隔线构造函数

public:
    explicit DigiBaseAction(QWidget *parent,
                                const QString &text,              // 文本
                                const QString &icon = QString()); // 图标

    static DigiBaseAction *CreateSeparatorAction(QWidget *parent);

    void SetScale(qreal scale);
    void SetWidth(qreal width);
    void SetHeight(qreal height);

    void setMenu(QMenu *menu);// 子菜单
    void setIcon(const QString &icon = QString());
    void setText(const QString &text);
    QString text();
    void setVisible(bool b);

    //调整Action控件边距、间距
    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void setSpacing(int spacing);

private:
    ActionWidget *m_pWidget;
    QString m_text;
};


/****************
 * ActionWidget *
****************/
class DigiBaseLabel;
class DigiIconPushBtn;
class ActionWidget : public DigiBaseFrame
{
    Q_OBJECT

public:
    ActionWidget(const QString &text, const QString &icon, bool bSeparator, QAction *action, QFrame *parent = nullptr);
    ~ActionWidget();

    void setSubMenu(QMenu *menu);
    void setIcon(const QString &icon);
    void setText(const QString &text);
    void SetCheckState(bool bChecked);
    void startTimer();
    void stopTimer();

    void setContentsMargins(const QMargins &margins);
    void setSpacing(int spacing);

signals:
    void toggled(bool);
    void triggered(bool checked = true);

protected:
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void SetCustomStyleSheet(qreal scale);

private:
    void InitNormalAction(const QString &text, const QString &icon);
    void InitSeparatorAction();

private slots:
    void OnHovered();
    void OnToggled(bool checked);

private:
    QString m_IconPath;  //保存构造函数或setIcon传入的图标路径
    QMenu *m_subMenu;
    QTimer *m_checkMouseTimer;
    QAction *m_action;
    DigiIconPushBtn *m_pBtnIcon;   //左侧图标
    DigiBaseLabel *m_pLabText;     //文本
    DigiIconPushBtn *m_pBtnArrow;  //右箭头
    QHBoxLayout *m_hLayout;
};

#endif // DIGIBASEMENU_H

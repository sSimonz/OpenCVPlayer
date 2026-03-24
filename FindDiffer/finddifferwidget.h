#ifndef FINDDIFFERWIDGET_H
#define FINDDIFFERWIDGET_H

#include "BaseUI/digibasewidget.h"


namespace Ui {
class FindDifferWidget;
}

class FindDifferManager;
class FindDifferWidget : public DigiBaseWidget
{
    Q_OBJECT

public:
    explicit FindDifferWidget(QWidget *parent = nullptr);
    ~FindDifferWidget();
    void SetScale(qreal scale) override;

private:
    void init();
    void createConnect();
    void SetCustomStyleSheet(qreal scale = 1.0) override;
    void saveConfig();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onLeftRectChanged(int value);
    void onRightRectChanged(int value);
    void onControlPosRsp();
    void onIdentifyRsp();
    void onClearAllRsp();
    void setWindowOnTop(bool onTop);
    void moveWindow(QPoint point);
    void closeWindow();

private:
    Ui::FindDifferWidget *ui;
    FindDifferManager *m_findDifferManager;

    QRect m_leftRect;
    QRect m_rightRect;
    QList<QRect> m_listRect;
    QPoint m_initPos;
    QString m_configPath;
    int m_borderWidth;
};

#endif // FINDDIFFERWIDGET_H

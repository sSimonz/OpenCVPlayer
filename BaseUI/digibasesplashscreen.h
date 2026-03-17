#ifndef DIGIBASESPLASHSCREEN_H
#define DIGIBASESPLASHSCREEN_H

/**
  * @brief DigiBaseSplashScreen 启动画面基础类
  *
  * version 1.0
  * @date 2024-2-22
  */
#include <QSplashScreen>
#include "Base/iwidgetbase.h"
#include "Base/istylesheetbase.h"
class DigiBaseSplashScreen : public QSplashScreen, public IWidgetBase, public IStyleSheetBase
{
    Q_OBJECT
public:
    explicit DigiBaseSplashScreen(const QPixmap &pixmap = QPixmap());
    ~DigiBaseSplashScreen();
    virtual void SetScale(qreal scale);
private:
    virtual void SetCustomStyleSheet(qreal scale);
signals:

public slots:
};

#endif // DIGIBASESPLASHSCREEN_H

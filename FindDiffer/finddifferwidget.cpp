#include "finddifferwidget.h"
#include "ui_finddifferwidget.h"
#include <QPainter>
#include <QScreen>
#include <QApplication>
#include <QDir>
#include <QJsonObject>
#include "finddiffermanager.h"

FindDifferWidget::FindDifferWidget(QWidget *parent)
    : DigiBaseWidget(parent)
    , ui(new Ui::FindDifferWidget)
{
    ui->setupUi(this);
    init();
    createConnect();
}

FindDifferWidget::~FindDifferWidget()
{
    saveConfig();
    delete ui;
}

void FindDifferWidget::SetScale(qreal scale)
{
    DigiBaseWidget::SetScale(scale);
    ui->m_titleBar->SetScale(scale);
}

void FindDifferWidget::SetCustomStyleSheet(qreal scale)
{
    DigiBaseWidget::SetCustomStyleSheet(scale);
    int m_Radius = 5;
    QString m_TitleBarBGColor = ColorManager::GetColorHex(ColorManager::CN_DIALOG_TITLE_BACKGROUND);
    QString qss;
    qss = QString("#titleBar{"
                       "border-top-left-radius: %1px;"
                       "border-top-right-radius: %1px;"
                       "background: %2;}")
                  .arg(m_Radius*scale)
                  .arg(m_TitleBarBGColor);
    ui->m_titleBar->setStyleSheet(qss);
}

void FindDifferWidget::init()
{
    ui->m_btnLeft->setAutoRepeat(true);
    ui->m_btnLeft->setAutoRepeatDelay(500);   // 按住 500ms 后开始触发
    ui->m_btnLeft->setAutoRepeatInterval(20); // 之后每 20ms 触发一次
    ui->m_btnRight->setAutoRepeat(true);
    ui->m_btnRight->setAutoRepeatDelay(500);
    ui->m_btnRight->setAutoRepeatInterval(20);
    ui->m_btnUp->setAutoRepeat(true);
    ui->m_btnUp->setAutoRepeatDelay(500);
    ui->m_btnUp->setAutoRepeatInterval(20);
    ui->m_btnDown->setAutoRepeat(true);
    ui->m_btnDown->setAutoRepeatDelay(500);
    ui->m_btnDown->setAutoRepeatInterval(20);
    ui->m_btnLeftUp->setAutoRepeat(true);
    ui->m_btnLeftUp->setAutoRepeatDelay(500);
    ui->m_btnLeftUp->setAutoRepeatInterval(20);
    ui->m_btnRightUp->setAutoRepeat(true);
    ui->m_btnRightUp->setAutoRepeatDelay(500);
    ui->m_btnRightUp->setAutoRepeatInterval(20);
    ui->m_btnLeftDown->setAutoRepeat(true);
    ui->m_btnLeftDown->setAutoRepeatDelay(500);
    ui->m_btnLeftDown->setAutoRepeatInterval(20);
    ui->m_btnRightDown->setAutoRepeat(true);
    ui->m_btnRightDown->setAutoRepeatDelay(500);
    ui->m_btnRightDown->setAutoRepeatInterval(20);
    ui->m_btnIdentify->setShortcut(QKeySequence("Ctrl+I"));
    ui->m_btnClear->setShortcut(QKeySequence("Ctrl+C"));
    ui->m_titleBar->SetTitleText(tr("Find Differ"));
    ui->m_titleBar->setObjectName("titleBar");

    m_initPos = QPoint(285, 188);
    m_borderWidth = 2;
    m_findDifferManager = new FindDifferManager(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOnTop(true);
    ui->m_frame->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->m_frame->SetBorderWidth(3);
    ui->m_frame->SetBorderColor(ColorManager::GetColor(ColorManager::CN_GROUPBOX_BORDER));
    ui->m_frameCtrlBar->SetBGColor(ColorManager::GetColor(ColorManager::CN_THEME_BACKGROUND));
    SetScale(1.0);

    // configuration
    QString appPath = QCoreApplication::applicationDirPath();
    #ifdef Q_OS_MAC
        QDir dir(appPath);
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
        appPath = dir.absolutePath();
    #endif
    m_configPath = QDir(appPath).filePath("config.json");
    QFile file(m_configPath);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray jsonData = file.readAll();
        file.close();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
        QJsonObject jsonObj = doc.object();
        QJsonObject imageObj = jsonObj["image1"].toObject();
        int x = imageObj["x"].toInt();
        ui->m_boxLeftX->setValue(x);
        int y = imageObj["y"].toInt();
        ui->m_boxLeftY->setValue(y);
        int w = imageObj["width"].toInt();
        ui->m_boxLeftW->setValue(w);
        int h = imageObj["height"].toInt();
        ui->m_boxLeftH->setValue(h);
        m_leftRect = QRect(x, y, w, h);
        imageObj = jsonObj["image2"].toObject();
        x = imageObj["x"].toInt();
        ui->m_boxRightX->setValue(x);
        y = imageObj["y"].toInt();
        ui->m_boxRightY->setValue(y);
        w = imageObj["width"].toInt();
        ui->m_boxRightW->setValue(w);
        h = imageObj["height"].toInt();
        m_rightRect = QRect(x, y, w, h);
        ui->m_boxRightH->setValue(h);
        QJsonObject posObj = jsonObj["position"].toObject();
        int posX = posObj["x"].toInt();
        int posY = posObj["y"].toInt();
        move(posX, posY);
        bool bOnTop = jsonObj["onTop"].toInt();
        ui->m_checkOnTop->setChecked(bOnTop);
        bool bAutoScan = jsonObj["autoScan"].toInt();
        ui->m_checkAutoScan->setChecked(bAutoScan);
    }
}

void FindDifferWidget::createConnect()
{
    connect(ui->m_boxLeftX, &QSpinBox::valueChanged, this, &FindDifferWidget::onLeftRectChanged);
    connect(ui->m_boxLeftY, &QSpinBox::valueChanged, this, &FindDifferWidget::onLeftRectChanged);
    connect(ui->m_boxLeftW, &QSpinBox::valueChanged, this, &FindDifferWidget::onLeftRectChanged);
    connect(ui->m_boxLeftH, &QSpinBox::valueChanged, this, &FindDifferWidget::onLeftRectChanged);
    connect(ui->m_boxRightX, &QSpinBox::valueChanged, this, &FindDifferWidget::onRightRectChanged);
    connect(ui->m_boxRightY, &QSpinBox::valueChanged, this, &FindDifferWidget::onRightRectChanged);
    connect(ui->m_boxRightW, &QSpinBox::valueChanged, this, &FindDifferWidget::onRightRectChanged);
    connect(ui->m_boxRightH, &QSpinBox::valueChanged, this, &FindDifferWidget::onRightRectChanged);
    connect(ui->m_btnIdentify, &QPushButton::clicked, this, &FindDifferWidget::onIdentifyRsp);
    connect(ui->m_btnClear, &QPushButton::clicked, this, &FindDifferWidget::onClearAllRsp);
    connect(ui->m_btnRefresh, &QPushButton::clicked, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnLeft, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnRight, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnUp, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnDown, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnLeftUp, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnRightUp, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnLeftDown, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_btnRightDown, &QPushButton::pressed, this, &FindDifferWidget::onControlPosRsp);
    connect(ui->m_checkOnTop, &QCheckBox::toggled, this, &FindDifferWidget::setWindowOnTop);
    connect(ui->m_titleBar, &DigiBaseDialogTitleBar::moveWindowRequest, this, &FindDifferWidget::moveWindow);
    connect(ui->m_titleBar, &DigiBaseDialogTitleBar::closeRequest, this, &FindDifferWidget::closeWindow);
}

void FindDifferWidget::saveConfig()
{
    QJsonObject jsonObj;
    QJsonObject imageObj;
    imageObj["x"] = ui->m_boxLeftX->value();
    imageObj["y"] = ui->m_boxLeftY->value();
    imageObj["width"] = ui->m_boxLeftW->value();
    imageObj["height"] = ui->m_boxLeftH->value();
    jsonObj["image1"] = imageObj;
    imageObj["x"] = ui->m_boxRightX->value();
    imageObj["y"] = ui->m_boxRightY->value();
    imageObj["width"] = ui->m_boxRightW->value();
    imageObj["height"] = ui->m_boxRightH->value();
    jsonObj["image2"] = imageObj;
    QJsonObject posObj;
    posObj["x"] = pos().x();
    posObj["y"] = pos().y();
    jsonObj["position"] = posObj;
    jsonObj["onTop"] = static_cast<int>(ui->m_checkOnTop->isChecked());
    jsonObj["autoScan"] = static_cast<int>(ui->m_checkAutoScan->isChecked());
    QJsonDocument doc(jsonObj);
    QFile file(m_configPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(doc.toJson());
    file.close();
}

void FindDifferWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // Rect
    painter.setPen(QPen(Qt::red, m_borderWidth));
    painter.drawRect(m_leftRect);
    painter.drawRect(m_rightRect);
    // List Rect
    for(int i = 0; i < m_listRect.size(); i++)
    {
        painter.setPen(QPen(Qt::blue, m_borderWidth));
        QRect rect = m_listRect.at(i);
        rect = QRect(rect.x()+m_rightRect.x(), rect.y()+m_rightRect.y(), rect.width(), rect.height());
        painter.drawRect(rect);
    }
    QWidget::paintEvent(event);
}

void FindDifferWidget::onLeftRectChanged(int value)
{
    int x = ui->m_boxLeftX->value();
    int y = ui->m_boxLeftY->value();
    int w = ui->m_boxLeftW->value();
    int h = ui->m_boxLeftH->value();
    m_leftRect = QRect(x, y, w, h);

    x = ui->m_boxRightX->value();
    y = ui->m_boxRightY->value();
    m_rightRect = QRect(x, y, w, h);
    update();
}

void FindDifferWidget::onRightRectChanged(int value)
{
    int x = ui->m_boxRightX->value();
    int y = ui->m_boxRightY->value();
    int w = ui->m_boxRightW->value();
    int h = ui->m_boxRightH->value();
    m_rightRect = QRect(x, y, w, h);

    x = ui->m_boxLeftX->value();
    y = ui->m_boxLeftY->value();
    m_leftRect = QRect(x, y, w, h);
    update();
}

void FindDifferWidget::onControlPosRsp()
{
    int x = pos().x();
    int y = pos().y();
    if(sender() == ui->m_btnLeft)
    {
        x -= 1;
    }
    else if(sender() == ui->m_btnRight)
    {
        x += 1;
    }
    else if(sender() == ui->m_btnUp)
    {
        y -= 1;
    }
    else if(sender() == ui->m_btnDown)
    {
        y += 1;
    }
    else if(sender() == ui->m_btnLeftUp)
    {
        x -= 1;
        y -= 1;
    }
    else if(sender() == ui->m_btnRightUp)
    {
        x += 1;
        y -= 1;
    }
    else if(sender() == ui->m_btnLeftDown)
    {
        x -= 1;
        y += 1;
    }
    else if(sender() == ui->m_btnRightDown)
    {
        x += 1;
        y += 1;
    }
    else if(sender() == ui->m_btnRefresh)
    {
        x = m_initPos.x();
        y = m_initPos.y();
    }
    move(x, y);
}

void FindDifferWidget::onIdentifyRsp()
{
    QPoint globalPos = ui->m_frame->mapToGlobal(QPoint(0, 0));
    QScreen * pScreen = QApplication::primaryScreen();
    QImage gameImage = pScreen->grabWindow(0,
                                           globalPos.x(),
                                           globalPos.y(),
                                           ui->m_frame->width(),
                                           ui->m_frame->height()).toImage();
    // 缩放至逻辑大小
    gameImage = gameImage.scaled(ui->m_frame->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 使用frame坐标系，减去标题栏高度
    int titleH = ui->m_titleBar->GetTitleHeight();
    QRect leftRect = QRect(m_leftRect.x()+m_borderWidth, m_leftRect.y()-titleH+m_borderWidth, m_leftRect.width()-2*m_borderWidth, m_leftRect.height()-2*m_borderWidth);
    QRect rightRect = QRect(m_rightRect.x()+m_borderWidth, m_rightRect.y()-titleH+m_borderWidth, m_rightRect.width()-2*m_borderWidth, m_rightRect.height()-2*m_borderWidth);
    
    QImage leftImage = gameImage.copy(leftRect);
    QImage rightImage = gameImage.copy(rightRect);
    if(!gameImage.save("/Users/mac/Desktop/game.png"))
    {
        qDebug() << "Failed to save game image";
    }

    QList<QRect> listRect = m_findDifferManager->findDifference(leftImage, rightImage);
    m_listRect = listRect;
    update();
    saveConfig();
}

void FindDifferWidget::onClearAllRsp()
{
    m_listRect.clear();
    update();
}

void FindDifferWidget::setWindowOnTop(bool onTop)
{
    if(onTop)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);

    show();
}

void FindDifferWidget::moveWindow(QPoint point)
{
    move(pos()+point);
}

void FindDifferWidget::closeWindow()
{
    close();
}

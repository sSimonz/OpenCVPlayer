#include "digimsgpopup.h"
#include "ui_digimsgpopup.h"

#include <QTimer>
#include <QScreen>
#include <QProcess>
#include <QPropertyAnimation>
#include <QFileInfo>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include "digibasedialog.h"

#define IDS_SCREENSHOT_CAPTURE_SUCCESS  "Screenshot captured successfully."
#define IDS_VIDEO_RECORD_SUCCESS  "Video recording successful."
#define IDS_FILE_EXPORT_SUCCESS  "File exported successfully."
#define IDS_PLAYBACK_IMAGE_DOWNLOAD  "Download successfully."
#define IDS_NEW_MESSAGES  "New messages"
#define IDS_NOTICE "Notice"
#define IDS_OPEN_FILE_LOCATION "Open the local file"

DigiMsgPopup* DigiMsgPopup::s_MsgPopup = nullptr;
DigiMsgPopup::DigiMsgPopup(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DigiMsgPopup),
    m_scale(0.1),
    m_Type(MSGHELPER_TEXT),
    m_CurSeconds(0),
    m_MaxSeconds(6),
    m_Timer(nullptr),
    m_TextColorHex(ColorManager::GetColorHex(ColorManager::CN_DIALOG_TITLT_TEXT)),
    m_PopupAnimation(nullptr),
    m_DisappearAnimation(nullptr),
    m_ShadowMargin(SHADOW_WIDTH,SHADOW_WIDTH,0,0),
    m_Radius(8),
    m_BGColor(ColorManager::GetColorHex(ColorManager::CN_DIALOG_BACKGROUND)),
    m_TitleBarBGColor(ColorManager::GetColorHex(ColorManager::CN_DIALOG_TITLE_BACKGROUND)),
    m_bShadowVisibel(true)
{
    ui->setupUi(this);

    m_width = 320;
    m_height = 240;

    m_Timer = new QTimer(this);
    m_Timer->setInterval(1000);

    InitWidget();
    CreateConnect();
}

DigiMsgPopup::~DigiMsgPopup()
{
    if (isVisible())
        close();

    delete ui;
}

void DigiMsgPopup::setMsg(const QString &text, bool bsuccess, int msgType, bool bVideo, quint8 sec)
{
    this->Popup(text, static_cast<MSG_TYPE>(msgType), bsuccess, sec);
}

DigiMsgPopup &DigiMsgPopup::operator <<(const QString &text)
{
    this->Popup(text);
    return *this;
}

void DigiMsgPopup::Popup(const QString &text, MSG_TYPE msgType, bool bState, quint8 sec)
{
    if (text.isEmpty())
        return;

    ui->m_LabImg->setVisible(false);
    if (!SetText(text,msgType))
        return;

    QString titleText;
    switch (msgType)
    {
        case MSGHELPER_PICTURE:
        case MSGHELPER_VIDEO:
        case MSGHELPER_FILE:
        case MSGHELPER_FILEPICLIST:
            titleText = IDS_NEW_MESSAGES;
            SetOpenFileBtnVisible(true);
            break;
        default: //case MSGHELPER_TEXT:
            titleText = IDS_NOTICE;
            SetOpenFileBtnVisible(false);
            break;
    }
    SetTitleText(titleText);
    SetMaxShowTime(sec);
    SetCustomStyle(bState);

    show();
}

bool DigiMsgPopup::SetText(const QString &text, MSG_TYPE msgType)
{
    ui->m_LabText->clear();
    m_Text = text;
    m_Type = msgType;

    switch (msgType){
    case MSGHELPER_PICTURE:
        ui->m_LabText->setText(IDS_SCREENSHOT_CAPTURE_SUCCESS);
        SetPicture(text);
        break;
    case MSGHELPER_VIDEO:
        ui->m_LabText->setText(IDS_VIDEO_RECORD_SUCCESS);
        SetPicture(text);
        break;
    case MSGHELPER_FILE:
    case MSGHELPER_FILEPICLIST:
        ui->m_LabText->setText(IDS_FILE_EXPORT_SUCCESS);
        break;
    case MSGHELPER_PICDOWNLOAD:
        ui->m_LabText->setText(IDS_PLAYBACK_IMAGE_DOWNLOAD);
        SetPicture(text);
        break;
    default: //case MSGHELPER_TEXT:
        ui->m_LabText->setText(text);
        break;
    }

    return true;
}

void DigiMsgPopup::SetTextColor(const QColor &color)
{
    m_TextColorHex = color.name(QColor::HexArgb);
    ui->m_LabText->SetTextColor(color);
    ui->m_LabText->UpdateTextColor();
}

void DigiMsgPopup::SetTitleText(const QString &text)
{
    ui->m_TitleBar->SetTitleText(text);
}

void DigiMsgPopup::SetTitleColor(const QColor &bgColor)
{
    m_TitleBarBGColor = bgColor.name(QColor::HexArgb);
}

void DigiMsgPopup::SetBGColor(const QColor &color)
{
    m_BGColor = color.name(QColor::HexArgb);
}

void DigiMsgPopup::SetPicture(const QString &path)
{
    ui->m_LabImg->setVisible(true);
    if(m_Type == MSGHELPER_VIDEO)
    {
        QFileInfo fileInfo(path);
        QString picPath = fileInfo.path() + "/" + fileInfo.completeBaseName() + ".png";
        // ui->m_LabImg->setLocalPicName(picPath);
    }
    else
    {
        // ui->m_LabImg->setLocalPicName(path);
    }
}

void DigiMsgPopup::SetMaxShowTime(quint8 sec)
{
    m_MaxSeconds = sec;
}

void DigiMsgPopup::SetCloseBtnVisible(bool visible)
{
    ui->m_TitleBar->SetCloseBtnVisible(visible);
}

void DigiMsgPopup::SetOpenFileBtnVisible(bool visible)
{
    ui->m_BtnOpenFile->setVisible(visible);
}

void DigiMsgPopup::SetRoundedRadius(qreal radius)
{
    m_Radius = radius;
}

void DigiMsgPopup::SetShadowVisibel(bool visible)
{
    m_bShadowVisibel = visible;
}

QString DigiMsgPopup::GetText()
{
    return m_Text;
}

void DigiMsgPopup::SetScale(qreal scale)
{
    //自身
    if (!qFuzzyIsNull(m_width))
        setFixedHeight(m_width * scale);
    if (!qFuzzyIsNull(m_height))
        setFixedHeight(m_height * scale);

    //label
    ui->m_LabImg->SetScale(scale);
    ui->m_LabText->SetScale(scale);
    ui->m_BtnOpenFile->SetScale(scale);

    //标题栏
    ui->m_TitleBar->SetScale(scale);

    //布局
    ui->m_DialogLayout->setSpacing(6*scale);

    SetCustomStyleSheet(scale);
}


void DigiMsgPopup::show()
{
    if (isVisible())
        close();

    if (m_MaxSeconds > ANIMATION_DURATION)
    {
        m_CurSeconds = 0;
        m_Timer->start();
    }

    setWindowOpacity(1);

    QDialog::show();
}

void DigiMsgPopup::close()
{
    if (m_Timer->isActive())
        m_Timer->stop();
    if (m_DisappearAnimation->state() != QAbstractAnimation::Stopped)
        m_DisappearAnimation->stop();

    QDialog::close();
}

void DigiMsgPopup::enterEvent(QEnterEvent *event)
{
    if (m_Timer->isActive())
        m_Timer->stop();
    if (m_DisappearAnimation->state() != QAbstractAnimation::Stopped)
        m_DisappearAnimation->stop();
    setWindowOpacity(1);

    activateWindow();

    QDialog::enterEvent(event);
}

void DigiMsgPopup::leaveEvent(QEvent *event)
{
    if (m_MaxSeconds > ANIMATION_DURATION)
    {
        m_CurSeconds = 0;
        m_Timer->start();
    }

    QDialog::leaveEvent(event);
}

void DigiMsgPopup::showEvent(QShowEvent *event)
{
    //找到鼠标所在的屏幕，弹窗显示在鼠标所在的屏幕
    QList<QScreen *>screens =  QGuiApplication::screens();
    QScreen *curScreen = nullptr;
    foreach (QScreen *screen, screens)
    {
        if (screen->availableGeometry().contains(QCursor::pos()))
        {
            curScreen = screen;
            break;
        }
    }

    //缩放检测
    qreal scale = GetScale(curScreen);
    if (!qFuzzyIsNull(scale) && (!qFuzzyCompare(scale,m_scale)))
    {
        m_scale = scale;
        this->SetScale(m_scale);
    }

//    adjustSize();

    //弹出位置
    QRect rect;
    if(!curScreen)
        rect = QRect(0, 0, 1920, 1080);
    else
        rect = curScreen->availableGeometry();
    m_PopupAnimation->setStartValue(QRect(rect.width() + rect.x() - width() - 1*m_scale,
                                          rect.height(),
                                          width(), height()));
    m_PopupAnimation->setEndValue(QRect(rect.width() + rect.x() - width() - 1*m_scale,
                                        rect.height() - height(),
                                        width(), height()));
    m_PopupAnimation->start();
    QDialog::showEvent(event);
}

void DigiMsgPopup::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);

    if (!m_bShadowVisibel)
        return;

    int shadow = SHADOW_WIDTH*m_scale;
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    // 圆角阴影边框;
    QColor color(150, 150, 150, 55);
    for (int i = 0; i < shadow; i++)
    {
        qreal alpha =  120*m_scale - qSqrt(i) * 50*m_scale;
        alpha = (alpha>0) ? alpha:0;
        color.setAlpha(alpha);
        painter.setPen(color);
        painter.drawRoundedRect(shadow - i,
                                shadow - i,
                                this->width()/* - (shadow - i) * 2*/,
                                this->height()/* - (shadow - i) * 2*/,
                                m_Radius*m_scale, m_Radius*m_scale);
    }
}

void DigiMsgPopup::SetCustomStyleSheet(qreal scale)
{
    QString qss;
    qss = QString("#dialog{background: %1}").arg(m_BGColor);
    ui->m_DialogWidget->setStyleSheet(qss);

    qss = QString("#titleBar{"
                     "border-top-left-radius: %1px;"
                     "border-top-right-radius: %1px;"
                     "background: %2}")
                 .arg(m_Radius*scale)
                 .arg(m_TitleBarBGColor);
    ui->m_TitleBar->setStyleSheet(qss);
}

qreal DigiMsgPopup::GetScale(QScreen *screen)
{
    if(screen != nullptr)
    {
        qreal physicalDpi  = screen->physicalDotsPerInch(); //显示器
        qreal logicalDpi = screen->logicalDotsPerInch();//系统设置
        qreal scale;

        if (qFuzzyCompare(logicalDpi, 96.0))
            scale = physicalDpi / 96.0;
        else
            scale = logicalDpi / 96.0;

        if (scale>0 && scale<=0.75)
            return 0.5;
        else if (scale>0.75 && scale<=1.25)
            return 1.0;
        else if (scale>1.25 && scale<=1.75)
            return 1.5;
        else if (scale>1.75 && scale<=2.25)
            return 2.0;
        else if (scale>2.25 && scale<=2.5)
            return 2.5;
        else
            return 3.0;
    }
    return 1.0;
}

void DigiMsgPopup::InitWidget()
{
    ui->m_LabText->SetWidth(280);
    ui->m_LabText->SetHeight(0);
    ui->m_LabText->setAlignment(Qt::AlignCenter);
    ui->m_LabText->setWordWrap(true);
    ui->m_LabText->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->m_LabText->SetTextWeight(QFont::Medium);
    ui->m_LabText->SetScale(1); //确保第一次显示时大小、位置正常
    ui->m_LabImg->SetWidth(174);
    ui->m_LabImg->SetHeight(100);
    ui->m_BtnOpenFile->setText(IDS_OPEN_FILE_LOCATION);
    ui->m_BtnOpenFile->SetWidth(120);
    ui->m_BtnOpenFile->SetHeight(32);
    QFont font = GetGlobalFont();
    font.setPixelSize(12);
    ui->m_BtnOpenFile->SetCustomFont(font);
    ui->m_DialogWidget->setObjectName("dialog");
    ui->m_TitleBar->setObjectName("titleBar");
    ui->m_ShadowLayout->setContentsMargins(m_ShadowMargin);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    ui->m_TitleBar->SetIconVisible(false);
    SetCustomStyle(true);

    //弹出动画
    m_PopupAnimation = new QPropertyAnimation(this,"geometry");
    m_PopupAnimation->setDuration(200);
    //消失动画
    m_DisappearAnimation = new QPropertyAnimation(this,"windowOpacity");
    m_DisappearAnimation->setDuration(ANIMATION_DURATION*1000);
    m_DisappearAnimation->setStartValue(1);
    m_DisappearAnimation->setEndValue(0);
}

void DigiMsgPopup::CreateConnect()
{
    connect(ui->m_BtnOpenFile, &QPushButton::clicked, this, &DigiMsgPopup::OpenFolder);
    connect(ui->m_TitleBar, &DigiBaseDialogTitleBar::closeRequest, this, &DigiMsgPopup::close);
    connect(m_Timer, &QTimer::timeout, this, [this]()
    {
        m_CurSeconds++;

        if (m_CurSeconds == m_MaxSeconds-ANIMATION_DURATION) //最后1秒，启动消失动画
        {
            m_DisappearAnimation->start();
        }
        else if (m_CurSeconds == m_MaxSeconds) //时间到，关闭弹窗
        {
            close();
        }
    });
}

void DigiMsgPopup::SetCustomStyle(bool bSuccess)
{
    SetTitleColor(ColorManager::GetColor(ColorManager::CN_DIALOG_TITLE_BACKGROUND));
}

void DigiMsgPopup::OpenFolder()
{
    QString url = m_Text;
#ifdef Q_OS_MAC
    QString strTemp;
    strTemp = url.replace("\\","/");
    strTemp.replace(" ", "\\ ");
    QStringList paramList;
    paramList<<"-R "<<strTemp;
    strTemp = "open -R " + strTemp;
    QByteArray ba = strTemp.toUtf8();
    char *c_str = ba.data();
    system(c_str);
#else
    QString strTemp = url;
    strTemp = strTemp.replace("/","\\");
    QStringList paramList;
    paramList<<"/select,"<<strTemp;
    QProcess::startDetached("explorer ",paramList);
#endif
}

void DigiMsgPopup::OpenPlayWidget()
{

}

DigiMsgPopup& Msg()
{
    if (DigiMsgPopup::s_MsgPopup == nullptr)
        DigiMsgPopup::s_MsgPopup = new DigiMsgPopup;

    return *DigiMsgPopup::s_MsgPopup;
//    return DigiMsgPopup::get_instance();
}

#ifndef DIGIVOLUMECONTROL_H
#define DIGIVOLUMECONTROL_H

/**
 * @brief 音量控件
 */
#include "BaseUI/digibasewidget.h"

namespace Ui {
class DigiVolumeControl;
}

class DigiVolumeControl : public DigiBaseWidget
{
    Q_OBJECT

public:
    explicit DigiVolumeControl(QWidget *parent = nullptr);
    ~DigiVolumeControl();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void setVolumeReq(int value);
    void btnVolumeChanged(bool b);

private slots:
    void onValueChanged(int value);
    void onBtnSoundRsp();

private:
    Ui::DigiVolumeControl *ui;

    bool m_bSound;
};

#endif // DIGIVOLUMECONTROL_H

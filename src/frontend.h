#pragma	once

#include <QWidget>
#include <QVector>
#include <QMainWindow>
#include <QApplication>
#include "pad.h"
#include "hotkey_config.h"
#include "samples_tab.h"
#include "buttons_tab.h"
#include "volume_tab.h"

class FrontEnd : public QMainWindow {
  public:
    FrontEnd (QVector<Pad*> pads, QWidget* parent = 0);
    void setPreset(QString preset);

  private:
    SamplesTab* samples;
    ButtonsTab* buttons;
    VolumeTab* volume;
    HotkeyConfig* hotkeys;
    QVector<Pad*> pads;

  private slots:
    void setLeftClusterConfig();
    void setRightClusterConfig();
    void setSplitConfig();
    void setNoConfig();

    void savePreset();
    void loadPreset();
};


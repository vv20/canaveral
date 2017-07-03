#pragma	once

#include <QVector>
#include <QWidget>
#include <QShortcut>
#include "pad.h"

class HotkeyConfig : public QObject {
  public:
    enum Preset {
      NO_PRESET,
      LEFT_CLUSTER_16,
      RIGHT_CLUSTER_16,
      SPLIT_16
    };

    HotkeyConfig (QVector<Pad*> pads, QWidget* parent = 0,
        HotkeyConfig::Preset preset = NO_PRESET);
    ~HotkeyConfig();

  private:
    QVector<QShortcut*> shortcuts;
    BackEnd* backend;

    void createShortcuts(QVector<QString> triggers, QWidget* parent);

  private slots:
    void silence();
};


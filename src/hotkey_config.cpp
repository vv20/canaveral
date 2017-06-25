#include <QKeySequence>
#include "hotkey_config.h"
#include "constants.h"

HotkeyConfig::HotkeyConfig (QVector<Pad*> pads, QWidget* parent,
    HotkeyConfig::Preset preset) : QObject() {
  switch (preset) {
    case NO_PRESET:
      return;

    case LEFT_CLUSTER_16:
      if (pads.size() != 16) {
        return;
      }
      createShortcuts((QVector<QString>){
          "1", "2", "3", "4",
          "q", "w", "e", "r",
          "a", "s", "d", "f",
          "z", "x", "c", "v"
          }, parent);
      for (int i = 0; i < pads.size(); i++) {
        connect(shortcuts.at(i), &QShortcut::activated,
            pads.at(i), &Pad::play);
      }
      break;

    case RIGHT_CLUSTER_16:
      if (pads.size() != 16) {
        return;
      }
      createShortcuts({
          "7", "8", "9", "0",
          "u", "i", "o", "p",
          "j", "k", "l", ";",
          "m", ",", ".", "/"
          }, parent);
      for (int i = 0; i < pads.size(); i++) {
        connect(shortcuts.at(i), &QShortcut::activated,
            pads.at(i), &Pad::play);
      }
      break;

    case SPLIT_16:
      if (pads.size() != 16) {
        return;
      }
      createShortcuts({
          "1", "2", "9", "0",
          "q", "w", "o", "p",
          "a", "s", "l", ";",
          "z", "x", ".", "/"
          }, parent);
      for (int i = 0; i < pads.size(); i++) {
        connect(shortcuts.at(i), &QShortcut::activated,
            pads.at(i), &Pad::play);
      }
      break;
  }
}

void HotkeyConfig::createShortcuts (QVector<QString> triggers, QWidget* parent) {
  for (QString trigger : triggers) {
    shortcuts.append(new QShortcut(QKeySequence(trigger), parent));
  }
}


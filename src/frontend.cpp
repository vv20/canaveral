#include <QSplitter>
#include <QVector>
#include <QMenu>
#include <QMenuBar>
#include "frontend.h"
#include "volume_tab.h"
#include "samples_tab.h"
#include "buttons_tab.h"
#include "hotkey_config.h"

FrontEnd::FrontEnd (QVector<Pad*> pads, QWidget* parent) : QMainWindow(parent) {
  // creating the hotkey configuration
  hotkeys = new HotkeyConfig(pads, this, HotkeyConfig::LEFT_CLUSTER_16);

  // create the menus
  QAction* addHotKey = new QAction("&Add Hotkey", this);
  QAction* removeHotKey = new QAction("&Remove Hotkey", this);
  QAction* changeHotKey = new QAction("&Change Hotkey", this);
  QAction* chooseHotKeyConfig = new QAction("&Select Configuration", this);
  QAction* saveCurrentConfig = new QAction("&Save Current Coniguration", this);
  QMenu* hotkey = menuBar()->addMenu("&Hotkeys");
  hotkey->addAction(addHotKey);
  hotkey->addAction(removeHotKey);
  hotkey->addAction(changeHotKey);
  hotkey->addSeparator();
  hotkey->addAction(chooseHotKeyConfig);
  hotkey->addAction(saveCurrentConfig);

  // initialising samples tab
  SamplesTab* samples = new SamplesTab(pads, this);
  
  // initialising buttons tab
  ButtonsTab* buttons = new ButtonsTab(pads, this);
  QSplitter* splitter1 = new QSplitter(Qt::Horizontal, this);
  splitter1->addWidget(samples);
  splitter1->addWidget(buttons);

  // initialising volume tab
  VolumeTab* volume = new VolumeTab(pads, this);

  // packing everything together
  QSplitter* splitter2 = new QSplitter(Qt::Vertical, this);
  splitter2->addWidget(splitter1);
  splitter2->addWidget(volume);

  QList<int> sizes({50, 100});
  splitter2->setSizes(sizes);

  setCentralWidget(splitter2);
}



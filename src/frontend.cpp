#include <QVector>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>
#include "frontend.h"
#include "volume_tab.h"
#include "samples_tab.h"
#include "buttons_tab.h"
#include "hotkey_config.h"

FrontEnd::FrontEnd (QVector<Pad*> padvector, QWidget* parent) 
  : QMainWindow(parent) {
  pads = padvector;

  // creating the hotkey configuration
  hotkeys = new HotkeyConfig(pads, this, HotkeyConfig::LEFT_CLUSTER_16);

  // create the menus
  QMenu* hotkey = menuBar()->addMenu("&Hotkeys");

  // actions of the hotkey menu
  QAction* leftCluster = new QAction("&Left Cluster", this); 
  QAction* rightCluster = new QAction("&Right Cluster", this); 
  QAction* split = new QAction("&Split", this);
  QAction* none = new QAction("&None", this); 

  // assemble the hotkey menu 
  hotkey->addAction(leftCluster);
  hotkey->addAction(rightCluster);
  hotkey->addAction(split);
  hotkey->addAction(none);

  // link the actions to the corresponding slots
  connect(leftCluster, &QAction::triggered, 
      this, &FrontEnd::setLeftClusterConfig);
  connect(rightCluster, &QAction::triggered, 
      this, &FrontEnd::setRightClusterConfig);
  connect(split, &QAction::triggered, 
      this, &FrontEnd::setSplitConfig);
  connect(none, &QAction::triggered, 
      this, &FrontEnd::setNoConfig);

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

void FrontEnd::setLeftClusterConfig() {
  delete hotkeys;
  hotkeys = new HotkeyConfig(pads, this, HotkeyConfig::LEFT_CLUSTER_16);
}

void FrontEnd::setRightClusterConfig() {
  delete hotkeys;
  hotkeys = new HotkeyConfig(pads, this, HotkeyConfig::RIGHT_CLUSTER_16);
}

void FrontEnd::setSplitConfig() {
  delete hotkeys;
  hotkeys = new HotkeyConfig(pads, this, HotkeyConfig::SPLIT_16);
}

void FrontEnd::setNoConfig() {
  delete hotkeys;
  hotkeys = new HotkeyConfig(pads, this, HotkeyConfig::NO_PRESET);
}

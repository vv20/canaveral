#include <QVector>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include "frontend.h"
#include "volume_tab.h"
#include "samples_tab.h"
#include "buttons_tab.h"
#include "hotkey_config.h"
#include "constants.h"
#include "preset_window.h"

FrontEnd::FrontEnd (QVector<Pad*> padvector, QWidget* parent) 
  : QMainWindow(parent) {
  pads = padvector;

  // creating the hotkey configuration
  hotkeys = new HotkeyConfig(pads, this, HotkeyConfig::LEFT_CLUSTER_16);

  // create the menus
  QMenu* hotkey = menuBar()->addMenu("&Hotkeys");
  QMenu* preset = menuBar()->addMenu("&Presets");

  // actions of the hotkey menu
  QAction* leftCluster = new QAction("&Left Cluster", this); 
  QAction* rightCluster = new QAction("&Right Cluster", this); 
  QAction* split = new QAction("&Split", this);
  QAction* none = new QAction("&None", this);

  // actions of the preset menu
  QAction* saveCur = new QAction("&Save Current");
  QAction* load = new QAction("&Load Preset");

  // assemble the hotkey menu 
  hotkey->addAction(leftCluster);
  hotkey->addAction(rightCluster);
  hotkey->addAction(split);
  hotkey->addAction(none);

  // assemble the preset menu
  preset->addAction(saveCur);
  preset->addAction(load);

  // link the actions to the corresponding slots
  connect(leftCluster, &QAction::triggered, 
      this, &FrontEnd::setLeftClusterConfig);
  connect(rightCluster, &QAction::triggered, 
      this, &FrontEnd::setRightClusterConfig);
  connect(split, &QAction::triggered, 
      this, &FrontEnd::setSplitConfig);
  connect(none, &QAction::triggered, 
      this, &FrontEnd::setNoConfig);

  connect(saveCur, &QAction::triggered,
      this, &FrontEnd::savePreset);
  connect(load, &QAction::triggered,
      this, &FrontEnd::loadPreset);

  // initialising samples tab
  samples = new SamplesTab(pads, this);
  
  // initialising buttons tab
  buttons = new ButtonsTab(pads, this);
  QSplitter* splitter1 = new QSplitter(Qt::Horizontal, this);
  splitter1->addWidget(samples);
  splitter1->addWidget(buttons);

  // initialising volume tab
  volume = new VolumeTab(pads, this);

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

void FrontEnd::savePreset () {
  QString presetName = NAME_OF_PRESET_DIR + 
    QInputDialog::getText(this, "Preset", "Name of Preset") + PRESET_EXT;

  QFile* presetFile = new QFile(presetName);
  if (presetFile->exists()) {
    QFile::remove(presetName);
  }
  if (!presetFile->open(QIODevice::ReadWrite)) {
    QTextStream out(stdout);
    out << "could not open file for saving the preset configuration" << endl;
    return;
  }
  QTextStream fileOut(presetFile);
  
  for (Pad* pad : pads) {
    fileOut << pad->getFilename() << endl;
  }

  presetFile->close();
  delete presetFile;
}

void FrontEnd::loadPreset () {
  PresetWindow* window = new PresetWindow(this);
  window->show();
}

void FrontEnd::setPreset (QString preset) {
  QString presetName = NAME_OF_PRESET_DIR + preset + PRESET_EXT;

  QFile* presetFile = new QFile(presetName);
  presetFile->open(QIODevice::ReadOnly);

  QVector<QString> filenames;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    filenames.append(QString(presetFile->readLine()).remove('\n'));
  }

  presetFile->close();
  delete presetFile;

  samples->loadPreset(filenames);
}


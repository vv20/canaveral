#include <QSplitter>
#include <QVector>
#include <QMenu>
#include <QMenuBar>
#include "frontend.h"
#include "volume_tab.h"
#include "samples_tab.h"
#include "buttons_tab.h"

FrontEnd::FrontEnd (QVector<Pad*> pads, QWidget* parent) : QMainWindow(parent) {
  // create the menus
  QAction* addHotKey = new QAction("&Add Hotkey", this);
  QAction* removeHotKey = new QAction("&Remove Hotkey", this);
  QAction* changeHotKey = new QAction("&Change Hotkey", this);
  QMenu* hotkey = menuBar()->addMenu("&Hotkeys");
  hotkey->addAction(addHotKey);
  hotkey->addAction(removeHotKey);
  hotkey->addAction(changeHotKey);

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



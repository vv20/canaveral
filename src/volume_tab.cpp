#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QVector>
#include "volume_tab.h"
#include "constants.h"
#include "pad.h"

VolumeTab::VolumeTab (QVector<Pad*> pads, QWidget* parent) : QWidget(parent) {
  QGridLayout* grid = new QGridLayout(this);

  QString s;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    QLabel* label = new QLabel(s.setNum(i+1), this);
    grid->addWidget(label, 0, i);

    pads.at(i)->createSlider(this);
    grid->addWidget(pads.at(i)->getSlider(), 1, i, 1, 3);
  }

  setLayout(grid);
}


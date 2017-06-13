#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include "volume_tab.h"
#include "constants.h"

VolumeTab::VolumeTab (QWidget* parent) : QWidget(parent) {
  QGridLayout* grid = new QGridLayout(this);

  QString s;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    QLabel* label = new QLabel(s.setNum(i+1), this);
    grid->addWidget(label, 0, i);

    QSlider* slider = new QSlider(Qt::Vertical, this);
    grid->addWidget(slider, 1, i, 1, 3);
  }

  setLayout(grid);
}


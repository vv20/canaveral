#include <QGridLayout>
#include <QPushButton>
#include <QVector>
#include "buttons_tab.h"
#include "constants.h"
#include "pad.h"

ButtonsTab::ButtonsTab (QVector<Pad*> pads, QWidget* parent) : QWidget(parent) {
  backend = pads.at(0)->getBackend();

  QGridLayout* grid = new QGridLayout(this);

  for (int i = 0; i < NUMBER_OF_BUTTON_COLUMNS; i++) {
    for (int j = 0; j < NUMBER_OF_BUTTON_ROWS; j++) {
      pads.at(i*NUMBER_OF_BUTTON_COLUMNS+j)->createButton(this);
      grid->addWidget(pads.at(i*NUMBER_OF_BUTTON_COLUMNS+j)->getButton(), i, j,
          Qt::AlignVCenter|Qt::AlignHCenter);
    }
  }

  // add a killswitch button
  QPushButton* killswitch = new QPushButton("Killswitch", this);
  connect(killswitch, &QPushButton::clicked, this, &ButtonsTab::silence);
  grid->addWidget(killswitch, NUMBER_OF_BUTTON_COLUMNS + 1,
      NUMBER_OF_BUTTON_ROWS / 2, Qt::AlignVCenter|Qt::AlignHCenter);

  setLayout(grid);
}

void ButtonsTab::silence() {
  backend->silence();
}

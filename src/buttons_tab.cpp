#include <QGridLayout>
#include <QPushButton>
#include "buttons_tab.h"
#include "constants.h"

ButtonsTab::ButtonsTab (QWidget* parent) : QWidget(parent) {
  QGridLayout* grid = new QGridLayout(this);

  QString s;
  for (int i = 0; i < NUMBER_OF_BUTTON_COLUMNS; i++) {
    for (int j = 0; j < NUMBER_OF_BUTTON_ROWS; j++) {
      QPushButton* button = 
        new QPushButton(s.setNum(i*NUMBER_OF_BUTTON_COLUMNS+j+1), this);
      grid->addWidget(button, i, j);
    }
  }

  setLayout(grid);
}


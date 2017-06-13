#include <QVBoxLayout>
#include "samples_tab.h"
#include "constants.h"

SamplesTab::SamplesTab (QWidget* parent) : QWidget(parent) {
  QVBoxLayout* vbox = new QVBoxLayout(this);
  QHBoxLayout* hbox = new QHBoxLayout();

  lw = new QListWidget(this);
 
  QString s; 
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    lw->addItem(s.setNum(i+1) + ": ---");
  }

  uploadBtn = new QPushButton("Load", this);
  ejectBtn = new QPushButton("Eject", this);

  hbox->addWidget(ejectBtn);
  hbox->addWidget(uploadBtn);

  vbox->addWidget(lw);
  vbox->addLayout(hbox);

  setLayout(vbox);
}


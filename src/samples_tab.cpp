#include <QVBoxLayout>
#include <QVector>
#include <QTextStream>
#include "samples_tab.h"
#include "constants.h"
#include "pad.h"

SamplesTab::SamplesTab (QVector<Pad*> pads, QWidget* parent) : QWidget(parent) {
  QTextStream out (stdout);
  QVBoxLayout* vbox = new QVBoxLayout(this);
  QHBoxLayout* hbox = new QHBoxLayout();

  lw = new QListWidget(this);
 
  QString s; 
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    out << "drawing sample " << QString::number(i) << endl;
    s.append(QString::number(i+1));
    s.append(": ");
    s.append(pads.at(i)->getSample());
    lw->addItem(s);
    s.clear();
  }

  uploadBtn = new QPushButton("Load", this);
  ejectBtn = new QPushButton("Eject", this);

  hbox->addWidget(ejectBtn);
  hbox->addWidget(uploadBtn);

  vbox->addWidget(lw);
  vbox->addLayout(hbox);

  setLayout(vbox);
}


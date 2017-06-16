#include <QVBoxLayout>
#include <QVector>
#include <QTextStream>
#include "samples_tab.h"
#include "constants.h"
#include "pad.h"
#include "select_window.h"

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

  connect(uploadBtn, &QPushButton::clicked, this, &SamplesTab::onLoad);
  connect(ejectBtn, &QPushButton::clicked, this, &SamplesTab::onEject);

  hbox->addWidget(ejectBtn);
  hbox->addWidget(uploadBtn);

  vbox->addWidget(lw);
  vbox->addLayout(hbox);

  setLayout(vbox);
}

void SamplesTab::onLoad () {
  QTextStream out (stdout);
  out << "loading a new sample" << endl;
  SelectWindow* window = new SelectWindow();
  window->show();
  out << "waiting for a file to be selected" << endl;
//  QString selectedSample = window.getSelectedSample();
//  out << "selected sample " << selectedSample << endl;
}

void SamplesTab::onEject () {

}


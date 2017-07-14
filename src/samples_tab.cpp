#include <QVBoxLayout>
#include <QVector>
#include <QTextStream>
#include "samples_tab.h"
#include "constants.h"
#include "pad.h"
#include "select_window.h"

SamplesTab::SamplesTab (QVector<Pad*> padVector, QWidget* parent) : QWidget(parent) {
  pads = padVector;

  QTextStream out (stdout);
  QVBoxLayout* vbox = new QVBoxLayout(this);
  QHBoxLayout* hbox = new QHBoxLayout();

  lw = new QListWidget(this);
  connect(lw, &QListWidget::itemDoubleClicked, this, &SamplesTab::onLoad);

  redrawList();

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

void SamplesTab::select(QString file) {
  QTextStream out (stdout);
  pads.at(lw->currentRow())->ejectSample();
  pads.at(lw->currentRow())->loadSample(file);
  redrawList();
}

void SamplesTab::loadPreset (QVector<QString> filenames) {
  for (int i = 0; i < filenames.size(); i++) {
    if (filenames.at(i) != "---") {
      pads.at(i)->ejectSample();
      pads.at(i)->loadSample(filenames.at(i));
    }
  }
  redrawList();
}

void SamplesTab::onLoad () {
  SelectWindow* window = new SelectWindow(this);
  window->show();
}

void SamplesTab::onEject () {
  int row = lw->currentRow();
  lw->currentItem()->setText(QString::number(row + 1) + ": ---");
  pads.at(lw->currentRow())->ejectSample();
}

void SamplesTab::redrawList () {
  lw->clear();
  QString s; 
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    s.append(QString::number(i+1));
    s.append(": ");
    s.append(pads.at(i)->getSample());
    lw->addItem(s);
    s.clear();
  }
}


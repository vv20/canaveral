#include <QVBoxLayout>
#include <QSplitter>
#include <QVector>
#include <QTextStream>
#include "frontend.h"
#include "volume_tab.h"
#include "samples_tab.h"
#include "buttons_tab.h"

FrontEnd::FrontEnd (QVector<Pad*> pads, QWidget* parent) : QWidget(parent) {
  QTextStream out(stdout);
  // the separator between the samples tab and the buttons tab
  QHBoxLayout* hbox = new QHBoxLayout(this);

  out << "initialising samples tab" << endl;
  SamplesTab* samples = new SamplesTab(pads, this);
  out << "initialising buttons tab" << endl;
  ButtonsTab* buttons = new ButtonsTab(pads, this);
  QSplitter* splitter1 = new QSplitter(Qt::Horizontal, this);
  splitter1->addWidget(samples);
  splitter1->addWidget(buttons);

  out << "initialising volume tab" << endl;
  VolumeTab* volume = new VolumeTab(pads, this);

  QSplitter* splitter2 = new QSplitter(Qt::Vertical, this);
  splitter2->addWidget(splitter1);
  splitter2->addWidget(volume);

  QList<int> sizes({50, 100});
  splitter2->setSizes(sizes);

  hbox->addWidget(splitter2);
}



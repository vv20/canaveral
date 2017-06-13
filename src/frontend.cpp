#include <QVBoxLayout>
#include <QSplitter>
#include "frontend.h"
#include "volume_tab.h"
#include "samples_tab.h"
#include "buttons_tab.h"

FrontEnd::FrontEnd (QWidget* parent) : QWidget(parent) {
  // the separator between the samples tab and the buttons tab
  QHBoxLayout* hbox = new QHBoxLayout(this);

  SamplesTab* samples = new SamplesTab(this);
  ButtonsTab* buttons = new ButtonsTab(this);
  QSplitter* splitter1 = new QSplitter(Qt::Horizontal, this);
  splitter1->addWidget(samples);
  splitter1->addWidget(buttons);

  VolumeTab* volume = new VolumeTab(this);

  QSplitter* splitter2 = new QSplitter(Qt::Vertical, this);
  splitter2->addWidget(splitter1);
  splitter2->addWidget(volume);

  QList<int> sizes({50, 100});
  splitter2->setSizes(sizes);

  hbox->addWidget(splitter2);
}



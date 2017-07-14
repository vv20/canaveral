#include <QVBoxLayout>
#include <QDir>
#include <QTextStream>
#include "preset_window.h"
#include "constants.h"

PresetWindow::PresetWindow (FrontEnd* fe, QWidget* parent) : QWidget (parent) {
  frontend = fe;

  // create the members
  QVBoxLayout* vbox = new QVBoxLayout(this);
  lw = new QListWidget(this);
  selectBtn = new QPushButton("Select", this);
  selectBtn->setEnabled(false);

  // populate the list
  QDir dir(NAME_OF_PRESET_DIR);
  dir.setSorting(QDir::Name);
  QFileInfoList fileList = dir.entryInfoList();
  QStringList split;
  for (QFileInfo file : fileList) {
    QTextStream out (stdout);
    split = file.fileName().split(".");
    out << split.last() << endl;
    if (file.fileName() == "." || file.fileName() == ".."
        || QString(".") + split.last() != PRESET_EXT) {
      continue;
    }
    lw->addItem(split.at(split.size() - 2));
  }

  // connect the signals to the handers
  connect(lw, &QListWidget::itemClicked, this, &PresetWindow::enableButton);
  connect(selectBtn, &QPushButton::clicked, this, &PresetWindow::select);

  // set layout
  vbox->addWidget(lw);
  vbox->addWidget(selectBtn);

  setLayout(vbox);
}

void PresetWindow::select () {
  frontend->setPreset(lw->currentItem()->text());
  this->hide();
}

void PresetWindow::enableButton () {
  selectBtn->setEnabled(true);
}


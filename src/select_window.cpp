#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QString>
#include <QDir>
#include <QGridLayout>
#include <QFileInfo>
#include <QRegularExpression>
#include "select_window.h"
#include "samples_tab.h"

SelectWindow::SelectWindow (SamplesTab* tab, QWidget* parent) : QWidget(parent) {
  parentTab = tab;

  // initialise the elements of the window
  backBtn = new QPushButton("<< Back", this);
  cancelBtn = new QPushButton("Cancel", this);
  selectBtn = new QPushButton("Select", this);
  curDir = new QLineEdit(this);
  dirList = new QListWidget(this);

  // connect the list click event to the toggle of select button
  connect(dirList, &QListWidget::currentTextChanged, 
      this, &SelectWindow::toggleSelect);

  // connect the double click event on the list widget to dir change
  connect(dirList, &QListWidget::itemDoubleClicked,
      this, &SelectWindow::onDoubleClick);

  // connect the buttons to the slots
  connect(backBtn, &QPushButton::clicked, this, &SelectWindow::onBack);
  connect(cancelBtn, &QPushButton::clicked, this, &SelectWindow::onCancel);
  connect(selectBtn, &QPushButton::clicked, this, &SelectWindow::onSelect);

  // display root
  curDir->setReadOnly(true);
  //displayDir("/");
  displayDir("/home/victor/Music/misc/");

  // lay out the grid
  QGridLayout* grid = new QGridLayout(this);
  grid->addWidget(backBtn, 0, 0);
  grid->addWidget(curDir, 0, 1, 1, 3);
  grid->addWidget(dirList, 1, 0, 4, 4);
  grid->addWidget(cancelBtn, 5, 0);
  grid->addWidget(selectBtn, 5, 4);
  setLayout(grid);
}

void SelectWindow::displayDir (QString dirName) {
  curDir->setText(dirName);
  dirList->clear();

  QDir dir(dirName);
  dir.setFilter(QDir::Files | QDir::AllDirs);
  dir.setSorting(QDir::Name);
  QFileInfoList fileList = dir.entryInfoList();

  QRegularExpression sampleRegEx("^.+[.][w][a][v]$");
  for (QFileInfo file : fileList) {
    if (file.isDir()) {
      dirList->addItem(file.fileName() + "/");
      continue;
    }
    if (sampleRegEx.match(file.fileName()).hasMatch()) {
      dirList->addItem(file.fileName());
    }
  }
}

void SelectWindow::onCancel () {
  this->close();
}

void SelectWindow::onSelect () {
  parentTab->select(curDir->text() + dirList->currentItem()->text());
  this->close(); 
}

void SelectWindow::onBack () {
  QString current = curDir->text();
  QVector<QStringRef> split = current.splitRef("/");
  QString newCur;

  newCur.append("/");
  for (int i = 0; i < split.size() - 2; i++) {
    if (split.at(i).size() > 0) {
      newCur.append(split.at(i));
      newCur.append("/");
    }
  }

  displayDir(newCur);
}

void SelectWindow::toggleSelect (QString curText) {  
  QRegularExpression sampleRegEx("^.+[.][w][a][v]$");
  if (sampleRegEx.match(curText).hasMatch()) {
    selectBtn->setDisabled(false);
  }
  else {
    selectBtn->setDisabled(true);
  }
}

void SelectWindow::onDoubleClick (QListWidgetItem* item) {
  QRegularExpression sampleRegEx("^.+[.][w][a][v]$");
  if (sampleRegEx.match(item->text()).hasMatch()) {
    onSelect();
  }
  else {
    displayDir(curDir->text() + item->text());
  }
}


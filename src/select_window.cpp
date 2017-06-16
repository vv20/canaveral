#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QString>
#include <QDir>
#include <QGridLayout>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTextStream>
#include "select_window.h"

SelectWindow::SelectWindow (QWidget* parent) : QWidget(parent) {
  QTextStream out (stdout);

  out << "initialise the elements of the window" << endl;
  backBtn = new QPushButton("<< Back", this);
  cancelBtn = new QPushButton("Cancel", this);
  selectBtn = new QPushButton("Select", this);
  curDir = new QLineEdit(this);
  dirList = new QListWidget(this);

  out << "connect the list click event to the toggle of select button" << endl;
  connect(dirList, &QListWidget::currentTextChanged, 
      this, &SelectWindow::toggleSelect);

  out << "connect the buttons to the slots" << endl;
  connect(backBtn, &QPushButton::clicked, this, &SelectWindow::onBack);
  connect(cancelBtn, &QPushButton::clicked, this, &SelectWindow::onCancel);
  connect(selectBtn, &QPushButton::clicked, this, &SelectWindow::onSelect);

  out << "display root" << endl;
  curDir->setReadOnly(true);
  displayDir("/");

  out << "lay out the grid" << endl;
  QGridLayout* grid = new QGridLayout(this);
  grid->addWidget(backBtn, 0, 0);
  grid->addWidget(curDir, 0, 1, 1, 3);
  grid->addWidget(dirList, 1, 0, 4, 4);
  grid->addWidget(cancelBtn, 5, 0);
  grid->addWidget(selectBtn, 5, 4);
  setLayout(grid);

  out << "prepare the result and mutex" << endl;
  selectedFile = "";
  selectedFileMutex = new QMutex();
  selectedFileMutex->lock();
  out << "yo" << endl;
}

void SelectWindow::displayDir (QString dirName) {
  QTextStream out(stdout);

  curDir->insert(dirName);

  out << "open the directory" << endl;
  QDir dir(dirName);
  dir.setFilter(QDir::Files | QDir::AllDirs);
  dir.setSorting(QDir::Name);
  QFileInfoList fileList = dir.entryInfoList();

  out << "add the directories and wav files to the list" << endl;
  QRegularExpression sampleRegEx("^.+$[.][w][a][v]");
  for (QFileInfo file : fileList ) {
    if (file.isDir()) {
      dirList->addItem(file.fileName());
      continue;
    }
    if (sampleRegEx.match(file.fileName()).hasMatch()) {
      dirList->addItem(file.fileName());
    }
  }
}

void SelectWindow::onCancel () {
  selectedFile = "";
  selectedFileMutex->unlock();
  this->close();
}

void SelectWindow::onSelect () {
  QString selected = dirList->currentItem()->text();
  selectedFile = curDir->text() + selected;
  selectedFileMutex->unlock();
  this->close(); 
}

void SelectWindow::onBack () {
  QString current = curDir->text();
  QVector<QStringRef> split = current.splitRef("/");
  QString newCur;

  for (QStringRef part : split) {
    newCur.append(part);
    newCur.append("/");
  }

  displayDir(newCur);
}

void SelectWindow::toggleSelect () {

}

QString SelectWindow::getSelectedSample () {
  selectedFileMutex->lock();
  delete selectedFileMutex;
  return selectedFile;
}


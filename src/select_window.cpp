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
#include <QThread>
#include "select_window.h"
#include "samples_tab.h"

SelectWindow::SelectWindow (SamplesTab* tab, QWidget* parent) : QWidget(parent) {
  parentTab = tab;

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

  for (QStringRef part : split) {
    newCur.append(part);
    newCur.append("/");
  }

  displayDir(newCur);
}

void SelectWindow::toggleSelect () {

}


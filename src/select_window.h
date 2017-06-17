#pragma	once

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QString>
#include "samples_tab.h"

class SelectWindow : public QWidget {
  Q_OBJECT

  public:
    SelectWindow (SamplesTab* tab, QWidget* parent = 0);
  
  private slots:
    void onCancel();
    void onSelect();
    void onBack();
    void toggleSelect(QString curText);

  private:
    void displayDir(QString dirName);

    QPushButton* backBtn;
    QPushButton* cancelBtn;
    QPushButton* selectBtn;
    QLineEdit* curDir;
    QListWidget* dirList;

    SamplesTab* parentTab;
};

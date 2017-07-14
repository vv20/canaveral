#pragma	once

#include <QString>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>
#include "frontend.h"

class PresetWindow : public QWidget {
  public:
    PresetWindow (FrontEnd* fe, QWidget* parent = 0);
    QString getPresetName();

  private:
    QListWidget* lw;
    QPushButton* selectBtn;
    FrontEnd* frontend;
    
  private slots:
    void select();
    void enableButton();
};


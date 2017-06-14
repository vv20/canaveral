#pragma	once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVector>
#include "pad.h"

class SamplesTab : public QWidget {
  Q_OBJECT

  public:
    SamplesTab (QVector<Pad*> pads, QWidget* parent = 0);

  private:
    QListWidget* lw;
    QPushButton* uploadBtn;
    QPushButton* ejectBtn;

};


#pragma	once

#include <QWidget>
#include <QVector>
#include "pad.h"
#include "backend.h"

class ButtonsTab : public QWidget {
  public:
    ButtonsTab (QVector<Pad*> pads, QWidget* parent = 0);

  private:
    BackEnd* backend;

  private slots:
    void silence();
};


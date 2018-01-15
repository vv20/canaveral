#pragma	once

#include <QWidget>
#include <QVector>
#include "pad.h"

class ButtonsTab : public QWidget {
  public:
    ButtonsTab (QVector<Pad*> pads, QWidget* parent = 0);

  private slots:
    void silence();
};


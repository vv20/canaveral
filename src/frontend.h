#pragma	once

#include <QWidget>
#include <QVector>
#include "pad.h"

class FrontEnd : public QWidget {
  public:
    FrontEnd (QVector<Pad*> pads, QWidget* parent = 0);
};


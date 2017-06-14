#pragma	once

#include <QWidget>
#include <QVector>
#include "pad.h"

class VolumeTab : public QWidget {
  public:
    VolumeTab (QVector<Pad*> pads, QWidget* parent = 0);
};


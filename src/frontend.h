#pragma	once

#include <QWidget>
#include <QVector>
#include <QMainWindow>
#include <QApplication>
#include "pad.h"

class FrontEnd : public QMainWindow {
  public:
    FrontEnd (QVector<Pad*> pads, QWidget* parent = 0);
};


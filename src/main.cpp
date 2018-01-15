#include <QApplication>
#include <QVector>
#include "frontend.h"
#include "constants.h"
#include "pad.h"
#include "kernel.h"

int main (int argc, char** argv) {
  QApplication app(argc, argv);
  init_kernel();

  QVector<Pad*> pads;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pads.append(new Pad(i));
  }

  FrontEnd fe(pads);

  fe.setWindowTitle(APP_WINDOW_TITLE);
  fe.resize(APP_WIDTH, APP_HEIGHT);
  fe.move(APP_X_POS, APP_Y_POS);
  fe.show();

  return app.exec();
}

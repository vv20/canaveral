#include <QApplication>
#include <QVector>
#include "frontend.h"
#include "backend.h"
#include "constants.h"
#include "pad.h"

int main (int argc, char** argv) {
  QApplication app(argc, argv);
  BackEnd* be = new BackEnd();

  QVector<Pad*> pads;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pads.append(new Pad(i, be));
  }

  FrontEnd fe(pads);

  fe.setWindowTitle(APP_WINDOW_TITLE);
  fe.resize(APP_WIDTH, APP_HEIGHT);
  fe.move(APP_X_POS, APP_Y_POS);
  fe.show();

  return app.exec();
}

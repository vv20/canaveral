#include <QApplication>
#include <QVector>
#include <QTextStream>
#include "frontend.h"
#include "backend.h"
#include "constants.h"
#include "pad.h"

int main (int argc, char** argv) {
  QTextStream out (stdout);
  QApplication app(argc, argv);
  out << "initialising back end" << endl;
  BackEnd* be = new BackEnd();

  QVector<Pad*> pads;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    out << "initialising pad " << QString::number(i) << endl;
    pads.append(new Pad(i, be));
  }

  out << "initialising front end" << endl;
  FrontEnd fe(pads);

  out << "creating gui" << endl;
  fe.setWindowTitle(APP_WINDOW_TITLE);
  fe.resize(APP_WIDTH, APP_HEIGHT);
  fe.move(APP_X_POS, APP_Y_POS);
  fe.show();

  return app.exec();
}

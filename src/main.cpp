#include <QApplication>
#include "frontend.h"
#include "constants.h"

int main (int argc, char** argv) {
  QApplication app(argc, argv);
  FrontEnd fe;

  fe.setWindowTitle(APP_WINDOW_TITLE);
  fe.resize(APP_WIDTH, APP_HEIGHT);
  fe.move(APP_X_POS, APP_Y_POS);
  fe.show();

  return app.exec();
}

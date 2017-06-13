#pragma	once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class SamplesTab : public QWidget {
  Q_OBJECT

  public:
    SamplesTab (QWidget* parent = 0);

  private:
    QListWidget* lw;
    QPushButton* uploadBtn;
    QPushButton* ejectBtn;

};


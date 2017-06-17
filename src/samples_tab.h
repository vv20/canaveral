#pragma	once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVector>
#include "pad.h"

class SamplesTab : public QWidget {
  Q_OBJECT

  public:
    SamplesTab (QVector<Pad*> pads, QWidget* parent = 0);
    void select(QString file);

  private slots:
    void onLoad();
    void onEject();

  private:
    QListWidget* lw;
    QPushButton* uploadBtn;
    QPushButton* ejectBtn;

};


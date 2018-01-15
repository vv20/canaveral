#pragma	once

#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include <QByteArray>
#include "sample.h"

class Pad : public QObject {
  Q_OBJECT

  public:
    /* ====================  LIFECYCLE     ======================================= */
    Pad (int pos);

    /* ====================  ACCESSORS     ======================================= */
    QPushButton* getButton();
    QSlider* getSlider();
    QString getSample();
    QString getFilename();
    /* ====================  MUTATORS      ======================================= */
    void createButton(QWidget* parent);
    void createSlider(QWidget* parent);

  public slots:
    void setVolume(int volume);
    void loadSample(QString sample);
    void ejectSample();
    void play();
  
  private:
    int position;
    QPushButton* button;
    QSlider* slider;
    Sample* sample;
};


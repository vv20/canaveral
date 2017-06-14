#pragma	once

#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include "backend.h"

class Pad : public QObject {
  Q_OBJECT

  public:
    /* ====================  LIFECYCLE     ======================================= */
    Pad (int pos, BackEnd* backend);

    /* ====================  ACCESSORS     ======================================= */
    QPushButton* getButton();
    QSlider* getSlider();
    QString* getSample();
    /* ====================  MUTATORS      ======================================= */
    void setSample(QString* sample);
    void createButton(QWidget* parent);
    void createSlider(QWidget* parent);

  public slots:
    void setVolume(int volume);
    void loadSample(QString* sample);
    void ejectSample();
    void play();
    void playWithSustain();
    void playWithRepeat();
  
  private:
    int position;
    float volumeIndex;
    QPushButton* button;
    QSlider* slider;
    QString* sample;
    BackEnd* backend;
};


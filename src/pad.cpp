#include <QPushButton>
#include <QSlider>
#include <QTextStream>
#include "pad.h"
#include "constants.h"

Pad::Pad (int pos, BackEnd* back) {
  position = pos;
  backend = back;
  sample = new Sample(NULL);
}

QPushButton* Pad::getButton () {
  return button;
}

QSlider* Pad::getSlider () {
  return slider;
}

QString Pad::getSample () {
  return sample->getSamplename();
}

void Pad::createButton (QWidget* parent) {
  button = new QPushButton(QString::number(position+1), parent);
  connect(button, &QPushButton::clicked, this, &Pad::play);
}

void Pad::createSlider (QWidget* parent) {
  slider = new QSlider(Qt::Vertical, parent);
  slider->setTickInterval(VOLUME_TICKS);
  connect(slider, &QSlider::valueChanged, this, &Pad::setVolume);
}

void Pad::setVolume (int volume) {
  sample->setVolume((float) volume / (100 / VOLUME_TICKS));
}

void Pad::loadSample (QString filename) {
  QTextStream out(stdout);
  delete sample; 
  sample = new Sample(filename);
}

void Pad::ejectSample () {
  delete sample;
  sample = new Sample(NULL);
}

void Pad::play () {
  backend->playSample(sample);
}


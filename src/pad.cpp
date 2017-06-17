#include <QPushButton>
#include <QSlider>
#include <QTextStream>
#include "pad.h"
#include "constants.h"

Pad::Pad (int pos, BackEnd* back) {
  position = pos;
  backend = back;
  sample = "---";
}

QPushButton* Pad::getButton () {
  return button;
}

QSlider* Pad::getSlider () {
  return slider;
}

QString Pad::getSample () {
  return sample;
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
  // if the middle is 1, i.e. original volume, then to get the index
  // by which the soundwave should be multiplied, we divide current
  // volume by original volume i.e. max volume / 2
  volumeIndex = (float) (volume * 2 / VOLUME_TICKS);
}

void Pad::loadSample (QString sam) { 
  sample = sam;
}

void Pad::ejectSample () {
  sample = "---";
}

void Pad::play () {
  QTextStream out(stdout);
  out << "playing sample " << sample << endl;
}

void Pad::playWithSustain () {
  QTextStream out(stdout);
  out << "playing sample " << sample << " with sustain" << endl;
}

void Pad::playWithRepeat () {
  QTextStream out(stdout);
  out << "playing sample " << sample << " with repeat" << endl;
}


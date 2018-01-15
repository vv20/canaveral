#include <QPushButton>
#include <QSlider>
#include <QTextStream>
#include "pad.h"
#include "constants.h"
#include "kernel.h"

Pad::Pad (int pos) {
  position = pos;
}

QPushButton* Pad::getButton () {
  return button;
}

QSlider* Pad::getSlider () {
  return slider;
}

QString Pad::getSample () {
    return get_sample_name(this->position);
}

QString Pad::getFilename () {
    return get_sample_file_name(this->position);
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
    set_volume(this->position, volume);
}

void Pad::loadSample (QString filename) {
    load_sample(this->position, filename);
}

void Pad::ejectSample () {
    eject_sample(this->position);
}

void Pad::play () {
  if (this->getFilename() != "---") {
      play_sample(this->position);
  }
}


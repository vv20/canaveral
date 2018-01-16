#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <math.h>
#include <sndfile.hh>
#include "sample.h"

typedef union {
  char array[4];
  float ieee;
} float_converter;

Sample::Sample(QString file) {
  if (file == "") {
    filename = "---";
    samplename = "---";
    return;
  }

  this->filename = file;
  this->volumeIndex = 0;

  QVector<QStringRef> split = file.splitRef("/");
  samplename = split.last().toString();

  SndfileHandle handle = SndfileHandle(file.toStdString());

  this->noOfChannels = handle.channels();
  this->sampleRate = handle.samplerate();
  this->numberOfFrames = handle.frames();
  this->data = (float*) malloc(sizeof(float) * this->numberOfFrames);

  handle.read(this->data, numberOfFrames);
}

QString Sample::getFilename () {
  return filename;
}

QString Sample::getSamplename () {
  return samplename;
}

long Sample::getNumberOfFrames() {
  return numberOfFrames;
}

long Sample::getSampleRate() {
  return sampleRate;
}

float Sample::getVolumeIndex() {
  return volumeIndex;
}

void Sample::setVolume (float volume) {
  volumeIndex = volume;
}

SampleInstance::SampleInstance(Sample* genericSample) {
	sample = genericSample;
    this->position = 0;
}

bool SampleInstance::getFrame (float* frame, long length, long rate) {
  long noFrames = sample->getNumberOfFrames();
  long sampleRate = sample->getSampleRate();
  float volumeIndex = sample->getVolumeIndex();
  int limit = std::min(length, noFrames - position);
  for (int i = 0; i < limit; i++) {
    frame[i] += sample->data[position + i * sampleRate / rate] * volumeIndex;
  }
  position += length * sampleRate / rate;
  return position <= noFrames;
}

Sample* SampleInstance::getSample () {
  return sample;
}


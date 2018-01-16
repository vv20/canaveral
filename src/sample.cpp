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
  this->leftData = (float*) malloc(sizeof(float) * this->numberOfFrames);
  this->rightData = (float*) malloc(sizeof(float) * this->numberOfFrames);

  file.read()
}

Sample::Sample (QString file) {
  if (file == "") {
    filename = "---";
    samplename = "---";
    return;
  }
  
  QByteArray wavedata;
  QTextStream out(stdout);
  filename = file;
  volumeIndex = 0;

  QVector<QStringRef> split = file.splitRef("/");
  samplename = split.last().toString();

  ChunkStream* stream;
  if ((stream = parseFile(file)) == NULL) {
    out << "could not parse wav file" << endl;
    filename = "---";
    samplename = "---";
    return;
  }

  Chunk* chunk = stream->next();
  if (chunk->type != RIFF_CHUNK) {
    out << "wav file corrupted, could not read RIFF tag" << endl;
    filename = "---";
    samplename = "---";
    clearStream(stream);
    return;
  }
  delete chunk;

  chunk = stream->next();
  if (chunk->type != WAVE_CHUNK) {
    out << "wav file corrupted, could not read WAVE tag" <<endl;
    filename = "---";
    samplename = "---";
    clearStream(stream);
    return;
  }
  delete chunk;

  while ((chunk = stream->next()) != NULL) {
    switch (chunk->type) {
      case FMT_CHUNK:
        noOfChannels = dynamic_cast<FmtChunk*>(chunk)->nChannels;
        sampleRate = dynamic_cast<FmtChunk*>(chunk)->samplingRate;
        bitsPerSample = dynamic_cast<FmtChunk*>(chunk)->bitsPerSample;
        format = dynamic_cast<FmtChunk*>(chunk)->format;
        break;

      case DATA_CHUNK:
        wavedata = dynamic_cast<DataChunk*>(chunk)->data;
        break;

      case OTHER_CHUNK:
        out << "unknown chunk " << dynamic_cast<OtherChunk*>(chunk)->tag << endl;
        break;
    }

    delete chunk;
  }
  out << "no of channels: " << QString::number(noOfChannels) << endl;
  out << "sample rate: " << QString::number(sampleRate) << endl;
  out << "bits per sample: " << QString::number(bitsPerSample) << endl;
  out << "format: " << QString::number(format) << endl;

  int bytesPerSample = bitsPerSample / 8;
  numberOfFrames = wavedata.size() / noOfChannels / bytesPerSample;

  leftData = (float*) malloc(sizeof(float) * numberOfFrames);
  rightData = (float*) malloc(sizeof(float) * numberOfFrames);
  
  float max = 0.0;
  float temp_max = 0.0;
  float temp_min = 0.0;
  // convert the wave data into separate channels of samples
  // different procedures, depending on the format
  switch (format) {
    case IEEE_FLOAT_FORMAT:
      for (int i = 0; i < numberOfFrames; i++) {
        leftData[i] = readIEEEFloat(wavedata.mid(i*noOfChannels*bytesPerSample, 
            bytesPerSample));

        if (noOfChannels == 1) {
          rightData[i] = leftData[i];
        }
        else {
          rightData[i] = readIEEEFloat(wavedata.mid(
                i*noOfChannels*bytesPerSample + bytesPerSample, bytesPerSample));
        }

        // to be used later for normalisation
        if (leftData[i] > temp_max) {
          temp_max = leftData[i];
        }
        if (rightData[i] > temp_max) {
          temp_max = rightData[i];
        }
        if (leftData[i] < temp_min) {
          temp_min = leftData[i];
        }
        if (rightData[i] < temp_min) {
          temp_min = rightData[i];
        }
      }

      // figure out the true maximum absolute value
      if (temp_min < -temp_max) {
        max = -temp_min;
      }
      else {
        max = temp_max;
      }

      break;

    case PCM_FORMAT:
      for (int i = 0; i < numberOfFrames; i++) {
        leftData[i] = readPCM(wavedata.mid(i*noOfChannels*bytesPerSample, 
            bytesPerSample));

        if (noOfChannels == 1) {
          rightData[i] = leftData[i];
        }
        else {
          rightData[i] = readPCM(wavedata.mid(
                i*noOfChannels*bytesPerSample + bytesPerSample, bytesPerSample));
        }

        max = fmaxf(max, abs(leftData[i]));
        max = fmaxf(max, abs(rightData[i]));
      }

      break;

    default:
      out << "unsupported format" << endl;
      exit(1);
  }

  // normalise data
  for (int i = 0; i < numberOfFrames; i++) {
    leftData[i] /= max;
    rightData[i] /= max;
  }
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
	curLeft = 0;
	curRight = 0;
}

bool SampleInstance::getLeftFrame (float* frame, long length, long rate) {
  long noFrames = sample->getNumberOfFrames();
  long sampleRate = sample->getSampleRate();
  float volumeIndex = sample->getVolumeIndex();
  int limit = std::min(length, noFrames - curLeft);
  for (int i = 0; i < limit; i++) {
    frame[i] += sample->leftData[curLeft + i * sampleRate / rate] * volumeIndex;
  }
  curLeft += length * sampleRate / rate;
  return curLeft <= noFrames;
}

bool SampleInstance::getRightFrame (float* frame, long length, long rate) {
  long noFrames = sample->getNumberOfFrames();
  long sampleRate = sample->getSampleRate();
  float volumeIndex = sample->getVolumeIndex();
  int limit = std::min(length, noFrames - curRight);
  for (int i = 0; i < limit; i++) {
    frame[i] += sample->rightData[curRight + i * sampleRate / rate] * volumeIndex;
  }
  curRight += length * sampleRate / rate;
  return curRight <= noFrames;
}

Sample* SampleInstance::getSample () {
  return sample;
}


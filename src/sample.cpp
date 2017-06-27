#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <math.h>
#include "sample.h"
#include "wav_parser.h"

/**
 * Deletes the contents of a chunk stream.
 */
void clearStream(ChunkStream* stream) {
  Chunk* chunk;
  while ((chunk = stream->next()) != NULL) {
    delete chunk;
  }
}

typedef union {
  char array[4];
  float ieee;
} float_converter;

/**
 * Converts a little-endian array of bytes into a float.
 */
float readIEEEFloat(QByteArray array) {
  float_converter conv;
  for (int i = 0; i < array.size(); i++) {
    conv.array[i] = array.at(array.size() - 1 - i);
  }
  return conv.ieee;
}

Sample::Sample (QString file) {
  if (file == "") {
    filename = "---";
    samplename = "---";
    return;
  }
  
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
  if (dynamic_cast<RiffChunk*>(chunk) == NULL) {
    out << "wav file corrupted, could not read RIFF tag" << endl;
    filename = "---";
    samplename = "---";
    clearStream(stream);
    return;
  }
  delete chunk;

  chunk = stream->next();
  if (dynamic_cast<WaveChunk*>(chunk) == NULL) {
    out << "wav file corrupted, could not read WAVE tag" <<endl;
    filename = "---";
    samplename = "---";
    clearStream(stream);
    return;
  }
  delete chunk;

  while ((chunk = stream->next()) != NULL) {
    if (dynamic_cast<FmtChunk*>(chunk) != NULL) {
      noOfChannels = dynamic_cast<FmtChunk*>(chunk)->nChannels;
      sampleRate = dynamic_cast<FmtChunk*>(chunk)->samplingRate;
      bitsPerSample = dynamic_cast<FmtChunk*>(chunk)->bitsPerSample;
      format = dynamic_cast<FmtChunk*>(chunk)->format;
    }

    if (dynamic_cast<DataChunk*>(chunk) != NULL) {
      wavedata = dynamic_cast<DataChunk*>(chunk)->data;
    }

    if (dynamic_cast<OtherChunk*>(chunk) != NULL) {
      out << "unknown chunk " << dynamic_cast<OtherChunk*>(chunk)->tag << endl;
    }

    delete chunk;
  }

  int bytesPerSample = bitsPerSample / 8;
  numberOfFrames = wavedata.size() / noOfChannels / bytesPerSample;

  leftData = (float*) malloc(sizeof(float) * numberOfFrames);
  rightData = (float*) malloc(sizeof(float) * numberOfFrames);
  float max = 0.0;
  float temp_max = 0.0;
  float temp_min = 0.0;
  
  // convert the wave data into separate channels of samples
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
    //max = fmaxf(max, abs(leftData[i]));
    //max = fmaxf(max, abs(rightData[i]));
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
  if (temp_min < -temp_max) {
    max = -temp_min;
  }
  else {
    max = temp_max;
  }
  out << max << endl;

  // normalise data
  for (int i = 0; i < numberOfFrames; i++) {
    leftData[i] /= max;
    rightData[i] /= max;
    if (leftData[i] > 1) {
      out << "yo" << endl;
    }
  }
}

Sample::Sample (Sample* sample) {
  leftData = sample->leftData;
  rightData = sample->rightData;
  curLeft = 0;
  curRight = 0;
  filename = sample->filename;
  samplename = sample->samplename;
  wavedata = sample->wavedata;
  numberOfFrames = sample->numberOfFrames;
  volumeIndex = sample->volumeIndex;
  noOfChannels = sample->noOfChannels;
  sampleRate = sample->sampleRate;
  bitsPerSample = sample->bitsPerSample;
  format = sample->format;
}

QString Sample::getFilename () {
  return filename;
}

QString Sample::getSamplename () {
  return samplename;
}

bool Sample::getLeftFrame (float* frame, long length) {
  for (int i = 0; i < length; i++) {
    if (curLeft + i > numberOfFrames) {
      return false;
    }
    frame[i] += leftData[curLeft + i] * volumeIndex;
  }
  curLeft += length;
  return true;
}

bool Sample::getRightFrame (float* frame, long length) {
  for (int i = 0; i < length; i++) {
    if (curRight + i > numberOfFrames) {
      return false;
    }
    frame[i] += rightData[curRight + i] * volumeIndex;
  }
  curRight += length;
  return true;
}

void Sample::setVolume (float volume) {
  volumeIndex = volume;
}


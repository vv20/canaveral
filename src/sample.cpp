#include <QString>
#include <QVector>
#include <QTextStream>
#include "sample.h"
#include "wav_parser.h"

void clearStream(ChunkStream* stream) {
  Chunk* chunk;
  while ((chunk = stream->next()) != NULL) {
    delete chunk;
  }
}

float readIEEEFloat(QByteArray array) {
  QByteArray reverse;
  for (int i = array.size() - 1; i >= 0; i--) {
    reverse.append(array.at(i));
  }
  return (float) *reverse.data() / 100;
}

Sample::Sample (QString file) {
  if (file == NULL) {
    filename = "---";
    samplename = "---";
    return;
  }
  
  QTextStream out(stdout);
  filename = file;

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
  }
}

QString Sample::getFilename () {
  return filename;
}

QString Sample::getSamplename () {
  return samplename;
}

float* Sample::getLeftFrame (long length) {
  float* frame = (float*) malloc(sizeof(float) * length);
  for (int i = 0; i < length; i++) {
    if (curLeft + i > numberOfFrames) {
      for (int j = i; j < length; j++) {
        frame[j] = 0;
      }
      break;
    }
    frame[i] = leftData[curLeft + i];
  }
  curLeft += length;
  return frame;
}

float* Sample::getRightFrame (long length) {
  float* frame = (float*) malloc(sizeof(float) * length);
  for (int i = 0; i < length; i++) {
    if (curRight + i > numberOfFrames) {
      for (int j = i; j < length; j++) {
        frame[j] = 0;
      }
      break;
    }
    frame[i] = rightData[curRight + i];
  }
  curRight += length;
  return frame;
}

void Sample::reset () {
  curLeft = 0;
  curRight = 0;
}


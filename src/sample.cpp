#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <math.h>
#include "sample.h"
#include "wav_parser.h"

typedef union {
  char array[4];
  float ieee;
} float_converter;

/**
 * Deletes the contents of a chunk stream.
 */
void clearStream(ChunkStream* stream) {
  Chunk* chunk;
  while ((chunk = stream->next()) != NULL) {
    delete chunk;
  }
}

/**
 * Converts an array of bytes into a float.
 */
float readIEEEFloat(QByteArray array) {
  float_converter conv;
  for (int i = 0; i < array.size(); i++) {
    conv.array[i] = array.at(i);
  }
  return conv.ieee;
}

int readPCM(QByteArray array) {
  // the first byte is offset binary, not two's complement
  int result = -pow(2, 7) - 1 + (unsigned char) array.at(0);
  for (int i = 1; i < array.size(); i++) {
    result += array.at(i) * pow(2, 8 * i);
  }
  return result;
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

Sample::Sample (Sample* sample) {
  leftData = sample->leftData;
  rightData = sample->rightData;
  curLeft = 0;
  curRight = 0;
  filename = sample->filename;
  samplename = sample->samplename;
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

bool Sample::getLeftFrame (float* frame, long length, long rate) {
  for (int i = 0; i < length; i++) {
    if (curLeft + i > numberOfFrames) {
      return false;
    }
    frame[i] += leftData[curLeft + i * sampleRate / rate] * volumeIndex;
  }
  curLeft += length * sampleRate / rate;
  return true;
}

bool Sample::getRightFrame (float* frame, long length, long rate) {
  for (int i = 0; i < length; i++) {
    if (curRight + i > numberOfFrames) {
      return false;
    }
    frame[i] += rightData[curRight + i * sampleRate / rate] * volumeIndex;
  }
  curRight += length * sampleRate / rate;
  return true;
}

void Sample::setVolume (float volume) {
  volumeIndex = volume;
}


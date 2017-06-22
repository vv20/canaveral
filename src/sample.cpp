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
}

QString Sample::getFilename () {
  return filename;
}

QString Sample::getSamplename () {
  return samplename;
}


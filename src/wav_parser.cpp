#include <QList>
#include <QByteArray>
#include <QString>
#include <tgmath.h>
#include "wav_parser.h"

Chunk::~Chunk() {}

RiffChunk::RiffChunk () {
  type = RIFF_CHUNK;
}

WaveChunk::WaveChunk () {
  type = WAVE_CHUNK;
}

FactChunk::FactChunk () {
  type = FACT_CHUNK;
}

FmtChunk::FmtChunk () {
  type = FMT_CHUNK;
}

DataChunk::DataChunk () {
  type = DATA_CHUNK;
}

OtherChunk::OtherChunk () {
  type = OTHER_CHUNK;
}

ChunkStream::ChunkStream () {
  index = 0;
}

Chunk* ChunkStream::next () {
  if (index < list.size()) {
    return list.at(index++);
  }
  else {
    return NULL;
  }
}

void ChunkStream::add (Chunk* newChunk) {
  list.append(newChunk);
}

/**
 * Parses a little-endian byte array into a number
 */
long readLittleEndian(QByteArray array) {
  long result = 0;
  for (int i = 0; i < array.size(); i++) {
    result += (((unsigned char) array.at(i)) * pow(2, 8 * i));
  }
  return result;
}

/**
 * Parses the following grammar:
 * (riff-ck) -> RIFF (length)
 */
RiffChunk* parseRiffChunk (QByteArray contents, long offset) {
  if (QString("RIFF") != contents.mid(offset, 4)) {
    return NULL;
  }
  RiffChunk* riff = new RiffChunk();
  riff->length = readLittleEndian(contents.mid(offset + 4, 4)) + 8;
  return riff;
}

/**
 * Parses the following grammar:
 * (wave-ck) -> WAVE
 */
WaveChunk* parseWaveChunk (QByteArray contents, long offset) {
  if (QString("WAVE") != contents.mid(offset, 4)) {
    return NULL;
  }
  WaveChunk* wave = new WaveChunk();
  wave->length = 4;
  return wave;
}

/**
 * Parses the grammar for the fmt chunk.
 */
FmtChunk* parseFmtChunk (QByteArray contents, long offset) {
  if (QString("fmt ") != contents.mid(offset, 4)) {
    return NULL;
  }
  FmtChunk* fmt = new FmtChunk();
  fmt->length = readLittleEndian(contents.mid(offset + 4, 4)) + 8;
  fmt->format = readLittleEndian(contents.mid(offset + 8, 2));
  fmt->nChannels = readLittleEndian(contents.mid(offset + 10, 2));
  fmt->samplingRate = readLittleEndian(contents.mid(offset + 12, 4));
  fmt->dataRate = readLittleEndian(contents.mid(offset + 16, 4));
  fmt->blockAlign = readLittleEndian(contents.mid(offset + 20, 2));
  fmt->bitsPerSample = readLittleEndian(contents.mid(offset + 22, 2));
  
  if (fmt->length > 16) {
    fmt->extSize = readLittleEndian(contents.mid(offset + 24, 2));
  }

  if (fmt->length > 18) {
    fmt->validBits = readLittleEndian(contents.mid(offset + 26, 2));
    fmt->channelMask = readLittleEndian(contents.mid(offset + 28, 4));
    fmt->subFormat = contents.mid(offset + 32, 16);
  }

  return fmt;
}

/**
 * Parses the following grammar:
 * (fact-ck) -> fact(length)(nSamplesPerChannel)
 */
FactChunk* parseFactChunk (QByteArray contents, long offset) {
  if (QString("fact") != contents.mid(offset, 4)) {
    return NULL;
  }
  FactChunk* fact = new FactChunk();
  fact->length = readLittleEndian(contents.mid(offset + 4, 4)) + 8;
  fact->sampPerChan = readLittleEndian(contents.mid(offset + 8, 4));
  return fact;
}

/**
 * Parses any arbitrary chunk.
 */
OtherChunk* parseOtherChunk (QByteArray contents, long offset) {
  if (QString("data") == contents.mid(offset, 4)) {
    return NULL;
  }
  OtherChunk* other = new OtherChunk();
  other->tag = QString(contents.mid(offset, 4));
  other->length = readLittleEndian(contents.mid(offset + 4, 4)) + 8;
  return other;
}

/**
 * Parses the following grammar:
 * (data-ck) -> data(length)(wave data)
 */
DataChunk* parseDataChunk (QByteArray contents, long offset) {
  if (QString("data") != contents.mid(offset, 4)) {
    return NULL;
  }
  DataChunk* data = new DataChunk();
  data->length = readLittleEndian(contents.mid(offset + 4, 4)) + 8;
  data->data = contents.mid(offset + 8, data->length - 8);
  return data;
}

/**
 * Parses the following grammar:
 * (other chunk)* -> ((fmt-ck)|(fact-ck)|(other))*
 */
QList<Chunk*> parseNonDataChunks(QByteArray contents, long offset) {
  QList<Chunk*> list;
  Chunk* chunk;

  while (true) {
    if (offset > contents.size()) {
      break;
    }
    chunk = parseFmtChunk(contents, offset);
    if (chunk != NULL) {
      list.append(chunk);
      offset += chunk->length;
      continue;
    }
    
    chunk = parseFactChunk(contents, offset);
    if (chunk != NULL) {
      list.append(chunk);
      offset += chunk->length;
      continue;
    }

    chunk = parseOtherChunk(contents, offset);
    if (chunk != NULL) {
      list.append(chunk);
      offset += chunk->length;
      continue;
    }

    break;
  }
  return list;
}

/**
 * Parses the following grammar:
 * (file) -> (riff-ck)(wave-ck)(other chunk)*(data-ck)(other chunk)*
 */
ChunkStream* parseFile (QString filename) {
  ChunkStream* chunkStream = new ChunkStream();
  QFile* file = new QFile(filename);
  file->open(QIODevice::ReadOnly);
  QByteArray contents = file->readAll();

  // parse riff chunk
  long offset = 0;
  RiffChunk* riff = parseRiffChunk(contents, offset);
  if (riff == NULL) {
    file->close();
    delete file;
    delete chunkStream;
    delete riff;
    return NULL;
  }
  chunkStream->add(riff);
  // the length stated in the riff tag is the length of the whole file since
  // riff is an overarching tag, so just add 8 to the offset
  offset += 8;

  //out << QString::number(contents.size()) << endl;
  //out << QString::number(riff->length) << endl;

  // parse wave chunk
  WaveChunk* wave = parseWaveChunk(contents, offset);
  if (wave == NULL) {
    file->close();
    delete file;
    delete chunkStream;
    delete riff;
    delete wave;
    return NULL;
  }
  chunkStream->add(wave);
  offset += wave->length;

  // parse other chunks before the data chunk
  QList<Chunk*> chunks1 = parseNonDataChunks(contents, offset);
  for (Chunk* chunk : chunks1) {
    chunkStream->add(chunk);
    offset += chunk->length;
  }

  // parse the data chunk
  DataChunk* data = parseDataChunk(contents, offset);
  if (data == NULL) {
    file->close();
    delete file;
    delete chunkStream;
    delete riff;
    delete wave;
    delete data;
    return NULL;
  }
  chunkStream->add(data);
  offset += data->length;

  // parse othe chunks after the data chunk
  QList<Chunk*> chunks2 = parseNonDataChunks(contents, offset);
  for (Chunk* chunk : chunks2) {
    chunkStream->add(chunk);
  }

  file->close();
  delete file;
  return chunkStream;
}

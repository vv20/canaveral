#pragma	once
#include <QByteArray>
#include <QFile>

class Chunk {
  public:
    virtual ~Chunk();
    long length;
};

class RiffChunk : public Chunk {
  public:
    RiffChunk ();
};

class WaveChunk : public Chunk {
  public:
    WaveChunk ();
};

class FactChunk : public Chunk {
  public:
    FactChunk ();
    long sampPerChan;
};

class FmtChunk : public Chunk {
  public:
    FmtChunk ();
    int format;
    int nChannels;
    long samplingRate;
    long dataRate;
    int blockAlign;
    int bitsPerSample;
    int extSize;
    int validBits;
    long channelMask;
    QByteArray subFormat;
};

class DataChunk : public Chunk {
  public:
    DataChunk ();
    QByteArray data;
};

class OtherChunk : public Chunk {
  public:
    OtherChunk ();
    QString tag;
};

class ChunkStream {
  public:
    ChunkStream ();
    Chunk* next();
    void add(Chunk* newChunk);

  private:
    QList<Chunk*> list;
    int index;
};

ChunkStream* parseFile(QString filename);

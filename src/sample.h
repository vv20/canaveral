#pragma	once
#include <QString>
#include <QByteArray>

#define IEEE_FLOAT_FORMAT (3)

class Sample {
  public:
    /* ====================  LIFECYCLE     ======================================= */
    Sample (QString filename);

    /* ====================  ACCESSORS     ======================================= */
    QString getFilename();
    QString getSamplename();

    float* getLeftFrame(long length);
    float* getRightFrame(long length);
    /* ====================  DATA MEMBERS  ======================================= */
  private:
    float* leftData;
    float* rightData;
    long curLeft;
    long curRight;

    QString filename;
    QString samplename;
    QByteArray wavedata;
    long numberOfFrames;

    int noOfChannels;
    long sampleRate;
    int bitsPerSample;
    int format;
    long curPos;
};


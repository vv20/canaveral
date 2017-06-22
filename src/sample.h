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

    long* getLeftFrame(long length);
    long* getRightFrame(long length);
    /* ====================  DATA MEMBERS  ======================================= */
  private:
    long* leftData;
    long* rightData;
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


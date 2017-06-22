#pragma	once
#include <QString>
#include <QByteArray>

class Sample {
  public:
    /* ====================  LIFECYCLE     ======================================= */
    Sample (QString filename);

    /* ====================  ACCESSORS     ======================================= */
    QString getFilename();
    QString getSamplename();
    /* ====================  DATA MEMBERS  ======================================= */
  private:
    QByteArray wavedata;
    QString filename;
    QString samplename;

    int noOfChannels;
    long sampleRate;
    int bitsPerSample;
    int format;
};


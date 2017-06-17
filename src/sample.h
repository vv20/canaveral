#pragma	once
#include <QString>

class Sample {
  public:
    /* ====================  LIFECYCLE     ======================================= */
    Sample (QString filename);

    /* ====================  ACCESSORS     ======================================= */
    QString getFilename();
    QString getSamplename();
    /* ====================  DATA MEMBERS  ======================================= */
  private:
    int noOfChannels;
    int wavedata[];
    QString filename;
    QString samplename;
};


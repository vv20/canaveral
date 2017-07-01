#pragma	once
#include <QString>
#include <QByteArray>

#define IEEE_FLOAT_FORMAT (3)
#define PCM_FORMAT (1)

class Sample {
  public:
    float* leftData;
    float* rightData;
    /* ====================  LIFECYCLE     ======================================= */
    Sample (QString filename);
    Sample (Sample* sample);

    /* ====================  ACCESSORS     ======================================= */
    QString getFilename();
    QString getSamplename();

    bool getLeftFrame(float* frame, long length, long rate);
    bool getRightFrame(float* frame, long length, long rate);
    
    /* ====================  MUTATORS     ======================================= */
    void setVolume(float volume);
  
  private:
    long curLeft;
    long curRight;

    QString filename;
    QString samplename;
    long numberOfFrames;

    float volumeIndex;
    
    int noOfChannels;
    long sampleRate;
    int bitsPerSample;
    int format;
};


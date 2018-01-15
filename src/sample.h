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

    /* ====================  ACCESSORS     ======================================= */
    QString getFilename();
    QString getSamplename();

    long getNumberOfFrames();
    long getSampleRate();
    float getVolumeIndex();
    
    /* ====================  MUTATORS     ======================================= */
    void setVolume(float volume);
  
  private:
    QString filename;
    QString samplename;
    long numberOfFrames;

    float volumeIndex;
    
    int noOfChannels;
    long sampleRate;
    int bitsPerSample;
    int format;
};

class SampleInstance {
  public:
    SampleInstance(Sample* genericSample);
    bool getLeftFrame(float* frame, long length, long rate);
    bool getRightFrame(float* frame, long length, long rate);
    Sample* getSample();

  protected:
    Sample* sample;
    long curLeft;
    long curRight;
};


#pragma	once
#include <QString>
#include <QByteArray>

#define IEEE_FLOAT_FORMAT (3)
#define PCM_FORMAT (1)

class Sample {
  public:
    float* data;
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
    float volumeIndex;
    
    long numberOfFrames;
    int noOfChannels;
    long sampleRate;
};

class SampleInstance {
  public:
    SampleInstance(Sample* genericSample);
    bool getFrame(float* frame, long length, long rate);
    Sample* getSample();

  protected:
    Sample* sample;
    long position;
};


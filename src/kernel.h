#pragma	once

#include <QVector>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <QList>
#include <QTextStream>
#include <QString>
#include "sample.h"
#include "constants.h"

typedef float sample_t;

static QVector<Sample*> samples(NUMBER_OF_BUTTONS);
static jack_port_t* left_out;
static jack_port_t* right_out;
static jack_port_t* midi_in;
static jack_nframes_t samplerate;
static QList<SampleInstance*> activeSamples;

void init_kernel();
void loadSample(int no, QString filename);
void ejectSample(int no);
void playSample(int no);
void setVolume(int no, int volume);
void panic();

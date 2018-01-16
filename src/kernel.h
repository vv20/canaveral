#pragma	once

#include <QVector>
#include <QList>
#include <QTextStream>
#include <QString>
#include <jack/jack.h>
#include <jack/midiport.h>
#include "sample.h"
#include "constants.h"

typedef float sample_t;

static QVector<Sample*> samples(NUMBER_OF_BUTTONS);
static jack_port_t* out_port;
static jack_port_t* midi_in;
static jack_nframes_t samplerate;
static QList<SampleInstance*> activeSamples;

void init_kernel();
void load_sample(int no, QString filename);
void eject_sample(int no);
void play_sample(int no);
void set_volume(int no, int volume);
QString get_sample_name(int no);
QString get_sample_file_name(int no);
void panic();

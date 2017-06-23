#include <jack/jack.h>
#include <QTextStream>
#include <QList>
#include "backend.h"
#include "sample.h"

jack_port_t* left_out; /** Left output port. */
jack_port_t* right_out; /** Right output port. */

typedef jack_default_audio_sample_t sample_t;

jack_nframes_t samplerate; /** Current sample rate of the JACK engine. */

QList<Sample*> playingSamples;

int callback (jack_nframes_t nframes, void* arg) {
  sample_t* left_buffer = (sample_t*) jack_port_get_buffer(left_out, nframes);
  sample_t* right_buffer = (sample_t*) jack_port_get_buffer(right_out, nframes);

  QList<long*> left_frames;
  QList<long*> right_frames;

  if (playingSamples.size() == 0) {
    for (unsigned int i = 0; i < nframes; i++) {
      left_buffer[i] = 0;
      right_buffer[i] = 0;
    }
    return 0;
  }

  for (Sample* sample : playingSamples) {
    left_frames.append(sample->getLeftFrame(nframes));
    right_frames.append(sample->getRightFrame(nframes));
  }

  for (unsigned int i = 0; i < nframes; i++) {
    for (int j = 0; j < playingSamples.size(); j++) {
      left_buffer[i] += left_frames.at(j)[i];
      right_buffer[i] += right_frames.at(j)[i];
    }
  }
  return 0;
}

void shutdown (void* arg) {
  exit(1);
}

BackEnd::BackEnd () {
  const char* client_name = "canaveral";
  const char* server_name = NULL;
  jack_client_t *client;
  jack_status_t status;

  QTextStream out (stdout);

  // open a client connection to jack
  client = jack_client_open(client_name, JackNullOption, &status, server_name);
  if (client == NULL) {
    out << "failed to open a client" << endl;
    if (status & JackServerFailed) {
      out << "failed to connect to JACK server" << endl;
    }
    exit (1);
  }

  // set the callbacks
  jack_set_process_callback(client, callback, 0);
  jack_on_shutdown(client, shutdown, 0);

  // create the output ports
  left_out = jack_port_register(client, "left",
      JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
  right_out = jack_port_register(client, "right",
      JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

  if (left_out == NULL || right_out == NULL) {
    out << "no more JACK ports available" << endl;
    exit(1);
  }

  // start the client 
  if (jack_activate (client)) {
    out << "cannot activate client" << endl;
    exit (1);
  }
}

void BackEnd::playSample (Sample* sample) {
  playingSamples.append(sample);
}

void BackEnd::stopSample (Sample* sample) {
  playingSamples.removeOne(sample);
}


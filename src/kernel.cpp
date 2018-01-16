#include "kernel.h"
#include "constants.h"

static QVector<Sample*> samples(NUMBER_OF_BUTTONS);
static jack_port_t* out_port;
static jack_port_t* midi_in;
static jack_nframes_t samplerate;
static QList<SampleInstance*> activeSamples;

int process(jack_nframes_t nframes, void* arg) {
    // retrieve the buffers
    sample_t* buffer = (sample_t*) jack_port_get_buffer(out_port, nframes);
    void* midi_buffer = jack_port_get_buffer(midi_in, nframes);

    // clear the buffers in case there was noise in them
    for (unsigned int i = 0; i < nframes; i++) {
        buffer[i] = 0;
    }

    // let each sample instance imprint its next frame onto the buffers
    for (int i = 0; i < activeSamples.size(); i++) {
        // if the instance has reached the end, delete it
        if (!activeSamples.at(i)->getFrame(buffer, nframes, samplerate)) {
            delete activeSamples.at(i);
            activeSamples.removeAt(i);
        }
    }

    // respond to midi events
    jack_nframes_t no_midi_events = jack_midi_get_event_count(midi_buffer);
    jack_midi_event_t midi_event;
    for (unsigned int i = 0; i < no_midi_events; i++) {
        // break if couldn't get the event
        if (jack_midi_event_get(&midi_event, midi_buffer, i)) break;
        int message_code = (midi_event.buffer[0] & 0xf0) >> 4;
        switch (message_code) {
            // note on
            case 8: {
                        if (midi_event.size != 3) break; // corrupt event, shouldnt happen
                        if (midi_event.time > nframes) break; // also shouldn't happen
                        // samples start from C2 (which is MIDI note number 36) because i have 
                        // so declared
                        int sample_no = midi_event.buffer[1] - 36;
                        // ignore notes out of bounds
                        if (sample_no < 0 || sample_no > NUMBER_OF_BUTTONS) continue;

                        SampleInstance* instance = new SampleInstance(samples.at(sample_no));
                        // start playing the sample from the time given in the event and then just add it
                        // to the playing samples
                        instance->getFrame(&buffer[midi_event.time], nframes - midi_event.time, samplerate);
                        activeSamples.append(new SampleInstance(samples.at(sample_no)));
                        break;
                    }
            // note off
            case 9: {
                        if (midi_event.size != 3) break;
                        if (midi_event.time > nframes) break;
                        int sample_no = midi_event.buffer[1] - 36;
                        if (sample_no < 0 || sample_no > NUMBER_OF_BUTTONS) continue;

                        QString name = samples.at(sample_no)->getFilename();
                        for (int j = 0; j < activeSamples.size(); j++) {
                            if (!QString::compare(activeSamples.at(i)->getSample()->getFilename(), name, Qt::CaseSensitive)) {
                                delete activeSamples.at(i);
                                activeSamples.removeAt(i);
                            }
                        }
                        break;
                    }
            // channel mode message
            case 11: {
                         if (midi_event.size != 3) break;
                         if (midi_event.buffer[1] == 123 && midi_event.buffer[2] == 0) {
                             panic();
                         }
                         break;
                         //TODO: implement
                     }
            default: {
                         // unsupported command
                         break;
                     }
        }
    }

    return 0;
}

void shutdown(void* arg) {
    QTextStream out(stdout);
    out << "server shut down, quitting" << endl;
    exit(1);
}

int rate_change(jack_nframes_t new_rate, void* arg) {
    QTextStream out(stdout);
    out << "sample rate changed to " << QString::number(new_rate) << endl;
    samplerate = new_rate;
    return 0;
}

void init_kernel() {
    QTextStream out(stdout);
    const char* client_name = "canaveral";
    jack_client_t* client;
    jack_status_t status;

    // initialise the sample list to all empty samples
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
        samples.insert(i, new Sample(""));
    }

    // open a jack client
    client = jack_client_open(client_name, JackNullOption, &status, NULL);
    if (client == NULL) {
        out << "failed to open a client" << endl;
        if (status & JackServerFailed) out << "failed to connect to JACK server" << endl;
        exit(1);
    }

    // set the callbacks
    jack_set_process_callback(client, process, 0);
    jack_on_shutdown(client, shutdown, 0);
    jack_set_sample_rate_callback(client, rate_change, 0);

    // set current sample rate
    samplerate = jack_get_sample_rate(client);
    out << "sample rate: " << QString::number(samplerate) << endl;

    // register ports
    out_port = jack_port_register(client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    midi_in = jack_port_register(client, "input", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);

    if (out_port == NULL || midi_in == NULL) {
        out << "no more JACK ports available" << endl;
        exit(1);
    }

    // start the client
    if (jack_activate(client)) {
        out << "cannot activate client" << endl;
        exit(1);
    }

    // try to connect to system ports
    const char** ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (ports == NULL) out << "no physical ports found" << endl;
    if (jack_connect(client, jack_port_name(out_port), ports[0])) 
        out << "could not connect left port" << endl;
}

void load_sample(int no, QString filename) {
    delete samples.at(no);
    samples.replace(no, new Sample(filename));
}

void eject_sample(int no) {
    load_sample(no, "");
}

void play_sample(int no) {
    activeSamples.append(new SampleInstance(samples.at(no)));
}

void set_volume(int no, int volume) {
    samples.at(no)->setVolume((float) volume / (100 / VOLUME_TICKS));
}

QString get_sample_name(int no) {
    return samples.at(no)->getSamplename();
}

QString get_sample_file_name(int no) {
    return samples.at(no)->getFilename();
}

void panic() {
    activeSamples.clear();
}

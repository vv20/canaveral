#include "kernel.h"

int process(jack_nframes_t nframes, void* arg) {
    // retrieve the buffers
    sample_t* left_buffer = (sample_t*) jack_port_get_buffer(left_out, nframes);
    sample_t* right_buffer = (sample_t*) jack_port_get_buffer(right_out, nframes);
    void* midi_buffer = jack_port_get_buffer(midi_in, nframes);

    // respond to midi events
    jack_nframes_t no_midi_events = jack_midi_get_event_count(midi_buffer);
    jack_midi_event_t midi_event;
    for (unsigned int i = 0; i < no_midi_events; i++) {
        // break if couldn't get the event
        if (jack_midi_event_get(&midi_event, midi_buffer, i)) break;
        int message_code = (midi_event.buffer[0] & 0xf0) << 4;
        switch (message_code) {
            // note on
            case 8: {
                        if (midi_event.size != 3) break; // corrupt event, shouldnt happen
                        int sample_no = midi_event.buffer[1];
                        // TODO: finish this
                        break;
                    }
            // note off
            case 9: {
                        break;
                        // TODO: implement
                    }
            // channel mode message
            case 11: {
                         break;
                         //TODO: implement
                     }
            default: {
                         // unsupported command
                         break;
                     }
        }
    }

    // clear the buffers in case there was noise in them
    for (unsigned int i = 0; i < nframes; i++) {
        left_buffer[i] = 0;
        right_buffer[i] = 0;
    }

    // let each sample instance imprint its next frame onto the buffers
    for (int i = 0; i < activeSamples.size(); i++) {
        // if the instance has reached the end, delete it
        if (!activeSamples.at(i)->getLeftFrame(left_buffer, nframes, samplerate) ||
                !activeSamples.at(i)->getRightFrame(right_buffer, nframes, samplerate)) {
            delete activeSamples.at(i);
            activeSamples.removeAt(i);
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
    left_out = jack_port_register(client, "left", JACK_DEFAULT_AUDIO_TYPE, 
            JackPortIsOutput, 0);
    right_out = jack_port_register(client, "right", JACK_DEFAULT_AUDIO_TYPE, 
            JackPortIsOutput, 0);
    midi_in = jack_port_register(client, "input", JACK_DEFAULT_MIDI_TYPE,
            JackPortIsInput, 0);

    if (left_out == NULL || right_out == NULL || midi_in == NULL) {
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
    if (jack_connect(client, jack_port_name(left_out), ports[0])) 
        out << "could not connect left port" << endl;
    if (jack_connect(client, jack_port_name(right_out), ports[1]))
        out << "could not connect right port" << endl;
}

void loadSample(int no, QString filename) {
    // TODO: implement
}

void ejectSample(int no) {
    // TODO: implement
}

void playSample(int no) {
    activeSamples.append(new SampleInstance(samples.at(no)));
}

void setVolume(int no, int volume) {
    samples.at(no)->setVolume((float) volume / (100 / VOLUME_TICKS));
}

void panic() {
    activeSamples.clear();
}

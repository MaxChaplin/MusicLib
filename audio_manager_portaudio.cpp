#include "audio_manager_portaudio.hpp"

#include <portaudio.h>
#include <iostream>
#include <memory>

namespace MusicLib {

static int portaudio_callback(const void *inputBuffer [[maybe_unused]],
    void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo [[maybe_unused]],
    PaStreamCallbackFlags statusFlags [[maybe_unused]], void *userData)
{
    float *out = (float*) outputBuffer;
    PortAudioData *data = (PortAudioData*) userData;
    Sequencer& seq = data->seq;
    InstrumentManager& ins_mgr = data->ins_mgr;

    // Write interleaved audio data.
    for(size_t i = 0; i < framesPerBuffer; i++)
    {
        ins_mgr.process(data->sample_duration, *out, *(out+1));
        out += 2;
        seq.tick();
    }

    return 0;
}

AudioManagerPortAudio::AudioManagerPortAudio(unsigned int sample_rate, unsigned int buffer_size, PortAudioData& callback_data)
: m_sample_rate{sample_rate}
, m_buffer_size{buffer_size}
, m_sample_duration{1.0f / buffer_size}
, m_stream{nullptr, cleanup_stream}
, m_callback_data{callback_data}
{
    PaError err;
    err = Pa_Initialize();
    manage_error(err);
}

AudioManagerPortAudio::~AudioManagerPortAudio() noexcept
{
    PaError err;
    err = Pa_CloseStream(m_stream.release());
    manage_error(err);
    err = Pa_Terminate();
    manage_error(err);
}

void AudioManagerPortAudio::play()
{
    PaError err;

    PaStream* stream = m_stream.get();

    // Open an audio I/O stream.
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, m_sample_rate,
        m_buffer_size, portaudio_callback, &m_callback_data);
    manage_error(err);

    err = Pa_StartStream(stream);
    manage_error(err);
}

void AudioManagerPortAudio::stop()
{
    PaError err = Pa_StopStream(m_stream.get());
    manage_error(err);
}

unsigned int AudioManagerPortAudio::sample_rate()
{
    return m_sample_rate;
}

float AudioManagerPortAudio::sample_duration()
{
    return m_sample_duration;
}

    void AudioManagerPortAudio::manage_error(PaError err)
{
    if(err != paNoError)
    {
        std::cerr << "PortAudio error " << err << std::endl;
    }
    
}

}
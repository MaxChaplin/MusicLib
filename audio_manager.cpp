#include "audio_manager.hpp"
#include "data.hpp"

#include <portaudio.h>
#include <iostream>
#include <memory>

namespace MusicLib
{
    static void manage_error(PaError err)
    {
        if(err != paNoError)
        {
            std::cerr << "PortAudio error " << err << std::endl;
        }
        
    }

    AudioManager::AudioManager(unsigned int sample_rate, unsigned int buffer_size, PaStreamCallback& callback, AudioData& callback_data)
    : m_sample_rate{sample_rate}
    , m_buffer_size{buffer_size}
    , m_time_element{1.0f / buffer_size}
    , m_stream{nullptr, cleanup_stream}
    , m_callback{callback}
    , m_callback_data{callback_data}
    {
        PaError err;
        err = Pa_Initialize();
        manage_error(err);
    }

    AudioManager::~AudioManager()
    {
        PaError err;
        err = Pa_CloseStream(m_stream.release());
        manage_error(err);
        err = Pa_Terminate();
        manage_error(err);
    }

    void AudioManager::play()
    {
        PaError err;

        PaStream* stream = m_stream.get();

        // Open an audio I/O stream.
        err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, m_sample_rate,
                                m_buffer_size, m_callback, &m_callback_data);
        manage_error(err);

        err = Pa_StartStream(stream);
        manage_error(err);
    }

    void AudioManager::stop()
    {
        PaError err = Pa_StopStream(m_stream.get());
        manage_error(err);
    }

    unsigned int AudioManager::sample_rate()
    {
        return m_sample_rate;
    }

    float AudioManager::time_element()
    {
        return m_time_element;
    }
}
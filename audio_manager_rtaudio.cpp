#include "audio_manager_rtaudio.hpp"
#include "data.hpp"

#include <RtAudio.h>
#include <iostream>
#include <memory>
// #include <vector>

namespace MusicLib
{
    int rtaudio_callback(void *outputBuffer, void *inputBuffer [[maybe_unused]], unsigned int nBufferFrames, double streamTime [[maybe_unused]], RtAudioStreamStatus status, void *userData)
    {
        float *out = (float*) outputBuffer;
        RtAudioData *data = (RtAudioData*) userData;
        Sequencer& sequencer = data->seq;
        InstrumentManager& ins_mgr = data->ins_mgr;

        if (status)
        {
            std::cout << "Stream underflow detected!" << std::endl;
        }

        // Write interleaved audio data.
        for (size_t i = 0; i < nBufferFrames; ++i)
        {
            ins_mgr.process(data->sample_duration, *out, *(out+1));
            out += 2;
            sequencer.handle_sample();
        }

        return 0;
    }

    AudioManagerRtAudio::AudioManagerRtAudio(unsigned int sample_rate, unsigned int buffer_size, RtAudioData& callback_data)
    : m_sample_rate{sample_rate}
    , m_buffer_size{buffer_size}
    , m_sample_duration{1.0f / buffer_size}
    , m_dac{}
    , m_callback_data{callback_data}
    , m_stream_parameters{}
    {
        // Set stream parameters.
        m_stream_parameters.deviceId = m_dac.getDefaultOutputDevice();
        m_stream_parameters.nChannels = 2;
        m_stream_parameters.firstChannel = 0;

        if (m_dac.getDeviceIds().size() < 1)
        {
            std::cerr << "No device IDs found!" << std::endl;
        }
    }

    AudioManagerRtAudio::~AudioManagerRtAudio() noexcept
    {
        if (m_dac.isStreamOpen())
        {
            m_dac.closeStream();
        }
    }

    void AudioManagerRtAudio::play()
    {
        auto err = m_dac.openStream(&m_stream_parameters, nullptr, RTAUDIO_FLOAT64, m_sample_rate, &m_buffer_size, &rtaudio_callback, (void*) &m_callback_data);
        // manage_error(err);

        err = m_dac.startStream();
        // manage_error(err);
    }

    void AudioManagerRtAudio::stop()
    {   
        if (m_dac.isStreamRunning())
        {
            m_dac.stopStream();
        }
    }

    unsigned int AudioManagerRtAudio::sample_rate()
    {
        return m_sample_rate;
    }

    float AudioManagerRtAudio::sample_duration()
    {
        return m_sample_duration;
    }

    // void AudioManagerRtAudio::manage_error(RtAudioErrorType err)
    // {
    //     if(err)
    //     {
    //         std::cerr << m_dac.getErrorText() << std::endl;
    //     }
    // }
}
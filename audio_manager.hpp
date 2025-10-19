#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include "instrument.hpp"
#include "sequencer.hpp"
#include "data.hpp"

#include <portaudio.h>
#include <memory>



namespace MusicLib
{
class AudioManager
    {
    public:
        AudioManager(unsigned int sample_rate, unsigned int buffer_size, PaStreamCallback& callback, AudioData& callback_data);
        virtual ~AudioManager() = 0;

        virtual void play() = 0;
        virtual void stop() = 0;

        virtual unsigned int sample_rate() = 0;
        virtual float time_element() = 0;
        virtual void callback_data(AudioData& callback_data) = 0;
    };

    class AudioManagerPortAudio : public AudioManager
    {
    public:
        explicit AudioManagerPortAudio(unsigned int sample_rate, unsigned int buffer_size, PaStreamCallback& callback, AudioData& callback_data);
        ~AudioManagerPortAudio() noexcept = default;

        void play() override;
        void stop() override;

        unsigned int sample_rate() override;
        float time_element() override;
        void callback_data(AudioData& callback_data) override;

    private:
        static void cleanup_stream(PaStream* stream)
        {
            if (stream)
            {
                Pa_CloseStream(stream);
            }
        }

    private:
        unsigned int m_sample_rate;
        unsigned int m_buffer_size;
        float m_time_element;
        std::unique_ptr<PaStream, decltype(&cleanup_stream)> m_stream;
        PaStreamCallback& m_callback;
        AudioData& m_callback_data;
    };

    #endif // AUDIO_MANAGER_H_
}
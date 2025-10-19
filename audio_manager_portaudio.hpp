#ifndef AUDIO_MANAGER_PORTAUDIO_H_
#define AUDIO_MANAGER_PORTAUDIO_H_

#include "audio_manager.hpp"
#include "instrument.hpp"
#include "sequencer.hpp"

#include <portaudio.h>
#include <memory>

namespace MusicLib
{
    struct PortAudioData
    {
        Sequencer& seq;
        InstrumentManager& ins_mgr;
        float sample_duration;
    };

    class AudioManagerPortAudio : public AudioManager
    {
    public:
        explicit AudioManagerPortAudio(unsigned int sample_rate,
            unsigned int buffer_size, PortAudioData& callback_data);
        ~AudioManagerPortAudio() noexcept;

        void play() override;
        void stop() override;

        unsigned int sample_rate() override;
        float sample_duration() override;
        void callback_data(PortAudioData& callback_data);

    private:
        void manage_error(PaError err);
    
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
        float m_sample_duration;
        std::unique_ptr<PaStream, decltype(&cleanup_stream)> m_stream;
        PortAudioData& m_callback_data;
    };
}

#endif // AUDIO_MANAGER_PORTAUDIO_H_
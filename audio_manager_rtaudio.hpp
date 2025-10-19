#ifndef AUDIO_MANAGER_RTAUDIO_H_
#define AUDIO_MANAGER_RTAUDIO_H_

#include "audio_manager.hpp"
#include "sequencer.hpp"
#include "instrument_manager.hpp"

#include <RtAudio.h>

namespace MusicLib {

struct RtAudioData
{
    Sequencer& seq;
    InstrumentManager& ins_mgr;
    float sample_duration;
};

class AudioManagerRtAudio : public AudioManager
{
public:
    explicit AudioManagerRtAudio(unsigned int sample_rate, unsigned int buffer_size, RtAudioData& callback_data);
    ~AudioManagerRtAudio() noexcept;

    void play() override;
    void stop() override;

    unsigned int sample_rate() override;
    float sample_duration() override;

private:
    void manage_error(RtAudioErrorType err);

private:
    unsigned int m_sample_rate;
    unsigned int m_buffer_size;
    float m_sample_duration;
    RtAudio m_dac;
    RtAudioData m_callback_data;
    RtAudio::StreamParameters m_stream_parameters;
};

}

#endif // AUDIO_MANAGER_H_
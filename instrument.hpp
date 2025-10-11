#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "envelope.hpp"
#include "voice.hpp"

#include <vector>
#include <memory>

namespace MusicLib
{
    class Instrument
    {
    public:
        explicit Instrument(Voice& voice, unsigned int polyphony = 4,
            float vol = 1, bool retrigger = false);
        ~Instrument() noexcept = default;

        Voice& voice(size_t num);

        float process(float sample_time);

        void note_on(unsigned int voice_num, float freq);
        void note_on(unsigned int voice_num);
        void note_off(unsigned int voice_num);

        void vol(float vol);

        void retrigger(bool retrigger);

    private:
        std::vector<std::shared_ptr<Voice>> m_voices;
        float m_vol;
        bool m_retrigger;
    };
}

#endif // INSTRUMENT_H_
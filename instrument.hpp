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
            float vol = 1, float pan = .5, bool retrigger = false);
        ~Instrument() noexcept = default;
        Instrument(const Instrument& other);
        Instrument& operator=(const Instrument& other);
        Instrument(Instrument&&) noexcept = default;
        Instrument& operator=(Instrument&&) noexcept = default;

        Voice& voice(size_t num);

        void process(float sample_time, float& out_left, float& out_right);

        void note_on(unsigned int voice_num, float freq);
        void note_on(unsigned int voice_num);
        void note_off(unsigned int voice_num);

        void vol(float vol);
        float vol() const;

        void pan(float freq);
        float pan() const;

        void retrigger(bool retrigger);

    private:
        std::vector<std::unique_ptr<Voice>> m_voices;
        float m_vol;
        float m_pan;
        bool m_retrigger;
    };
}

#endif // INSTRUMENT_H_
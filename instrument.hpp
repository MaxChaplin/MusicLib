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

        void note_on(size_t voice_num, float freq);
        void note_off(size_t voice_num);

        void vol(float vol);

        void retrigger(bool retrigger);

    private:
        std::vector<std::shared_ptr<Voice>> m_voices;
        float m_vol;
        bool m_retrigger;

    };

    class InstrumentManager
    {
    public:
        explicit InstrumentManager(size_t buffer_size);
        ~InstrumentManager() noexcept = default;

        Instrument& instrument(size_t num);

        void add_instrument(Instrument& instrument);
        float process(float time_element);

    private:
        std::vector<Instrument> m_instruments;
        std::vector<float> m_buffer;
        size_t m_buffer_cursor;
    };
}

#endif // _INSTRUMENT_H_
#include "instrument.hpp"
#include "voice.hpp"
#include <iostream>
#include <memory>

namespace MusicLib
{
    Instrument::Instrument(Voice& voice, unsigned int polyphony, float vol, bool retrigger)
    : m_voices{std::vector<std::shared_ptr<Voice>>{}}
    , m_vol{vol}
    , m_retrigger{retrigger}
    {
        m_voices.reserve(polyphony);

        for (size_t i = 0; i < polyphony; ++i)
        {
            m_voices.push_back(voice.clone());
        }
    }

    Voice& Instrument::voice(size_t num)
    {
        return *m_voices[num];
    }

    float Instrument::process(float sample_time)
    {
        float sample = 0;
        for (size_t i=0; i < m_voices.size(); ++i)
        {
            if (m_voices[i]->is_on())
            {
                sample += m_vol * m_voices[i]->process(sample_time);
            }
        }

        return sample;
    }

    void Instrument::note_on(size_t voice_num, float freq)
    {
        if (voice_num >= m_voices.size())
        {
            // No voice with this number.
            return;
        }

        if (freq == 0)
        {
            m_voices[voice_num]->note_off();
        }
        else
        {
            m_voices[voice_num]->note_on(freq);
        }
    }

    void Instrument::note_off(size_t voice_num)
    {
        m_voices[voice_num]->note_off();
    }

    void Instrument::vol(float vol)
    {
        m_vol = vol;
    }

    void Instrument::retrigger(bool retrigger)
    {
        m_retrigger = retrigger;
    }

    InstrumentManager::InstrumentManager(size_t buffer_size)
    : m_instruments{}
    , m_buffer(buffer_size, 0)
    , m_buffer_cursor{}
    {
    }

    Instrument& InstrumentManager::instrument(size_t num)
    {
        return m_instruments[num];
    }

    void InstrumentManager::add_instrument(Instrument& instrument)
    {
        m_instruments.push_back(instrument);
    }

    float InstrumentManager::process(float time_element)
    {
        float sample = 0;
        for (auto it = m_instruments.begin(); it != m_instruments.end(); ++it)
        {
            sample += it->process(time_element);
        }

        return sample;
    }
}
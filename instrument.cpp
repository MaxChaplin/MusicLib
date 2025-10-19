#include "instrument.hpp"
#include "voice.hpp"
#include <iostream>
#include <memory>

namespace MusicLib
{
    Instrument::Instrument(Voice& voice, unsigned int polyphony, float vol, float pan, bool retrigger)
    : m_voices{std::vector<std::shared_ptr<Voice>>{}}
    , m_vol{vol}
    , m_pan{pan}
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

    void Instrument::process(float sample_time, float& out_left, float& out_right)
    {
        out_left = 0;
        out_right = 0;

        // for (size_t i=0; i < m_voices.size(); ++i)
        for (auto v : m_voices)
        {
            if (v->is_on())
            {
                float temp;
                v->process(sample_time, temp);
                out_left += m_vol * temp;
            }
        }

        // Pan
        out_right = out_left * m_pan;
        out_left *= 1 - m_pan;
    }

    void Instrument::note_on(unsigned int voice_num, float freq)
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

    void Instrument::note_on(unsigned int voice_num)
    {
        note_on(voice_num, m_voices[voice_num]->freq());
    }

    void Instrument::note_off(unsigned int voice_num)
    {
        m_voices[voice_num]->note_off();
    }

    void Instrument::vol(float vol)
    {
        m_vol = vol;
    }

    float Instrument::vol() const
    {
        return m_vol;
    }

    void Instrument::pan(float pan)
    {
        m_pan = pan;
    }

    float Instrument::pan() const
    {
        return m_pan;
    }

    void Instrument::retrigger(bool retrigger)
    {
        m_retrigger = retrigger;
    }
}
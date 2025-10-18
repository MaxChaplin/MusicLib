#include "instrument.hpp"
#include "voice.hpp"
#include <iostream>
#include <memory>

namespace MusicLib {

Instrument::Instrument(Voice& voice, unsigned int polyphony, float vol, float pan, bool retrigger)
: m_voices{std::vector<std::unique_ptr<Voice>>{}}
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

Instrument::Instrument(const Instrument& other)
: m_voices{}
, m_vol{other.m_vol}
, m_pan{other.m_pan}
, m_retrigger{other.m_retrigger}
{
    for (const auto& v : other.m_voices)
    {
        m_voices.emplace_back(v->clone());
    }
}

Instrument& Instrument::operator=(const Instrument& other)
{
    if (this != &other)
    {
        m_voices = std::vector<std::unique_ptr<Voice>>{};
        for (const auto& v : other.m_voices)
        {
            m_voices.emplace_back(v->clone());
        }

        m_vol = other.m_vol;
        m_pan = other.m_pan;
        m_retrigger = other.m_retrigger;
    }
    return *this;
}

// Voice& Instrument::voice(size_t num)
// {
//     return *m_voices[num];
// }

void Instrument::process(float sample_duration, float& out_left, float& out_right)
{
    out_left = 0;
    out_right = 0;
    static float temp;

    for (auto& v : m_voices)
    {
        if (v->is_on())
        {
            
            v->process(sample_duration, temp);
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

Voice& Instrument::voice(size_t index)
{
    return *m_voices[index];
}

}
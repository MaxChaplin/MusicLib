#include "instrument.hpp"
#include "voice.hpp"

#include <iostream>
#include <memory>


namespace MusicLib {

InstrumentMono::InstrumentMono(Voice& voice, float vol, float pan, bool retrigger)
: m_voice{voice.clone()}
, m_vol{vol}
, m_pan{pan}
, m_retrigger{retrigger}
{

}

InstrumentMono::InstrumentMono(const InstrumentMono& other)
: m_voice{other.m_voice->clone()}
, m_vol{other.m_vol}
, m_pan{other.m_pan}
, m_retrigger{other.m_retrigger}
{

}

InstrumentMono& InstrumentMono::operator=(const InstrumentMono& other)
{
    if (this != &other)
    {
        m_voice = other.m_voice->clone();
        m_vol = other.m_vol;
        m_pan = other.m_pan;
        m_retrigger = other.m_retrigger;
    }
    return *this;
}

std::unique_ptr<Device> InstrumentMono::clone() const
{
    return std::make_unique<InstrumentMono>(*this);
}

void InstrumentMono::process(float sample_duration, float& out_left, float& out_right)
{
    out_left = 0;
    out_right = 0;
    static float temp;

    m_voice->process(sample_duration, temp);
    out_left += m_vol * temp;

    // Pan
    out_right = out_left * m_pan;
    out_left *= 1 - m_pan;
}

void InstrumentMono::note_on(float freq)
{
    if (freq == 0)
    {
        m_voice->note_off();
    }
    else
    {
        m_voice->note_on(freq);
    }
}

void InstrumentMono::note_on()
{
    note_on(m_voice->freq());
}

void InstrumentMono::note_off()
{
    m_voice->note_off();
}

void InstrumentMono::vol(float vol)
{
    m_vol = vol;
}

float InstrumentMono::vol() const
{
    return m_vol;
}

void InstrumentMono::pan(float pan)
{
    m_pan = pan;
}

float InstrumentMono::pan() const
{
    return m_pan;
}

void InstrumentMono::retrigger(bool retrigger)
{
    m_retrigger = retrigger;
}

InstrumentPoly::InstrumentPoly(Voice& voice, unsigned int polyphony, float vol, float pan, bool retrigger)
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

InstrumentPoly::InstrumentPoly(const InstrumentPoly& other)
: m_voices{}
, m_vol{other.m_vol}
, m_pan{other.m_pan}
, m_retrigger{other.m_retrigger}
{
    m_voices.reserve(other.m_voices.size());

    for (const auto& v : other.m_voices)
    {
        m_voices.push_back(v->clone());
    }
}

InstrumentPoly& InstrumentPoly::operator=(const InstrumentPoly& other)
{
    if (this != &other)
    {
        m_voices.clear();
        m_voices.reserve(other.m_voices.size());

        for (const auto& v : other.m_voices)
        {
            m_voices.push_back(v->clone());
        }

        m_vol = other.m_vol;
        m_pan = other.m_pan;
        m_retrigger = other.m_retrigger;
    }
    return *this;
}

std::unique_ptr<Device> InstrumentPoly::clone() const
{
    return std::make_unique<InstrumentPoly>(*this);
}

void InstrumentPoly::process(float sample_duration, float& out_left, float& out_right)
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

void InstrumentPoly::note_on(unsigned int voice_num, float freq)
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

void InstrumentPoly::note_on(unsigned int voice_num)
{
    note_on(voice_num, m_voices[voice_num]->freq());
}

void InstrumentPoly::note_off(unsigned int voice_num)
{
    m_voices[voice_num]->note_off();
}

void InstrumentPoly::vol(float vol)
{
    m_vol = vol;
}

float InstrumentPoly::vol() const
{
    return m_vol;
}

void InstrumentPoly::pan(float pan)
{
    m_pan = pan;
}

float InstrumentPoly::pan() const
{
    return m_pan;
}

void InstrumentPoly::retrigger(bool retrigger)
{
    m_retrigger = retrigger;
}

Voice& InstrumentPoly::voice(size_t index)
{
    return *m_voices[index];
}

}
#include "instrument.hpp"
#include "voice.hpp"

#include <iostream>
#include <memory>


namespace MusicLib {

InstrumentMonoDynamic::InstrumentMonoDynamic(Voice& voice, float vol, float pan, bool retrigger)
: m_voice{voice.clone()}
, m_vol{vol}
, m_pan{pan}
, m_retrigger{retrigger}
{

}

InstrumentMonoDynamic::InstrumentMonoDynamic(const InstrumentMonoDynamic& other)
: m_voice{other.m_voice->clone()}
, m_vol{other.m_vol}
, m_pan{other.m_pan}
, m_retrigger{other.m_retrigger}
{

}

InstrumentMonoDynamic& InstrumentMonoDynamic::operator=(const InstrumentMonoDynamic& other)
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

std::unique_ptr<Device> InstrumentMonoDynamic::clone() const
{
    return std::make_unique<InstrumentMonoDynamic>(*this);
}

void InstrumentMonoDynamic::process(float sample_duration, float& out_left, float& out_right)
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

void InstrumentMonoDynamic::note_on(float freq)
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

void InstrumentMonoDynamic::note_on()
{
    note_on(m_voice->freq());
}

void InstrumentMonoDynamic::note_off()
{
    m_voice->note_off();
}

void InstrumentMonoDynamic::vol(float vol)
{
    m_vol = vol;
}

float InstrumentMonoDynamic::vol() const
{
    return m_vol;
}

void InstrumentMonoDynamic::pan(float pan)
{
    m_pan = pan;
}

float InstrumentMonoDynamic::pan() const
{
    return m_pan;
}

void InstrumentMonoDynamic::retrigger(bool retrigger)
{
    m_retrigger = retrigger;
}

InstrumentPolyDynamic::InstrumentPolyDynamic(Voice& voice, unsigned int polyphony, float vol, float pan, bool retrigger)
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

InstrumentPolyDynamic::InstrumentPolyDynamic(const InstrumentPolyDynamic& other)
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

InstrumentPolyDynamic& InstrumentPolyDynamic::operator=(const InstrumentPolyDynamic& other)
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

std::unique_ptr<Device> InstrumentPolyDynamic::clone() const
{
    return std::make_unique<InstrumentPolyDynamic>(*this);
}

void InstrumentPolyDynamic::process(float sample_duration, float& out_left, float& out_right)
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

void InstrumentPolyDynamic::note_on(unsigned int index, float freq)
{
    if (index >= m_voices.size())
    {
        // No voice with this number.
        return;
    }

    if (freq == 0)
    {
        m_voices[index]->note_off();
    }
    else
    {
        m_voices[index]->note_on(freq);
    }
}

void InstrumentPolyDynamic::note_on(unsigned int index)
{
    note_on(index, m_voices[index]->freq());
}

void InstrumentPolyDynamic::note_off(unsigned int index)
{
    m_voices[index]->note_off();
}

void InstrumentPolyDynamic::vol(float vol)
{
    m_vol = vol;
}

float InstrumentPolyDynamic::vol() const
{
    return m_vol;
}

void InstrumentPolyDynamic::pan(float pan)
{
    m_pan = pan;
}

float InstrumentPolyDynamic::pan() const
{
    return m_pan;
}

void InstrumentPolyDynamic::retrigger(bool retrigger)
{
    m_retrigger = retrigger;
}

Voice& InstrumentPolyDynamic::voice(size_t index)
{
    return *m_voices[index];
}

}
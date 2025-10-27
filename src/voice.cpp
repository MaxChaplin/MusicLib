#include "envelope.hpp"
#include "osc.hpp"
#include "voice.hpp"

#include <memory>

namespace MusicLib {

VoiceMultiDynamic::VoiceMultiDynamic(std::vector<std::unique_ptr<Voice>>& voices, Envelope& env, float freq, float vol)
: m_voices{}
, m_env{env.clone()}
, m_freq{freq}
, m_phase{0}
, m_vol{vol}
{
    for (const auto& v : voices)
    {
        m_voices.push_back(v->clone());
    }
}

VoiceMultiDynamic::VoiceMultiDynamic(const VoiceMultiDynamic& other)
: m_voices{}
, m_env{other.m_env->clone()}
, m_freq{other.m_freq}
, m_phase{other.m_phase}
, m_vol{other.m_vol}
{
    m_voices.reserve(other.m_voices.size());

    for (const auto& v : other.m_voices)
    {
        m_voices.push_back(v->clone());
    }
}

VoiceMultiDynamic& VoiceMultiDynamic::operator=(const VoiceMultiDynamic& other)
{
    if (this != &other)
    {
        m_voices.clear();
        for (const auto& v : other.m_voices)
        {
            m_voices.push_back(v->clone());
        }

        m_env = other.m_env->clone();
        m_freq = other.m_freq;
        m_phase = other.m_phase;
        m_vol = other.m_vol;
    }
    return *this;
}

std::unique_ptr<Voice> VoiceMultiDynamic::clone() const
{
    return std::make_unique<VoiceMultiDynamic>(*this);
}

void VoiceMultiDynamic::env(const Envelope& env)
{
    m_env = env.clone();
}

Envelope& VoiceMultiDynamic::env()
{
    return *m_env;
}

const Envelope& VoiceMultiDynamic::env() const
{
    return *m_env;
}

void VoiceMultiDynamic::freq(float freq)
{
    m_freq = freq;
}

float VoiceMultiDynamic::freq() const
{
    return m_freq;
}

void VoiceMultiDynamic::vol(float vol)
{
    m_vol = vol;
}

void VoiceMultiDynamic::note_on(float freq)
{
    // Reset the phase unless the note was already on (to prevent clicks).
    if (!is_on())
    {
        m_phase = 0;
    }

    m_freq = freq;
    m_env->trig(true);

    for (auto& v : m_voices)
    {
        v->freq(freq);
        v->env().trig(true);
    }
}

void VoiceMultiDynamic::note_off()
{
    m_env->trig(false);

    for (auto& v : m_voices)
    {
        v->env().trig(false);
    }
}

bool VoiceMultiDynamic::is_on() const
{
    return m_env->is_on();
}

void VoiceMultiDynamic::process(float sample_duration, float& output)
{
    output = 0;
    for (auto& v : m_voices)
    {
        float temp;
        v->process(sample_duration, temp);
        output += m_vol * temp;
    }
}

VoiceOscDynamic::VoiceOscDynamic(Oscillator& osc, Envelope& env, float freq, float vol)
: m_osc{osc.clone()}
, m_env{env.clone()}
, m_freq{freq}
, m_phase{0}
, m_vol{vol}
{}

VoiceOscDynamic::VoiceOscDynamic(const VoiceOscDynamic& other)
: m_osc{other.m_osc->clone()}
, m_env{other.m_env->clone()}
, m_freq{other.m_freq}
, m_phase{other.m_phase}
, m_vol{other.m_vol}
{

}

VoiceOscDynamic& VoiceOscDynamic::operator=(const VoiceOscDynamic& other)
{
    if (this != &other)
    {
        m_osc = other.m_osc->clone();
        m_env = other.m_env->clone();
        m_freq = other.m_freq;
        m_phase = other.m_phase;
        m_vol = other.m_vol;
    }
    return *this;
}

std::unique_ptr<Voice> VoiceOscDynamic::clone() const
{
    return std::make_unique<VoiceOscDynamic>(*m_osc, *m_env, m_freq, m_vol);
}

void VoiceOscDynamic::env(const Envelope& env)
{
    m_env = env.clone();
}

Envelope& VoiceOscDynamic::env()
{
    return *m_env;
}

const Envelope& VoiceOscDynamic::env() const
{
    return *m_env;
}

void VoiceOscDynamic::freq(float freq)
{
    m_freq = freq;
}

float VoiceOscDynamic::freq() const
{
    return m_freq;
}

void VoiceOscDynamic::vol(float vol)
{
    m_vol = vol;
}

void VoiceOscDynamic::note_on(float freq)
{
    // Reset the phase unless the note was already on (to prevent clicks).
    if (!is_on())
    {
        m_phase = 0;
    }

    m_freq = freq;
    m_env->trig(true);
}

void VoiceOscDynamic::note_off()
{
    m_env->trig(false);
}

bool VoiceOscDynamic::is_on() const
{
    return m_env->is_on();
}

void VoiceOscDynamic::process(float sample_duration, float& output)
{
    output = m_vol * m_osc->value(m_phase) * m_env->process(sample_duration);

    // Propagate phase.
    m_phase += sample_duration * m_freq;
    if (m_phase >= 1)
    {
        m_phase -= 1;
    }
}

void VoiceOscDynamic::osc(Oscillator& osc)
{
    m_osc = osc.clone();
}

}
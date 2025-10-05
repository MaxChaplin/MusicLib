#include "voice.hpp"

#include <memory>
namespace MusicLib
{
    VoiceOsc::VoiceOsc(Oscillator& osc, Envelope env, float freq, float vol)
    : m_osc{osc.clone()}
    , m_env{env}
    , m_freq{freq}
    , m_phase{0}
    , m_vol{vol}
    {}

    std::shared_ptr<Voice> VoiceOsc::clone() const
    {
        return std::make_shared<VoiceOsc>(*this);
    }

    Envelope& VoiceOsc::env()
    {
        return m_env;
    }

    void VoiceOsc::freq(float freq)
    {
        m_freq = freq;
    }

    void VoiceOsc::vol(float vol)
    {
        m_vol = vol;
    }

    void VoiceOsc::note_on(float freq)
    {
        // Reset the phase unless the note was already on (to prevent clicks).
        if (!is_on())
        {
            m_phase = 0;
        }

        m_freq = freq;
        m_env.trig(true);
    }

    void VoiceOsc::note_off()
    {
        m_env.trig(false);
    }

    bool VoiceOsc::is_on() const
    {
        return m_env.is_on();
    }

    float VoiceOsc::process(float sample_time)
    {
        float sample = m_vol * m_osc->value(m_phase) * m_env.process(sample_time);

        // Propagate phase.
        m_phase += sample_time * m_freq;
        if (m_phase >= 1)
        {
            m_phase -= 1;
        }

        // std::cout << out << std::endl;
        return sample;
    }

    VoiceMulti::VoiceMulti(std::vector<std::shared_ptr<Voice>>& voices, Envelope env, float freq, float vol)
    : m_voices{voices}
    , m_env{env}
    , m_freq{freq}
    , m_phase{0}
    , m_vol{vol}
    {}

    std::shared_ptr<Voice> VoiceMulti::clone() const
    {
        return std::make_shared<VoiceOsc>(*this);
    }

    Envelope& VoiceMulti::env()
    {
        return m_env;
    }

    void VoiceMulti::freq(float freq)
    {
        m_freq = freq;
    }

    void VoiceMulti::vol(float vol)
    {
        m_vol = vol;
    }

    void VoiceMulti::note_on(float freq)
    {
        // Reset the phase unless the note was already on (to prevent clicks).
        if (!is_on())
        {
            m_phase = 0;
        }

        m_freq = freq;
        m_env.trig(true);

        for (size_t i=0; i < m_voices.size(); ++i)
        {
            m_voices[i]->freq(freq);
            m_voices[i]->env().trig(true);
        }
    }

    void VoiceMulti::note_off()
    {
        m_env.trig(false);

        for (size_t i=0; i < m_voices.size(); ++i)
        {
            m_voices[i]->env().trig(false);
        }
    }

    bool VoiceMulti::is_on() const
    {
        return m_env.is_on();
    }

    float VoiceMulti::process(float sample_time)
    {
        float sample = 0;
        for (size_t i=0; i < m_voices.size(); ++i)
        {
            sample += m_vol * m_voices[i]->process(sample_time);
        }

        return sample;
    }

}
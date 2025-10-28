#include "osc.hpp"

#include <memory>
#include <vector>
#include <iostream>

namespace MusicLib {
 
float osc_saw(float phase)
{
    return 2 * phase - 1;
}

float osc_square(float phase)
{
    if (phase < .5)
    {
        return -1;
    }

    return 1;
}

float osc_triangle(float phase)
{
    if (phase < .5)
    {
        return 4 * phase - 1;
    }

    return 3 - 4 * phase;
}

OscillatorBasic::OscillatorBasic(std::function<float(float)> osc_func)
: m_osc_func{osc_func}
{

}

std::unique_ptr<Oscillator> OscillatorBasic::clone() const
{
    return std::make_unique<OscillatorBasic>(*this);
}


float OscillatorBasic::value(float phase) const
{
    return m_osc_func(phase);
}

OscillatorPulse::OscillatorPulse(float pulsewidth)
: m_pulsewidth{pulsewidth}
{

}

std::unique_ptr<Oscillator> OscillatorPulse::clone() const
{
    return std::make_unique<OscillatorPulse>(*this);
}


float OscillatorPulse::value(float phase) const
{
    if (phase < m_pulsewidth)
    {
        return 1;
    }

    return -1;
}

OscillatorWavetable::OscillatorWavetable(std::vector<float> wavetable, bool antialiasing)
: m_wavetable{std::move(wavetable)}
, m_antialiasing{antialiasing}
{

}

std::unique_ptr<Oscillator> OscillatorWavetable::clone() const
{
    return std::make_unique<OscillatorWavetable>(*this);
}

float OscillatorWavetable::value(float phase) const
{
    float phase_rescaled = phase * m_wavetable.size();
    int index = (int)phase_rescaled;

    if (!m_antialiasing)
    {
        return m_wavetable[index];
    }

    // Linear antialiasing
    float res = phase_rescaled - index;

    float p1 = m_wavetable[index];
    float p2 = m_wavetable[(int)(phase_rescaled + 1) % m_wavetable.size()];
    return p1 * (1.0f - res) + p2 * res;
}

}
#include "osc.hpp"

#include <memory>
#include <vector>

namespace MusicLib
{
    float osc_tri(float phase)
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

    std::shared_ptr<Oscillator> OscillatorBasic::clone() const
    {
        return std::make_shared<OscillatorBasic>(*this);
    }


    float OscillatorBasic::value(float phase) const
    {
        return m_osc_func(phase);
    }

    OscillatorWavetable::OscillatorWavetable(std::vector<float>&wavetable, bool antialiasing)
    : m_wavetable{wavetable}
    , m_antialiasing{antialiasing}
    {

    }

    std::shared_ptr<Oscillator> OscillatorWavetable::clone() const
    {
        return std::make_shared<OscillatorWavetable>(*this);
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
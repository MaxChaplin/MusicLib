#include "wave_shaper.hpp"
#include <cmath>

namespace MusicLib {

WaveShaperHardSync::WaveShaperHardSync(float ratio)
: m_ratio{ratio}
{

}

std::shared_ptr<WaveShaper> WaveShaperHardSync::clone() const
{
    return std::make_shared<WaveShaperHardSync>(*this);
}

float WaveShaperHardSync::value(float phase) const
{
    float phase_mult = phase * m_ratio;
    return phase_mult - floor(phase_mult);
}

}
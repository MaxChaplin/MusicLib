#include "instrument.hpp"
#include "instrument_manager.hpp"
#include "voice.hpp"

#include <iostream>
#include <memory>

namespace MusicLib {

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

void InstrumentManager::clone_instrument(Instrument& instrument)
{
    m_instruments.push_back(instrument);
}

void InstrumentManager::process(float sample_duration, float& out_left, float& out_right)
{
    static float temp_left, temp_right;

    out_left = 0;
    out_right = 0;
    for (auto& ins : m_instruments)
    {
        ins.process(sample_duration, temp_left, temp_right);
        out_left += temp_left;
        out_right += temp_right;
    }
}

}
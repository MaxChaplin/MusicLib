#include "instrument.hpp"
#include "instrument_manager.hpp"
#include "voice.hpp"

#include <iostream>
#include <memory>

namespace MusicLib
{
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

    void InstrumentManager::clone_instrument(Instrument instrument)
    {
        m_instruments.push_back(instrument);
    }

    void InstrumentManager::process(float sample_time, float& out_left, float& out_right)
    {
        float temp_left, temp_right;

        out_left = 0;
        out_right = 0;
        // for (auto it = m_instruments.begin(); it != m_instruments.end(); ++it)
        for (auto ins : m_instruments)
        {
            ins.process(sample_time, temp_left, temp_right);
            out_left += temp_left;
            out_right += temp_right;
        }
    }
}
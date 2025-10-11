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

    void InstrumentManager::add_instrument(Instrument& instrument)
    {
        m_instruments.push_back(instrument);
    }

    float InstrumentManager::process(float time_element)
    {
        float sample = 0;
        for (auto it = m_instruments.begin(); it != m_instruments.end(); ++it)
        {
            sample += it->process(time_element);
        }

        return sample;
    }
}
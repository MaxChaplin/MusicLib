#ifndef INSTRUMENT_MANAGER_H_
#define INSTRUMENT_MANAGER_H_

#include "instrument.hpp"

#include <vector>
#include <memory>

namespace MusicLib
{
    class InstrumentManager
    {
    public:
        explicit InstrumentManager(size_t buffer_size);
        ~InstrumentManager() noexcept = default;

        Instrument& instrument(size_t num);

        void add_instrument(Instrument& instrument);
        float process(float time_element);

    private:
        std::vector<Instrument> m_instruments;
        std::vector<float> m_buffer;
        size_t m_buffer_cursor;
    };
}

#endif // INSTRUMENT_MANAGER_H_
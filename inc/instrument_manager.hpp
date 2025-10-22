#ifndef INSTRUMENT_MANAGER_H_
#define INSTRUMENT_MANAGER_H_

#include "instrument.hpp"

#include <vector>
#include <memory>

namespace MusicLib {
class InstrumentManager
{
public:
    explicit InstrumentManager(size_t buffer_size);
    ~InstrumentManager() noexcept = default;

    Instrument& instrument(size_t num);

    void clone_instrument(Instrument& instrument);
    void process(float sample_duration, float& out_left, float& out_right);

private:
    std::vector<Instrument> m_instruments;
    std::vector<float> m_buffer;
    size_t m_buffer_cursor;
};

}

#endif // INSTRUMENT_MANAGER_H_
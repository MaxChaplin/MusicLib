#ifndef INSTRUMENT_MANAGER_H_
#define INSTRUMENT_MANAGER_H_

#include "device.hpp"
#include "util.hpp"

#include <vector>
#include <memory>

namespace MusicLib {

/**
 * @brief A simple device manager that holds only instruments of a given type.
 * 
 * @tparam I an implementation of the Instrument interface
 */
template <typename I = Instrument>
class InstrumentManager : public DeviceOut
{
public:
    explicit InstrumentManager(size_t buffer_size)
    : m_instruments{}
    , m_buffer(buffer_size, 0)
    , m_buffer_cursor{}
    {}

    ~InstrumentManager() noexcept = default;
    
    InstrumentManager(const InstrumentManager& other)
    : m_instruments{}
    , m_buffer{other.m_buffer}
    , m_buffer_cursor{other.m_buffer_cursor}    
    {
        m_instruments.reserve(other.m_instruments.size());

        for (const auto& ins : other.m_instruments)
        {
            m_instruments.push_back(Util::clone<I>(*ins));
        }
    }

    InstrumentManager& operator=(const InstrumentManager& other)
    {
        if (this != &other)
        {
            m_instruments.clear();
            for (const auto& ins : other.m_instruments)
            {
                m_instruments.push_back(Util::clone<I>(*ins));
            }
            m_buffer = other.m_buffer;
            m_buffer_cursor = other.m_buffer_cursor;
        }
        return *this;
    }

    InstrumentManager(InstrumentManager&&) noexcept = default;
    InstrumentManager& operator=(InstrumentManager&&) noexcept = default;

    /**
     * @brief Return a reference to the numbered instrument. If a template
     * parameter is given, the instrument is casted to it.
     * 
     * @tparam I2 return Instrument type
     * @param index 
     * @return I2& 
     */
    template <typename I2 = I>
    I2& instrument(unsigned int index) const
    {
        return static_cast<I2&>(*m_instruments[index]); 
    }

    std::unique_ptr<Device> clone() const
    {
        return std::make_unique<InstrumentManager<I>>(*this);
    }

    void clone_instrument(I& instrument)
    {
        m_instruments.push_back(Util::clone<I>(instrument));
    }


    void process(float sample_duration, float& out_left, float& out_right) override
    {
        static float temp_left, temp_right;

        out_left = 0;
        out_right = 0;
        for (auto& ins : m_instruments)
        {
            ins->process(sample_duration, temp_left, temp_right);
            out_left += temp_left;
            out_right += temp_right;
        }
    }

private:
    std::vector<std::unique_ptr<I>> m_instruments;
    std::vector<float> m_buffer;
    size_t m_buffer_cursor;
};

}

#endif // INSTRUMENT_MANAGER_H_
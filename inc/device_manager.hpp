#ifndef DEVICE_MANAGER_H_
#define DEVICE_MANAGER_H_

#include "device.hpp"

#include <vector>
#include <memory>

namespace MusicLib {

class DeviceManager
{
public:
    DeviceManager() = default;
    virtual ~DeviceManager() = default;
};

/**
 * @brief A device manager that has one stereo output.
 * 
 */
class DeviceManagerOut : public DeviceManager
{
public:
    DeviceManagerOut() = default;
    virtual ~DeviceManagerOut() = default;

    virtual void process(float sample_duration, float& out_left, float& out_right) = 0;
};

/**
 * @brief A device manager that has one stereo input and one stereo output.
 * 
 */
class DeviceManagerInOut : public DeviceManager
{
public:
    DeviceManagerInOut() = default;
    virtual ~DeviceManagerInOut() = default;

    virtual void process(float sample_duration, float& in_left, float& in_right, float& out_left, float& out_right) = 0;
};

/**
 * @brief A simple device manager that holds only instruments.
 * 
 */
class InstrumentManager : public DeviceManagerOut
{
public:
    explicit InstrumentManager(size_t buffer_size);
    ~InstrumentManager() noexcept = default;

    Instrument& instrument(unsigned int index) const;

    void clone_instrument(Instrument& instrument);
    void process(float sample_duration, float& out_left, float& out_right) override;

private:
    std::vector<std::unique_ptr<Instrument>> m_instruments;
    std::vector<float> m_buffer;
    size_t m_buffer_cursor;
};

}

#endif // DEVICE_MANAGER_H_
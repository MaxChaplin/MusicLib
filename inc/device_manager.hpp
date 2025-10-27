#ifndef DEVICE_MANAGER_H_
#define DEVICE_MANAGER_H_

#include "device.hpp"
#include "instrument.hpp"

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

}

#endif // DEVICE_MANAGER_H_
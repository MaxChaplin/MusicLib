#ifndef DEVICE_H_
#define DEVICE_H_

#include "envelope.hpp"
#include "voice.hpp"

#include <vector>
#include <memory>

namespace MusicLib {

class Device
{
public:
    Device() = default;
    virtual ~Device() = default;

    virtual std::unique_ptr<Device> clone() const = 0;
};

/**
 * @brief A device with one stereo output.
 * 
 */
class DeviceOut : public Device
{
public:
    DeviceOut() = default;
    virtual ~DeviceOut() = default;

    virtual std::unique_ptr<Device> clone() const = 0;

    virtual void process(float sample_duration, float& out_left, float& out_right) = 0;
};


/**
 * @brief A device that has one stereo input and one stereo output.
 * 
 */
class DeviceInOut : public Device
{
public:
    DeviceInOut() = default;
    virtual ~DeviceInOut() = default;

    virtual void process(float sample_duration, float& in_left, float& in_right, float& out_left, float& out_right) = 0;
};

}

#endif // DEVICE_H_
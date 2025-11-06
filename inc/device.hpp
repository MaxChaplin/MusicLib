#ifndef DEVICE_H_
#define DEVICE_H_

#include "envelope.hpp"
#include "voice.hpp"

#include <memory>
#include <type_traits>
#include <vector>

namespace MusicLib {

class IDevice
{
public:
    virtual ~IDevice() = default;
};

class InputNone
{
public:
    InputNone() = default;
    virtual ~InputNone() = default;
};
    
class InputMono
{
public:
    InputMono() = default;
    virtual ~InputMono() = default;
};

class InputStereo
{
public:
    InputStereo() = default;
    virtual ~InputStereo() = default;

    virtual void pan(float pan) = 0;
    virtual float pan() const = 0;

};

class OutputMono
{
public:
    OutputMono() = default;
    virtual ~OutputMono() = default;
};

class OutputStereo
{
public:
    OutputStereo() = default;
    virtual ~OutputStereo() = default;

    virtual void pan(float pan) = 0;
    virtual float pan() const = 0;
};

template <typename I = InputNone, typename O = OutputStereo>
class Device : public IDevice, public I, public O
{
public:
    Device() = default;
    virtual ~Device() = default;

    virtual std::unique_ptr<Device> clone() const = 0;

    virtual void vol(float vol) = 0;
    virtual float vol() const = 0;

};

template<>
class Device<InputNone, OutputMono> : public IDevice, public InputNone, public OutputMono
{
public:
    Device() = default;
    virtual ~Device() = default;

    virtual std::unique_ptr<Device> clone() const = 0;

    virtual void vol(float vol) = 0;
    virtual float vol() const = 0;

    virtual void process(float sample_duration, float& out) = 0;
};

template<>
class Device<InputNone, OutputStereo> : public IDevice, public InputNone, public OutputStereo
{
public:
    Device() = default;
    virtual ~Device() = default;

    virtual std::unique_ptr<Device> clone() const = 0;

    virtual void vol(float vol) = 0;
    virtual float vol() const = 0;

    virtual void pan(float pan) = 0;
    virtual float pan() const = 0;

    virtual void process(float sample_duration, float& out_left, float& out_right) = 0;
};

template<>
class Device<InputMono, OutputMono> : public IDevice, public InputMono, public OutputMono
{
public:
    Device() = default;
    virtual ~Device() = default;

    virtual std::unique_ptr<Device> clone() const = 0;

    virtual void vol(float vol) = 0;
    virtual float vol() const = 0;

    virtual void process(float sample_duration, float in, float& out) = 0;
};

template<>
class Device<InputStereo, OutputStereo> : public IDevice, public InputStereo, public OutputStereo
{
public:
    Device() = default;
    virtual ~Device() = default;

    virtual std::unique_ptr<Device> clone() const = 0;

    virtual void vol(float vol) = 0;
    virtual float vol() const = 0;

    virtual void pan(float pan) = 0;
    virtual float pan() const = 0;

    virtual void process(float sample_duration, float in_left, float in_right, float& out_left, float& out_right) = 0;
};

}

#endif // DEVICE_H_
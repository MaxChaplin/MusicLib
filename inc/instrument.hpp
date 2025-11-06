#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "device.hpp"
#include "util.hpp"

#include <memory>
#include <vector>

namespace MusicLib {

class IInstrument
{
public:
    virtual ~IInstrument() = default;
};

template <typename V = Voice, typename O = OutputStereo>
class Instrument : public IInstrument, public Device<InputNone, O>
{
public:
    ~Instrument() = default;
};

template <typename V>
class Instrument<V, OutputMono> : public Device<InputNone, OutputMono>
{
public:
    explicit Instrument(V& voice, float vol = 1)
    : m_voice{Util::clone<V>(voice)}
    , m_vol{vol}
    {
    }

    ~Instrument() noexcept = default;

    Instrument(const Instrument& other)
    : m_voice{Util::clone<V>(other.m_voice)}
    , m_vol{other.m_vol}
    {

    }

    Instrument& operator=(const Instrument& other)
    {
        if (this != &other)
        {
            m_voice = Util::clone<V>(other.m_voice);
            m_vol = other.m_vol;
        }
        return *this;
    }

    Instrument(Instrument&&) noexcept = default;
    Instrument& operator=(Instrument&&) noexcept = default;

    std::unique_ptr<Device> clone() const override
    {
        return std::make_unique<Instrument<V, OutputMono>>(*this);
    }
   
    void note_on(unsigned int index [[maybe_unused]], float freq)
    {
        note_on(freq);
    }

    void note_on(float freq)
    {
        if (freq == 0)
        {
            m_voice->note_off();
        }
        else
        {
            m_voice->note_on(freq);
        }
    }

    void note_on(unsigned int index [[maybe_unused]])
    {
        note_on(m_voice->freq());
    }

    void note_on()
    {
        note_on(m_voice->freq());
    }

    void note_off(unsigned int index [[maybe_unused]])
    {
        m_voice->note_off();
    }

    void note_off()
    {
        m_voice->note_off();
    }

    void vol(float vol) override
    {
        m_vol = vol;
    }

    float vol() const override
    {
        return m_vol;
    }

    template <typename V2 = V>
    V2& voice()
    {
        return static_cast<V2&>(*m_voice);
    }

    template <typename V2 = V>
    V2& voice(unsigned int index)
    {
        return static_cast<V2&>(*m_voice);
    }

    template <typename V2 = V>
    const V2& voice() const
    {
        return static_cast<const V2&>(*m_voice);
    }

    template <typename V2 = V>
    const V2& voice(unsigned int index) const
    {
        return static_cast<const V2&>(*m_voice[index]);
    }

    void process(float sample_duration, float& out) 
    {
        float temp = 0;
        m_voice.process(sample_duration, temp);
        out = temp * m_vol;
    }

private:
    std::unique_ptr<V> m_voice;
    float m_vol;
};

template <typename V>
class Instrument<V, OutputStereo> : public Device<InputNone, OutputStereo>
{
public:
   explicit Instrument(V& voice, float vol = 1, float pan = .5)
    : m_voice{Util::clone<V>(voice)}
    , m_vol{vol}
    , m_pan{pan}
    {
    }

    ~Instrument() noexcept = default;

    Instrument(const Instrument& other)
    : m_voice{Util::clone<V>(*other.m_voice)}
    , m_vol{other.m_vol}
    , m_pan{other.m_pan}
    {

    }

    Instrument& operator=(const Instrument& other)
    {
        if (this != &other)
        {
            m_voice = Util::clone<V>(*other.m_voice);
            m_vol = other.m_vol;
            m_pan = other.m_pan;
        }
        return *this;
    }

    Instrument(Instrument&&) noexcept = default;
    Instrument& operator=(Instrument&&) noexcept = default;

    std::unique_ptr<Device> clone() const override
    {
        return std::make_unique<Instrument<V, OutputStereo>>(*this);
    }
   
    void note_on(unsigned int index [[maybe_unused]], float freq)
    {
        note_on(freq);
    }

    void note_on(float freq)
    {
        if (freq == 0)
        {
            m_voice->note_off();
        }
        else
        {
            m_voice->note_on(freq);
        }
    }

    void note_on(unsigned int index [[maybe_unused]])
    {
        note_on(m_voice->freq());
    }

    void note_on()
    {
        note_on(m_voice->freq());
    }

    void note_off(unsigned int index [[maybe_unused]])
    {
        m_voice->note_off();
    }

    void note_off()
    {
        m_voice->note_off();
    }

    void vol(float vol) override
    {
        m_vol = vol;
    }

    float vol() const override
    {
        return m_vol;
    }

    void pan(float pan) override
    {
        m_pan = pan;
    }

    float pan() const override
    {
        return m_pan;
    }

    template <typename V2 = V>
    V2& voice()
    {
        return static_cast<V2&>(*m_voice);
    }

    template <typename V2 = V>
    V2& voice(unsigned int index [[maybe_unused]])
    {
        return static_cast<V2&>(*m_voice);
    }

    template <typename V2 = V>
    const V2& voice() const
    {
        return static_cast<const V2&>(*m_voice);
    }

    template <typename V2 = V>
    const V2& voice(unsigned int index [[maybe_unused]]) const
    {
        return static_cast<const V2&>(*m_voice);
    }

    void process(float sample_duration, float& out_left, float& out_right) override
    {
        out_left = 0;
        out_right = 0;
        static float temp;
        m_voice->process(sample_duration, temp);
        out_left += m_vol * temp;

        // Pan
        out_right = out_left * m_pan;
        out_left *= 1 - m_pan;
    }

private:
    std::unique_ptr<V> m_voice;
    float m_vol;
    float m_pan;
};

}

#endif // INSTRUMENT_H_
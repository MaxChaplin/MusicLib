#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "device.hpp"
#include "util.hpp"

#include <memory>
#include <vector>

namespace MusicLib {

class Instrument : public DeviceOut
{
public:
    Instrument() = default;
    virtual ~Instrument() = default;

    virtual std::unique_ptr<Device> clone() const = 0;

    virtual void process(float sample_duration, float& out_left, float& out_right) = 0;

    virtual void vol(float vol) = 0;
    virtual float vol() const = 0;

    virtual void pan(float pan) = 0;
    virtual float pan() const = 0;
};

/**
 * @brief  A basic instrument with one voice.
 * 
 * @tparam V Voice type
 */
template <typename V = Voice>
class InstrumentMono : public Instrument
{
public:
    explicit InstrumentMono(V& voice, float vol = 1, float pan = .5)
    : m_voice{Util::clone<V>(voice)}
    , m_vol{vol}
    , m_pan{pan}
    {}

    ~InstrumentMono() noexcept = default;

    InstrumentMono(const InstrumentMono& other)
    : m_voice{Util::clone<V>(*other.m_voice)}
    , m_vol{other.m_vol}
    , m_pan{other.m_pan}
    {

    }

    InstrumentMono& operator=(const InstrumentMono& other)
    {
        if (this != &other)
        {
            m_voice = Util::clone<V>(*other.m_voice);
            m_vol = other.m_vol;
            m_pan = other.m_pan;
        }
        return *this;
    }

    InstrumentMono(InstrumentMono&&) noexcept = default;
    InstrumentMono& operator=(InstrumentMono&&) noexcept = default;

    std::unique_ptr<Device> clone() const override
    {
        return std::make_unique<InstrumentMono<V>>(*this);
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

    void note_on()
    {
        note_on(m_voice->freq());
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
    const V2& voice() const
    {
        return static_cast<const V2&>(*m_voice);
    }

private:
    std::unique_ptr<V> m_voice;
    float m_vol;
    float m_pan;
};

/**
 * @brief A basic instrument with multiple voices, all copies of each other.
 * 
 * @tparam V Voice type
 */
template <typename V = Voice>
class InstrumentPoly : public Instrument
{
public:
    explicit InstrumentPoly(Voice& voice, unsigned int polyphony = 4,
        float vol = 1, float pan = .5)
    : m_voices{}
    , m_vol{vol}
    , m_pan{pan}
    {
        m_voices.reserve(polyphony);

        for (size_t i = 0; i < polyphony; ++i)
        {
            m_voices.push_back(Util::clone<V>(voice));
        }
    }

    ~InstrumentPoly() noexcept = default;

    InstrumentPoly(const InstrumentPoly& other)
    : m_voices{}
    , m_vol{other.m_vol}
    , m_pan{other.m_pan}
    {
        m_voices.reserve(other.m_voices.size());

        for (const auto& v : other.m_voices)
        {
            m_voices.push_back(Util::clone<V>(*v));
        }
    }

    InstrumentPoly& operator=(const InstrumentPoly& other)
    {
        if (this != &other)
        {
            m_voices.clear();
            for (const auto& v : other.m_voices)
            {
                m_voices.push_back(Util::clone<V>(*v));
            }

            m_vol = other.m_vol;
            m_pan = other.m_pan;
        }
        return *this;
    }

    InstrumentPoly(InstrumentPoly&&) noexcept = default;
    InstrumentPoly& operator=(InstrumentPoly&&) noexcept = default;

    std::unique_ptr<Device> clone() const override
    {
        return std::make_unique<InstrumentPoly<V>>(*this);
    }

    void process(float sample_duration, float& out_left, float& out_right) override
    {
        out_left = 0;
        out_right = 0;
        static float temp;

        for (auto& v : m_voices)
        {
            if (v.is_on())
            {
                
                v.process(sample_duration, temp);
                out_left += m_vol * temp;
            }
        }

        // Pan
        out_right = out_left * m_pan;
        out_left *= 1 - m_pan;
    }

    void note_on(unsigned int index, float freq)
    {
        if (index >= m_voices.size())
        {
            // No voice with this number.
            return;
        }

        if (freq == 0)
        {
            m_voices[index]->note_off();
        }
        else
        {
            m_voices[index]->note_on(freq);
        }
    }

    void note_on(unsigned int index)
    {
        note_on(m_voices[index]->freq());
    }

    void note_off(unsigned int index)
    {
        m_voices[index]->note_off();
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
    V2& voice(size_t index)
    {
        return static_cast<V2&>(*m_voices[index]);
    }

    template <typename V2 = V>
    const V2& voice(size_t index) const
    {
        return static_cast<const V2&>(*m_voices[index]);
    }

    /**
     * @brief Call a given method of each of the voices.
     * 
     * @tparam M 
     * @tparam Args 
     * @param method pointer to a method that V has (example: &VoiceOsc::freq)
     * @param args arguments for the method, if there are any
     */
    template <typename M, typename... Args>
    void call_all_voices(M method, Args&&... args)
    {
        for (auto& v : m_voices)
        {
            (v.*method)(std::forward<Args>(args)...);
        }
    }

private:
    std::vector<std::unique_ptr<V>> m_voices;
    float m_vol;
    float m_pan;
};

}

#endif // INSTRUMENT_H_
#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "device.hpp"

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

    virtual void retrigger(bool retrigger) = 0;
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
    explicit InstrumentMono(V& voice, float vol = 1, float pan = .5, bool retrigger = false)
    : m_voice{voice}
    , m_vol{vol}
    , m_pan{pan}
    , m_retrigger{retrigger}
    {}

    ~InstrumentMono() noexcept = default;

    std::unique_ptr<Device> clone() const override
    {
        return std::make_unique<InstrumentMono<V>>(*this);
    }

    void process(float sample_duration, float& out_left, float& out_right) override
    {
        out_left = 0;
        out_right = 0;
        static float temp;
        m_voice.process(sample_duration, temp);
        out_left += m_vol * temp;

        // Pan
        out_right = out_left * m_pan;
        out_left *= 1 - m_pan;
    }

    void note_on(float freq)
    {
        if (freq == 0)
        {
            m_voice.note_off();
        }
        else
        {
            m_voice.note_on(freq);
        }
    }

    void note_on()
    {
        note_on(m_voice.freq());
    }

    void note_off()
    {
        m_voice.note_off();
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

    void retrigger(bool retrigger) override
    {
        m_retrigger = retrigger;
    }

    V& voice()
    {
        return m_voice;
    }

    const V& voice() const
    {
        return m_voice;
    }

private:
    V m_voice;
    float m_vol;
    float m_pan;
    bool m_retrigger;
};

/**
 * @brief A basic instrument with one voice. The voice can be set to any 
 * descendant of the Voice class.
 * 
 */
class InstrumentMonoDynamic : public Instrument
{
public:
    explicit InstrumentMonoDynamic(Voice& voice, float vol = 1, float pan = .5, bool retrigger = false);
    ~InstrumentMonoDynamic() noexcept = default;
    InstrumentMonoDynamic(const InstrumentMonoDynamic& other);
    InstrumentMonoDynamic& operator=(const InstrumentMonoDynamic& other);
    InstrumentMonoDynamic(InstrumentMonoDynamic&&) noexcept = default;
    InstrumentMonoDynamic& operator=(InstrumentMonoDynamic&&) noexcept = default;

    std::unique_ptr<Device> clone() const override;

    void process(float sample_duration, float& out_left, float& out_right) override;

    void note_on(float freq);
    void note_on();
    void note_off();

    void vol(float vol) override;
    float vol() const override;

    void pan(float freq) override;
    float pan() const override;

    void retrigger(bool retrigger) override;

    template <typename V = Voice>
    V& voice()
    {
        return static_cast<V&>(*m_voice);
    }

private:
    std::unique_ptr<Voice> m_voice;
    float m_vol;
    float m_pan;
    bool m_retrigger;
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
        float vol = 1, float pan = .5, bool retrigger = false)
    : m_voices{}
    , m_vol{vol}
    , m_pan{pan}
    , m_retrigger{retrigger}
    {
        m_voices.reserve(polyphony);

        for (size_t i = 0; i < polyphony; ++i)
        {
            m_voices.push_back(voice);
        }
    }

    ~InstrumentPoly() noexcept = default;

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
            m_voices[index].note_off();
        }
        else
        {
            m_voices[index].note_on(freq);
        }
    }

    void note_on(unsigned int index)
    {
        note_on(m_voices[index].freq());
    }

    void note_off(unsigned int index)
    {
        m_voices[index].note_off();
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

    void retrigger(bool retrigger) override
    {
        m_retrigger = retrigger;
    }

    V& voice(size_t index)
    {
        return m_voices[index];
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
    std::vector<V> m_voices;
    float m_vol;
    float m_pan;
    bool m_retrigger;
};

/**
 * @brief A basic instrument with multiple voices, all copies of each other.
 * The voices can be of any descendant of the Voice class.
 * 
 */
class InstrumentPolyDynamic : public Instrument
{
public:
    explicit InstrumentPolyDynamic(Voice& voice, unsigned int polyphony = 4,
        float vol = 1, float pan = .5, bool retrigger = false);
    ~InstrumentPolyDynamic() noexcept = default;
    InstrumentPolyDynamic(const InstrumentPolyDynamic& other);
    InstrumentPolyDynamic& operator=(const InstrumentPolyDynamic& other);
    InstrumentPolyDynamic(InstrumentPolyDynamic&&) noexcept = default;
    InstrumentPolyDynamic& operator=(InstrumentPolyDynamic&&) noexcept = default;

    std::unique_ptr<Device> clone() const override;

    void process(float sample_duration, float& out_left, float& out_right) override;

    void note_on(unsigned int index, float freq);
    void note_on(unsigned int index);
    void note_off(unsigned int index);

    void vol(float vol);
    float vol() const;

    void pan(float freq);
    float pan() const;

    void retrigger(bool retrigger);

    Voice& voice(size_t index);

    template <typename V>
    V& voice(size_t index)
    {
        return static_cast<V&>(*m_voices[index]);
    }

    /**
     * @brief Call a given method of each of the voices.
     * 
     * @tparam V concrete derived class of Voice
     * @tparam M 
     * @tparam Args 
     * @param method pointer to a method that V has (example: &VoiceOsc::freq)
     * @param args arguments for the method, if there are any
     */
    template <typename V, typename M, typename... Args>
    void call_all_voices(M method, Args&&... args)
    {
        for (auto& v : m_voices)
        {
            (static_cast<V*>(v.get())->*method)(std::forward<Args>(args)...);
        }
    }

private:
    std::vector<std::unique_ptr<Voice>> m_voices;
    float m_vol;
    float m_pan;
    bool m_retrigger;
};

}

#endif // INSTRUMENT_H_
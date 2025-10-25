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

    virtual void pan(float freq) = 0;
    virtual float pan() const = 0;

    virtual void retrigger(bool retrigger) = 0;
};

/**
 * @brief A basic instrument with one voice.
 * 
 */
class InstrumentMono : public Instrument
{
public:
    explicit InstrumentMono(Voice& voice, float vol = 1, float pan = .5, bool retrigger = false);
    ~InstrumentMono() noexcept = default;
    InstrumentMono(const InstrumentMono& other);
    InstrumentMono& operator=(const InstrumentMono& other);
    InstrumentMono(InstrumentMono&&) noexcept = default;
    InstrumentMono& operator=(InstrumentMono&&) noexcept = default;

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
 */
class InstrumentPoly : public Instrument
{
public:
    explicit InstrumentPoly(Voice& voice, unsigned int polyphony = 4,
        float vol = 1, float pan = .5, bool retrigger = false);
    ~InstrumentPoly() noexcept = default;
    InstrumentPoly(const InstrumentPoly& other);
    InstrumentPoly& operator=(const InstrumentPoly& other);
    InstrumentPoly(InstrumentPoly&&) noexcept = default;
    InstrumentPoly& operator=(InstrumentPoly&&) noexcept = default;

    std::unique_ptr<Device> clone() const override;

    void process(float sample_duration, float& out_left, float& out_right) override;

    void note_on(unsigned int voice_num, float freq);
    void note_on(unsigned int voice_num);
    void note_off(unsigned int voice_num);

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
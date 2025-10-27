#ifndef VOICE_H_
#define VOICE_H_

#include "envelope.hpp"
#include "osc.hpp"
#include "wave_shaper.hpp"

#include <functional>
#include <memory>
#include <type_traits>
#include <stdexcept>

namespace MusicLib {
    
/**
 * @brief An interface for the primary component of an instrument, where the
 * synthesis is implemented. Typically monophonic, with polyphony
 * implemented by having the instrument hold multiple objects.
 */
class Voice
{
public:
    Voice() = default;
    virtual ~Voice() = default;

    virtual std::unique_ptr<Voice> clone() const = 0;

    virtual void env(const Envelope& env) = 0;
    virtual Envelope& env() = 0;
    virtual const Envelope& env() const = 0;

    virtual void freq(float m_freq) = 0;
    virtual float freq() const = 0;

    virtual void vol(float m_vol) = 0;

    virtual void note_on(float freq) = 0;
    virtual void note_off() = 0;

    virtual bool is_on() const = 0;

    /**
     * @brief Progress the voice's audio signal a single sample.
     * 
     * @param sample_duration The length of a sample in seconds (the inverse 
     * of the sample rate).
     * @param output 
     */
    virtual void process(float sample_duration, float& output) = 0;
};

/**
 * @brief An adapter class for playing multiple voices in unison.
 * 
 * @tparam V interior voice type
 * @tparam E envelope type
 */
template <typename V = Voice, typename E = Envelope>
class VoiceMulti : public Voice
{
public:
    explicit VoiceMulti(std::vector<std::unique_ptr<V>>& voices,
                        E& env, float freq = 440, float vol = 1.)
    : m_voices{}
    , m_env{env}
    , m_freq{freq}
    , m_phase{0}
    , m_vol{vol}
    {
        for (const auto& v : voices)
        {
            m_voices.push_back(v.clone());
        }
    }

    ~VoiceMulti() noexcept = default;
    VoiceMulti(const VoiceMulti& other)
    : m_voices{}
    , m_env{other.m_env}
    , m_freq{other.m_freq}
    , m_phase{other.m_phase}
    , m_vol{other.m_vol}
    {
        m_voices.reserve(other.m_voices.size());

        for (const auto& v : other.m_voices)
        {
            m_voices.push_back(v->clone());
        }
    }

    VoiceMulti& operator=(const VoiceMulti& other)
    {
        if (this != &other)
        {
            m_voices.clear();
            for (const auto& v : other.m_voices)
            {
                m_voices.push_back(v->clone());
            }

            m_env = other.m_env;
            m_freq = other.m_freq;
            m_phase = other.m_phase;
            m_vol = other.m_vol;
        }
        return *this;
    }


    VoiceMulti(VoiceMulti&&) noexcept = default;
    VoiceMulti& operator=(VoiceMulti&&) noexcept = default;

    std::unique_ptr<Voice> clone() const override
    {
        return std::make_unique<VoiceMulti>(*this);
    }

    void env(const E& env) override
    {
        m_env = env;
    }

    E& env() override
    {
        return m_env;
    }

    const E& env() const override
    {
        return m_env;
    }

    Voice& voice(size_t index);
    
    template <typename V2>
    V2& voice(size_t index)
    {
        return static_cast<V2&>(m_voices[index]);
    }

    void freq(float freq) override
    {
        m_freq = freq;
    }

    float freq() const override
    {
        return m_freq;
    }

    void vol(float vol) override
    {
        m_vol = vol;
    }

    void note_on(float freq) override
    {
        // Reset the phase unless the note was already on (to prevent clicks).
        if (!is_on())
        {
            m_phase = 0;
        }

        m_freq = freq;
        m_env.trig(true);

        for (auto& v : m_voices)
        {
            v->freq(freq);
            v->env().trig(true);
        }
    }

    void note_off() override
    {
        m_env.trig(false);

        for (auto& v : m_voices)
        {
            v->env().trig(false);
        }
    }

    bool is_on() const override
    {
        return m_env.is_on();
    }

    void process(float sample_duration, float& output) override
    {
        output = 0;
        for (auto& v : m_voices)
        {
            float temp;
            v->process(sample_duration, temp);
            output += m_vol * temp;
        }
    }

private:
    std::vector<std::unique_ptr<Voice>> m_voices;
    E m_env;
    float m_freq;
    float m_phase;
    float m_vol;
};

class VoiceMultiDynamic : public Voice
{
public:
    explicit VoiceMultiDynamic(std::vector<std::unique_ptr<Voice>>& voices,
                        Envelope& env, float freq = 440, float vol = 1.);
    ~VoiceMultiDynamic() noexcept = default;
    VoiceMultiDynamic(const VoiceMultiDynamic& other);
    VoiceMultiDynamic& operator=(const VoiceMultiDynamic& other);
    VoiceMultiDynamic(VoiceMultiDynamic&&) noexcept = default;
    VoiceMultiDynamic& operator=(VoiceMultiDynamic&&) noexcept = default;

    std::unique_ptr<Voice> clone() const override;

    void env(const Envelope& env) override;
    Envelope& env() override;
    const Envelope& env() const override;

    template <typename E>
    E& env(size_t index)
    {
        return static_cast<E&>(*m_env);
    }

    Voice& voice(size_t index);
    
    template <typename V>
    V& voice(size_t index)
    {
        return static_cast<V&>(*m_voices[index]);
    }

    void freq(float m_freq) override;
    float freq() const override;

    void vol(float m_vol) override;

    void note_on(float freq) override;
    void note_off() override;

    bool is_on() const override;

    void process(float sample_duration, float& output) override;

private:
    std::vector<std::unique_ptr<Voice>> m_voices;
    std::unique_ptr<Envelope> m_env;
    float m_freq;
    float m_phase;
    float m_vol;
};

/**
 * @brief A voice based around an oscillator object, with a single velocity
 * envelope.
 * 
 * @tparam O Oscillator type 
 * @tparam E Envelopy type
 */
template <typename O = Oscillator, typename E = Envelope>
class VoiceOsc : public Voice
{
public:
    explicit VoiceOsc(O& osc, E& env, float freq = 440,
                        float vol = 1.)
    : m_osc{osc}
    , m_env{env}
    , m_freq{freq}
    , m_phase{0}
    , m_vol{vol}
    {
        static_assert(std::is_base_of_v<Oscillator, O>, "class O must be derived from Oscillator");
        static_assert(std::is_base_of_v<Envelope, E>, "class E must be derived from Envelope");
    }

    ~VoiceOsc() noexcept = default;

    std::unique_ptr<Voice> clone() const override
    {
        return std::make_unique<VoiceOsc>(*this);
    }

    void env(const Envelope& env) override
    {
        if (typeid(env) == typeid(E))
        {
            m_env = static_cast<const E&>(env);
        }
        else
        {
            throw std::invalid_argument("invalid envelope type");
        }
        
    }

    E& env() override
    {
        return m_env;
    }

    const E& env() const override
    {
        return m_env;
    }


    void freq(float freq) override
    {
        m_freq = freq;
    }

    float freq() const override
    {
        return m_freq;
    }

    void vol(float vol) override
    {
        m_vol = vol;
    }

    void note_on(float freq) override
    {
        // Reset the phase unless the note was already on (to prevent clicks).
        if (!is_on())
        {
            m_phase = 0;
        }

        m_freq = freq;
        m_env.trig(true);
    }

    void note_off() override
    {
        m_env.trig(false);
    }

    bool is_on() const override
    {
        return m_env.is_on();
    }

    void process(float sample_duration, float& output) override
    {
        output = m_vol * m_osc.value(m_phase) * m_env.process(sample_duration);

        // Propagate phase.
        m_phase += sample_duration * m_freq;
        if (m_phase >= 1)
        {
            m_phase -= 1;
        }
    }

    void osc(Oscillator& osc)
    {
        m_osc = osc;
    }

    O& osc()
    {
        return m_osc;
    }

    const O& osc() const
    {
        return m_osc;
    }

private:
    O m_osc;
    E m_env;
    float m_freq;
    float m_phase;
    float m_vol;
};

/**
 * @brief A voice based around an oscillator object, with a single velocity
 * envelope. The oscillator and envelope can be set to descendant of the
 * Oscillator and Envelope classes respectively.
 * 
 */
class VoiceOscDynamic : public Voice
{
public:
    explicit VoiceOscDynamic(Oscillator& m_osc, Envelope& env, float freq = 440, float vol = 1.);
    ~VoiceOscDynamic() noexcept = default;
    VoiceOscDynamic(const VoiceOscDynamic& other);
    VoiceOscDynamic& operator=(const VoiceOscDynamic& other);
    VoiceOscDynamic(VoiceOscDynamic&&) noexcept = default;
    VoiceOscDynamic& operator=(VoiceOscDynamic&&) noexcept = default;

    std::unique_ptr<Voice> clone() const override;

    void env(const Envelope& env) override;
    Envelope& env() override;
    const Envelope& env() const override;

    template <typename E>
    E& env()
    {
        return static_cast<E&>(*m_env);
    }

    void freq(float freq) override;
    float freq() const override;

    void vol(float vol) override;

    void note_on(float freq) override;
    void note_off() override;

    bool is_on() const override;

    void process(float sample_duration, float& output) override;

    void osc(Oscillator& osc);
    // Oscillator& osc();

    template <typename O = Oscillator>
    O& osc()
    {
        return static_cast<O&>(*m_osc);
    }

private:
    std::unique_ptr<Oscillator> m_osc;
    std::unique_ptr<Envelope> m_env;
    float m_freq;
    float m_phase;
    float m_vol;
};

/**
 * @brief The simplest FM synth.
 * 
 */
class VoiceFM2 : public Voice
{
public:
    explicit VoiceFM2(Oscillator& carrier, Oscillator& modulator, Envelope& env,
        float freq_carrier = 440, float freq_modulator = 440,
        float fm_level = 0, float vol = 1., unsigned int oversamping = 1);
    ~VoiceFM2() noexcept = default;
    VoiceFM2(const VoiceFM2& other);
    VoiceFM2& operator=(const VoiceFM2& other);
    VoiceFM2(VoiceFM2&&) noexcept = default;
    VoiceFM2& operator=(VoiceFM2&&) noexcept = default;

    std::unique_ptr<Voice> clone() const override;

    Envelope& env() override;

    template <typename E>
    E& env()
    {
        return static_cast<E&>(*m_env);
    }

    void freq(float m_freq) override;
    float freq() const override;

    void vol(float m_vol) override;

    void note_on(float freq) override;
    void note_off() override;

    bool is_on() const override;

    void process(float sample_duration, float& output) override;

private:
    std::unique_ptr<Oscillator> m_carrier;
    std::unique_ptr<Oscillator> m_modulator;
    std::unique_ptr<Envelope> m_env;

    float m_freq;
    float m_phase;
    float m_vol;
    float m_fm_level;
    unsigned int m_oversamping;
};

/**
 * @brief A phase distortion synth. Passes the oscillator's phase parameter
 * through distortion function, which causes the wave shape to get
 * distorted.
 */
class VoicePD : public Voice
{
public:
    explicit VoicePD(Oscillator& m_osc, WaveShaper& phase_func,
        Envelope& env, float freq = 440, float vol = 1.);
    ~VoicePD() noexcept = default;
    VoicePD(const VoicePD& other);
    VoicePD& operator=(const VoicePD& other);
    VoicePD(VoicePD&&) noexcept = default;
    VoicePD& operator=(VoicePD&&) noexcept = default;

    std::unique_ptr<Voice> clone() const override;

    Envelope& env() override;

    void freq(float freq) override;
    float freq() const override;
    
    void vol(float vol) override;

    void note_on(float freq) override;
    void note_off() override;

    bool is_on() const override;

    void process(float sample_duration, float& output) override;

private:
    std::unique_ptr<Oscillator> m_osc;
    std::unique_ptr<WaveShaper> m_phase_func;
    std::unique_ptr<Envelope> m_env;
    float m_freq;
    float m_phase;
    float m_vol;
};

}

#endif // VOICE_H_
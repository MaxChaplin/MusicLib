#ifndef VOICE_H_
#define VOICE_H_

#include "envelope.hpp"
#include "osc.hpp"
#include "wave_shaper.hpp"
#include "util.hpp"

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
    , m_env{Util::clone<E>(env)}
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
    , m_env{Util::clone<E>(other.m_env)}
    , m_freq{other.m_freq}
    , m_phase{other.m_phase}
    , m_vol{other.m_vol}
    {
        m_voices.reserve(other.m_voices.size());

        for (const auto& v : other.m_voices)
        {
            m_voices.push_back(Util::clone<V>(*v));
        }
    }

    VoiceMulti& operator=(const VoiceMulti& other)
    {
        if (this != &other)
        {
            m_voices.clear();
            for (const auto& v : other.m_voices)
            {
                m_voices.push_back(Util::clone<V>(*v));
            }

            m_env = Util::clone<E>(*other.m_env);
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

    void env(const Envelope& env) override
    {
        m_env = clone<E>(env);
    }

    E& env() override
    {
        return *m_env;
    }

    const E& env() const override
    {
        return *m_env;
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
        m_env->trig(true);

        for (auto& v : m_voices)
        {
            v->freq(freq);
            v->env().trig(true);
        }
    }

    void note_off() override
    {
        m_env->trig(false);

        for (auto& v : m_voices)
        {
            v->env().trig(false);
        }
    }

    bool is_on() const override
    {
        return m_env->is_on();
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
    std::vector<std::unique_ptr<V>> m_voices;
    std::unique_ptr<E> m_env;
    float m_freq;
    float m_phase;
    float m_vol;
};

/**
 * @brief A voice based around an oscillator object, with a single velocity
 * envelope.
 * 
 * Each of the template parameters can be used in one of three ways. If not set,
 * any oscillator/envelope can be used. If set to an abstract interface, only
 * descendants of the interface can be used. If set to a concrete class, only
 * it can be used (provided concrete classes are not inherited).
 * 
 * In the first two cases, a getter templates needs the appropriate type as a 
 * template parameter in order to use a derived class' method, but not when
 * using the abstract interface's methods. In the last case, the getters don't
 * need template parameters.
 * 
 * @tparam O Oscillator type (Oscillator by default)
 * @tparam E Envelopy type (Envelope by default)
 */
template <typename O = Oscillator, typename E = Envelope>
class VoiceOsc : public Voice
{
public:
    explicit VoiceOsc(O& osc, E& env, float freq = 440, float vol = 1.)
    : m_osc{Util::clone<O>(osc)}
    , m_env{Util::clone<E>(env)}
    , m_freq{freq}
    , m_phase{0}
    , m_vol{vol}
    {
        static_assert(std::is_base_of_v<Oscillator, O>, "class O must be derived from Oscillator");
        static_assert(std::is_base_of_v<Envelope, E>, "class E must be derived from Envelope");
    }

    ~VoiceOsc() noexcept = default;

    VoiceOsc(const VoiceOsc& other)
    : m_osc{Util::clone<O>(*other.m_osc)}
    , m_env{Util::clone<E>(*other.m_env)}
    , m_freq{other.m_freq}
    , m_phase{other.m_phase}
    , m_vol{other.m_vol}
    {

    }

    VoiceOsc& operator=(const VoiceOsc& other)
    {
        if (this != &other)
        {
            m_osc = Util::clone<O>(*other.m_osc);
            m_env = Util::clone<E>(*other.m_env);
            m_freq = other.m_freq;
            m_phase = other.m_phase;
            m_vol = other.m_vol;
        }
        return *this;
    }
    
    VoiceOsc(VoiceOsc&&) noexcept = default;
    VoiceOsc& operator=(VoiceOsc&&) noexcept = default;

    std::unique_ptr<Voice> clone() const override
    {
        return std::make_unique<VoiceOsc>(*this);
    }

    void env(const Envelope& env) override
    {
        m_env = Util::clone<E>(env);        
    }

    E& env() override
    {
        return *m_env;
    }

    const E& env() const override
    {
        return *m_env;
    }

    template <typename E2 = E>
    E2& env()
    {
        return static_cast<E2&>(*m_env);
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
        m_env->trig(true);
    }

    void note_off() override
    {
        m_env->trig(false);
    }

    bool is_on() const override
    {
        return m_env->is_on();
    }

    void process(float sample_duration, float& output) override
    {
        output = m_vol * m_osc->value(m_phase) * m_env->process(sample_duration);

        // Propagate phase.
        m_phase += sample_duration * m_freq;
        if (m_phase >= 1)
        {
            m_phase -= 1;
        }
    }

    void osc(O& osc)
    {
        m_osc = clone<O>(osc);
    }

    O& osc()
    {
        return *m_osc;
    }

    const O& osc() const
    {
        return *m_osc;
    }

    template <typename O2 = Oscillator>
    O2& osc()
    {
        return static_cast<O2&>(*m_osc);
    }

private:
    std::unique_ptr<O> m_osc;
    std::unique_ptr<E> m_env;
    float m_freq;
    float m_phase;
    float m_vol;
};

}

#endif // VOICE_H_
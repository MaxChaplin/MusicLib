#ifndef OSC_H_
#define OSC_H_

#include "util.hpp"

#include <functional>
#include <vector>
#include <memory>
#include <stdexcept>

namespace MusicLib {

// Basic oscillator functions
float osc_saw(float phase);
float osc_square(float phase);
float osc_triangle(float phase);

/**
 * @brief An oscillator interface. Used by the VoiceOsc class as the first
 * step of the synthesis chain. Generally doesn't handle time or frequency,
 * only the basic waveshape.
 */
class Oscillator
{
public:
    Oscillator() = default;
    virtual ~Oscillator() = default;

    virtual std::unique_ptr<Oscillator> clone() const = 0;

    /**
     * @brief Give the value of the oscillator at the given phase.
     * 
     * @param phase a number between 0 (including) and 1 (not including).
     * The phase typically evolves in time like a upwards ramp function.
     * @return A sample. 
     */
    virtual float value(float phase) const = 0;
};

/**
 * @brief An oscillator that can be switched between multiple given oscillators.
 * 
 * @tparam O Contained oscillator type
 */
template <typename O = Oscillator>
class OscillatorSwitch : public Oscillator
{
public:
    explicit OscillatorSwitch(unsigned int osc_index = 0)
    : m_oscs{}
    , m_osc_index{osc_index}
    {

    }

    explicit OscillatorSwitch(std::vector<std::unique_ptr<Oscillator>>& oscs, unsigned int osc_index = 0)
    : m_oscs{}
    , m_osc_index{osc_index}
    {
        for (const auto& o : oscs)
        {
            m_oscs.emplace_back(o->clone());
        }
    }

    explicit OscillatorSwitch(const OscillatorSwitch& other)
    : m_oscs{}
    , m_osc_index{other.m_osc_index}
    {
        for (const auto& o : other.m_oscs)
        {
            m_oscs.emplace_back(o->clone());
        }
    }

    OscillatorSwitch& operator=(const OscillatorSwitch& other)
    {
        if (this != &other)
        {
            m_oscs = std::vector<std::unique_ptr<Oscillator>>{};
            for (const auto& v : other.m_oscs)
            {
                m_oscs.emplace_back(v->clone());
            }

            m_osc_index = other.m_osc_index;
        }
        return *this;
    }

    ~OscillatorSwitch() noexcept = default;

    std::unique_ptr<Oscillator> clone() const override
    {
        return std::make_unique<OscillatorSwitch>(*this);
    }


    float value(float phase) const override
    {
        return m_oscs[m_osc_index]->value(phase);
    }

    void add_osc(Oscillator& osc)
    {
        m_oscs.emplace_back(Util::clone<O>(osc));
    }

    void select(unsigned int osc_index)
    {
        if (osc_index >= m_oscs.size())
        {
            throw std::invalid_argument("oscillator index is out of bounds");
        }
        else
        {
            m_osc_index = osc_index;
        }
    }

private:
    std::vector<std::unique_ptr<Oscillator>> m_oscs;
    unsigned int m_osc_index;
};


/**
 * @brief The simplest oscillator, revolving around a single pure function.
 */
class OscillatorBasic : public Oscillator
{
public:
    explicit OscillatorBasic(std::function<float(float)> osc_func);
    ~OscillatorBasic() noexcept = default;

    std::unique_ptr<Oscillator> clone() const override;

    float value(float phase) const override;

private:
    std::function<float(float)> m_osc_func;
};

/**
 * @brief A pulse oscillator.
 */
class OscillatorPulse : public Oscillator
{
public:
    explicit OscillatorPulse(float pulsewidth);
    ~OscillatorPulse() noexcept = default;

    std::unique_ptr<Oscillator> clone() const override;

    float value(float phase) const override;

    void pulsewidth(float pulsewidth);
    float pulsewidth() const;        

private:
    float m_pulsewidth;
};

/**
 * @brief A wavetable oscillator.
 */
class OscillatorWavetable : public Oscillator
{
public:
    explicit OscillatorWavetable(std::vector<float> wavetable, bool antialiasing = false);
    ~OscillatorWavetable() noexcept = default;

    std::unique_ptr<Oscillator> clone() const override;

    float value(float phase) const override;

private:
    std::vector<float> m_wavetable;
    bool m_antialiasing;
};

}
#endif // OSC_H_
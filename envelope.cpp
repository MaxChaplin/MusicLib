#include "envelope.hpp"

namespace MusicLib
{
    EnvelopeZero::EnvelopeZero()
    : m_is_on{false}
    {

    }

    std::shared_ptr<Envelope> EnvelopeZero::clone() const
    {
        return std::make_shared<EnvelopeZero>(*this);
    }

    void EnvelopeZero::trig(bool is_on)
    {
        m_is_on = is_on;
    }

    float EnvelopeZero::process(float sample_time [[maybe_unused]])
    {
        if (m_is_on)
        {
            return 1;
        }

        return 0;
    }

    bool EnvelopeZero::is_on() const
    {
        return m_is_on;
    }

    void EnvelopeZero::set_retrigger(bool is_retrigger [[maybe_unused]])
    {}

    EnvelopeADSR::EnvelopeADSR(float attack, float decay, float sustain, float release, bool is_retrigger)
    : m_level{0}
    , m_level_raw{0}
    , m_attack{attack}
    , m_decay{decay}
    , m_sustain{sustain}
    , m_release{release}
    , m_stage{Stage::OFF}
    , m_is_retrigger{is_retrigger}
    {

    }

    std::shared_ptr<Envelope> EnvelopeADSR::clone() const
    {
        return std::make_shared<EnvelopeADSR>(*this);
    }

    float EnvelopeADSR::attack() const
    {
        return m_attack;
    }

    void EnvelopeADSR::attack(float attack)
    {
        m_attack = attack;
    }

    float EnvelopeADSR::decay() const
    {
        return m_decay;
    }


    void EnvelopeADSR::decay(float decay)
    {
        m_decay = decay;
    }

    float EnvelopeADSR::sustain() const
    {
        return m_sustain;
    }

    void EnvelopeADSR::sustain(float sustain)
    {
        m_sustain = sustain;
    }

    float EnvelopeADSR::release() const
    {
        return m_release;
    }

    void EnvelopeADSR::release(float release)
    {
        m_release = release;
    }

    void EnvelopeADSR::trig(bool is_on)
    {
        if (is_on)
        {
            m_stage = Stage::ATTACK;

            if (m_is_retrigger)
            {
                m_level = 0;
            }
        }
        else
        {
            if (m_stage != Stage::OFF)
            {
                m_stage = Stage::RELEASE;
            }
        }
    }

    float EnvelopeADSR::process(float sample_time)
    {
        if (m_stage == Stage::OFF)
        {
            m_level_raw = 0;
            m_level = 0;
        }    
        else if (m_stage == Stage::ATTACK)
        {
            m_level_raw += sample_time / m_attack;

            if (m_level_raw >= 1)
            {
                m_level_raw = 1;
                m_stage = Stage::DECAY;
            }

            // m_level = 2 * m_level_raw - m_level_raw * m_level_raw;
        }
        else if (m_stage == Stage::DECAY)
        {
            m_level_raw -= sample_time / m_decay;

            if (m_level_raw <= m_sustain)
            {
                m_level_raw = m_sustain;
                m_stage = Stage::SUSTAIN;
            }

            // m_level = m_level_raw * m_level_raw;
        }
        else if (m_stage == Stage::SUSTAIN)
        {
            // Do nothing
            
        }
        else // Release
        {
            m_level_raw -= sample_time / m_release;

            if (m_level_raw <= 0)
            {
                m_level_raw = 0;
                m_stage = Stage::OFF;
            }

            // m_level = m_level_raw * m_level_raw;
        }

        m_level = m_level_raw;

        return m_level;
    }

    bool EnvelopeADSR::is_on() const
    {
        return m_stage != Stage::OFF;
    }

    void EnvelopeADSR::set_retrigger(bool is_retrigger)
    {
        m_is_retrigger = is_retrigger;
    }
}
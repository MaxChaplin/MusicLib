#ifndef ENVELOPE_H_
#define ENVELOPE_H_

#include <memory>

namespace MusicLib {
class Envelope
{
public:
    Envelope() = default;
    virtual ~Envelope() = default;

    virtual std::unique_ptr<Envelope> clone() const = 0;

    virtual void trig(bool is_on) = 0;
    virtual float process(float time) = 0;

    virtual void set_retrigger(bool is_retrigger) = 0;

    virtual bool is_on() const = 0;
};

class EnvelopeZero : public Envelope
{
public:
    explicit EnvelopeZero();
    ~EnvelopeZero() noexcept = default;

    std::unique_ptr<Envelope> clone() const override;

    void trig(bool is_on) override;
    float process(float time) override;

    void set_retrigger(bool is_retrigger) override;

    bool is_on() const override;

private:
    bool m_is_on;
};

class EnvelopeADSR : public Envelope
{
public:
    explicit EnvelopeADSR(float attack = .01, float decay = 1, float sustain = 1, float release = .01, bool is_retrigger = true);
    ~EnvelopeADSR() noexcept = default;

    std::unique_ptr<Envelope> clone() const override;

    float attack() const;
    void attack(float attack);

    float decay() const;
    void decay(float decay);

    float sustain() const;
    void sustain(float sustain);

    float release() const;
    void release(float release);

    void trig(bool is_on) override;
    float process(float time) override;

    void set_retrigger(bool is_retrigger) override;

    bool is_on() const;

private:
    enum Stage {
        OFF,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

private:
    float m_level;
    float m_level_raw;

    float m_attack;
    float m_decay;
    float m_sustain;
    float m_release;

    Stage m_stage;

    bool m_is_retrigger;
};

}

#endif // ENVELOPE_H_
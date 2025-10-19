#ifndef TIME_MANAGER_H_
#define TIME_MANAGER_H_

namespace MusicLib {
    
/**
 * @brief A time manager is used by a sequencer to keep time, in particular
 *        to decide when to perform a step.
 * 
 */
class TimeManager
{
public:
    TimeManager() = default;
    virtual ~TimeManager() = default;

    virtual void playing(bool playing) = 0;
    virtual bool playing() const = 0;

    virtual bool count_sample() = 0;
};

class TimeManagerEventBased : public TimeManager
{
public:
    explicit TimeManagerEventBased();
    ~TimeManagerEventBased() noexcept = default;

    void playing(bool playing) override;
    bool playing() const override;

    bool count_sample() override;
    void reset_counter(unsigned long samples_until_next_step);

private:
    bool m_playing;
    unsigned long m_sample_counter;
};

class TimeManagerTempo : public TimeManager
{
public:
    explicit TimeManagerTempo(unsigned long sample_rate, float bpm = 120, unsigned int steps_per_beat = 4, float shuffle = .5);
    ~TimeManagerTempo() noexcept = default;

    void playing(bool playing) override;
    bool playing() const override;

    bool count_sample() override;

    float bpm() const;
    void bpm(float bpm);

    void steps_per_beat(unsigned int steps_per_beat);

    float step_duration();

private:
    bool m_playing;
    float m_bpm;
    unsigned int m_steps_per_beat;
    float m_step_duration;
    unsigned long m_sample_rate;
    unsigned long m_samples_per_step;
    unsigned long m_sample_counter;
    float m_shuffle;
};
}

#endif // TIME_MANAGER_H_
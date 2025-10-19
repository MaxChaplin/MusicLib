#ifndef TIME_MANAGER_H_
#define TIME_MANAGER_H_

namespace MusicLib
{
    class TimeManager
    {
    public:
        TimeManager() = default;
        virtual ~TimeManager() = default;

        virtual bool count_sample() = 0;
    };

    class TimeManagerEventBased : public TimeManager
    {
    public:
        explicit TimeManagerEventBased();
        ~TimeManagerEventBased() noexcept = default;

        bool count_sample() override;
        void reset_counter(unsigned long samples_until_next_step);
    
    private:
        unsigned long m_sample_counter;
    };

    class TimeManagerTempo : public TimeManager
    {
    public:
        explicit TimeManagerTempo(unsigned long sample_rate, float bpm = 120, unsigned int steps_per_beat = 4, float shuffle = .5);
        ~TimeManagerTempo() noexcept = default;

        bool count_sample() override;

        float bpm() const;
        void bpm(float bpm);

        void steps_per_beat(unsigned int steps_per_beat);

        float step_duration();

    private:
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
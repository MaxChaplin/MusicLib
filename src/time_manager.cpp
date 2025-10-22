#include "time_manager.hpp"

namespace MusicLib {

TimeManagerEventBased::TimeManagerEventBased()
: m_playing{false}
, m_sample_counter{}
{
    
}

void TimeManagerEventBased::playing(bool playing)
{
    m_playing = playing;
}

bool TimeManagerEventBased::playing() const
{
    return m_playing;
}

bool TimeManagerEventBased::count_sample()
{
    if (m_sample_counter > 0)
    {
        --m_sample_counter;
    }

    return m_sample_counter == 0;
}

void TimeManagerEventBased::reset_counter(unsigned long samples_until_next_step)
{
    m_sample_counter = samples_until_next_step;
}

TimeManagerTempo::TimeManagerTempo(unsigned long sample_rate, float bpm, unsigned int steps_per_beat, float shuffle)
: m_playing{false}
, m_bpm{bpm}
, m_steps_per_beat{steps_per_beat}
, m_step_duration{60.0f / (bpm * steps_per_beat)}
, m_sample_rate{sample_rate}
, m_samples_per_step{(unsigned long) (sample_rate * m_step_duration)}
, m_sample_counter{m_samples_per_step}
, m_shuffle{shuffle}
{
    long sps = (unsigned long) (sample_rate * m_step_duration);

    if (sps >= 0)
    {
        m_samples_per_step = sps;
    }
}

void TimeManagerTempo::playing(bool playing)
{
    m_playing = playing;
}

bool TimeManagerTempo::playing() const
{
    return m_playing;
}

bool TimeManagerTempo::count_sample()
{
    if (m_sample_counter > 0)
    {
        --m_sample_counter;
    }

    if (m_sample_counter == 0)
    {
        m_sample_counter = m_samples_per_step;
        return true;
    }

    return false;
}

float TimeManagerTempo::step_duration()
{
    return 60.0 / m_bpm;
}

float TimeManagerTempo::bpm() const
{
    return m_bpm;
}

void TimeManagerTempo::bpm(float bpm)
{
    if (bpm < 0)
    {
        return;
    }
    
    m_bpm = bpm;
    m_step_duration = 60.0f / (bpm * m_steps_per_beat);
    m_samples_per_step = (unsigned long) (m_step_duration * m_sample_rate);
}

}
#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "instrument.hpp"

#include <portaudio.h>
#include <functional>
#include <memory>
#include <atomic>
#include <thread>

#include <vector>

using Command = std::pair<size_t, float>;
using Sequence = std::vector<std::vector<Command>>;

namespace MusicLib
{
    class Sequencer
    {
    public:
        explicit Sequencer(float bpm, unsigned int steps_per_beat,
            unsigned int sample_rate, InstrumentManager& instrument_manager,
            Sequence& sequence);
        ~Sequencer() noexcept = default;

        void step();

        float beat_duration();

        unsigned long samples_per_beat() const;

        const float& bpm() const;
        float& bpm(float bpm);
        Sequence& sequence();

        void handle_sample();

    private:
        Sequence& m_sequence;
        size_t m_cursor;

        InstrumentManager& m_instrument_manager;
        float m_bpm;
        unsigned int m_steps_per_beat;
        float m_step_duration;
        unsigned long m_sample_rate;
        unsigned long m_samples_per_step;
        unsigned long m_step_counter;
    };
}

#endif // SEQUENCER_H_
#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "command.hpp"
#include "command_stream.hpp"
#include "command_processor.hpp"
#include "instrument.hpp"
#include "time_manager.hpp"

#include <functional>
#include <memory>
#include <atomic>
#include <thread>
#include <vector>

using Command = std::pair<size_t, float>;
using Sequence = std::vector<std::vector<Command>>;

namespace MusicLib
{
    // class Sequencer
    // {
    // public:
    //     explicit Sequencer(float bpm, unsigned int steps_per_beat,
    //         unsigned int sample_rate, InstrumentManager& instrument_manager,
    //         Sequence& sequence);
    //     ~Sequencer() noexcept = default;

    //     void step();

    //     float beat_duration();

    //     unsigned long samples_per_beat() const;

    //     const float& bpm() const;
    //     float& bpm(float bpm);
    //     Sequence& sequence();

    //     void handle_sample();

    // private:
    //     Sequence& m_sequence;
    //     size_t m_cursor;

    //     InstrumentManager& m_instrument_manager;
    //     float m_bpm;
    //     unsigned int m_steps_per_beat;
    //     float m_step_duration;
    //     unsigned long m_sample_rate;
    //     unsigned long m_samples_per_step;
    //     unsigned long m_step_counter;
    // };

    class Sequencer
    {
    public:
        Sequencer() = default;
        virtual ~Sequencer() = default;

        virtual void handle_sample() = 0;
        virtual void step() = 0;
    };

    class SequencerBasic : public Sequencer
    {
    public:
        explicit SequencerBasic(TimeManager& time_mgr,
            InstrumentManager& ins_mgr,
            CommandStream& cmd_stream,
            CommandProcessor& cmd_processor);
        ~SequencerBasic() noexcept = default;

        void handle_sample() override;
        void step() override;

    private:
        TimeManager& m_time_mgr;
        CommandStream& m_cmd_stream;
        std::reference_wrapper<InstrumentManager> m_ins_mgr;
        CommandProcessor& m_cmd_processor;

        // std::function<void(Command* cmd, CommandStream* cmd_stream)> m_cmd_stream_handler;
        // std::function<void(Command* cmd, TimeManager* time_manager)> m_time_handler;

    };

    class SequencerMultiChannel : public Sequencer
    {
    public:
        explicit SequencerMultiChannel(TimeManager& time_mgr, InstrumentManager& ins_mgr,
            std::vector<CommandStream>& cmd_streams, CommandProcessor& cmd_processor);
        ~SequencerMultiChannel() noexcept = default;

        void handle_sample() override;
        void step() override;

    private:
        TimeManager& m_time_mgr;
        std::vector<CommandStream>& m_cmd_streams;
        std::reference_wrapper<InstrumentManager> m_ins_mgr;
        CommandProcessor& m_cmd_processor;
    };

    class MultiSequencer : public Sequencer
    {
    public:
        explicit MultiSequencer(std::vector<Sequencer>& seqs);
        ~MultiSequencer() noexcept = default;

        void handle_sample() override;
        void step() override;

    private:
        std::vector<Sequencer>& m_seqs;
    };  
}

#endif // SEQUENCER_H_
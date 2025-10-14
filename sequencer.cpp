#include "sequencer.hpp"
#include "instrument.hpp"
#include "command_stream.hpp"

#include <portaudio.h>
#include <functional>

namespace MusicLib
{
    SequencerBasic::SequencerBasic(TimeManager& time_mgr, InstrumentManager& ins_mgr,
            CommandStream& cmd_stream, CommandProcessor& cmd_processor)
    : m_time_mgr{time_mgr}
    , m_cmd_stream{cmd_stream}
    , m_ins_mgr{ins_mgr}
    , m_cmd_processor{cmd_processor}
    {
        m_time_mgr.playing(true);
    }

    void SequencerBasic::handle_sample()
    {
        if (m_time_mgr.playing() && m_time_mgr.count_sample())
        {
            step();
        }
    }

    void SequencerBasic::step()
    {
        Command& cmd = m_cmd_stream.current();
        
        // Handle the command.
        m_cmd_processor.handle_command_stream(cmd, m_cmd_stream);
        m_cmd_processor.handle_instrument_manager(cmd, m_ins_mgr);
        m_cmd_processor.handle_time_manager(cmd, m_time_mgr);

        // Go to next command.
        m_cmd_stream.step();

        if (m_cmd_stream.finished())
        {
            m_time_mgr.playing(false);
        }
    }

    SequencerMultiChannel::SequencerMultiChannel(TimeManager& time_mgr, InstrumentManager& ins_mgr, std::vector<CommandStream>& cmd_streams, CommandProcessor& cmd_processor)
    : m_time_mgr{time_mgr}
    , m_cmd_streams{cmd_streams}
    , m_ins_mgr{ins_mgr}
    , m_cmd_processor{cmd_processor}
    {

    }

    void SequencerMultiChannel::handle_sample()
    {
        if (m_time_mgr.count_sample() == 0)
        {
            step();
        }
    }

    void SequencerMultiChannel::step()
    {
        for (auto cs = m_cmd_streams.begin(); cs != m_cmd_streams.end(); ++cs)
        {
            Command& cmd = cs->current();
            
            // Handle the command.
            m_cmd_processor.handle_command_stream(cmd, *cs);
            m_cmd_processor.handle_instrument_manager(cmd, m_ins_mgr);
            m_cmd_processor.handle_time_manager(cmd, m_time_mgr);
            
            // Go to next command.
            cs->step();
        }
    }

    MultiSequencer::MultiSequencer(std::vector<Sequencer>& seqs)
    : m_seqs{seqs}
    {

    }

    void MultiSequencer::handle_sample()
    {
        for (auto seq = m_seqs.begin(); seq != m_seqs.end(); ++seq)
        {
            seq->handle_sample();
        }
    }

    void MultiSequencer::step()
    {
        for (auto seq = m_seqs.begin(); seq != m_seqs.end(); ++seq)
        {
            seq->step();
        }
    }

}
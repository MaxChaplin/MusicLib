#include "sequencer.hpp"
#include "device.hpp"
#include "command_stream.hpp"

#include <portaudio.h>
#include <functional>

namespace MusicLib {

MultiSequencer::MultiSequencer(std::vector<Sequencer>& seqs)
: m_seqs{seqs}
{

}

void MultiSequencer::tick()
{
    for (auto& seq : m_seqs)
    {
        seq.tick();
    }
}

void MultiSequencer::step()
{
    for (auto& seq : m_seqs)
    {
        seq.step();
    }
}

void MultiSequencer::reset()
{
    for (auto& seq : m_seqs)
    {
        seq.reset();
    }
}

SequencerBasic::SequencerBasic(TimeManager& time_mgr, IDevice& device,
        CommandStream& cmd_stream, CommandProcessor& cmd_processor)
: m_time_mgr{time_mgr}
, m_cmd_stream{cmd_stream}
, m_device{device}
, m_cmd_processor{cmd_processor}
{
    m_time_mgr.playing(false);
}

void SequencerBasic::tick()
{
    if (m_time_mgr.playing() && m_time_mgr.count_sample())
    {
        step();
    }
}

void SequencerBasic::step()
{
    auto cmd = m_cmd_stream.current();

    if (!cmd)
    {
        m_time_mgr.playing(false);
        return;
    }

    // Handle the command.
    m_cmd_processor.handle_command_stream(*cmd, m_cmd_stream);
    m_cmd_processor.handle_time_manager(*cmd, m_time_mgr);
    m_cmd_processor.handle_device(*cmd, m_device);

    // Go to next command.
    m_cmd_stream.step();

    if (m_cmd_stream.finished())
    {
        m_time_mgr.playing(false);
    }
}

void SequencerBasic::reset()
{
    m_cmd_stream.reset();
}

SequencerMultiChannel::SequencerMultiChannel(TimeManager& time_mgr, IDevice& device, std::vector<CommandStream>& cmd_streams, CommandProcessor& cmd_processor)
: m_time_mgr{time_mgr}
, m_cmd_streams{cmd_streams}
, m_device{device}
, m_cmd_processor{cmd_processor}
{

}

void SequencerMultiChannel::tick()
{
    if (m_time_mgr.count_sample() == 0)
    {
        step();
    }
}

void SequencerMultiChannel::step()
{
    for (auto& cs : m_cmd_streams)
    {
        auto cmd = cs.current();
        
        // Handle the command.
        m_cmd_processor.handle_command_stream(*cmd, cs);
        m_cmd_processor.handle_device(*cmd, m_device);
        m_cmd_processor.handle_time_manager(*cmd, m_time_mgr);
        
        // Go to next command.
        cs.step();
    }
}

void SequencerMultiChannel::reset()
{
    for (auto& cs : m_cmd_streams)
    {
        cs.reset();
    }
}

}
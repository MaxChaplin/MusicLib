#include "command_processor.hpp"
#include "command.hpp"
#include "command_stream.hpp"
#include "instrument.hpp"
#include "time_manager.hpp"

namespace MusicLib
{
    // CommandProcessorBasic::CommandProcessorBasic(auto cmd_stream_handler,
    //     auto ins_mgr_handler,  auto time_handler)
    // : m_cmd_stream_handler{cmd_stream_handler}
    // , m_ins_mgr_handler{ins_mgr_handler}
    // , m_time_handler{time_handler}
    // {

    // }

    CommandProcessorBasic::CommandProcessorBasic()
    : m_cmd_stream_handler{}
    , m_ins_mgr_handler{}
    , m_time_handler{}
    {

    }

    void CommandProcessorBasic::handle_command_stream(Command& cmd, CommandStream& cmd_stream)
    {
        m_cmd_stream_handler(cmd, cmd_stream);
    }

    void CommandProcessorBasic::handle_instrument_manager(Command& cmd, InstrumentManager& ins_mgr) 
    {
        m_ins_mgr_handler(cmd, ins_mgr);
    }

    void CommandProcessorBasic::handle_time_manager(Command& cmd, TimeManager& time_mgr)
    {
        m_time_handler(cmd, time_mgr);
    }

    // void CommandProcessorBasic::set_command_stream_handler(auto cmd_stream_handler)
    // {
    //     m_cmd_stream_handler = [cmd_stream_handler]
    // }

    // void CommandProcessorBasic::set_instrument_handler(auto ins_mgr_handler)
    // {

    // }

    // void CommandProcessorBasic::set_time_handler(auto time_handler)
    // {

    // }

//     void CommandProcessor::operator()(const Command& cmd)
//     {
//         std::visit(*this, cmd);
//     }

//     void CommandProcessor::operator()(const CommandNoteOn& cmd)
//     {
//         float freq = freq_from_pitch[cmd.pitch];
//         // m_commands.freq(freq);
//         m_commands.instrument().note_on(m_commands.voice_num(), freq);
//     }
    
//     void CommandProcessor::operator()(const CommandNoteOnFreq& cmd)
//     {
//         // m_commands.freq(cmd.freq);
//         m_commands.instrument().note_on(m_commands.voice_num(), cmd.freq);
//     }
    
//     void CommandProcessor::operator()(const CommandNoteOff& cmd)
//     {
//         m_commands.instrument().note_off(m_commands.voice_num());
//     }

//     void CommandProcessor::operator()(const CommandVol& cmd)
//     {
//         // m_commands.vol(cmd.vol);
//         m_commands.instrument().vol(cmd.vol);
//     }
}
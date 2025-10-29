#include "command_processor.hpp"
#include "command.hpp"
#include "command_stream.hpp"
#include "device.hpp"
#include "time_manager.hpp"

namespace MusicLib {

CommandProcessorBasic::CommandProcessorBasic()
: m_cmd_stream_handler{}
, m_device_handler{}
, m_time_handler{}
{

}

void CommandProcessorBasic::handle_command_stream(Command& cmd, CommandStream& cmd_stream)
{
    m_cmd_stream_handler(cmd, cmd_stream);
}

void CommandProcessorBasic::handle_device(Command& cmd, Device& dev_mgr) 
{
    m_device_handler(cmd, dev_mgr);
}

void CommandProcessorBasic::handle_time_manager(Command& cmd, TimeManager& time_mgr)
{
    m_time_handler(cmd, time_mgr);
}

}
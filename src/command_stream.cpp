#include "command.hpp"
#include "command_stream.hpp"
#include "device.hpp"

namespace MusicLib {
    
CommandStreamBasic::CommandStreamBasic(bool looping)
: m_commands{}
, m_cursor{0}
, m_looping{looping}
{

}

CommandStreamBasic::CommandStreamBasic(std::vector<std::unique_ptr<Command>>& commands, bool looping)
: m_commands{}
, m_cursor{0}
, m_looping{looping}
{
    m_commands.reserve(commands.size());

    for (const auto& c : commands)
    {
        m_commands.push_back(c->clone());
    }
}


CommandStreamBasic::CommandStreamBasic(const CommandStreamBasic& other)
: m_commands{}
, m_cursor{other.m_cursor}
, m_looping{other.m_looping}
{
    m_commands.reserve(other.m_commands.size());

    for (const auto& c : other.m_commands)
    {
        m_commands.push_back(c->clone());
    }
}

CommandStreamBasic& CommandStreamBasic::operator=(const CommandStreamBasic& other)
{
    if (this != &other)
    {
        m_commands.clear();
        m_commands.reserve(other.m_commands.size());
        
        for (const auto& c : other.m_commands)
        {
            m_commands.push_back(c->clone());
        }

        m_cursor = other.m_cursor;
        m_looping = other.m_looping;
    }

    return *this;
}

Command* CommandStreamBasic::current() const
{
    if (m_cursor >= m_commands.size())
    {
        return nullptr;
    }
    
    return m_commands[m_cursor].get();
}

bool CommandStreamBasic::finished() const
{
    return !m_looping && m_cursor >= m_commands.size() - 1;
}

void CommandStreamBasic::reset()
{
    cursor(0);
}

unsigned long CommandStreamBasic::step()
{
    if (m_cursor < m_commands.size() - 1)
    {
        ++m_cursor;
    }
    else
    {
        if (m_looping)
        {
            m_cursor = 0;
        }
    }

    return m_cursor;
}

void CommandStreamBasic::cursor(unsigned long cursor)
{
    if (cursor < m_commands.size())
    {
        m_cursor = cursor;
    }
}

void CommandStreamBasic::add(Command& command)
{
    m_commands.push_back(command.clone());
}





CommandStreamInstrument::CommandStreamInstrument(IInstrument& ins, bool looping)
: m_commands{}
, m_cursor{0}
, m_looping{looping}
, m_ins{ins}
{

}

CommandStreamInstrument::CommandStreamInstrument(std::vector<std::unique_ptr<Command>>& commands, IInstrument& ins, bool looping)
: m_commands{}
, m_cursor{0}
, m_looping{looping}
, m_ins{ins}
{
    m_commands.reserve(commands.size());

    for (const auto& c : commands)
    {
        m_commands.push_back(c->clone());
    }
}

CommandStreamInstrument::CommandStreamInstrument(const CommandStreamInstrument& other)
: m_commands{}
, m_cursor{other.m_cursor}
, m_looping{other.m_looping}
, m_ins{other.m_ins}
{
    m_commands.reserve(other.m_commands.size());

    for (const auto& c : other.m_commands)
    {
        m_commands.push_back(c->clone());
    }

    m_cursor = other.m_cursor;
    m_looping = other.m_looping;
    m_ins = other.m_ins;
}

CommandStreamInstrument& CommandStreamInstrument::operator=(const CommandStreamInstrument& other)
{
    if (this != &other)
    {
        m_commands.clear();
        m_commands.reserve(other.m_commands.size());
        
        for (const auto& c : other.m_commands)
        {
            m_commands.push_back(c->clone());
        }

        m_cursor = other.m_cursor;
        m_looping = other.m_looping;
        m_ins = other.m_ins;
    }

    return *this;
}

Command* CommandStreamInstrument::current() const
{
    if (m_cursor >= m_commands.size())
    {
        return nullptr;
    }

    return m_commands[m_cursor].get();
}



bool CommandStreamInstrument::finished() const
{
    return !m_looping && m_cursor >= m_commands.size() - 1;
}

void CommandStreamInstrument::reset()
{
    cursor(0);
}

unsigned long CommandStreamInstrument::step()
{
    if (m_cursor < m_commands.size() - 1)
    {
        ++m_cursor;
    }
    else
    {
        if (m_looping)
        {
            m_cursor = 0;
        }
    }

    return m_cursor;
}

void CommandStreamInstrument::cursor(unsigned long cursor)
{
    if (cursor < m_commands.size())
    {
        m_cursor = cursor;
    }
}

void CommandStreamInstrument::add(Command& command)
{
    m_commands.push_back(command.clone());
}

void CommandStreamInstrument::instrument(IInstrument& ins)
{
    m_ins = ins;
}

IInstrument& CommandStreamInstrument::instrument()
{
    return m_ins;
}

}
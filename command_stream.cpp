#include "command.hpp"
#include "command_stream.hpp"
#include "instrument.hpp"

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

void CommandStreamBasic::add(Command& command)
{
    m_commands.push_back(command.clone());
}

Command* CommandStreamBasic::current()
{
    if (m_cursor >= m_commands.size())
    {
        return nullptr;
    }
    
    return m_commands[m_cursor].get();
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

bool CommandStreamBasic::finished()
{
    return !m_looping && m_cursor >= m_commands.size() - 1;
}

CommandStreamInstrument::CommandStreamInstrument(Instrument& ins, bool looping)
: m_commands{}
, m_cursor{0}
, m_looping{looping}
, m_ins{ins}
{

}

CommandStreamInstrument::CommandStreamInstrument(std::vector<std::unique_ptr<Command>>& commands, Instrument& ins, bool looping)
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


void CommandStreamInstrument::add(Command& command)
{
    m_commands.push_back(command.clone());
}

Command* CommandStreamInstrument::current()
{
    if (m_cursor >= m_commands.size())
    {
        return nullptr;
    }

    return m_commands[m_cursor].get();
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

bool CommandStreamInstrument::finished()
{
    return !m_looping && m_cursor >= m_commands.size() - 1;
}

void CommandStreamInstrument::instrument(Instrument& ins)
{
    m_ins = ins;
}

Instrument& CommandStreamInstrument::instrument()
{
    return m_ins;
}

}
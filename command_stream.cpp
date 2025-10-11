#include "command.hpp"
#include "command_stream.hpp"
#include "instrument.hpp"

#include <iostream>

namespace MusicLib
{
    CommandStreamBasic::CommandStreamBasic(std::vector<std::shared_ptr<Command>>& commands, bool looping)
    : m_commands{commands}
    , m_cursor{0}
    , m_looping{looping}
    {

    }

    CommandStreamBasic::~CommandStreamBasic() noexcept = default;

    Command& CommandStreamBasic::current()
    {
        return *m_commands[m_cursor];
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
        if (cursor <= m_commands.size())
        {
            m_cursor = cursor;
        }       
    }

    CommandStreamInstrument::CommandStreamInstrument(std::vector<std::shared_ptr<Command>> commands, Instrument& ins, bool looping)
    : m_commands{commands}
    , m_looping{looping}
    , m_ins{std::make_shared<Instrument>(ins)}
    {

    }

    CommandStreamInstrument::~CommandStreamInstrument() noexcept = default;

    Command& CommandStreamInstrument::current()
    {
        return *m_commands[m_cursor];
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
        m_cursor = cursor;
    }

    void CommandStreamInstrument::instrument(Instrument& ins)
    {
        m_ins = std::make_shared<Instrument>(ins);
    }

    Instrument& CommandStreamInstrument::instrument()
    {
        return *m_ins;
    }

}
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
    , m_finished{false}
    {

    }

    // CommandStreamBasic::~CommandStreamBasic() noexcept = default;

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
            else
            {
                m_finished = true;
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

        m_finished = cursor >= m_commands.size() - 1;
    }

    bool CommandStreamBasic::finished()
    {
        return m_finished;
    }

    CommandStreamInstrument::CommandStreamInstrument(std::vector<std::shared_ptr<Command>> commands, Instrument& ins, bool looping)
    : m_commands{commands}
    , m_looping{looping}
    , m_finished{false}
    , m_ins{ins}
    {

    }

    // CommandStreamInstrument::~CommandStreamInstrument() noexcept = default;

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
            else
            {
                m_finished = true;
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

        m_finished = cursor >= m_commands.size() - 1;
    }

    bool CommandStreamInstrument::finished()
    {
        return m_finished;
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
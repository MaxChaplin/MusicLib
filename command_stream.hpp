#ifndef COMMAND_STREAM_H_
#define COMMAND_STREAM_H_

#include <variant>
#include <vector>
#include <memory>
#include <functional>

#include "command.hpp"

namespace MusicLib
{
    class Instrument; // Forward declaration

    /**
     * @brief An interface for classes that contain an organized sequence of 
     * commands, to be read and acted upon by the sequencer.
     */
    class CommandStream
    {
    public:
        CommandStream() = default;
        virtual ~CommandStream() = default;

        virtual Command& current() = 0;
        virtual unsigned long step() = 0;
        virtual void cursor(unsigned long index) = 0;
    };

    /**
     * @brief The simplest command stream, with a single vector of commands.
     * offloads the entire effort of playing the instruments and keeping track 
     * of parameters to the the instrument manager.
     */
    class CommandStreamBasic : public CommandStream
    {
    public:
        explicit CommandStreamBasic(std::vector<std::shared_ptr<Command>>& commands, bool looping = false);
        ~CommandStreamBasic();

        Command& current() override;
        unsigned long step() override;
        void cursor(unsigned long cursor) override;

    private:
        std::vector<std::shared_ptr<Command>> m_commands;
        unsigned long m_cursor;
        bool m_looping;
    };

    /**
     * @brief A command stream that holds a reference to an instrument. This
     * allows for commands that don't include the instrument number.
     */
    class CommandStreamInstrument : public CommandStream
    {
    public:
        explicit CommandStreamInstrument(std::vector<std::shared_ptr<Command>> commands,
            Instrument& ins, bool looping = false);
        ~CommandStreamInstrument();

        Command& current() override;
        unsigned long step() override;
        void cursor(unsigned long cursor) override;

        void instrument(Instrument& ins);
        Instrument& instrument();

    private:
        std::vector<std::shared_ptr<Command>> m_commands;
        unsigned long m_cursor;
        bool m_looping;

        std::reference_wrapper<Instrument> m_ins;
    };

};

#endif // COMMAND_STREAM_H_
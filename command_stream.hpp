#ifndef COMMAND_STREAM_H_
#define COMMAND_STREAM_H_

#include <variant>
#include <vector>
#include <memory>

#include "command.hpp"

namespace MusicLib
{
    class Instrument;

    class CommandStream
    {
    public:
        CommandStream() = default;
        virtual ~CommandStream() = default;

        virtual Command& current() = 0;
        virtual unsigned long step() = 0;
        virtual void cursor(unsigned long index) = 0;
    };

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

        std::shared_ptr<Instrument> m_ins;
    };

};

#endif // COMMAND_STREAM_H_
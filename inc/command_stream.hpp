#ifndef COMMAND_STREAM_H_
#define COMMAND_STREAM_H_

#include <variant>
#include <vector>
#include <memory>
#include <functional>

#include "command.hpp"
#include "instrument.hpp"

namespace MusicLib {

/**
 * @brief An interface for classes that contain an organized sequence of 
 * commands, to be read and acted upon by the sequencer.
 */
class CommandStream
{
public:
    CommandStream() = default;
    virtual ~CommandStream() = default;

    virtual void add(Command& command) = 0;

    virtual Command* current() const = 0;
    virtual bool finished() const = 0;

    virtual void reset() = 0;
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
    explicit CommandStreamBasic(bool looping = false);
    explicit CommandStreamBasic(std::vector<std::unique_ptr<Command>>& commands, bool looping = false);
    ~CommandStreamBasic() noexcept = default;
    CommandStreamBasic(const CommandStreamBasic& other);
    CommandStreamBasic& operator=(const CommandStreamBasic& other);
    CommandStreamBasic(CommandStreamBasic&&) noexcept = default;
    CommandStreamBasic& operator=(CommandStreamBasic&&) noexcept = default;

    void add(Command& command) override;

    Command* current() const override;
    bool finished() const override;

    void reset() override;
    unsigned long step() override;
    void cursor(unsigned long cursor) override;

private:
    std::vector<std::unique_ptr<Command>> m_commands;
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
    explicit CommandStreamInstrument(IInstrument& ins, bool looping = false);
    explicit CommandStreamInstrument(std::vector<std::unique_ptr<Command>>& commands, IInstrument& ins, bool looping = false);
    ~CommandStreamInstrument() noexcept = default;
    CommandStreamInstrument(const CommandStreamInstrument& other);
    CommandStreamInstrument& operator=(const CommandStreamInstrument& other);
    CommandStreamInstrument(CommandStreamInstrument&&) noexcept = default;
    CommandStreamInstrument& operator=(CommandStreamInstrument&&) noexcept = default;

    void add(Command& command) override;

    Command* current() const override;
    bool finished() const override;
    
    void reset() override;
    unsigned long step() override;
    void cursor(unsigned long cursor) override;

    void instrument(IInstrument& ins);
    IInstrument& instrument();

private:
    std::vector<std::unique_ptr<Command>> m_commands;
    unsigned long m_cursor;
    bool m_looping;

    std::reference_wrapper<IInstrument> m_ins;
};

};

#endif // COMMAND_STREAM_H_
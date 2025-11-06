#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "command.hpp"
#include "command_stream.hpp"
#include "command_processor.hpp"
#include "device.hpp"
#include "time_manager.hpp"

#include <functional>
#include <memory>
#include <atomic>
#include <thread>
#include <vector>

using Command = std::pair<size_t, float>;
using Sequence = std::vector<std::vector<Command>>;

namespace MusicLib {
    
/**
 * @brief A sequencer interface that can be called from the audio engine
 * callback.
 * 
 */
class Sequencer
{
public:
    Sequencer() = default;
    virtual ~Sequencer() = default;

    /**
     * @brief A method to call from the audio engine callback for every
     * sample, to handle the progression of the track in time.
     */
    virtual void tick() = 0;

    /**
     * @brief Perform a single step in the arrangement.
     * Generally called from tick(), but kept public for the option
     * to progress the track manually.
     */
    virtual void step() = 0;

    
    virtual void reset() = 0;
};

/**
 * @brief Runs multiple sequencers in parallel, each of which handles its own
 * time.
 * 
 */
class MultiSequencer : public Sequencer
{
public:
    explicit MultiSequencer(std::vector<Sequencer>& seqs);
    ~MultiSequencer() noexcept = default;

    /**
     * @brief Call the tick() method of each of the sequencers.
     * 
     */
    void tick() override;

    /**
     * @brief Perform a step in each of the sequencers. This function is
     *        only here for completeness.
     */
    void step() override;

    /**
     * @brief Reset all sequencers.
     * 
     */
    void reset() override;

private:
    std::vector<Sequencer>& m_seqs;
};

/**
 * @brief A simple sequencer class - one time manager, one command stream,
 * performs actions once a step.
 */
class SequencerBasic : public Sequencer
{
public:
    explicit SequencerBasic(TimeManager& time_mgr,
        IDevice& device,
        CommandStream& cmd_stream,
        CommandProcessor& cmd_processor);
    ~SequencerBasic() noexcept = default;

    void tick() override;

    /**
     * @brief Receive a command from the command stream, call the command
     * processor's handlers to execute it and progress the command stream
     * one step.
     */
    void step() override;

    void reset() override;

private:
    TimeManager& m_time_mgr;
    CommandStream& m_cmd_stream;
    std::reference_wrapper<IDevice> m_device;
    CommandProcessor& m_cmd_processor;
};

/**
 * @brief A sequencer that can hold multiple command streams and play them
 * in parallel.
 */
class SequencerMultiChannel : public Sequencer
{
public:
    explicit SequencerMultiChannel(TimeManager& time_mgr, IDevice& device,
        std::vector<CommandStream>& cmd_streams, CommandProcessor& cmd_processor);
    ~SequencerMultiChannel() noexcept = default;

    void tick() override;

    /**
     * @brief Receive and execute a command from each of the command
     *        streams.
     */
    void step() override;

    /**
     * @brief Reset all channels (return their playhead to the first command)
     * 
     */
    void reset() override;

private:
    TimeManager& m_time_mgr;
    std::vector<CommandStream>& m_cmd_streams;
    std::reference_wrapper<IDevice> m_device;
    CommandProcessor& m_cmd_processor;
};

}

#endif // SEQUENCER_H_
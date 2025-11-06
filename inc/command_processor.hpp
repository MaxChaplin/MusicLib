#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

#include "command_stream.hpp"
#include "device.hpp"
#include "time_manager.hpp"

#include <memory>
#include <variant>
#include <functional>

namespace MusicLib {
    
/**
 * @brief A command processor is given to the sequencer, which invokes the 
 * handle functions in its step() method.
 * 
 */
class CommandProcessor
{
public:
    CommandProcessor() = default;
    virtual ~CommandProcessor() = default;

    virtual void handle_command_stream(Command& cmd, CommandStream& cmd_stream) = 0;
    virtual void handle_device(Command& cmd, IDevice& device) = 0;
    virtual void handle_time_manager(Command& cmd, TimeManager& time_mgr) = 0;
};

/**
 * @brief This command processor supports one concrete type for Command and
 * each of the other handled objects.
 * The handle functions must be given before playing.
 * Since the handle methods perform upcasting, it is the responsibility of
 * the given handlers to ensure that every command can be executed safely
 * (e.g. if one of the commands calls an device's method, all 
 * devices have it.)
 */
class CommandProcessorBasic : public CommandProcessor
{
public:
    explicit CommandProcessorBasic();
    ~CommandProcessorBasic() noexcept = default;

    void handle_command_stream(Command& cmd, CommandStream& cmd_stream) override;
    void handle_device(Command& cmd, IDevice& device) override;
    void handle_time_manager(Command& cmd, TimeManager& time_mgr) override;

    template <typename C, typename CS>
    void set_command_stream_handler(std::function<void(C&, CS&)> cmd_stream_handler)
    {
        m_cmd_stream_handler = [cmd_stream_handler](Command& cmd, CommandStream& cmd_stream)
        {
            auto& cmd_cast = static_cast<C&>(cmd);
            auto& cmd_stream_cast = static_cast<CS&>(cmd_stream);
            cmd_stream_handler(cmd_cast, cmd_stream_cast);
        };
    }

    template <typename C, typename DM>
    void set_device_handler(std::function<void(C&, DM&)> device_handler)
    {
        m_device_handler = [device_handler](Command& cmd, IDevice& device)
        {
            auto& cmd_cast = static_cast<C&>(cmd);
            auto& device_cast = static_cast<DM&>(device);
            device_handler(cmd_cast, device_cast);
        };
    }

    template <typename C, typename TM>
    void set_time_handler(std::function<void(C&, TM&)> time_handler)
    {
        m_time_handler = [time_handler](Command& cmd, TimeManager& time_mgr)
        {
            auto& cmd_cast = static_cast<C&>(cmd);
            auto& time_mgr_cast = static_cast<TM&>(time_mgr);
            time_handler(cmd_cast, time_mgr_cast);
        };
    }

private:
    std::function<void(Command&, CommandStream&)> m_cmd_stream_handler;
    std::function<void(Command&, IDevice&)> m_device_handler;
    std::function<void(Command&, TimeManager&)> m_time_handler;
};

}



#endif // COMMAND_PROCESSOR_H_
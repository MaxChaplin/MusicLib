#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

#include "command_stream.hpp"
#include "instrument_manager.hpp"
#include "time_manager.hpp"

#include <memory>
#include <variant>
#include <functional>

namespace MusicLib
{
    // class CommandProcessor
    // {
    // public:
    //     CommandProcessor();
    //     virtual ~CommandProcessor() = default;

    //     template <typename Command, typename CommandStream>
    //     void handle_command_stream(Command& cmd, CommandStream& cmd_stream);

    //     template <typename Command, typename InstrumentManager>
    //     void handle_instrument_manager(Command& cmd, InstrumentManager& ins_mgr);

    //     template <typename Command, typename TimeManager>
    //     void handle_time_manager(Command& cmd, TimeManager& time_mgr);

    //     // void operator()(const Command& cmd)
    //     // {
    //     //     std::visit(*this, cmd);
    //     // }

    //     // void operator()(const CommandNoteOn& cmd);
    //     // void operator()(const CommandNoteOnFreq& cmd);
    //     // void operator()(const CommandNoteOff& cmd);
    //     // void operator()(const CommandVol& cmd);
    // };

    // class CommandProcessorBasic : public CommandProcessor
    // {
    // public:
    //     explicit CommandProcessorBasic();
    //     ~CommandProcessorBasic() noexcept = default;

    //     template <typename Command, typename CommandStream>
    //     void handle_command_stream(Command& cmd, CommandStream& cmd_stream);

    //     template <typename Command, typename InstrumentManager>
    //     void handle_instrument_manager(Command& cmd, InstrumentManager& ins_mgr);

    //     template <typename Command, typename TimeManager>
    //     void handle_time_manager(Command& cmd, TimeManager& time_mgr);
    
    // private:
    //     std::function<void(Command* cmd, CommandStream* cmd_stream)> m_cmd_stream_handler;
    //     std::function<void(Command* cmd, TimeManager* ins_mgr)> m_ins_mgr_handler;
    //     std::function<void(Command* cmd, TimeManager* time_manager)> m_time_handler;
    // };

    class CommandProcessor
    {
    public:
        CommandProcessor() = default;
        virtual ~CommandProcessor() = default;

        virtual void handle_command_stream(Command& cmd, CommandStream& cmd_stream) = 0;
        virtual void handle_instrument_manager(Command& cmd, InstrumentManager& ins_mgr) = 0;
        virtual void handle_time_manager(Command& cmd, TimeManager& time_mgr) = 0;
    };

    // template <typename C, typename CS, typename IM, typename TM>
    // class CommandProcessorBasic : public CommandProcessor
    // {
    // public:
    //     explicit CommandProcessorBasic();
    //     ~CommandProcessorBasic() noexcept = default;

        
    //     void handle_command_stream(C& cmd, CS& cmd_stream) override;
    //     void handle_instrument_manager(C& cmd, IM& ins_mgr) override;
    //     void handle_time_manager(C& cmd, TM& time_mgr) override;
    // };

    class CommandProcessorBasic : public CommandProcessor
    {
    public:
        // explicit CommandProcessorBasic(auto m_cmd_stream_handler = [](){},        auto m_ins_mgr_handler = [](){},  auto m_time_handler = [](){});
        explicit CommandProcessorBasic();
        ~CommandProcessorBasic() noexcept = default;

        void handle_command_stream(Command& cmd, CommandStream& cmd_stream) override;
        void handle_instrument_manager(Command& cmd, InstrumentManager& ins_mgr) override;
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

        template <typename C, typename IM>
        void set_instrument_handler(std::function<void(C&, IM&)> ins_mgr_handler)
        {
            m_ins_mgr_handler = [ins_mgr_handler](Command& cmd, InstrumentManager& ins_mgr)
            {
                auto& cmd_cast = static_cast<C&>(cmd);
                auto& ins_mgr_cast = static_cast<IM&>(ins_mgr);
                ins_mgr_handler(cmd_cast, ins_mgr_cast);
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
        std::function<void(Command&, InstrumentManager&)> m_ins_mgr_handler;
        std::function<void(Command&, TimeManager&)> m_time_handler;
    };

}



#endif // COMMAND_PROCESSOR_H_
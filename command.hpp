#ifndef COMMAND_H_
#define COMMAND_H_

#include <variant>

namespace MusicLib
{

    // struct CommandNoteOn
    // {
    //     unsigned int pitch;
    // };

    // struct CommandNoteOnFreq
    // {
    //     float freq;
    // };

    // struct CommandNoteOff
    // {
    // };

    // struct CommandNotePlay
    // {
    //     unsigned int pitch;
    //     unsigned int duration;
    // };

    // struct CommandNotePlayFreq
    // {
    //     float freq;
    //     unsigned int duration;
    // };

    // struct CommandVol
    // {
    //     float vol;
    // };

    // using Command = std::variant<CommandNoteOn,
    //                              CommandNoteOnFreq,
    //                              CommandNoteOff,
    //                              CommandNotePlay,
    //                              CommandNotePlayFreq,
    //                              CommandVol,
    //                              >;

    // class Command
    // {
    // public:
    //     Command() = default;
    //     virtual ~Command() = default;

    //     virtual int type() const noexcept = 0;
    //     virtual const void* args() const noexcept = 0;
    // };

    struct Command
    {

    };
};

#endif // COMMAND_H_
#include "command.hpp"
#include <memory>

enum class Waveshape
{
    Tri,
    Saw,
    Pulse
};

struct CommandDemo : public MusicLib::Command
{
public:
    std::unique_ptr<Command> clone() const override
    {
        return std::make_unique<CommandDemo>(*this);
    } 

public:
    enum class Type
    {
        Note,
        Pan,
        Vol,
        Waveshape,

        // Envelope
        Attack,
        Decay,
        Sustain,
        Release
    } type;

    union 
    {
        struct
        {
            unsigned int ins;
            unsigned int pitch;
            float duration;
        } note;

        struct
        {
            unsigned int ins;
            float amount;
        } param_float;

        struct
        {
            unsigned int ins;
            float duration;
        } param_time;

        struct
        {
            unsigned int ins;
            Waveshape shape;
        } wave;
    };
};
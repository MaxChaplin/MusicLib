#ifndef COMMAND_H_
#define COMMAND_H_

#include <memory>

namespace MusicLib {

struct Command
{
    virtual std::unique_ptr<Command> clone() const = 0; 
};

};

#endif // COMMAND_H_
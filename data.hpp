#ifndef DATA_H_
#define DATA_H_

#include "instrument.hpp"
#include "sequencer.hpp"

namespace MusicLib
{
    struct PortAudioData
    {
        Sequencer& seq;
        InstrumentManager& ins_mgr;
        float time_element;
    };
}

#endif // DATA_H_
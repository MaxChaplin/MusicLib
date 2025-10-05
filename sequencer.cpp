#include "sequencer.hpp"
#include "instrument.hpp"

#include <portaudio.h>
#include <iostream>

// static void manage_error(PaError err)
// {
//     if(err != paNoError)
//     {
//         std::cerr << "PortAudio error " << err << std::endl;
//     }
    
// }
namespace MusicLib
{
    Sequencer::Sequencer(float bpm, unsigned int steps_per_beat,
        unsigned int sample_rate, InstrumentManager& instrument_manager,
        Sequence& sequence)
    : m_sequence{sequence}
    , m_cursor{0}
    , m_instrument_manager{instrument_manager}
    , m_bpm{bpm}
    , m_steps_per_beat{steps_per_beat}
    , m_step_duration{60.0f / (bpm * steps_per_beat)}
    , m_sample_rate{sample_rate}
    , m_samples_per_step{(unsigned long) (m_step_duration * sample_rate)}
    , m_step_counter{0}
    {

    }

    void Sequencer::step()
    {
        auto commands = m_sequence[m_cursor++ % m_sequence.size()];

        for (auto it = commands.begin(); it != commands.end(); ++it)
        {
            m_instrument_manager.instrument(0).note_on(it->first, it->second);
            std::cout << "ins " << it->first << " freq " << it->second << std::endl;
        }
    }


    float Sequencer::beat_duration()
    {
        return 60.0 / m_bpm;
    }

    void Sequencer::handle_sample()
    {
        m_step_counter++;

        if (m_step_counter >= m_samples_per_step)
        {
            m_step_counter = 0;
            step();
        }
    }

    const float& Sequencer::bpm() const
    {
        return m_bpm;
    }

    float& Sequencer::bpm(float bpm)
    {
        if (bpm < 0)
        {
            return m_bpm;
        }
        
        m_bpm = bpm;
        m_step_duration = 60.0f / (bpm * m_steps_per_beat);
        m_samples_per_step = (unsigned long) (m_step_duration * m_sample_rate);
        // std::cout << m_step_duration << std::endl;

        return m_bpm;
    }

    Sequence& Sequencer::sequence()
    {
        return m_sequence;
    }
}
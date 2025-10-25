#include "command_demo.hpp"

#include "audio_manager_portaudio.hpp"
#include "command_stream.hpp"
#include "instrument.hpp"
#include "device_manager.hpp"
#include "sequencer.hpp"
#include "time_manager.hpp"
#include "pitch.hpp"
#include "voice.hpp"
#include "envelope.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <optional>

#include <portaudio.h>

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 512
#define NUM_INSTRUMENTS_MAX 32

/**
 * @brief 
 * 
 * @param tokens a string that contains a command identifier followed by
 *     arguments.
 *     Note command: N [instrument] [duration] [pitch]
 *     Volume command: V [instrument] [volume]
 * @return CommandDemo 
 */
std::optional<CommandDemo> make_command(std::vector<std::string> tokens, unsigned int& ins_num)
{
    CommandDemo cmd;

    try
    {
        switch (tokens[0][0])
        {
        case 'N':
            cmd.type = CommandDemo::Type::Note;
            cmd.note.ins = std::stoi(tokens[1]);
            cmd.note.duration = std::stof(tokens[2]);
            cmd.note.pitch = std::stoi(tokens[3]);

            ins_num = cmd.note.ins;
            break;
            
        case 'P':
            cmd.type = CommandDemo::Type::Pan;
            cmd.param_float.ins = std::stoi(tokens[1]);
            cmd.param_float.amount = std::stof(tokens[2]);

            ins_num = cmd.param_float.ins;
            break;
    
        case 'V':
            cmd.type = CommandDemo::Type::Vol;
            cmd.param_float.ins = std::stoi(tokens[1]);
            cmd.param_float.amount = std::stof(tokens[2]);

            ins_num = cmd.param_float.ins;
            break;
    
        case 'W':
            cmd.type = CommandDemo::Type::Waveshape;
            cmd.wave.ins = std::stoi(tokens[1]);
            cmd.wave.shape = (Waveshape) std::stoi(tokens[2]);

            ins_num = cmd.wave.ins;
            break;

        case 'A':
            cmd.type = CommandDemo::Type::Attack;
            cmd.param_float.ins = std::stoi(tokens[1]);
            cmd.param_float.amount = std::stof(tokens[1]);

            ins_num = cmd.param_float.ins;
            break;
        
        case 'D':
            cmd.type = CommandDemo::Type::Decay;
            cmd.param_float.ins = std::stoi(tokens[1]);
            cmd.param_float.amount = std::stof(tokens[1]);

            ins_num = cmd.param_float.ins;
            break;

        case 'S':
            cmd.type = CommandDemo::Type::Sustain;
            cmd.param_float.ins = std::stoi(tokens[1]);
            cmd.param_float.amount = std::stof(tokens[1]);

            ins_num = cmd.param_float.ins;
            break;

        case 'R':
            cmd.type = CommandDemo::Type::Release;
            cmd.param_float.ins = std::stoi(tokens[1]);
            cmd.param_float.amount = std::stof(tokens[1]);
            
            ins_num = cmd.param_float.ins;
            break;
    
        default:
            return std::nullopt;
        }

        if (ins_num >= NUM_INSTRUMENTS_MAX)
        {
            // This instrument number is too large. This command will be
            // ignored.
            ins_num = 0;
            return std::nullopt;
        }
    }
    catch(const std::exception& e)
    {
        return std::nullopt;
    }
    

    return cmd;
}

MusicLib::CommandStreamBasic parse_file(std::string song_filename, unsigned int& max_ins_num)
{
    auto cmd_stream = MusicLib::CommandStreamBasic{true};

    std::ifstream file(song_filename);
    std::string line;
    std::string token;
    std::vector<std::string> tokens;
    unsigned int ins_num = 0;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::stringstream ss(line);

            while (std::getline(ss, token, ' '))
            {
                tokens.push_back(token);
            }
            if (tokens.size() > 0)
            {
                auto cmd = make_command(tokens, ins_num);
                
                if (cmd.has_value())
                {
                    cmd_stream.add(cmd.value());

                    // Check if number of instruments need to get increased.
                    if (ins_num > max_ins_num)
                    {
                        max_ins_num = ins_num;
                    }
                }
                tokens = {};
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "File not found." << std::endl;
    }

    // auto cmd_stream = MusicLib::CommandStreamBasic{commands, true};
    return cmd_stream;
}

void handle_command_stream(CommandDemo& cmd [[maybe_unused]], MusicLib::CommandStreamBasic& cmd_stream  [[maybe_unused]])
{

}

void handle_instrument_manager(CommandDemo& cmd, MusicLib::InstrumentManager& ins_mgr)
{
    float freq;

    switch (cmd.type)
    {
    case CommandDemo::Type::Note:
        freq = MusicLib::freq_from_pitch[cmd.note.pitch];
        if (freq > 0)
        {
            ins_mgr
            .instrument<MusicLib::InstrumentMono>(cmd.wave.ins)
            .note_on(freq);
        }
        else
        {
            ins_mgr
            .instrument<MusicLib::InstrumentMono>(cmd.wave.ins)
            .note_off();
        }

        return;

    case CommandDemo::Type::Pan:
        ins_mgr
        .instrument(cmd.note.ins)
        .pan(cmd.param_float.amount);
        return;

    case CommandDemo::Type::Vol:
        // ins_mgr.instrument(cmd.wave.ins).call_all_voices<MusicLib::VoiceOsc>(&MusicLib::VoiceOsc::vol, cmd.param_float.amount);
        ins_mgr
        .instrument(cmd.wave.ins)
        .vol(cmd.param_float.amount);
        return;
    
    case CommandDemo::Type::Waveshape:
        ins_mgr
        .instrument<MusicLib::InstrumentMono>(cmd.wave.ins)
        .voice<MusicLib::VoiceOsc>()
        .osc<MusicLib::OscillatorSwitch>()
        .select((int) cmd.wave.shape);
        return;

    case CommandDemo::Type::Attack:
        ins_mgr
        .instrument<MusicLib::InstrumentMono>(cmd.wave.ins)
        .voice<MusicLib::VoiceOsc>()
        .env<MusicLib::EnvelopeADSR>()
        .attack(cmd.param_time.duration);
        // ins_mgr.instrument(cmd.wave.ins)
        return;
    
    case CommandDemo::Type::Decay:
        ins_mgr
        .instrument<MusicLib::InstrumentMono>(cmd.wave.ins)
        .voice<MusicLib::VoiceOsc>()
        .env<MusicLib::EnvelopeADSR>()
        .decay(cmd.param_time.duration);
        return;
    
    case CommandDemo::Type::Sustain:
        ins_mgr.instrument<MusicLib::InstrumentMono>(cmd.wave.ins)
        .voice<MusicLib::VoiceOsc>()
        .env<MusicLib::EnvelopeADSR>()
        .sustain(cmd.param_float.amount);
        return;

    case CommandDemo::Type::Release:
        ins_mgr.instrument<MusicLib::InstrumentMono>(cmd.wave.ins)
        .voice<MusicLib::VoiceOsc>()
        .env<MusicLib::EnvelopeADSR>()
        .release(cmd.param_time.duration);
        return;

    default:
        return;
    }
}

void handle_time_manager(CommandDemo& cmd, MusicLib::TimeManagerEventBased& time_mgr)
{
    switch (cmd.type)
    {
    case CommandDemo::Type::Note:
        time_mgr.reset_counter(cmd.note.duration * SAMPLE_RATE);
        return;

    case CommandDemo::Type::Vol:
        return;        

    default:
        return;
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cout << "Please give a song file as an argument." << std::endl;
        return 0;
    }
    std::string song_filename(argv[1]);

    unsigned int max_ins_num = 0; // Largest instrument number
    MusicLib::CommandStreamBasic cmd_stream = parse_file(song_filename, max_ins_num);

    MusicLib::CommandProcessorBasic cmd_processor;
    cmd_processor.set_command_stream_handler<CommandDemo, MusicLib::CommandStreamBasic>(handle_command_stream);
    cmd_processor.set_device_manager_handler<CommandDemo, MusicLib::InstrumentManager>(handle_instrument_manager);
    cmd_processor.set_time_handler<CommandDemo, MusicLib::TimeManagerEventBased>(handle_time_manager);

    MusicLib::InstrumentManager ins_mgr{BUFFER_SIZE};
    MusicLib::OscillatorBasic osc_triangle(MusicLib::osc_triangle);
    MusicLib::OscillatorBasic osc_saw(MusicLib::osc_saw);
    MusicLib::OscillatorBasic osc_square(MusicLib::osc_square);
    
    MusicLib::OscillatorSwitch osc_switch;
    osc_switch.add_osc(osc_triangle);
    osc_switch.add_osc(osc_saw);
    osc_switch.add_osc(osc_square);

    MusicLib::EnvelopeADSR env{.01, 2, .2, .5};
    MusicLib::VoiceOsc voice{osc_switch, env};
    MusicLib::InstrumentMono ins{voice, 1};

    for (size_t i = 0; i <= max_ins_num; ++i)
    {
        ins_mgr.clone_instrument(ins);
    }

    // Parse the song file.
    std::cout << "Loading song: " << song_filename << std::endl;


    MusicLib::TimeManagerEventBased time_mgr;
    MusicLib::SequencerBasic seq{time_mgr, ins_mgr, cmd_stream, cmd_processor};

    // Set audio manager.
    MusicLib::PortAudioDataOut data{seq, ins_mgr, 1. / SAMPLE_RATE};
    MusicLib::AudioManagerPortAudio audio_manager{SAMPLE_RATE, BUFFER_SIZE, data};

    char input;
    audio_manager.start();
    bool playing = false;

    std::cout << "Options: [p]lay/pause, [s]top, [q]uit" <<std::endl;

    do
    {
        time_mgr.playing(playing);
        std::cin.get(input);

        switch(input)
        {
        case 'p':
            playing = !playing;
            break;

        case 's':
            playing = false;
            time_mgr.playing(false);
            seq.reset();
            break;            
        }
    } while(playing);
    

    // audio_manager.stop();

    

    return 0;
}
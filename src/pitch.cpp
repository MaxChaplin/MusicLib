#include "pitch.hpp"
#include "util.hpp"

#include <array>
#include <regex>
#include <string>
#include <optional>

namespace MusicLib {

std::optional<PitchDiatonic::Class> get_diatonic_pitch_class(char pitch_class_char)
{
    switch (pitch_class_char)
    {
    case 'C':
        return PitchDiatonic::Class::C;
    case 'D':
        return PitchDiatonic::Class::D;
    case 'E':
        return PitchDiatonic::Class::E;
    case 'F':
        return PitchDiatonic::Class::F;
    case 'G':
        return PitchDiatonic::Class::G;
    case 'A':
        return PitchDiatonic::Class::A;
    case 'B':
        return PitchDiatonic::Class::B;
    default:
        return std::nullopt;
    }
}

static auto get_diatonic_pitch_class_sharp = std::array<PitchDiatonic::Class, 12>{
    PitchDiatonic::Class::C,
    PitchDiatonic::Class::C,
    PitchDiatonic::Class::D,
    PitchDiatonic::Class::D,
    PitchDiatonic::Class::E,
    PitchDiatonic::Class::F,
    PitchDiatonic::Class::F,
    PitchDiatonic::Class::G,
    PitchDiatonic::Class::G,
    PitchDiatonic::Class::A,
    PitchDiatonic::Class::A,
    PitchDiatonic::Class::B
};

static auto get_diatonic_pitch_class_flat = std::array<PitchDiatonic::Class, 12>
{
    PitchDiatonic::Class::C,
    PitchDiatonic::Class::D,
    PitchDiatonic::Class::D,
    PitchDiatonic::Class::E,
    PitchDiatonic::Class::E,
    PitchDiatonic::Class::F,
    PitchDiatonic::Class::G,
    PitchDiatonic::Class::G,
    PitchDiatonic::Class::A,
    PitchDiatonic::Class::A,
    PitchDiatonic::Class::B,
    PitchDiatonic::Class::B
};

static auto get_accidental_sharp = std::array<PitchDiatonic::Accidental, 12>
{
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Sharp,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Sharp,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Sharp,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Sharp,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Sharp,
    PitchDiatonic::Accidental::Natural,
};

static auto get_accidental_flat = std::array<PitchDiatonic::Accidental, 12>
{
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Flat,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Flat,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Flat,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Flat,
    PitchDiatonic::Accidental::Natural,
    PitchDiatonic::Accidental::Flat,
    PitchDiatonic::Accidental::Natural,
};


std::optional<PitchDiatonic::Accidental> get_accidental(std::string accidental_str)
{
    if (accidental_str == "")
    {
        return PitchDiatonic::Accidental::Natural;
    }
    else if (accidental_str == "b")
    {
        return PitchDiatonic::Accidental::Flat;
    }
    else if (accidental_str == "#")
    {
        return PitchDiatonic::Accidental::Sharp;
    }
    else if (accidental_str == "bb")
    {
        return PitchDiatonic::Accidental::DoubleFlat;
    }
    else if (accidental_str == "x")
    {
        return PitchDiatonic::Accidental::DoubleSharp;
    }
    else
    {
        return std::nullopt;
    }
}

std::optional<PitchDiatonic> parse_diatonic_pitch(std::string& str)
{
    std::regex pattern("^([A-F])(#|b{,2}|x?)(-?[0-9]+)$");
    std::smatch matches;
    
    if (std::regex_match(str, matches, pattern))
    {
        char pitch_class_char = matches[1].str()[0];
        std::string accidental_str = matches[2];
        std::string octave_str = matches[3];

        auto pitch_class = get_diatonic_pitch_class(pitch_class_char);
        auto accidental = get_accidental(accidental_str);
        int octave = std::stoi(octave_str);

        if (pitch_class.has_value() && accidental.has_value())
        {
            return PitchDiatonic(pitch_class.value(), accidental.value(), octave);
        }
    }
   
    return std::nullopt;
}

PitchChromatic::PitchChromatic(Class pitch_class, int octave)
: m_class{pitch_class}
, m_octave{octave}
{

}

PitchChromatic::PitchChromatic(unsigned int midi_value)
: m_class{(Class) (midi_value % 12)}
, m_octave{(int) midi_value / 12 - 1}
{
    
}

unsigned int PitchChromatic::midi_value() const
{
    return (m_octave + 1) * 12 + (int) m_class;
}

float PitchChromatic::freq() const
{
    return freq_from_pitch[midi_value()];
}

void PitchChromatic::transpose(int amount)
{
    *this = PitchChromatic(midi_value() + amount);
}

PitchDiatonic::PitchDiatonic(Class pitch_class, Accidental m_acc, int octave)
: m_class{pitch_class}
, m_acc{m_acc}
, m_octave{octave}
{

}

PitchDiatonic::PitchDiatonic(unsigned int midi_value, bool use_flats)
: m_class{}
, m_acc{}
, m_octave{(int) midi_value / 12 - 1}
{
    if (use_flats)
    {
        m_class = get_diatonic_pitch_class_flat[midi_value % 12];
        m_acc = get_accidental_flat[midi_value % 12];
    }
    else
    {
        m_class = get_diatonic_pitch_class_sharp[midi_value % 12];
        m_acc = get_accidental_sharp[midi_value % 12];
    }
}

unsigned int PitchDiatonic::midi_value() const
{
    return (m_octave + 1) * 12 + (int) m_class + (int) m_acc;
}

float PitchDiatonic::freq() const
{
    return freq_from_pitch[midi_value()];
}

void PitchDiatonic::transpose(int amount)
{
    *this = PitchDiatonic(midi_value() + amount);
}

}
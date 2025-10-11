#ifndef VOICE_H_
#define VOICE_H_

#include "envelope.hpp"
#include "osc.hpp"
#include "wave_shaper.hpp"

#include <functional>
#include <memory>

namespace MusicLib
{
    class Voice
    {
    public:
        Voice() = default;
        virtual ~Voice() = default;

        virtual std::shared_ptr<Voice> clone() const = 0;

        virtual Envelope& env() = 0;

        virtual void freq(float m_freq) = 0;
        virtual float freq() const = 0;

        virtual void vol(float m_vol) = 0;

        virtual void note_on(float freq) = 0;
        virtual void note_off() = 0;

        virtual bool is_on() const = 0;

        virtual float process(float sample_time) = 0;
    };

    class VoiceOsc : public Voice
    {
    public:
        explicit VoiceOsc(Oscillator& m_osc, Envelope& env, float freq = 440, float vol = 1.);
        ~VoiceOsc() noexcept = default;

        std::shared_ptr<Voice> clone() const override;

        Envelope& env() override;

        void freq(float m_freq) override;
        float freq() const override;

        void vol(float m_vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        float process(float sample_time) override;

    private:
        std::shared_ptr<Oscillator> m_osc;
        std::shared_ptr<Envelope> m_env;
        float m_freq;
        float m_phase;
        float m_vol;
    };

    class VoiceMulti : public Voice
    {
    public:
        explicit VoiceMulti(std::vector<std::shared_ptr<Voice>>& voices, Envelope& env, float freq = 440, float vol = 1.);
        ~VoiceMulti() noexcept = default;

        std::shared_ptr<Voice> clone() const override;

        Envelope& env() override;

        void freq(float m_freq) override;
        float freq() const override;

        void vol(float m_vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        float process(float sample_time) override;

    private:
        std::vector<std::shared_ptr<Voice>> m_voices;
        std::shared_ptr<Envelope> m_env;
        float m_freq;
        float m_phase;
        float m_vol;
    };

    class VoiceFM2 : public Voice
    {
    public:
        explicit VoiceFM2(Voice& carrier, Voice& modulator, Envelope env, float freq1 = 440, float freq2 = 440, float fm_level = 0, float vol = 1., unsigned int oversamping = 1);
        ~VoiceFM2() noexcept = default;

        std::shared_ptr<Voice> clone() const override;

        Envelope& env() override;

        void freq(float m_freq) override;
        float freq() const override;

        void vol(float m_vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        float process(float sample_time) override;

    private:
        std::shared_ptr<Voice> m_carrier;
        std::shared_ptr<Voice> m_modulator;
        std::shared_ptr<Envelope> m_env;

        float m_freq;
        float m_phase;
        float m_vol;
        float m_fm_level;
        unsigned int m_oversamping;
    };

    class VoicePD : public Voice
    {
    public:
        explicit VoicePD(Oscillator& m_osc, WaveShaper& phase_func, Envelope env, float freq = 440, float vol = 1.);
        ~VoicePD() noexcept = default;

        std::shared_ptr<Voice> clone() const override;

        Envelope& env() override;

        void freq(float m_freq) override;
        float freq() const override;
        
        void vol(float m_vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        float process(float sample_time) override;

    private:
        std::shared_ptr<Oscillator> m_osc;
        std::shared_ptr<WaveShaper> m_phase_func;
        std::shared_ptr<Envelope> m_env;
        float m_freq;
        float m_phase;
        float m_vol;
    };

}

#endif // VOICE_H_
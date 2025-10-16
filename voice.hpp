#ifndef VOICE_H_
#define VOICE_H_

#include "envelope.hpp"
#include "osc.hpp"
#include "wave_shaper.hpp"

#include <functional>
#include <memory>

namespace MusicLib
{
    /**
     * @brief An interface for the primary component of an instrument, where the
     * synthesis is implemented. Typically monophonic, with polyphony
     * implemented by having the instrument hold multiple objects.
     */
    class Voice
    {
    public:
        Voice() = default;
        virtual ~Voice() = default;

        virtual std::unique_ptr<Voice> clone() const = 0;

        virtual Envelope& env() = 0;

        virtual void freq(float m_freq) = 0;
        virtual float freq() const = 0;

        virtual void vol(float m_vol) = 0;

        virtual void note_on(float freq) = 0;
        virtual void note_off() = 0;

        virtual bool is_on() const = 0;

        /**
         * @brief Progress the voice's audio signal a single sample.
         * 
         * @param sample_time The length of a sample in seconds (the inverse 
         * of the sample rate).
         * @param output 
         */
        virtual void process(float sample_time, float& output) = 0;
    };

    /**
     * @brief A voice based around an oscillator object, with a single velocity
     * envelope.
     */
    class VoiceOsc : public Voice
    {
    public:
        explicit VoiceOsc(Oscillator& m_osc, Envelope& env, float freq = 440,
                          float vol = 1.);
        ~VoiceOsc() noexcept = default;
        VoiceOsc(const VoiceOsc& other);
        VoiceOsc& operator=(const VoiceOsc& other);
        VoiceOsc(VoiceOsc&&) noexcept = default;
        VoiceOsc& operator=(VoiceOsc&&) noexcept = default;

        std::unique_ptr<Voice> clone() const override;

        Envelope& env() override;

        void freq(float freq) override;
        float freq() const override;

        void vol(float vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        void process(float sample_time, float& output) override;

    private:
        std::unique_ptr<Oscillator> m_osc;
        std::unique_ptr<Envelope> m_env;
        float m_freq;
        float m_phase;
        float m_vol;
    };

    /**
     * @brief An adaptor class for playing multiple voices in unison.
     * 
     */
    class VoiceMulti : public Voice
    {
    public:
        explicit VoiceMulti(std::vector<std::unique_ptr<Voice>>& voices,
                            Envelope& env, float freq = 440, float vol = 1.);
        ~VoiceMulti() noexcept = default;
        VoiceMulti(const VoiceMulti& other);
        VoiceMulti& operator=(const VoiceMulti& other);
        VoiceMulti(VoiceMulti&&) noexcept = default;
        VoiceMulti& operator=(VoiceMulti&&) noexcept = default;

        std::unique_ptr<Voice> clone() const override;

        Envelope& env() override;

        Voice& voice(size_t num);

        void freq(float m_freq) override;
        float freq() const override;

        void vol(float m_vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        void process(float sample_time, float& output) override;

    private:
        std::vector<std::unique_ptr<Voice>> m_voices;
        std::unique_ptr<Envelope> m_env;
        float m_freq;
        float m_phase;
        float m_vol;
    };

    /**
     * @brief The simplest FM synth.
     * 
     */
    class VoiceFM2 : public Voice
    {
    public:
        explicit VoiceFM2(Oscillator& carrier, Oscillator& modulator, Envelope& env,
            float freq_carrier = 440, float freq_modulator = 440,
            float fm_level = 0, float vol = 1., unsigned int oversamping = 1);
        ~VoiceFM2() noexcept = default;
        VoiceFM2(const VoiceFM2& other);
        VoiceFM2& operator=(const VoiceFM2& other);
        VoiceFM2(VoiceFM2&&) noexcept = default;
        VoiceFM2& operator=(VoiceFM2&&) noexcept = default;


        std::unique_ptr<Voice> clone() const override;

        Envelope& env() override;

        void freq(float m_freq) override;
        float freq() const override;

        void vol(float m_vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        void process(float sample_time, float& output) override;

    private:
        std::unique_ptr<Oscillator> m_carrier;
        std::unique_ptr<Oscillator> m_modulator;
        std::unique_ptr<Envelope> m_env;

        float m_freq;
        float m_phase;
        float m_vol;
        float m_fm_level;
        unsigned int m_oversamping;
    };

    /**
     * @brief A phase distortion synth. Passes the oscillator's phase parameter
     * through distortion function, which causes the wave shape to get
     * distorted.
     */
    class VoicePD : public Voice
    {
    public:
        explicit VoicePD(Oscillator& m_osc, WaveShaper& phase_func,
            Envelope& env, float freq = 440, float vol = 1.);
        ~VoicePD() noexcept = default;
        VoicePD(const VoicePD& other);
        VoicePD& operator=(const VoicePD& other);
        VoicePD(VoicePD&&) noexcept = default;
        VoicePD& operator=(VoicePD&&) noexcept = default;

        std::unique_ptr<Voice> clone() const override;

        Envelope& env() override;

        void freq(float freq) override;
        float freq() const override;
        
        void vol(float vol) override;

        void note_on(float freq) override;
        void note_off() override;

        bool is_on() const override;

        void process(float sample_time, float& output) override;

    private:
        std::unique_ptr<Oscillator> m_osc;
        std::unique_ptr<WaveShaper> m_phase_func;
        std::unique_ptr<Envelope> m_env;
        float m_freq;
        float m_phase;
        float m_vol;
    };

}

#endif // VOICE_H_
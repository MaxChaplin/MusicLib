#ifndef OSC_H_
#define OSC_H_

#include <functional>
#include <vector>
#include <memory>

namespace MusicLib
{
    float osc_tri(float phase);

    /**
     * @brief An oscillator interface. Used by the VoiceOsc class as the first
     * step of the synthesis chain. Generally doesn't handle time or frequency,
     * only the basic waveshape.
     */
    class Oscillator
    {
    public:
        Oscillator() = default;
        virtual ~Oscillator() = default;

        virtual std::shared_ptr<Oscillator> clone() const = 0;

        /**
         * @brief Give the value of the oscillator at the given phase.
         * 
         * @param phase a number between 0 (including) and 1 (not including).
         * The phase typically evolves in time like a upwards ramp function.
         * @return A sample. 
         */
        virtual float value(float phase) const = 0;
    };

    /**
     * @brief The simplest oscillator, revolving around a single pure function.
     */
    class OscillatorBasic : public Oscillator
    {
    public:
        explicit OscillatorBasic(std::function<float(float)> osc_func);
        ~OscillatorBasic() noexcept = default;

        std::shared_ptr<Oscillator> clone() const override;

        float value(float phase) const override;

    private:
        std::function<float(float)> m_osc_func;
    };

    class OscillatorWavetable : public Oscillator
    {
    public:
        explicit OscillatorWavetable(std::vector<float>& wavetable, bool antialiasing = false);
        ~OscillatorWavetable() noexcept = default;

        std::shared_ptr<Oscillator> clone() const override;

        float value(float phase) const override;

    private:
        std::vector<float> m_wavetable;
        bool m_antialiasing;
    };
}
#endif // OSC_H_
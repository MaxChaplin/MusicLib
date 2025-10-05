#ifndef PHASE_FUNCTION_H_
#define PHASE_FUNCTION_H_

#include <functional>
#include <vector>
#include <memory>

namespace MusicLib
{
    float osc_tri(float phase);

    class WaveShaper
    {
    public:
        WaveShaper() = default;
        virtual ~WaveShaper() = default;

        virtual std::shared_ptr<WaveShaper> clone() const = 0;

        virtual float value(float phase) const = 0;
    };

    class WaveShaperBasic : public WaveShaper
    {
    public:
        explicit WaveShaperBasic(std::function<float(float)> waveshaper_func);
        ~WaveShaperBasic() noexcept = default;

        std::shared_ptr<WaveShaper> clone() const override;

        float value(float phase) const override;

    private:
        std::function<float(float)> m_waveshaper_func;
    };

    class WaveShaperHardSync : public WaveShaper
    {
    public:
        explicit WaveShaperHardSync(float ratio);
        ~WaveShaperHardSync() noexcept = default;

        std::shared_ptr<WaveShaper> clone() const override;

        void ratio(float ratio);
        float value(float phase) const override;

    private:
        float m_ratio;
    };

}
#endif // PHASE_FUNCTION_H_
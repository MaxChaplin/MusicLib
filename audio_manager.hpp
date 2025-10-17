#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

namespace MusicLib
{
class AudioManager
    {
    public:
        AudioManager() = default;
        virtual ~AudioManager() = default;

        virtual void play() = 0;
        virtual void stop() = 0;

        virtual unsigned int sample_rate() = 0;
        virtual float sample_duration() = 0;
    };
}

#endif // AUDIO_MANAGER_H_
# MusicLib

This library is a collection of interfaces and class templates that can be used to build the engines of synthesizers, trackers and other musical apps. It aims to strike a balance between ease of use and flexibility.

Tested in Ubuntu 20.04.

Work in progress.

## Dependencies

* [PortAudio](https://github.com/PortAudio/portaudio)

## Building

In the main directory, run the following commands:

    cmake build
    cmake --build build

In the demos directory there are several example projects, each can be compiled
independently.

## Component overview

### Main compoments

The following components are expected to be present in any project that produces music, altough it's technically possible to design a project that skips any of these. All of the listed components are abstract interfaces with one or more implementation.

* **AudioManager** - top of a MusicLib project's structure, which operates the audio engine's stream. The audio callback function, which is passed by the adio manager to the audio engine, should generally call only two of the project's components - the main device and the main sequencer.
* **Device** - a component that can do DSP work, or relegate it to other components. The main device is used by the audio callback function to repeatedly calculate the next output samples. Typically, the main device serves as the manager of a system of secondary devices - instruments and effects.
* **Sequencer** - the component in charge of coordinating the arrangement's progress in time. Senses the passage of time through the `tick` method called by the audio callback function and acts accordingly.
* **TimeManager** - in charge of managing the tempo. Used by the sequencer to tell when to perform a step.
* **CommandStream** - contains the song in the form of a sequence of commands, or, in generative music, generates it according to internal logic. Several derived classes of CommandStream can be used to form a nested structure (e.g. a song with channels and sub-channels).
* **Command** - a single element of a musical arrangement (e.g. make instrument 1 play C#4, set the tempo to 112 BPM, transfer the playhead to the beginning with a probability of 0.5).
* **CommandProcessor** - the component that holds the logic of implementing commands on the command stream, time manager and device manager.

### Secondary components

These components are not essential, but highly helpful.

* **Instrument** - a device that produces audio output. A monophonic instrument holds a single voice; a polyphonic instrument - several of the same type.
* **Effect** - a device the processes incoming audio input.
* **DeviceChain** - a sequence of several devices that can be used to chain instruments and effects.
* **Voice** - a single sound-generating unit of the instrument. Holds a volume envelope.
* **Envelope** - a one-shot signal activated by a trigger, used to modulate parameters.
* **Oscillator** - a class used to generate repetitive and quasi-repetitive signals. Doesn't know about time or frequency, since both are factored into the phase parameter it receives.
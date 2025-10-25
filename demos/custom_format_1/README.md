# MusicLib demo - Custom Format 1

This is a demonstration of the ability to define a custom command set in MusicLib. The program reads a text file given to the program as an argument and parses it into a command stream. Lines that don't contain well-defined commands are ignored.

The number of instruments is set to accomodate the highest instrument number (e.g. if instrument number 10 is used then there will be 11 instruments, 0-10, regardless of whether the other numbers are used). The highest allowed number of instruments is 32 - a command with instrument no. 32 or higher will be ignored.

In order to build the project, in the demo directory. run

    cmake build
    cmake --build build

In order to execute the demo, run

    ./build/demo [song file]

Example songs are in the songs directory.

## Commands

* Note: N [instrument number] [duration] [pitch]
  * instrument number: integer
  * duration: float (in seconds) 
  * pitch: integer from 0 to 256 corresponding to the MIDI pitch.
* Volume: V [instrument number] [level]
  * instrument number: integer
  * level: float from 0 to 1
* Waveshape: W [instrument number] [wave shape]
  * instrument number: integer
  * wave shape: one of the following values:
    * 0: triangle
    * 1: saw
    * 2: square
* Pan: P [instrument number] [pan]
  * instrument number: integer
  * pan: float from 0 to 1. 0: left, 0.5: centered, 1: right.

Envelope commands:

* Attack: A [instrument number] [duration]
  * instrument number: integer
  * duration: float (in seconds)
* Decay: A [instrument number] [duration]
  * instrument number: integer
  * duration: float (in seconds)
* Sustain: A [instrument number] [level]
  * instrument number: integer
  * level: float from 0 to 1
* Release: A [instrument number] [duration]
  * instrument number: integer
  * duration: float (in seconds)
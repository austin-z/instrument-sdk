#ifndef GLOBAL_H
#define GLOBAL_H

#define NOT_EXPORT __attribute__((visibility("hidden")))

enum class Instrument : char
{
    Invalid = 0,
    Probe,
    Scissors,
    Forceps,
    Endoscope,
    Ultrasound,
};

#endif
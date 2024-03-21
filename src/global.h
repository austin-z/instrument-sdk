#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdint>

#define NOT_EXPORT __attribute__((visibility("hidden")))

enum class Instrument : uint8_t
{
    Invalid = 0,
    Probe,
    Scissors,
    Forceps,
    Endoscope,
    Ultrasound,
};

#endif
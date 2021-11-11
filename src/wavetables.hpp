#include "plugin.hpp"
#include "types.hpp"

static inline float wave_sine(phase phase)
{
    return std::sin(2.0f * M_PI * phase);
}

static inline float wave_square(phase phase)
{
    if (phase > 0.1f) return 1;
    if (phase < 0.1f) return 0;
    return (phase + 0.1f) / M_PI;
}

static inline float wave_wild_sine(phase phase)
{
    return (
       wave_sine(phase) +
       wave_sine(phase * 2.0f) +
       wave_sine(phase + 5.0f / 7)) / 3.0f;
}

static inline float wave_fm_one(phase phase)
{
    return (
        wave_sine(phase * 2.0f) +
        wave_sine(phase * 4.0f) +
        wave_sine(phase * 6.0f) +
        wave_sine(phase * 9.0f)) / 4.0f;
}
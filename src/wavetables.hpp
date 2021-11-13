#include "plugin.hpp"
#include "types.hpp"

namespace WaveShapes
{
    static inline float sin(phase phase)
    {
        return std::sin(2.0f * M_PI * phase);
    }

    static inline float triangle(phase phase)
    {
        return sin(phase);
    }

    static inline float square(phase phase)
    {
        return phase > 0.1f ? 1 : 0;
    }

    static inline float wild_sin(phase phase)
    {
        return (
                       sin(phase) +
                       sin(phase * 2.0f) +
                       sin(phase + 5.0f / 7)) / 3.0f;
    }

    static inline float fm_one(phase phase)
    {
        return (
                       sin(phase * 2.0f) +
                       sin(phase * 4.0f) +
                       sin(phase * 6.0f) +
                       sin(phase * 9.0f)) / 4.0f;
    }

    static inline float fm_two(phase phase)
    {
        return (
                       sin(phase * 2.0f) +
                       sin(phase * 4.0f) +
                       sin(phase * 6.0f) +
                       sin(phase * 9.0f) +
                       fm_one(phase)) / 5.0f;
    }

    static inline float fm_three(phase phase)
    {
        return (
                       sin(phase * 2.0f) +
                       sin(phase * 4.0f) +
                       sin(phase * 6.0f) +
                       fm_one(phase)) / 4.0f;
    }

    static inline float fm_four(phase phase)
    {
        return (
                       sin(phase * 2.0f) +
                       sin(phase * 4.0f) +
                       fm_one(phase)) / 3.0f;
    }

    static inline float fm_five(phase phase)
    {
        return (
                        sin(phase) +
                        sin(phase * 1.575f) +
                        sin(phase * 2.03f) +
                        sin(phase * 4.0f)) / 4.0f;
    }
}
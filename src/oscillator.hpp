#include <boost/circular_buffer.hpp>

#include "plugin.hpp"
#include "types.hpp"

#include "wavetables.hpp"

class WavetableOscillator
{
public:
    enum Wavetable
    {
        SINE,
        SQUARE,
        CRAZY_SINE,
        FM_ONE
    };

    float getNextWavepoint(phase phase, float wave_shape_param) const
    {
        float result = 0.0f;

        if (wave_shape_param < 0.25) result = wave_sine(phase);
        else if (wave_shape_param < 0.25) result = wave_fm_one(phase);
        else if (wave_shape_param < 0.75) result = wave_wild_sine(phase);
        else result = wave_sine(phase);

        return result;
    }
};
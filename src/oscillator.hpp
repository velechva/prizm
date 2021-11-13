#include <math.h>

#include <boost/circular_buffer.hpp>

#include "plugin.hpp"
#include "types.hpp"

#include "wavetables.hpp"

class WavetableOscillator
{
public:
    float wavepoint(phase phase, float wave_shape_param) const
    {
        if (wave_shape_param < 0.1) {
            return WaveShapes::sin(phase);
        }
        if (wave_shape_param < 0.2) {
            return WaveShapes::triangle(phase);
        }
        if (wave_shape_param < 0.3) {
            return WaveShapes::square(phase);
        }
        if (wave_shape_param < 0.4) {
            return WaveShapes::wild_sin(phase);
        }
        if (wave_shape_param < 0.5) {
            return WaveShapes::fm_one(phase);
        }
        if (wave_shape_param < 0.6) {
            return WaveShapes::fm_two(phase);
        }
        if (wave_shape_param < 0.7) {
            return WaveShapes::fm_three(phase);
        }
        if (wave_shape_param < 0.8) {
            return WaveShapes::fm_four(phase);
        }
        if (wave_shape_param < 0.9) {
            return WaveShapes::fm_five(phase);
        }

        return WaveShapes::fm_one(phase);
    }
};
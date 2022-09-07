#include <math.h>

#include <boost/circular_buffer.hpp>

#include "plugin.hpp"
#include "types.hpp"

#include "wavetables.hpp"

class WavetableOscillator
{
private:
    const static size_t NUM_WAVE_SHAPES = 9;
    const WaveShape waveShapes[NUM_WAVE_SHAPES] = {
            WaveShapes::sin,
            WaveShapes::triangle,
            WaveShapes::square,
            WaveShapes::wild_sin,
            WaveShapes::fm_one,
            WaveShapes::fm_two,
            WaveShapes::fm_three,
            WaveShapes::fm_four,
            WaveShapes::fm_five
    };

public:
    float wavepoint(phase phase, float wave_shape_param) const
    {
        auto waveShapeParamScaled = wave_shape_param * NUM_WAVE_SHAPES;

        auto previousIndex = std::min(static_cast<int>(NUM_WAVE_SHAPES - 1), static_cast<int>(floor(waveShapeParamScaled)));
        auto nextIndex     = std::min(static_cast<int>(NUM_WAVE_SHAPES - 1), static_cast<int>(ceil(waveShapeParamScaled)));
        auto ratio         = waveShapeParamScaled - previousIndex;

        auto previousWaveform = waveShapes[previousIndex](phase);
        auto nextWaveform     = waveShapes[nextIndex](phase);

        return (1 - ratio) * previousWaveform + ratio * nextWaveform;
    }
};
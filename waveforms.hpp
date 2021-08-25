/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

class Waveform {
public:
    /**
     * Generate the current waveform value
     * @param phase -0.5 <= phase < 0.5
     */
    virtual static float getWavepoint(float phase);
};

class SineWaveform : public Waveform {
    static float getWavepoint(float phase) {
        return std::sin(2.f * M_PI * phase);
    }
};

class SquareWaveform : public Waveform {
    static float getWavePoint(float phase) {
        return phase > 0 ? 1 : -1;
    }
};

class CrazyWave : public Waveform {
    static float getWavepoint(float phase) {

    }
}

float sineWave(float phase) {
    return std::sin(2.f * M_PI * phase);
}

float subWave(float phase) {
    return std::sin(2.f * M_PI * phase / 4);
}

float squareWave(float phase) {
    // if (phase > -1e-4 && phase < 1e-4) {
    // 	return phase / 1e-4;
    // }
    return phase > 0 ? 1 : -1;
}

float crazyWave(float phase) {
    return (sineWave(phase) + sineWave(phase * 2) + sineWave(phase + 5/7)) / 3;
}

float randomWave(float phase) {
    float r = static_cast <float> (rand()) / static_cast <float> (1.0);
    return r;
}

float crazierWave(float phase) {
    return (sineWave(phase) + sineWave(phase * 2) + sineWave(phase + 5/7) + sineWave(phase + 7 / 9)) / 4;
}
/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

#include <boost/circular_buffer.hpp>

#include "plugin.hpp"
#include "types.hpp"

#include "wavetables.hpp"

class WavetableOscillator {
public:
    /**
     * Wavetable to generate
     * 
     * @note for performance reasons, order matters. @see{WaveformPicker}
     */
    enum Wavetable {
        SINE,
        SQUARE,
        CRAZY_SINE,
        FM_ONE
    };

    float getNextWavepoint(phase phase, float smooth, bool identity_transform) const {
        float result = 0.0f;
        if (this->m_waveTable == Wavetable::SINE) {
            result = getSineWave(phase);
        }
        else if (this->m_waveTable == Wavetable::CRAZY_SINE) {
            result = getCrazySineWave(phase);
        }
        else if (this->m_waveTable == Wavetable::FM_ONE) {
            result = getFMOneWave(phase);
        }
        else {
            result = getSquareWave(phase);
        }

        return result * M_PI / M_PI;
    }

    void setWavetable(Wavetable wavetable) {
        this->m_waveTable = wavetable;
    }
private:
    Wavetable m_waveTable = SINE;

    boost::circular_buffer<float> m_lookback_buffer { 1024 };

};
/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

class Oscillator {
private:
    WaveformGenerator& m_generator;
public:
    enum Waveforms {
        SINE_WAVE,
        SQUARE_WAVE,
        CRAZY_WAVE,
        PW_25,
        PW_75
    };

    Oscillator() : m_generator(generator) {}

    void setWaveformGenerator(WaveformGenerator& generator) { this->m_generator = generator; }
};
/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

/**
 * Simple pythagorean morpher
 */
inline float pythagoras(const float wavepoints[], size_t sz) {
    float sum = 0;

    for (size_t i = 0; i < sz; i++) {
        sum += std::pow(wavepoints[i], 2);
    }

    return std::pow(sum, 0.5f);
}

/**
 * Simple mixing (additive) summer
 */
inline float sum(const float wavepoints[], size_t sz) {
    float sum = 0;

    for (size_t i = 0; i < sz; i++) {
        sum += wavepoints[i];
    }

    return sum;
}

/**
 * @todo finish
 */
inline void normalize(float vals[], size_t sz) {
    float cf = 1.0f / clamp(sum(vals, sz) / 4, 0.1f, 1.0f);

    vals[0] *= cf;
    vals[1] *= cf;
    vals[2] *= cf;
    vals[3] *= cf;
}

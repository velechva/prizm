/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

float pythagoras(float a, float b, float c, float d) {
    float sum = 0;
    sum += std::pow(a, 2);
    sum += std::pow(b, 2);
    sum += std::pow(c, 2);
    sum += std::pow(d, 2);
    return std::pow(sum, 1.0 / 4);
}

void normalize(float& a, float& b, float& c, float& d) {
    float sum = a + b + c + d;
    if (sum < 1e-3) { return; }
    float div = 1.0 / sum;
    a = a * div;
    b = b * div;
    c = c * div;
    d = d * div;
}
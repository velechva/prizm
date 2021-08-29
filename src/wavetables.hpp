#include "plugin.hpp"
#include "types.hpp"

static inline float getSineWave(phase phase) {
    return std::sin(2.0f * M_PI * phase);
}

static inline float getSquareWave(phase phase) {
    return phase > 0 ? 1 : -1;
}

static inline float getCrazySineWave(phase phase) {
    return (
        getSineWave(phase) + 
        getSineWave(phase * 2.0f) + 
        getSineWave(phase + 5.0f/7)) / 3.0f;
}

static inline float getFMOneWave(phase phase) {
    return (getSineWave(phase * 2.0f) + getSineWave(phase * 4.0f) + getSineWave(phase * 6.0f) + getSineWave(phase * 9.0f)) / 4.0f;
}
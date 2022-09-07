namespace dsp
{

inline float pythagoras(const float samples[], const size_t numSamples)
{
    float sum = 0;

    for (size_t i = 0; i < numSamples; i++)
    {
        sum += std::pow(samples[i], 2);
    }

    return std::pow(sum, 0.5f);
}

inline float sum(const float samples[], const size_t sz)
{
    float sum = 0;

    for (size_t i = 0; i < sz; i++) {
        sum += samples[i];
    }

    return sum;
}

inline void normalize(float vals[], size_t sz) {
    float cf = 1.0f / clamp(sum(vals, sz) / static_cast<float>(sz), 0.01f, 1.0f);

    vals[0] *= cf;
    vals[1] *= cf;
    vals[2] *= cf;
    vals[3] *= cf;
}

}

class EnveloperFollower
{
protected:
    double a;
    double r;

    double envelope;
public:
    EnveloperFollower()
    {
        envelope = 0;
    }

    void setup(millis attack, millis release, int sampleRate)
    {
        a = pow(0.01, 1.0 / (attack * sampleRate * 0.001));
        r = pow(0.01, 1.0 / (release * sampleRate * 0.001));
    }

    void process(size_t count, const T *src)
    {
        while(count--)
        {
            float v = fabs(*src);
            src += skip;

            if (v > envelope)   envelope = a * (envelope - v) + v;
            else                envelope = r * (envelope - v) + v;
        }
    }
};

struct Limiter
{
private:
    EnvelopeFollower e;

public:
    void setup(millis attack, millis release, int sampleRate)
    {
        e.setup(attack, release, sampleRate);
    }

    template<class T>
    T process(size_t nSamples);
};
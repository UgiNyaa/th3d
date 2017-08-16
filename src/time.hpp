#ifndef TIME_HPP
#define TIME_HPP

struct Time
{
    float full;
    float delta;

    Time()
        : full(0)
        , delta(0)
    { }

    float full_seconds() const { return full; }
    float delta_seconds() const { return delta; }

    void add(float delta)
    {
        this->delta = delta;
        full += delta;
    }
};

#endif /* end of include guard: TIME_HPP */

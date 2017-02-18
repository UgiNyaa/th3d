#ifndef TIME_HPP
#define TIME_HPP

struct Time
{
    float delta;
    float full;

    Time() { }

    float full_seconds() const { return full; }
    float delta_seconds() const { return delta; }

    void add(float delta)
    {
        this->delta = delta;
        full += delta;
    }
};

#endif /* end of include guard: TIME_HPP */

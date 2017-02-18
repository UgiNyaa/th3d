#include <math.h>

extern "C" void zero(float& x, float& y, float& z, float t)
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

extern "C" void circle_forward(float& x, float& y, float& z, float t)
{
    x = sin(t*4) * 2 + 5;
    y = cos(t*4) * 2;
    z = cos(t*4) * 2 + t / 2;
}

extern "C" void circle_stand(float& x, float& y, float& z, float t)
{
    x = -5.0f;
    y = sin(t*4) * 4;
    z = cos(t*4) * 4;
}

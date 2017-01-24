#ifndef BMAP_HPP
#define BMAP_HPP

#include <vector>
#include <string>

#include <bullet.hpp>
#include <shapes/shape.hpp>
#include <shapes/cubeshape.hpp>

static const Shape bmap_shapes[] =
{
    CubeShape()
};

struct BMap
{
    std::vector<Bullet*> bullets;
};

Bmap& read_bmap(std::string path);

#endif /* end of include guard: BMAP_HPP */

#ifndef BMAP_HPP
#define BMAP_HPP

#include <vector>
#include <string>

#include <bullet.hpp>
#include <shapes/shape.hpp>
#include <shapes/cubeshape.hpp>

static Shape const* bmap_shapes =
{
    new CubeShape()
};

struct BMap
{
    std::vector<Bullet*> bullets;
};

BMap& read_bmap_json(std::string path);

#endif /* end of include guard: BMAP_HPP */

#ifndef BMAP_HPP
#define BMAP_HPP

#include <vector>
#include <string>
#include <memory>

#include <bullet.hpp>
#include <shapes/shape.hpp>
#include <shapes/cubeshape.hpp>

struct BMap
{
    std::vector<Bullet*> Bullets;
    std::vector<void*> Libs;
    float gametime;

    void update(float deltatime);

    static BMap from_json_file
    (
        std::string path,
        const std::vector<Shape*> shapes
    );
};

#endif /* end of include guard: BMAP_HPP */

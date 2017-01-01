#ifndef BMREADER_HPP
#define BMREADER_HPP

#include <vector>

#include <bullet.hpp>
#include <shapes/shape.hpp>
#include <shapes/cube.hpp>

std::vector<Bullet*> bm_json_read
(
    const char* bm_json,
    Shape** shapes,
    int shapes_n
);

#endif /* end of include guard: BMREADER_HPP */

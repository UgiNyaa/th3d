#ifndef BMREADER_HPP
#define BMREADER_HPP

#include <bullet.hpp>
#include <vector>
#include <shapes/shape.hpp>
#include <shapes/cube.hpp>

std::vector<Bullet*> bm_json_read
(
    const char* bm_json,
    Shape* shapes[]
);

#endif /* end of include guard: BMREADER_HPP */

#ifndef BMREADER_HPP
#define BMREADER_HPP

#include <vector>

#include <bullet.hpp>
#include <shapes/shape.hpp>
#include <shapes/cubeshape.hpp>

std::vector<Bullet*> bm_json_read
(
    const char* bm_json,
    Shape** p_shapes,
    int n_shapes
);

#endif /* end of include guard: BMREADER_HPP */

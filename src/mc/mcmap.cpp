#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

#include <zlib.h>

#include <mc/nbt/nbt.hpp>
#include <mc/nbt/compression.hpp>

#include <mc/mcmap.hpp>

MCMap::MCMap(std::string path)
{
    auto region = path + "/region/r.0.0.mca";

    do_region(region);
}

MCMap::~MCMap()
{

}

void MCMap::initialize()
{
    cubedrawer.initialize();
}

void MCMap::draw(glm::mat4 view, glm::mat4 projection)
{
    for (size_t i = 0; i < positions.size() / 1000; i++)
    {
        auto& p = positions[i];
        glm::mat4 model = glm::translate(p) * glm::scale(glm::vec3(0.5f));
        cubedrawer.draw
        (
            model,
            view,
            projection,
            glm::vec3(0.5, 0.5, 0.5)
        );
    }
}

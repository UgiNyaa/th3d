#include <stdlib.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

#include <zlib.h>

#include <mcmap.hpp>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim))
        *(result++) = item;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

MCMap::MCMap(std::string path)
{
    auto region = path + "/region/r.0.0.mca";

    do_region(region);
}

MCMap::~MCMap()
{

}

void MCMap::do_region(std::string path)
{
    std::string region_file_name = path.substr(path.rfind("/")+1, path.size());

    auto coords = split(region_file_name, '.');

    auto regionX = std::stoi(coords[1]);
    auto regionY = std::stoi(coords[2]);

    // pain in the ass to read binary with ifstream
    // so using just the C interface
    std::ifstream in(path);
    std::cout << path << '\n';
    uint8_t b;

    uint32_t offset;
    uint8_t sc;
    for (size_t i = 0; i < 1024; i++)
    {
        in.get(*reinterpret_cast<char*>(&b));
        offset = b * 255 * 255;

        in.get(*reinterpret_cast<char*>(&b));
        offset += b * 255;

        in.get(*reinterpret_cast<char*>(&b));
        offset += b;

        in.get(*reinterpret_cast<char*>(&b));
        sc = b;

        std::cout << "offset: " << (unsigned int)offset << '\n';
        std::cout << "section count: " << (unsigned int)sc << '\n';
    }

    uint32_t timestamp;
    for (size_t i = 0; i < 1024; i++)
    {
        in.get(*reinterpret_cast<char*>(&b));
        timestamp = b * 255 * 255 * 255;

        in.get(*reinterpret_cast<char*>(&b));
        timestamp += b * 255 * 255;

        in.get(*reinterpret_cast<char*>(&b));
        timestamp += b * 255;

        in.get(*reinterpret_cast<char*>(&b));
        timestamp += b;

        std::cout << "timestamp: " << timestamp << '\n';
    }

    while (!in.eof())
        do_chunck(in);


    std::cout << "eof was reached" << '\n';
}

void MCMap::do_chunck(std::ifstream& in)
{
    uint8_t b;
    uint32_t length;
    uint8_t comp_type;

    in.get(*reinterpret_cast<char*>(&b));
    length = b * 255 * 255 * 255;

    in.get(*reinterpret_cast<char*>(&b));
    length += b * 255 * 255;

    in.get(*reinterpret_cast<char*>(&b));
    length += b * 255;

    in.get(*reinterpret_cast<char*>(&b));
    length += b;

    in.get(*reinterpret_cast<char*>(&b));
    comp_type = b;

    std::cout << "chunck length: " << length << '\n';
    std::cout << "compression type: " << (int)comp_type << '\n';

    if (comp_type != 2)
    {
        std::cerr << "compression type " << comp_type << " not supported" << '\n';
        exit(-1);
    }

    

    exit(-1);
}

#include <mc/nbt/nbt.hpp>
#include <mc/nbt/compression.hpp>

#include <mc/mcmap.hpp>

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

    std::vector<std::tuple<uint32_t, uint8_t, uint32_t>> header;
    uint32_t offset;
    uint8_t sc;
    uint32_t timestamp;
    for (size_t i = 0; i < 1024; i++)
    {
        // go to timestamp
        in.seekg(i * 4 + 4096);

        // read timestamp
        in.get(*reinterpret_cast<char*>(&b));
        timestamp = b * 256 * 256 * 256;
        in.get(*reinterpret_cast<char*>(&b));
        timestamp += b * 256 * 256;
        in.get(*reinterpret_cast<char*>(&b));
        timestamp += b * 256;
        in.get(*reinterpret_cast<char*>(&b));
        timestamp += b;

        // go back to initial position
        in.seekg(i * 4);

        // read offset
        in.get(*reinterpret_cast<char*>(&b));
        offset = b * 256 * 256;
        in.get(*reinterpret_cast<char*>(&b));
        offset += b * 256;
        in.get(*reinterpret_cast<char*>(&b));
        offset += b;

        // read sector count
        in.get(*reinterpret_cast<char*>(&b));
        sc = b;

        // if both fields are zero
        // chunk is not present
        // and has not to be processed later
        if (offset == 0 && sc == 0)
            continue;

        // add to header vector
        header.push_back(std::make_tuple(offset, sc, timestamp));
    }

    for (auto& t : header)
    {
        in.seekg(std::get<0>(t) * 4096);

        do_chunck(in);

        auto processed = (int)in.tellg() - (std::get<0>(t) * 4096);
        if (processed > (std::get<1>(t) * 4096))
        {
            std::cerr << "error happened while processing chunks" << '\n';
            std::cerr << "chunk processed more bytes than sector count indicates" << '\n';
            std::cerr << "chunk offset: " << std::get<0>(t) << '\n';
            std::cerr << "chunk sector count: " << std::get<1>(t) << '\n';
            std::cerr << "processed(raw bytes): " << processed << '\n';
            exit(-1);
        }
    }

    std::cout << "positions size: " << positions.size() << '\n';

    std::cout << "end" << '\n';
}

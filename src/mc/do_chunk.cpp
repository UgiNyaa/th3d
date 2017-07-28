#include <mc/nbt/nbt.hpp>
#include <mc/nbt/compression.hpp>

#include <mc/mcmap.hpp>

bool is_surrounded(int x, int y, int z, int8_t* blocks)
{
    int block_pos = y*16*16 + z*16 + x;

    int surrounder = y*16*16 + z*16 + (x+1);
    if (blocks[surrounder] == 0)
        return true;

    surrounder = y*16*16 + z*16 + (x-1);
    if (blocks[surrounder] == 0)
        return true;

    surrounder = y*16*16 + (z+1)*16 + x;
    if (blocks[surrounder] == 0)
        return true;

    surrounder = y*16*16 + (z-1)*16 + x;
    if (blocks[surrounder] == 0)
        return true;


    surrounder = (y+1)*16*16 + z*16 + x;
    if (blocks[surrounder] == 0)
        return true;

    surrounder = (y-1)*16*16 + z*16 + x;
    if (blocks[surrounder] == 0)
        return true;

    return false;
}

void MCMap::do_chunck(std::ifstream& in)
{
    uint8_t b;
    uint32_t length;
    uint8_t comp_type;

    in.get(*reinterpret_cast<char*>(&b));
    length = b * 256 * 256 * 256;

    in.get(*reinterpret_cast<char*>(&b));
    length += b * 256 * 256;

    in.get(*reinterpret_cast<char*>(&b));
    length += b * 256;

    in.get(*reinterpret_cast<char*>(&b));
    length += b;

    in.get(*reinterpret_cast<char*>(&b));
    comp_type = b;

    if (comp_type != 2)
    {
        std::cerr << "compression type " << comp_type << " not supported" << '\n';
        exit(-1);
    }

    std::string compressed, data;
    compressed.resize(length - 1);
    in.read(&compressed[0], length - 1);

    NBT::decompress(&data, compressed.data(), compressed.size());

    NBT::Tag nbt_data(reinterpret_cast<const NBT::UByte*>(data.data()));
    int chunkX = (int)nbt_data[""]["Level"]["xPos"];
    int chunkZ = (int)nbt_data[""]["Level"]["zPos"];

    auto secs = (NBT::List)nbt_data[""]["Level"]["Sections"];

    for (size_t i = 0; i < secs.size; i++)
    {
        auto sec = secs.value[i];
        auto blocks = (NBT::ByteArray)sec["Blocks"];
        auto secY = (int8_t)sec["Y"];

        for (size_t x = 0; x < 16; x++)
            for (size_t y = 0; y < 16; y++)
                for (size_t z = 0; z < 16; z++)
                {
                    int block_pos = y*16*16 + z*16 + x;
                    auto block_id = (int8_t)blocks.value[block_pos];
                    if (block_id != 0 && is_surrounded(x, y, z, blocks.value))
                    {
                        positions.push_back(glm::vec3
                        (
                            chunkX + x,
                            (secY*16) + y,
                            chunkZ + z
                        ));
                    }
                }
    }
}


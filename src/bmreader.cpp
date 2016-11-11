#include <bmreader.hpp>
#include <json.hpp>
using json = nlohmann::json;
#include <game.hpp>

std::vector<Bullet*> bm_json_read(const char* bm_json)
{
    auto bullets = std::vector<Bullet*>();
    auto j = json::parse(bm_json);
    for (auto j_bullet : j)
    {
        auto bullet = new Bullet();

        auto j_x = j_bullet["x"];
        if (j_x["format"] != "const")
            continue;
        float x = j_x["value"];
        bullet->f_position_x = [=](float t) -> float
            { return x; };

        auto j_y = j_bullet["y"];
        if (j_y["format"] != "const")
            continue;
        float y = j_y["value"];
        bullet->f_position_y = [=](float t) -> float
            { return y; };

        auto j_z = j_bullet["z"];
        if (j_z["format"] != "const")
            continue;
        float z = j_z["value"];
        bullet->f_position_z = [=](float t) -> float
            { return z; };

        bullets.push_back(bullet);
    }

    return bullets;
}

#include <dlfcn.h>

#include <functional>

#include <json.hpp>
#include <exprtk.hpp>

#include <bmap.hpp>

using json = nlohmann::json;

BMap BMap::from_json_file
(
    std::string json_str,
    const Time& t,
    const std::vector<Shape*> shapes
)
{
    BMap bmap;

    auto j = json::parse(json_str);

    for (auto j_lib : j["libs"])
    {
        auto pathToLib = j_lib.get<std::string>();
        auto lib = dlopen(pathToLib.c_str(), RTLD_LAZY);
        if (!lib)
        {
            std::cerr << dlerror() << '\n';
            exit(1);
        }
        bmap.Libs.push_back(lib);
    }

    for (auto j_bullet : j["bullets"])
    {
        Shape const* shape;
        auto j_shape = j_bullet["shape"].get<std::string>();
        for (size_t i = 0; i < shapes.size(); i++)
        {
            auto s = shapes[i];
            if (s->name() == j_shape)
            {
                shape = s;
                break;
            }
        }

        auto bullet = new Bullet(t, shape);
        auto funcname = j_bullet["position"].get<std::string>();
        void (*func)(float& x, float& y, float& z, float t);

        for (auto lib : bmap.Libs)
        {
            dlerror();
            *(void **)(&func) = dlsym(lib, funcname.c_str());
            if (!func)
                std::cerr << dlerror() << '\n';
            else
                break;
        }

        if (!func)
        {
            std::cout << "func is null" << '\n';
            exit(1);
        }

        bullet->pos = func;

        bmap.Bullets.push_back(bullet);
    }

    return bmap;
}

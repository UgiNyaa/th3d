#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <lua.hpp>
#include <shape.hpp>
#include <bullet.hpp>

#ifdef DEBUG
    #define DOUT(x) std::cout << x << '\n';
#else
    #define DOUT(x)
#endif

struct Pattern
{
    const Shape* shape;

    lua_State* L;
    int id;
    int unit_id;
    Time* t;
    float ux, uy, uz;

    Pattern() { }

    void start(std::vector<Bullet*>& bullets)
    {
        DOUT("start pattern and bullet creation")

        // STATE: 0
        DOUT("STATE: 0")

        lua_getglobal(L, "world");
        // STATE: world
        DOUT("STATE: world")

        lua_getfield(L, -1, "bullets");
        // STATE: world - bullets
        DOUT("STATE: world - bullets")

        lua_len(L, -1);
        // STATE: world - bullets - len
        DOUT("STATE: world - bullets - len")

        int offset = lua_tointeger(L, -1) + 1;

        lua_pop(L, 2);
        // STATE: world
        DOUT("STATE: world")

        lua_getfield(L, -1, "units");
        // STATE: world - units
        DOUT("STATE: world - units")

        lua_geti(L, -1, unit_id);
        // STATE: world - units - units[unit_id]
        DOUT("STATE: world - units - units[unit_id]")

        lua_getfield(L, -1, "patterns");
        // STATE: world - units - units[unit_id] - patterns
        DOUT("STATE: world - units - units[unit_id] - patterns")

        lua_geti(L, -1, id);
        // STATE: world - units - units[unit_id] - patterns - patterns[id]
        DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id]")

        lua_getfield(L, -1, "n");
        // STATE: world - units - units[unit_id] - patterns - patterns[id] - n
        DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id] - n")

        int n = lua_tointeger(L, -1);

        lua_pop(L, 1);
        // STATE: world - units - units[unit_id] - patterns - patterns[id]
        DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id]")

        lua_getfield(L, -5, "bullets");
        // STATE: world - units - units[unit_id] - patterns - patterns[id]
        // STATE: - bullets
        DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id]"
            << " - bullets")

        for (int i = 0; i < n; i++)
        {
            lua_getfield(L, -2, "create");
            // STATE: world - units - units[unit_id] - patterns - patterns[id]
            // STATE: - bullets - create
            DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id] - bullets - create")

            lua_pushinteger(L, i);
            // STATE: world - units - units[unit_id] - patterns - patterns[id]
            // STATE: - bullets - create - i
            DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id] - bullets - create - i")

            lua_call(L, 1, 1);
            // STATE: world - units - units[unit_id] - patterns - patterns[id]
            // STATE: - bullets - bullet
            DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id] - bullets - bullet")

            lua_getfield(L, -1, "start");
            // STATE: world - units - units[unit_id] - patterns - patterns[id]
            // STATE: - bullets - bullet - start

            auto start = lua_tonumber(L, -1);

            lua_pop(L, 1);
            // STATE: world - units - units[unit_id] - patterns - patterns[id]
            // STATE: - bullets - bullet

            lua_seti(L, -2, offset + i);
            // STATE: world - units - units[unit_id] - patterns - patterns[id]
            // STATE: - bullets
            DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id] - bullets")

            auto b = new Bullet();
            b->id = offset + i;
            b->shape = shape;
            b->t = t;
            b->L = L;
            b->ux = ux;
            b->uy = uy;
            b->uz = uz;
            b->start = start;

            DOUT("bullet added with id")
            DOUT(offset + i)

            bullets.push_back(b);
        }
        // STATE: world - units - units[unit_id] - patterns - patterns[id]
        // STATE: - bullets
        DOUT("STATE: world - units - units[unit_id] - patterns - patterns[id] - bullets")

        lua_pop(L, 6);
        // STATE: 0
        DOUT("STATE: 0")
    }
};

#endif /* end of include guard: ENGINE_HPP */

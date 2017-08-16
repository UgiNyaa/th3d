#ifndef UNIT_HPP
#define UNIT_HPP

#include <lua.hpp>

#include "shape.hpp"
#include "bullet.hpp"
#include "pattern.hpp"
#include "time.hpp"

#ifdef DEBUG
    #define DOUT(x) std::cout << x << '\n';
#else
    #define DOUT(x)
#endif

struct Unit
{
    const Shape* shape;
    int id;
    lua_State* L;
    Time* t;

    std::vector<Bullet*> bullets;
    std::vector<Pattern*> patterns;

    void pos(float& x, float& y, float& z) const
    {
        DOUT("unit position calculation")

        DOUT(lua_gettop(L))

        // STATE: 0
        DOUT("STATE: 0")

        lua_getglobal(L, "world");
        // STATE: world
        DOUT("STATE: world")

        lua_getfield(L, -1, "units");
        // STATE: world - units
        DOUT("STATE: world - units")

        lua_geti(L, -1, id);
        // STATE: world - units - units[id]
        DOUT("STATE: world - units - units[id]")

        lua_getfield(L, -1, "position");
        // STATE: world -units - units[id] - position
        DOUT("STATE: world -units - units[id] - position")

        lua_pushnumber(L, t->full);
        // STATE: world - units - units[id] - position - t
        DOUT("STATE: world - units - units[id] - position - t")

        int result = lua_pcall(L, 1, 3, 0);

        if ( result != LUA_OK )
        {
            std::cout << "error occured in lua" << '\n';
            const char* message = lua_tostring(L, -1);
            puts(message);
            lua_pop(L, 1);
            exit(-1);
        }
        // STATE: world - units - units[id] - x - y - z
        DOUT("STATE: world - units - units[id] - x - y - z")

        z = lua_tonumber(L, -1);
        y = lua_tonumber(L, -2);
        x = lua_tonumber(L, -3);

        lua_pop(L, 6);
        // STATE: 0
        DOUT("STATE: 0")
    }
};

#endif /* end of include guard: UNIT_HPP */

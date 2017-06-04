#ifndef BULLET_HPP
#define BULLET_HPP

#include <lua.hpp>
#include <time.hpp>
#include <shapes/shape.hpp>

#ifdef DEBUG
    #define DOUT(x) std::cout << x << '\n';
#else
    #define DOUT(x)
#endif

struct Bullet
{
    const Shape* shape;

    lua_State* L;
    int id;
    Time* t;
    float ux, uy, uz;
    float start;

    Bullet()
        : shape(nullptr)
        , id(0)
        , t(nullptr)
    { }

    void pos(float& x, float& y, float& z) const
    {
        DOUT("bullet position calculation of id")
        DOUT(id)

        // STATE: 0
        DOUT("STATE: 0")

        lua_getglobal(L, "world");
        // STATE: world
        DOUT("STATE: world")

        lua_getfield(L, -1, "bullets");
        // STATE: world - bullets
        DOUT("STATE: world - bullets")

        lua_geti(L, -1, id);
        // STATE: world - bullets - bullets[id]
        DOUT("STATE: world - bullets - bullets[id]")

        lua_getfield(L, -1, "position");
        // STATE: world - bullets - bullets[id] - position
        DOUT("STATE: world - bullets - bullets[id] - position")

        lua_insert(L, -2);
        // STATE: world -ubblets- position - bullets[id]
        DOUT("STATE: world - bullets - position - bullets[id]")

        lua_pushnumber(L, t->full);
        // STATE: world - bullets - bullets[id] - position - t
        DOUT("STATE: world - bullets - position - bullets[id] - t")

        int result = lua_pcall(L, 2, 3, 0);
        // STATE: world - bullets - bullets[id] - x - y - z
        DOUT("STATE: world - bullets - x - y - z")

        if ( result != LUA_OK )
        {
            std::cout << "error occured in lua" << '\n';
            const char* message = lua_tostring(L, -1);
            puts(message);
            lua_pop(L, 1);
            exit(-1);
        }

        z = lua_tonumber(L, -1) + uz;
        y = lua_tonumber(L, -2) + uy;
        x = lua_tonumber(L, -3) + ux;

        DOUT(x)
        DOUT(y)
        DOUT(z)

        lua_pop(L, 5);
        // STATE: 0
        DOUT("STATE: 0")
    }
};

#endif /* end of include guard: BULLET_HPP */

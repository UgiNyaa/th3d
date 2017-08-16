#include "testgame.hpp"

void TestGame::update_entities()
{
    // STATE: 0
    D(std::cout << "STATE: 0" << '\n';)
    
    lua_getglobal(L, "world");
    // STATE: world
    D(std::cout << "STATE: world" << '\n';)

    lua_getfield(L, -1, "entities");
    // STATE: world - entities
    D(std::cout << "STATE: world - entities" << '\n';)

    lua_pushnil(L);
    // STATE: world - entities - nil
    D(std::cout << "STATE: world - entities - nil" << '\n';)

    while (lua_next(L, -2))
    {
        // STATE: world - entities - key - value
        D(std::cout << "STATE: world - entities - key - value" << '\n';)
        
        update_entity();
        // STATE: world - entities - key - value
        D(std::cout << "STATE: world - entities - key - value" << '\n';)

        lua_getfield(L, -1, "patterns");
        // STATE: world - entities - key - value - patterns
        D(std::cout << "STATE: world - entities - key - value - patterns" << '\n';)

        if (!lua_isnil(L, -1))
        {
            lua_pushnil(L);
            // STATE: world - entities - key - value - patterns - nil
            D(std::cout << "STATE: world - entities - key - value - patterns - nil" << '\n';)

            while (lua_next(L, -2))
            {
                // STATE: world - entities - key - value - patterns - key - value
                D(std::cout << "STATE: world - entities - key - value - patterns - key - value" << '\n';)

                update_pattern();
                // STATE: world - entities - key - value - patterns - key - value
                D(std::cout << "STATE: world - entities - key - value - patterns - key - value" << '\n';)
            }
        }

        lua_pop(L, 1);
        // STATE: world - entities - key
        D(std::cout << "STATE: world - entities - key" << '\n';)
    }
    // STATE: world - entities
    D(std::cout << "STATE: world - entities" << '\n';)

    lua_pop(L, 2);
    // STATE: 0
    D(std::cout << "STATE: 0" << '\n';)
}

void TestGame::update_entity()
{
    // STATE: world - entities - key - value
    D(std::cout << "STATE: world - entities - key - value" << '\n';)
    D(std::cout << "type of -1: " << lua_typename(L, lua_type(L, -1)) << '\n';)
    D(std::cout << "type of -2: " << lua_typename(L, lua_type(L, -2)) << '\n';)

    auto e = entities[lua_tointeger(L, -2) - 1];
    e->shape = &s;

    lua_getfield(L, -1, "position");
    // STATE: world - entities - key - value - position
    D(std::cout << "STATE: world - entities - key - value - position" << '\n';)

    lua_pushnumber(L, t.full);
    // STATE: world - entities - key - value - position - t
    D(std::cout << "STATE: world - entities - key - value - position - t" << '\n';)

    int result = lua_pcall(L, 1, 3, 0);

    if (result != LUA_OK)
    {
        std::cout << "error occured in lua" << '\n';
        const char* message = lua_tostring(L, -1);
        puts(message);
        lua_pop(L, 1);
        exit(-1);
    }
    // STATE: world - entities - key - value - x - y - z
    D(std::cout << "STATE: world - entities - key - value - x - y - z" << '\n';)

    e->position = glm::vec3(
        lua_tonumber(L, -3),
        lua_tonumber(L, -2),
        lua_tonumber(L, -1)
    );

    lua_pop(L, 3);
    // STATE: world - entities - key - value
    D(std::cout << "STATE: world - entities - key - value" << '\n';)
}

void TestGame::update_pattern()
{
    // STATE: world - entities - key - value - patterns - key - value

    lua_getfield(L, -1, "n");
    // STATE: world - entities - key - value - patterns - key - value - n

    auto n = lua_tointeger(L, -1);

    lua_pop(L, 1);
    // STATE: world - entities - key - value - patterns - key - value

    lua_getfield(L, -1, "create");
    // STATE: world - entities - key - value - patterns - key - value - create

    for (size_t i = 0; i < n; i++)
    {
        lua_pushnumber(L, n);
        // STATE: world - entities - key - value - patterns - key - value - create - n

        int result = lua_pcall(L, 1, 1, 0);

        if (result != LUA_OK)
        {
            std::cout << "error occured in lua" << '\n';
            const char* message = lua_tostring(L, -1);
            puts(message);
            lua_pop(L, 1);
            exit(-1);
        }
        // STATE: world - entities - key - value - patterns - key - value - bullet
    }
}
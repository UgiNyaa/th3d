#include <stdlib.h>
#include <time.h>
#include <lua.hpp>

#include <games/testgame/testgame.hpp>

#ifdef DEBUG
    #define DOUT(x) std::cout << x << '\n';
#else
    #define DOUT(x)
#endif

int set_lua_path(lua_State* L, std::string path)
{
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring( L, -1 ); // grab path string from top of stack
    cur_path.append( ";" ); // do your path magic here
    cur_path.append( path );
    lua_pop( L, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( L, cur_path.c_str() ); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}

void print_error(lua_State* L)
{
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(L, -1);
  puts(message);
  lua_pop(L, 1);
}

void TestGame::init_bmap(std::string lua)
{
    DOUT("initializing bmap from " + lua + " path")

    lua_State* L = luaL_newstate();
    // STATE: 0
    DOUT("STATE: 0")

    luaL_openlibs(L);
    DOUT("lua standard libraries loaded")

    int result;

    set_lua_path(L, lua + "/?.lua");
    result = luaL_loadfile(L, (lua + "/world.lua").c_str());
    DOUT("lua " + lua + " loaded")
    DOUT("result: " + result)

    if (result != LUA_OK)
    {
        std::cout << "error occured in lua" << '\n';
        print_error(L);
        exit(-1);
    }

    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    DOUT("running the file")
    DOUT("result: " + result)

    if ( result != LUA_OK )
    {
        std::cout << "error occured in lua" << '\n';
        print_error(L);
        exit(-1);
    }

    lua_getglobal(L, "world");
    // STATE: world
    DOUT("STATE: world")

    lua_newtable(L);
    // STATE: world - {}
    DOUT("STATE: world - {}")

    lua_setfield(L, -2, "bullets");
    // STATE: world
    DOUT("STATE: world")

    lua_getfield(L, -1, "units");
    // STATE: world - units
    DOUT("STATE: world - units")

    lua_pushnil(L);
    // STATE: world - units - nil
    DOUT("STATE: world - units - nil")

    while (lua_next(L, -2))
    {
        // STATE: world - units - key - value
        DOUT("STATE: world - units - key - value")

        auto u = new Unit();
        u->id = lua_tointeger(L, -2);
        u->L = L;
        u->t = &t;

        lua_getfield(L, -1, "shape");
        // STATE: world - units - key - value - shape
        DOUT("STATE: world - units - key - value - shape");

        std::string model(lua_tostring(L, -1));

        lua_pop(L, 1);
        // STATE: world - units - key - value
        DOUT("STATE: world - units - key - value");

        if (shapes.find(model) == shapes.end())
            shapes[model] = new Shape(lua + "shapes/" + model + ".obj");

        u->shape = shapes[model];

        lua_getfield(L, -1, "patterns");
        // STATE: world - units - key - value - patterns
        DOUT("STATE: world - units - key - value - patterns");

        lua_pushnil(L);
        // STATE: world - units - key - value - patterns - nil
        DOUT("STATE: world - units - key - value - patterns - nil");

        while (lua_next(L, -2))
        {
            // STATE: world - units - key - value - patterns - key - value
            DOUT("STATE: world - units - key - value - patterns - key - value");

            auto p = new Pattern();
            p->id = lua_tointeger(L, -2);
            p->unit_id = u->id;
            p->L = L;
            p->t = &t;

            lua_getfield(L, -1, "shape");
            // STATE: world - units - key - value - patterns - key - value
            // STATE: - shape
            DOUT("STATE: world - units - key - value - patterns - key - value - shape");

            std::string model(lua_tostring(L, -1));
    
            lua_pop(L, 1);
            // STATE: world - units - key - value - patterns - key - value
            DOUT("STATE: world - units - key - value - patterns - key - value");

            if (shapes.find(model) == shapes.end())
                shapes[model] = new Shape(lua + "shapes/" + model + ".obj");

            p->shape = shapes[model];
            u->patterns.push_back(p);

            lua_pop(L, 1);
            // STATE: world - units - key - value - patterns - key
            DOUT("STATE: world - units - key - value - patterns - key");
        }
        // STATE: world - units - key - value - patterns
        DOUT("STATE: world - units - key - value - patterns");
        lua_pop(L, 1);
        // STATE: world - units - key - value
        DOUT("STATE: world - units - key - value");

        units.push_back(u);

        lua_pop(L, 1);
        // STATE: world - units - key
        DOUT("STATE: world - units - key");
    }
    // STATE: world - units
    DOUT("STATE: world - units");

    lua_pop(L, 2);
    // STATE: 0
    DOUT("STATE:0")

    DOUT("initialized")
}

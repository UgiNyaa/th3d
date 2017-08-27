#include <stdlib.h>
#include <time.h>

#include "testgame.hpp"

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
    D(std::cout << "initializing bmap from " + lua + " path" << '\n');

    L = luaL_newstate();
    // STATE: 0
    D(std::cout << "STATE: 0" << '\n');

    luaL_openlibs(L);
    D(std::cout << "lua standard libraries loaded" << '\n');

    int result;

    set_lua_path(L, lua + "/?.lua");
    result = luaL_loadfile(L, (lua + "/world.lua").c_str());
    D(std::cout << "lua " + lua + " loaded" << '\n');
    D(std::cout << "result: " + result << '\n');

    if (result != LUA_OK)
    {
        std::cout << "error occured in lua" << '\n';
        print_error(L);
        exit(-1);
    }

    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    D(std::cout << "running the file" << '\n');
    D(std::cout << "result: " + result << '\n');

    if ( result != LUA_OK )
    {
        std::cout << "error occured in lua" << '\n';
        print_error(L);
        exit(-1);
    }

    lua_getglobal(L, "world");
    // STATE: world
    D(std::cout << "STATE: world" << '\n');

    lua_getfield(L, -1, "entities");
    // STATE: world - entities
    D(std::cout << "STATE: world - entities" << '\n');

    lua_pushnil(L);
    // STATE: world - entities - nil
    D(std::cout << "STATE: world - entities - nil" << '\n');

    while (lua_next(L, -2))
    {
        // STATE: world - entities - key - value
        D(std::cout << "STATE: world - entities - key - value" << '\n');
        
        entities.push_back(new Entity());

        lua_pop(L, 1);
        // STATE: world - entities - key
        D(std::cout << "STATE: world - entities - key" << '\n');
    }
    // STATE: world - entities
    D(std::cout << "STATE: world - entities" << '\n');

    lua_pop(L, 2);
    // STATE: 0
    D(std::cout << "STATE: 0" << '\n');

    D(std::cout << "initialized" << '\n');
}

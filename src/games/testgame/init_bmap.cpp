#include <stdlib.h>
#include <time.h>
#include <lua.hpp>

#include <games/testgame/testgame.hpp>

std::string replace(std::string source, const std::string& from, const std::string& to)
{
    auto str = source;
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return str;
    str.replace(start_pos, from.length(), to);
    return str;
}

int set_lua_path( lua_State* L, const char* path )
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

void print_error(lua_State* state)
{
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(state, -1);
  puts(message);
  lua_pop(state, 1);
}

void TestGame::init_bmap(std::string lua)
{
    lua_State *state = luaL_newstate();

    luaL_openlibs(state);

    int result;

    set_lua_path(state, replace(lua, "world.lua", "?.lua").c_str());
    result = luaL_loadfile(state, lua.c_str());

    if (result != LUA_OK)
    {
        std::cout << "error occured in lua" << '\n';
        print_error(state);
        exit(-1);
    }

    result = lua_pcall(state, 0, LUA_MULTRET, 0);

    if ( result != LUA_OK )
    {
        std::cout << "error occured in lua" << '\n';
        print_error(state);
        exit(-1);
    }

    lua_getglobal(state, "world");

    lua_getfield(state, -1, "units");
    lua_pushnil(state);

    while (lua_next(state, -2))
    {
        lua_pop(state, 1);
    }
    lua_pop(state, 1);

    std::cout << "exit" << '\n';
    exit(1);
}

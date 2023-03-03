#include "luahost.h"

#include <unordered_map>
#include <functional>
#include <stdexcept>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

typedef int(*LuaFunction)(lua_State*) ;
std::unordered_map<std::string, LuaFunction> mLuaFunctions;
std::vector<lua_State*> mLuaStates;

static const luaL_Reg mLuaLibs[] = {
    { "", luaopen_base },
    { LUA_TABLIBNAME, luaopen_table },
    { LUA_STRLIBNAME, luaopen_string },
    { LUA_MATHLIBNAME, luaopen_math },
    { LUA_DBLIBNAME, luaopen_debug },
    { LUA_UTF8LIBNAME, luaopen_utf8 },
    { nullptr, nullptr }
};

void LuaHost::Initialize() {

}

void LuaHost::Bind(std::pair<std::string, GameBinding> binding) {
    mBindings.insert(binding);

    LuaFunction func = [](lua_State* state) -> int {
        auto *api = (LuaHost*) lua_topointer(state, lua_upvalueindex(1));
        const auto *binding = (const GameBinding*) lua_topointer(state, lua_upvalueindex(2));

        auto* result = new MethodCall(api, state);
        binding->execute(result);
        std::vector<ResultType> results = result->result();
        if (result->succeed()) {
            for (auto& value : results) {
                if (value.index() == 1) {
                    lua_pushstring(state, std::get<std::string>(value).c_str());
                } else if (value.index() == 2) {
                    lua_pushinteger(state, std::get<int>(value));
                } else if (value.index() == 3) {
                    lua_pushnumber(state, std::get<double>(value));
                } else if (value.index() == 4) {
                    lua_pushboolean(state, std::get<int>(value));
                }
            }
        } else {
            lua_pushstring(state, std::get<std::string>(results[0]).c_str());
        }
        return (int) results.size();
    };

    mLuaFunctions.insert({ binding.first, func });
}

AllowedTypes LuaHost::GetArgument(int index, void* context) {
    auto* state = (lua_State*) context;
    index += 1;

    if (lua_isstring(state, index)) {
        return luaL_checkstring(state, index);
    }

    if (lua_isnumber(state, index)) {
        return luaL_checknumber(state, index);
    }

    return nullptr;
}

uint16_t LuaHost::Execute(const std::string& script) {
    lua_State* state = luaL_newstate();

    const luaL_Reg *lib = mLuaLibs;
    for (; lib->func; lib++) {
        lua_pushcfunction(state, lib->func);
        lua_pushstring(state, lib->name);
        lua_call(state, 1, 0);
    }


    for (auto& func : mLuaFunctions) {
        lua_pushlightuserdata(state, this);
        lua_pushlightuserdata(state, &this->mBindings[func.first]);
        lua_pushcclosure(state, func.second, 2);
        lua_setglobal(state, func.first.c_str());
    }

    const int ret = luaL_dostring(state, script.c_str());
    if (ret != LUA_OK) {
        const char *error = lua_tostring(state, -1);
        printf("Lua Error: %s \n", error);
        lua_close(state);
        return 0;
    }

    mLuaStates.push_back(state);
    return (uint16_t) mLuaStates.size();
}

bool LuaHost::Kill(uint16_t pid) {
    if (pid > mLuaStates.size()) {
        return false;
    }
    lua_close(mLuaStates[pid]);
    mLuaStates.erase(mLuaStates.begin() + pid);
}
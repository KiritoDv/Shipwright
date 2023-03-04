#include "luahost.h"
#include "soh/Enhancements/scripting-layer/exceptions/hostapiexception.h"
#include "soh/Enhancements/scripting-layer/types/methodcall.h"

#include <unordered_map>
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
    { nullptr, nullptr }
};

bool LuaHost::Initialize() {
//    this->Bind("import", { BindingType::KFunction, [](MethodCall* call) {
//        auto library =
//    }});
    return true;
}

void LuaHost::Bind(std::string name, GameBinding binding) {
    mBindings[name] = binding;

    LuaFunction func = [](lua_State* state) -> int {
        auto* api = (LuaHost*) lua_topointer(state, lua_upvalueindex(1));
        const auto* binding = (const GameBinding*) lua_topointer(state, lua_upvalueindex(2));

        auto* result = new MethodCall(api, state);
        binding->execute(result);
        std::vector<std::any> results = result->result();
        if (result->succeed()) {
            for (auto& value : results) {
                if (IS_TYPE(std::string, value)) {
                    lua_pushstring(state, std::any_cast<std::string>(value).c_str());
                } else if (IS_TYPE(int, value) || IS_TYPE(bool, value)) {
                    lua_pushinteger(state, std::any_cast<int>(value));
                } else if (IS_TYPE(double, value)) {
                    lua_pushnumber(state, std::any_cast<double>(value));
                } else if (IS_TYPE(float, value)) {
                    lua_pushboolean(state, std::any_cast<int>(value));
                }
            }
        } else {
            lua_pushstring(state, std::any_cast<std::string>(results[0]).c_str());
        }
        return (int) results.size();
    };

    mLuaFunctions.insert({ name, func });
}

std::any LuaHost::GetArgument(int index, void* context) {
    auto* state = (lua_State*) context;
    index += 1;

    if (lua_isstring(state, index)) {
        return std::string(luaL_checkstring(state, index));
    }

    if (lua_isnumber(state, index)) {
        return luaL_checknumber(state, index);
    }

    if (lua_isboolean(state, index)) {
        return luaL_checkinteger(state, index);
    }

    if (lua_isnoneornil(state, index)) {
        return nullptr;
    }

    throw HostAPIException("Unknown argument type");
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
        std::string error(lua_tostring(state, -1));
        lua_close(state);
        throw HostAPIException(error);
    }

    mLuaStates.push_back(state);
    return (uint16_t) mLuaStates.size();
}

void LuaHost::Kill(uint16_t pid) {
    if (pid > mLuaStates.size()) {
        return;
    }
    lua_close(mLuaStates[pid]);
    mLuaStates.erase(mLuaStates.begin() + pid);
}
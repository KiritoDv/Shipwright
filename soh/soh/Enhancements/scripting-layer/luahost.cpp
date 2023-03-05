#include "luahost.h"
#include "soh/Enhancements/scripting-layer/exceptions/hostapiexception.h"
#include "soh/Enhancements/scripting-layer/types/methodcall.h"
#include "soh/Enhancements/scripting-layer/types/hostfunction.h"

#include <unordered_map>
#include <stdexcept>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

typedef int(*LuaFunction)(lua_State*);
typedef std::variant<LuaFunction, std::any> LuaBinding;
std::unordered_map<ModName, std::unordered_map<std::string, LuaBinding>> mLuaBindings;
std::vector<lua_State*> mLuaStates;

const char* tryCatchImpl = R"(
function try(f, catch_f)
    local status, exception = pcall(f)
    if not status then
        catch_f(exception)
    end
end
)";

static const luaL_Reg mLuaLibs[] = {
    { "", luaopen_base },
    { LUA_TABLIBNAME, luaopen_table },
    { LUA_STRLIBNAME, luaopen_string },
    { LUA_MATHLIBNAME, luaopen_math },
    { LUA_DBLIBNAME, luaopen_debug },
    { nullptr, nullptr }
};

bool LuaHost::Initialize() {
    return true;
}

void LuaHost::PushIntoLua(uintptr_t context, const std::any& value){
    auto* state = (lua_State*) context;

    if (IS_TYPE(std::string, value)) {
        lua_pushstring(state, std::any_cast<std::string>(value).c_str());
    } else if (IS_TYPE(bool, value)) {
        lua_pushboolean(state, std::any_cast<bool>(value));
    } else if (IS_TYPE(int, value)) {
        lua_pushinteger(state, std::any_cast<int>(value));
    } else if (IS_TYPE(double, value)) {
        lua_pushnumber(state, std::any_cast<double>(value));
    } else if (IS_TYPE(float, value)) {
        lua_pushboolean(state, std::any_cast<int>(value));
    } else if (IS_TYPE(std::vector<std::any>, value)) {
        lua_newtable(state);
        int index = 1;
        for (auto& item : std::any_cast<std::vector<std::any>>(value)) {
            printf("Pushing into lua: %s\n", item.type().name());
            PushIntoLua(context, item);
            lua_rawseti(state, -2, index++);
        }
    } else if (IS_TYPE(std::monostate, value) || IS_TYPE(nullptr, value)) {
        lua_pushnil(state);
    } else {
        throw HostAPIException("Unknown type" + std::string(value.type().name()));
    }
}

void LuaHost::Bind(std::string name, GameBinding binding) {
    mBindings[name] = binding;
    if (binding.type == BindingType::KFunction) {
        LuaFunction func = [](lua_State* state) -> int {
            auto* api = (LuaHost*) lua_topointer(state, lua_upvalueindex(1));
            const auto* binding = (const GameBinding*) lua_topointer(state, lua_upvalueindex(2));

            auto* result = new MethodCall(api, (uintptr_t) state);
            auto execute = std::get<FunctionPtr>(binding->binding);
            execute(result);

            std::vector<std::any> results = result->result();

            if(results.empty()){
                throw HostAPIException("No results returned from function");
            }

            if (result->succeed()) {
                for (auto& value : results) {
                    LuaHost::PushIntoLua((uintptr_t) state, value);
                }
            } else {
                luaL_error(state, std::any_cast<std::string>(results[0]).c_str());
            }
            return (int) results.size();
        };

        mLuaBindings[binding.modName].insert({ name, func });
        return;
    }

    if (binding.type == BindingType::KField) {
        mLuaBindings[binding.modName].insert({ name, std::get<std::any>(binding.binding) });
    }
}

void LuaHost::Call(uintptr_t context, uintptr_t function, const std::vector<std::any> &arguments) {

    auto* state = (lua_State*) context;
    auto func   = (char*) function;

    lua_getglobal(state, func);

    for (auto& argument : arguments) {
        LuaHost::PushIntoLua((uintptr_t) state, argument);
    }

    if (lua_pcall(state, (int) arguments.size(), 0, 0) != 0) {
        throw HostAPIException("Error while calling function: " + std::string(lua_tostring(state, -1)));
    }
}

std::any LuaHost::GetArgument(int index, uintptr_t context) {
    auto* state = (lua_State*) context;
    index += 1;

    if (lua_isinteger(state, index)) {
        return (int) luaL_checkinteger(state, index);
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

    if (lua_isstring(state, index)) {
        std::string str = luaL_checkstring(state, index);
        if(str.starts_with("func:")){
            return new HostFunction(this, (uintptr_t) state, (uintptr_t) strdup(str.substr(5).c_str()));
        }
        return str;
    }

    throw HostAPIException("Unknown argument type: " + std::string(typeid(index).name()));
}

size_t LuaHost::GetArgumentCount(uintptr_t context) {
    auto* state = (lua_State*) context;
    return lua_gettop(state);
}

uint16_t LuaHost::Execute(const std::string& script) {
    lua_State* state = luaL_newstate();

    const luaL_Reg *lib = mLuaLibs;
    for (; lib->func; lib++) {
        lua_pushcfunction(state, lib->func);
        lua_pushstring(state, lib->name);
        lua_call(state, 1, 0);
    }

    for (auto& [mod, functions] : mLuaBindings) {

        bool isModule = mod.index() == 0;

        if(isModule){
            luaL_checkversion(state);
            lua_createtable(state, 0, (int) (functions.size() - 1));
        }

        for (auto& func : functions) {

            LuaBinding binding = func.second;

            if(binding.index() == 0){
                lua_pushlightuserdata(state, this);
                lua_pushlightuserdata(state, &this->mBindings[func.first]);
                lua_pushcclosure(state, std::get<LuaFunction>(binding), 2);
            } else {
                auto value = std::get<std::any>(binding);
                LuaHost::PushIntoLua((uintptr_t) state, value);
            }

            if(isModule) {
                lua_setfield(state, -2, func.first.c_str());
            } else {
                lua_setglobal(state, func.first.c_str());
            }
        }

        if(isModule){
            lua_setglobal(state, std::get<std::string>(mod).c_str());
        }
    }

    luaL_dostring(state, tryCatchImpl);

    const int ret = luaL_dostring(state, script.c_str());
    if (ret != LUA_OK) {
        std::string error(lua_tostring(state, -1));
        lua_close(state);
        throw HostAPIException(error);
    }

    mLuaStates.push_back(state);
    return (uint16_t) mLuaStates.size() - 1;
}

void LuaHost::Kill(uint16_t pid) {
    if (pid > mLuaStates.size()) {
        return;
    }
    lua_close(mLuaStates[pid]);
    mLuaStates.erase(mLuaStates.begin() + pid);
}
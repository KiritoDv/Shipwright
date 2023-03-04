#pragma once

#include <utility>
#include <string>
#include <any>
#include <vector>

struct GameBinding;

#define IS_TYPE(t, value) value.type() == typeid(t)

class HostAPI {
public:
    virtual bool Initialize() = 0;
    virtual void Bind(std::string name, GameBinding binding) = 0;
    virtual void Call(uintptr_t context, uintptr_t function, const std::vector<std::any>& arguments) = 0;
    // TODO: force_string is a hack for the lua host, if we prioritize the string type over the other types it forces lua to return an string
    virtual std::any GetArgument(int index, uintptr_t context, bool force_string) = 0;
    virtual uint16_t Execute(const std::string& script) = 0;
    virtual void Kill(uint16_t pid) = 0;
};
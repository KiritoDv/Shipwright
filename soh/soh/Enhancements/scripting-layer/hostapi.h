#pragma once

#include <utility>
#include <string>
#include <any>
#include <vector>

struct GameBinding;

#define IS_TYPE(type, value) (typeid(value) == typeid(type))

class HostAPI {
public:
    virtual bool Initialize() = 0;
    virtual void Bind(std::string name, GameBinding binding) = 0;
    virtual void Call(void* context, void* function, const std::vector<std::any>& arguments) = 0;
    virtual std::any GetArgument(int index, void* context) = 0;
    virtual uint16_t Execute(const std::string& script) = 0;
    virtual void Kill(uint16_t pid) = 0;
};
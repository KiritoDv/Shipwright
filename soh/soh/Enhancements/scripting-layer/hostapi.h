#pragma once

#include <utility>
#include <string>
#include "soh/Enhancements/scripting-layer/types/methodcall.h"

class HostAPI {
public:
    virtual void Initialize() = 0;
    virtual void Bind(std::pair<std::string, GameBinding> binding) = 0;
    virtual AllowedTypes GetArgument(int index, void* context) = 0;
    virtual uint16_t Execute(const std::string& script) = 0;
    virtual bool Kill(uint16_t pid) = 0;
};
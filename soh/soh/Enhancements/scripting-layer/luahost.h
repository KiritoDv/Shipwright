#pragma once

#include "hostapi.h"
#include <unordered_map>

class LuaHost : public HostAPI {
private:
    std::unordered_map<std::string, GameBinding> mBindings;
public:
    void Initialize() override;
    void Bind(std::pair<std::string, GameBinding> binding) override;
    AllowedTypes GetArgument(int index, void* context) override;
    uint16_t Execute(const std::string& script) override;
    bool Kill(uint16_t pid) override;
};
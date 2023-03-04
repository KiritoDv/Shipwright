#pragma once

#include "hostapi.h"
#include <unordered_map>

class LuaHost : public HostAPI {
private:
    std::unordered_map<std::string, GameBinding> mBindings;
public:
    bool Initialize() override;
    void Bind(std::string name, GameBinding binding) override;
    void Call(uintptr_t context, uintptr_t function, const std::vector<std::any>& arguments) override;
    std::any GetArgument(int index, uintptr_t context, bool force_string) override;
    uint16_t Execute(const std::string& script) override;
    void Kill(uint16_t pid) override;
};
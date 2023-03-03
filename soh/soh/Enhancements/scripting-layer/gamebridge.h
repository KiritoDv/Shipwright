#pragma once

#include "hostapi.h"
#include "types/methodcall.h"
#include <unordered_map>
#include <string>
#include <memory>

class GameBridge {
private:
    std::unordered_map<std::string, std::shared_ptr<HostAPI>> hosts;
    std::unordered_map<std::string, GameBinding> bindings;
    void RegisterHost(const std::string& name, std::shared_ptr<HostAPI> host);
    void BindFunction(const std::string& name, FunctionPtr function);
public:
    GameBridge() {
        this->Initialize();
    }
    static GameBridge* Instance;
    void Initialize();
    uint16_t Execute(const std::string& script, const std::string& hostname);
    bool Kill(uint16_t pid, const std::string& hostname);
};
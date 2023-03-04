#pragma once

#include "hostapi.h"
#include "types/methodcall.h"
#include <unordered_map>
#include <string>
#include <memory>

class GameBridge {
private:
    std::unordered_map<std::string, std::shared_ptr<HostAPI>> hosts;
    std::unordered_map<uint16_t, std::shared_ptr<HostAPI>> pids;
    void RegisterHost(const std::string& name, std::shared_ptr<HostAPI> host);
public:
    GameBridge() {
        this->Initialize();
    }
    static GameBridge* Instance;
    void Initialize();
    uint16_t Execute(const std::string& script, const std::string& hostname);
    void BindFunction(const std::string& name, FunctionPtr function, const std::variant<std::string, std::monostate>& mod_name = std::monostate());
    void Kill(uint16_t pid);
};
#include "gamebridge.h"
#include "luahost.h"
#include "ImGuiImpl.h"
#include <filesystem>
#include <fstream>

static bool RunScript(std::shared_ptr<Ship::Console> console, const std::vector<std::string>& args) {
    if (args.size() > 1) {
        auto path = std::filesystem::path("scripts") / args[1];

        if(!std::filesystem::exists(path)) {
            console->SendErrorMessage("Script not found: %s", args[1].c_str());
            return CMD_SUCCESS;
        }

        std::ifstream file(path);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        uint16_t pid = GameBridge::Instance->Execute(content, "lua");
        console->SendInfoMessage("Script %s started with pid %d", args[1].c_str(), pid);
    }
    return CMD_SUCCESS;
}

void GameBridge::Initialize() {
    this->RegisterHost("lua", std::make_shared<LuaHost>());
    this->BindFunction("print", [](MethodCall *method) {
        std::string message = std::get<std::string>(method->getArgument(0));
        SohImGui::GetConsole()->SendInfoMessage(message.c_str());
        method->success();
    });
    SohImGui::GetConsole()->AddCommand("run", { RunScript, "Tries to execute an script", {
        { "path", Ship::ArgumentType::TEXT, true }
    }});
}

uint16_t GameBridge::Execute(const std::string& script, const std::string& hostname){
    auto host = this->hosts.find(hostname);
    if(host == this->hosts.end()){
        return -1;
    }

    return host->second->Execute(script);
}

bool GameBridge::Kill(uint16_t pid, const std::string& hostname){
    auto host = this->hosts.find(hostname);
    if(host == this->hosts.end()){
        return false;
    }

    return host->second->Kill(pid);
}

void GameBridge::RegisterHost(const std::string& name, std::shared_ptr<HostAPI> host){
    this->hosts[name] = std::move(host);
}

void GameBridge::BindFunction(const std::string& name, FunctionPtr function){
    this->bindings[name] = { BindingType::KFunction, function };
    for(auto& host : this->hosts){
        host.second->Bind(std::make_pair(name, this->bindings[name]));
    }
}

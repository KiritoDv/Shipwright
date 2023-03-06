#include "gamebridge.h"
#include "luahost.h"
#include "ImGuiImpl.h"
#include <filesystem>
#include <fstream>
#include "soh/Enhancements/scripting-layer/exceptions/gamebridgeexception.h"
#include "soh/Enhancements/scripting-layer/types/hostfunction.h"
#include "soh/Enhancements/game-interactor/GameInteractor.h"

static bool RunScript(const std::shared_ptr<Ship::Console>& console, const std::vector<std::string>& args) {
    if (args.size() > 1) {
        auto path = std::filesystem::path("scripts") / args[1];
        auto ext = path.extension().string().substr(1);

        if(!std::filesystem::exists(path)) {
            console->SendErrorMessage("Script not found: %s", args[1].c_str());
            return CMD_SUCCESS;
        }

        if(ext == "moon"){
            ext = "lua";
        }

        std::ifstream file(path);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        if(content.empty()) {
            console->SendErrorMessage("Script is empty: %s", args[1].c_str());
            return CMD_SUCCESS;
        }

        try {
            uint16_t pid = GameBridge::Instance->Execute(content, ext);
            console->SendInfoMessage("Script %s started with pid %d", args[1].c_str(), pid);
        } catch (GameBridgeException& e) {
            console->SendErrorMessage("Error while executing script: %s", e.what());
        } catch (HostAPIException& e) {
            console->SendErrorMessage("Error while executing script: %s", e.what());
        }
    } else {
        console->SendErrorMessage("Usage: run <path>");
    }
    return CMD_SUCCESS;
}

static bool KillScript(const std::shared_ptr<Ship::Console>& console, const std::vector<std::string>& args) {
    if (args.size() > 1) {
        try {
            uint16_t pid = std::stoi(args[1]);
            GameBridge::Instance->Kill(pid);
            console->SendInfoMessage("Script with pid %d killed", pid);
        } catch (std::invalid_argument& e) {
            console->SendErrorMessage("Invalid pid: %s", args[1].c_str());
        }
    } else {
        console->SendErrorMessage("Usage: kill <pid>");
    }
    return CMD_SUCCESS;
}

void GameBridge::Initialize() {
    this->RegisterHost("lua", std::make_shared<LuaHost>());
    this->BindFunction("print", [](uintptr_t ctx, MethodCall* method) {
        size_t count = method->ArgumentCount();
        std::stringstream message;
        for(size_t i = 0; i < count; i++) {
            message << " ";
            std::any value = method->RawArgument(i);
            if (IS_TYPE(std::string, value)) {
                message << std::any_cast<std::string>(value);
            } else if (IS_TYPE(bool, value)) {
                message << (std::any_cast<bool>(value) ? "true" : "false");
            } else if (IS_TYPE(int, value)) {
                message << std::any_cast<int>(value);
            } else if (IS_TYPE(double, value)) {
                message << std::any_cast<double>(value);
            } else if (IS_TYPE(std::monostate, value) || IS_TYPE(nullptr, value)) {
                message << "null";
            } else {
                message << "unknown[" << value.type().name() << "]";
            }
        }
        SohImGui::GetConsole()->SendInfoMessage(message.str().c_str());
        method->success();
    });
    this->BindFunction("hook", [](uintptr_t ctx, MethodCall* method) {
        auto hook = method->GetArgument<std::string>(0);
        auto function = method->GetArgument<HostFunction*>(1);
        if(hook == "update"){
            GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([function]() {
               try {
                    function->execute();
                } catch (HostAPIException& e) {
                    SohImGui::GetConsole()->SendErrorMessage("Error while executing script: %s", e.what());
                }
            });
        }
        method->success();
    }, "game");
    SohImGui::GetConsole()->AddCommand("run", { RunScript, "Tries to execute an script", {
        { "path", Ship::ArgumentType::TEXT, true }
    }});
    SohImGui::GetConsole()->AddCommand("stop", { KillScript, "Tries to kill a program with its pid", {
        { "pid", Ship::ArgumentType::NUMBER, true }
    }});
}

uint16_t GameBridge::Execute(const std::string& script, const std::string& hostname){
    auto host = this->hosts.find(hostname);
    if(host == this->hosts.end()){
        throw GameBridgeException("Host api not found: " + hostname);
    }

    uint16_t pid = host->second->Execute(script);
    this->pids[pid] = host->second;

    return pid;
}

void GameBridge::Kill(uint16_t pid){
    if(!this->pids.contains(pid)){
        return;
    }

    auto host = this->pids.find(pid);
    return host->second->Kill(pid);
}

void GameBridge::RegisterHost(const std::string& name, std::shared_ptr<HostAPI> host){
    if(!host->Initialize()){
        throw GameBridgeException("Failed to initialize host api: " + name);
    }
    this->hosts[name] = std::move(host);
}

void GameBridge::BindField(const std::string& name, const std::any& field, const std::variant<std::string, std::monostate>& mod_name){
    for(auto& host : this->hosts){
        host.second->Bind(name, { BindingType::KField, field, mod_name });
    }
}

void GameBridge::BindFunction(const std::string& name, FunctionPtr function, const std::variant<std::string, std::monostate>& mod_name){
    for(auto& host : this->hosts){
        host.second->Bind(name, { BindingType::KFunction, function, mod_name });
    }
}

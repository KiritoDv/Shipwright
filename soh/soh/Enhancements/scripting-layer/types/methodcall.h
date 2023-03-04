#pragma once

#include <string>
#include <vector>
#include <variant>
#include <cstdarg>
#include <any>
#include "soh/Enhancements/scripting-layer/hostapi.h"
#include "soh/Enhancements/scripting-layer/exceptions/hostapiexception.h"

class MethodCall;

typedef void (*FunctionPtr)(MethodCall*);

enum class BindingType {
    KFunction
};

struct GameBinding {
    BindingType type;
    FunctionPtr execute;
    ModName modName;
};

class MethodCall {
private:
    HostAPI* mHost;
    uintptr_t mContext;
    std::vector<std::any> mResult;
    bool mSuccess = false;
public:
    explicit MethodCall(HostAPI* host, uintptr_t context) : mHost(host), mContext(context) {}

    template<typename T>
    T GetArgument(int index, bool force_string = false){
        try {
            return std::any_cast<T>(this->mHost->GetArgument(index, this->mContext, force_string));
        } catch (const std::bad_any_cast& e) {
            throw HostAPIException("Argument " + std::to_string(index) + " is not of type " + typeid(T).name());
        }
    }

    std::any RawArgument(int index){
        return this->mHost->GetArgument(index, this->mContext, false);
    }

    // Response methods
    void success();
    void success(std::any result...);
    void error(const std::string& error);

    [[nodiscard]] bool succeed() const {
        return this->mSuccess;
    }

    [[nodiscard]] std::vector<std::any> result() const {
        return this->mResult;
    }
};
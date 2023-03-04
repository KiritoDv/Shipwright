#pragma once

#include <string>
#include <vector>
#include <variant>
#include <cstdarg>
#include <any>
#include "soh/Enhancements/scripting-layer/hostapi.h"

class MethodCall;

typedef void (*FunctionPtr)(MethodCall*);

enum class BindingType {
    KFunction
};

struct GameBinding {
    BindingType type;
    FunctionPtr execute;
};

class MethodCall {
private:
    HostAPI* mHost;
    void* mContext;
    std::vector<std::any> mResult;
    bool mSuccess = false;
public:
    explicit MethodCall(HostAPI* host, void* context) : mHost(host), mContext(context) {}

    template<typename T>
    T GetArgument(int index){
        return std::any_cast<T>(this->mHost->GetArgument(index, this->mContext));
    }

    // Response methods
    void success();
    void success(std::any result, ...);
    void error(const std::string& error);

    [[nodiscard]] bool succeed() const {
        return this->mSuccess;
    }

    [[nodiscard]] std::vector<std::any> result() const {
        return this->mResult;
    }
};
#pragma once

#include <string>
#include <vector>
#include <variant>
#include <cstdarg>

class HostAPI;
class MethodCall;

typedef std::variant<std::monostate, std::string, int32_t, double, float> AllowedTypes;
typedef std::variant<std::monostate, std::string, int32_t, double, float, std::vector<AllowedTypes>> ResultType;
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
    std::vector<ResultType> mResult;
    bool mSuccess = false;
public:
    explicit MethodCall(HostAPI* host, void* context) : mHost(host), mContext(context) {}

    AllowedTypes getArgument(int index);

    // Response methods
    void success();
    void success(ResultType result, ...);
    void error(const std::string& error);

    [[nodiscard]] bool succeed() const {
        return this->mSuccess;
    }

    [[nodiscard]] std::vector<ResultType> result() const {
        return this->mResult;
    }
};
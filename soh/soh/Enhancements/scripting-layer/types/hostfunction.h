#pragma once

#include <vector>
#include "soh/Enhancements/scripting-layer/hostapi.h"

class HostFunction {
private:
    HostAPI* mHost;
    void* mContext;
    void* mFunction;
    std::vector<std::any> mArguments;
public:
    explicit HostFunction(HostAPI* host, void* context, void* function) : mHost(host), mContext(context), mFunction(function) {}

    void PushArgument(const std::any& argument);
    void execute();
};
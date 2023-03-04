#pragma once

#include <vector>
#include "soh/Enhancements/scripting-layer/hostapi.h"

class HostFunction {
private:
    HostAPI* mHost;
    uintptr_t mContext;
    uintptr_t mFunction;
public:
    explicit HostFunction(HostAPI* host, uintptr_t context, uintptr_t function) : mHost(host), mContext(context), mFunction(function) {}
    void execute();
    void execute(std::any result...);
};
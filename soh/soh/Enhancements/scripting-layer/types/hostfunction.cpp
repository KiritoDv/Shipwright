#include "hostfunction.h"

void HostFunction::PushArgument(const std::any& argument) {
    mArguments.push_back(argument);
}

void HostFunction::execute() {
    mHost->Call(mContext, mFunction, mArguments);
}
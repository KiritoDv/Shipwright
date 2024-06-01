#include "hostfunction.h"

void HostFunction::execute() const {
    mHost->Call(mContext, mFunction, {});
}

void HostFunction::execute(std::any result...) const {
    mHost->Call(mContext, mFunction, { std::move(result) });
}
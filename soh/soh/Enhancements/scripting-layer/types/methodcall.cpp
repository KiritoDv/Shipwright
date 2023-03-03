#include "methodcall.h"
#include "../hostapi.h"

AllowedTypes MethodCall::getArgument(int index){
    return this->mHost->GetArgument(index, this->mContext);
}

void MethodCall::success(){
    this->mResult.emplace_back(std::monostate());
    this->mSuccess = true;
}

void MethodCall::success(ResultType result, ...){
    this->mSuccess = true;
    va_list args;
    va_start(args, result);
    while(true){
        auto arg = va_arg(args, ResultType);
        if(arg.index() == 0){
            break;
        }
        this->mResult.emplace_back(std::move(arg));
    }
}

void MethodCall::error(const std::string& error){
    this->mResult.emplace_back(error);
    this->mSuccess = false;
}

#include "methodcall.h"
#include "../hostapi.h"

void MethodCall::success(){
    this->mResult.emplace_back(std::monostate());
    this->mSuccess = true;
}

void MethodCall::success(std::any result, ...){
    this->mSuccess = true;
    va_list args;
    va_start(args, result);
    while(true){
        auto arg = va_arg(args, std::any);
        if(arg.type() == typeid(std::monostate)){
            break;
        }
        this->mResult.emplace_back(std::move(arg));
    }
}

void MethodCall::error(const std::string& error){
    this->mResult.emplace_back(error);
    this->mSuccess = false;
}

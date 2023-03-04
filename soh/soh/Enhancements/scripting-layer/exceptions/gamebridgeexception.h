#pragma once

#include <exception>
#include <string>
#include <utility>

class GameBridgeException : public std::exception {
private:
    std::string mMessage;
public:
    explicit GameBridgeException(std::string message) : mMessage(std::move(message)) {}
    [[nodiscard]] const char* what() const noexcept override {
        return mMessage.c_str();
    }
};
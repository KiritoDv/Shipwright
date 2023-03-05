#include "soh-bridge.h"
#include "soh/UIWidgets.hpp"
#include "soh/Enhancements/scripting-layer/gamebridge.h"
#include "ImGuiImpl.h"
#include "soh/Enhancements/scripting-layer/types/hostfunction.h"

void BindUIWidgets(){
    GameBridge::Instance->BindFunction("WrappedText", [](MethodCall *method) {
        auto text = method->GetArgument<std::string>(0);
        auto charactersPerLine = method->GetArgument<int>(1);
        auto result = UIWidgets::WrappedText(text, charactersPerLine);
        method->success(std::string(result));
    }, "UIWidgets");
    GameBridge::Instance->BindFunction("SetLastItemHoverText", [](MethodCall *method) {
        auto text = method->GetArgument<std::string>(0);
        UIWidgets::SetLastItemHoverText(text);
        method->success();
    }, "UIWidgets");
    GameBridge::Instance->BindFunction("InsertHelpHoverText", [](MethodCall *method) {
        auto text = method->GetArgument<std::string>(0);
        UIWidgets::InsertHelpHoverText(text);
        method->success();
    }, "UIWidgets");
    GameBridge::Instance->BindFunction("Tooltip", [](MethodCall *method) {
        auto text = method->GetArgument<std::string>(0);
        UIWidgets::Tooltip(text.c_str());
        method->success();
    }, "UIWidgets");
    GameBridge::Instance->BindFunction("Tooltip", [](MethodCall *method) {
        auto text = method->GetArgument<std::string>(0);
        UIWidgets::Tooltip(text.c_str());
        method->success();
    }, "UIWidgets");
    GameBridge::Instance->BindFunction("Spacer", [](MethodCall *method) {
        auto height = method->GetArgument<float>(0);
        UIWidgets::Spacer(height);
        method->success();
    }, "UIWidgets");
}

void BindSohImgui(){
    GameBridge::Instance->BindFunction("AddWindow", [](MethodCall *method) {
        auto category = method->GetArgument<std::string>(0);
        auto name = method->GetArgument<std::string>(1);
        auto callback = method->GetArgument<HostFunction*>(2);
        SohImGui::AddWindow(category, name, [callback](bool& open){
            callback->execute(open);
        });
        method->success();
    }, "SohImGui");
}

void SOHBridge::Initialize() {
    BindUIWidgets();
    BindSohImgui();
}
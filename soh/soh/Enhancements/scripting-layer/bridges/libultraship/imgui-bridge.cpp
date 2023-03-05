#include "imgui-bridge.h"
#include <extern/ImGui/imgui.h>
#include "soh/Enhancements/scripting-layer/gamebridge.h"

#define BIND_FLAG(flag) GameBridge::Instance->BindField(#flag, (int) flag, "ImGui");

void ImGuiBridge::Initialize() {
    BIND_FLAG(ImGuiWindowFlags_None);
    BIND_FLAG(ImGuiWindowFlags_NoTitleBar);
    BIND_FLAG(ImGuiWindowFlags_NoResize);
    BIND_FLAG(ImGuiWindowFlags_NoMove);
    BIND_FLAG(ImGuiWindowFlags_NoScrollbar);
    BIND_FLAG(ImGuiWindowFlags_NoScrollWithMouse);
    BIND_FLAG(ImGuiWindowFlags_NoCollapse);
    BIND_FLAG(ImGuiWindowFlags_AlwaysAutoResize);
    BIND_FLAG(ImGuiWindowFlags_NoBackground);
    BIND_FLAG(ImGuiWindowFlags_NoSavedSettings);
    BIND_FLAG(ImGuiWindowFlags_NoMouseInputs);
    BIND_FLAG(ImGuiWindowFlags_MenuBar);
    BIND_FLAG(ImGuiWindowFlags_HorizontalScrollbar);
    BIND_FLAG(ImGuiWindowFlags_NoFocusOnAppearing);
    BIND_FLAG(ImGuiWindowFlags_NoBringToFrontOnFocus);
    BIND_FLAG(ImGuiWindowFlags_AlwaysVerticalScrollbar);
    BIND_FLAG(ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    BIND_FLAG(ImGuiWindowFlags_AlwaysUseWindowPadding);
    BIND_FLAG(ImGuiWindowFlags_NoNavInputs);
    BIND_FLAG(ImGuiWindowFlags_NoNavFocus);
    BIND_FLAG(ImGuiWindowFlags_UnsavedDocument);
    BIND_FLAG(ImGuiWindowFlags_NoNav);
    BIND_FLAG(ImGuiWindowFlags_NoDecoration);
    BIND_FLAG(ImGuiWindowFlags_NoInputs);
    BIND_FLAG(ImGuiWindowFlags_NavFlattened);
    BIND_FLAG(ImGuiWindowFlags_ChildWindow);
    BIND_FLAG(ImGuiWindowFlags_Tooltip);
    BIND_FLAG(ImGuiWindowFlags_Popup);
    BIND_FLAG(ImGuiWindowFlags_Modal);
    BIND_FLAG(ImGuiWindowFlags_ChildMenu);
    BIND_FLAG(ImGuiButtonFlags_None);
    BIND_FLAG(ImGuiButtonFlags_MouseButtonLeft);
    BIND_FLAG(ImGuiButtonFlags_MouseButtonRight);
    BIND_FLAG(ImGuiButtonFlags_MouseButtonMiddle);
    BIND_FLAG(ImGuiButtonFlags_MouseButtonMask_);
    BIND_FLAG(ImGuiButtonFlags_MouseButtonDefault_);

    GameBridge::Instance->BindFunction("Begin", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::Begin(name.c_str(), nullptr, flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("End", [](MethodCall *method) {
        ImGui::End();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginChild", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto size = method->GetArgument<ImVec2>(1);
        auto border = method->GetArgument<bool>(2);
        auto flags = method->GetArgument<int>(3);
        auto result = ImGui::BeginChild(name.c_str(), size, border, flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndChild", [](MethodCall *method) {
        ImGui::EndChild();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginChildFrame", [](MethodCall *method) {
        auto id = method->GetArgument<int>(0);
        auto size = method->GetArgument<ImVec2>(1);
        auto flags = method->GetArgument<int>(2);
        auto result = ImGui::BeginChildFrame(id, size, flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndChildFrame", [](MethodCall *method) {
        ImGui::EndChildFrame();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginTabBar", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginTabBar(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndTabBar", [](MethodCall *method) {
        ImGui::EndTabBar();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginTabItem", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(2);
        auto result = ImGui::BeginTabItem(name.c_str(), nullptr, flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndTabItem", [](MethodCall *method) {
        ImGui::EndTabItem();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginMenuBar", [](MethodCall *method) {
        auto result = ImGui::BeginMenuBar();
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndMenuBar", [](MethodCall *method) {
        ImGui::EndMenuBar();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginMainMenuBar", [](MethodCall *method) {
        auto result = ImGui::BeginMainMenuBar();
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndMainMenuBar", [](MethodCall *method) {
        ImGui::EndMainMenuBar();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginMenu", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto enabled = method->GetArgument<bool>(1);
        auto result = ImGui::BeginMenu(name.c_str(), enabled);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndMenu", [](MethodCall *method) {
        ImGui::EndMenu();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopup", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopup(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndPopup", [](MethodCall *method) {
        ImGui::EndPopup();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupModal", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(2);
        auto result = ImGui::BeginPopupModal(name.c_str(), nullptr, flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndPopup", [](MethodCall *method) {
        ImGui::EndPopup();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextItem", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextItem(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextWindow", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextWindow(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextVoid", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextVoid(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginTooltip", [](MethodCall *method) {
        ImGui::BeginTooltip();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("EndTooltip", [](MethodCall *method) {
        ImGui::EndTooltip();
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextItem", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextItem(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextWindow", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextWindow(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextVoid", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextVoid(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextItem", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextItem(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextWindow", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextWindow(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("BeginPopupContextVoid", [](MethodCall *method) {
        auto name = method->GetArgument<std::string>(0);
        auto flags = method->GetArgument<int>(1);
        auto result = ImGui::BeginPopupContextVoid(name.c_str(), flags);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("MenuItem", [](MethodCall *method) {
        auto label = method->GetArgument<std::string>(0);
        auto shortcut = method->GetArgument<std::string>(1);
        auto selected = method->GetArgument<bool>(2);
        auto enabled = method->GetArgument<bool>(3);
        auto result = ImGui::MenuItem(label.c_str(), shortcut.c_str(), selected, enabled);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("Text", [](MethodCall *method) {
        auto fmt = method->GetArgument<std::string>(0);
        ImGui::Text(fmt.c_str());
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("TextColored", [](MethodCall *method) {
        auto raw = method->GetArgument<std::vector<std::any>>(0);
        auto fmt = method->GetArgument<std::string>(1);
        auto col = ImVec4(std::any_cast<double>(raw[0]), std::any_cast<double>(raw[1]), std::any_cast<double>(raw[2]), std::any_cast<double>(raw[3]));
        ImGui::TextColored(col, fmt.c_str());
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("TextDisabled", [](MethodCall *method) {
        auto fmt = method->GetArgument<std::string>(0);
        ImGui::TextDisabled(fmt.c_str());
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("TextWrapped", [](MethodCall *method) {
        auto fmt = method->GetArgument<std::string>(0);
        ImGui::TextWrapped(fmt.c_str());
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("LabelText", [](MethodCall *method) {
        auto label = method->GetArgument<std::string>(0);
        auto fmt = method->GetArgument<std::string>(1);
        ImGui::LabelText(label.c_str(), fmt.c_str());
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("BulletText", [](MethodCall *method) {
        auto fmt = method->GetArgument<std::string>(0);
        ImGui::BulletText(fmt.c_str());
        method->success();
    }, "ImGui");
    GameBridge::Instance->BindFunction("Button", [](MethodCall *method) {
        auto count = method->ArgumentCount();
        if(count == 1) {
            auto label = method->GetArgument<std::string>(0);
            auto result = ImGui::Button(label.c_str());
            method->success(result);
        } else if(count == 3) {
            auto label = method->GetArgument<std::string>(0);
            auto width = method->GetArgument<float>(1);
            auto height = method->GetArgument<float>(2);
            auto result = ImGui::Button(label.c_str(), ImVec2(width, height));
            method->success(result);
        }
    }, "ImGui");
    GameBridge::Instance->BindFunction("SmallButton", [](MethodCall *method) {
        auto label = method->GetArgument<std::string>(0);
        auto result = ImGui::SmallButton(label.c_str());
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("InvisibleButton", [](MethodCall *method) {
        auto label = method->GetArgument<std::string>(0);
        auto size = method->GetArgument<std::vector<std::any>>(1);
        auto result = ImGui::InvisibleButton(label.c_str(), ImVec2(std::any_cast<double>(size[0]), std::any_cast<double>(size[1])));
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("ArrowButton", [](MethodCall *method) {
        auto label = method->GetArgument<std::string>(0);
        auto dir = method->GetArgument<int>(1);
        auto result = ImGui::ArrowButton(label.c_str(), dir);
        method->success(result);
    }, "ImGui");
    GameBridge::Instance->BindFunction("SameLine", [](MethodCall *method) {
        ImGui::SameLine();
        method->success();
    }, "ImGui");
}
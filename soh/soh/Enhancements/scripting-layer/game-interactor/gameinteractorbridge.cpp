#include "gameinteractorbridge.h"
#include "soh/Enhancements/game-interactor/GameInteractor.h"

void GameInteractorBridge::Initialize() {
    GameBridge::Instance->BindFunction("AddOrRemoveHealthContainers", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::AddOrRemoveHealthContainers((int16_t) amount);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("AddOrRemoveMagic", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::AddOrRemoveMagic((int8_t) amount);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("HealOrDamagePlayer", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::HealOrDamagePlayer((int16_t) amount);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("SetPlayerHealth", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::SetPlayerHealth((int16_t) amount);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("SetLinkInvisibility", [](MethodCall *method) {
        auto active = method->GetArgument<bool>(0);
        GameInteractor::RawAction::SetLinkInvisibility(active);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("SetWeatherStorm", [](MethodCall *method) {
        auto active = method->GetArgument<bool>(0);
        GameInteractor::RawAction::SetWeatherStorm(active);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("ForceEquipBoots", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::ForceEquipBoots((int8_t) amount);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("FreezePlayer", [](MethodCall *method) {
        GameInteractor::RawAction::FreezePlayer();
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("BurnPlayer", [](MethodCall *method) {
        GameInteractor::RawAction::BurnPlayer();
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("ElectrocutePlayer", [](MethodCall *method) {
        GameInteractor::RawAction::ElectrocutePlayer();
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("KnockbackPlayer", [](MethodCall *method) {
        auto strength = method->GetArgument<double>(0);
        GameInteractor::RawAction::KnockbackPlayer((float) strength);
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("GiveDekuShield", [](MethodCall *method) {
        GameInteractor::RawAction::GiveDekuShield();
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("SpawnCuccoStorm", [](MethodCall *method) {
        GameInteractor::RawAction::SpawnCuccoStorm();
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("ForceInterfaceUpdate", [](MethodCall *method) {
        GameInteractor::RawAction::ForceInterfaceUpdate();
        method->success();
    }, "GI");
    GameBridge::Instance->BindFunction("SpawnEnemyWithOffset", [](MethodCall *method) {
        auto enemyId     = method->GetArgument<int>(0);
        auto enemyParams = method->GetArgument<int>(1);
        int result = GameInteractor::RawAction::SpawnEnemyWithOffset(enemyId, enemyParams);
        method->success(result);
    }, "GI");
}
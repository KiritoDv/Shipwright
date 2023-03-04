#include "gameinteractorbridge.h"
#include "soh/Enhancements/game-interactor/GameInteractor.h"

void GameInteractorBridge::Initialize() {
    GameBridge::Instance->BindFunction("GI_AddOrRemoveHealthContainers", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::AddOrRemoveHealthContainers((int16_t) amount);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_AddOrRemoveMagic", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::AddOrRemoveMagic((int8_t) amount);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_HealOrDamagePlayer", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::HealOrDamagePlayer((int16_t) amount);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_SetPlayerHealth", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::SetPlayerHealth((int16_t) amount);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_SetLinkInvisibility", [](MethodCall *method) {
        auto active = method->GetArgument<bool>(0);
        GameInteractor::RawAction::SetLinkInvisibility(active);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_SetWeatherStorm", [](MethodCall *method) {
        auto active = method->GetArgument<bool>(0);
        GameInteractor::RawAction::SetWeatherStorm(active);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_ForceEquipBoots", [](MethodCall *method) {
        auto amount = method->GetArgument<int>(0);
        GameInteractor::RawAction::ForceEquipBoots((int8_t) amount);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_FreezePlayer", [](MethodCall *method) {
        GameInteractor::RawAction::FreezePlayer();
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_BurnPlayer", [](MethodCall *method) {
        GameInteractor::RawAction::BurnPlayer();
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_ElectrocutePlayer", [](MethodCall *method) {
        GameInteractor::RawAction::ElectrocutePlayer();
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_KnockbackPlayer", [](MethodCall *method) {
        auto strength = method->GetArgument<double>(0);
        GameInteractor::RawAction::KnockbackPlayer((float) strength);
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_GiveDekuShield", [](MethodCall *method) {
        GameInteractor::RawAction::GiveDekuShield();
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_SpawnCuccoStorm", [](MethodCall *method) {
        GameInteractor::RawAction::SpawnCuccoStorm();
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_ForceInterfaceUpdate", [](MethodCall *method) {
        GameInteractor::RawAction::ForceInterfaceUpdate();
        method->success();
    });
    GameBridge::Instance->BindFunction("GI_SpawnEnemyWithOffset", [](MethodCall *method) {
        auto enemyId     = method->GetArgument<int>(0);
        auto enemyParams = method->GetArgument<int>(1);
        int result = GameInteractor::RawAction::SpawnEnemyWithOffset(enemyId, enemyParams);
        method->success(result);
    });
}
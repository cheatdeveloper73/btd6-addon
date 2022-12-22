#include "hooks.h"

#include "../scripting/scripting.h"
#include "../ui/ui.h"

bool hooks::hook_function(void* function_pointer, void* callback_function)
{

	hooked_function fn;
	fn.callback_function = callback_function;
	fn.target_address = function_pointer;

	MH_CreateHook((uintptr_t*)function_pointer, callback_function, (LPVOID*)&fn.original_function);

	hooks.push_back(fn);

	return MH_EnableHook(function_pointer) == MH_OK;

}

using f_simulate = void(__cdecl*)(Assets_Scripts_Simulation_Simulation_o*);
void __cdecl hk_simulate(Assets_Scripts_Simulation_Simulation_o* thisptr)
{

	for (auto script : scripting.get_current_scripts())
		for (auto callback : script.callbacks)
			if (callback.callback_name == "OnSimulate")
				scripting.run_script_from_function(callback.function, thisptr);

	if (ui::infinite_cash)
		il2cpp::call_function<void>(il2cpp::get_method("Assets.Scripts.Simulation", "Simulation", "SetCash"), thisptr, static_cast<double>(999999999.0), -1);
	if (ui::infinite_health)
		il2cpp::call_function<void>(il2cpp::get_method("Assets.Scripts.Simulation", "Simulation", "SetHealth"), thisptr, 99999999.f);
	if (ui::instant_kill_bloons)
		il2cpp::call_function<void>(il2cpp::get_method("Assets.Scripts.Simulation", "Simulation", "DestroyAllBloons"), thisptr);
	if (ui::instant_win_game)
	{

		int end_round = il2cpp::call_function<int>(il2cpp::get_method("Assets.Scripts.Simulation", "Simulation", "GetEndRound"), thisptr);
		il2cpp::call_function<void>(il2cpp::get_method("Assets.Scripts.Simulation", "Simulation", "RoundEnd"), thisptr, end_round, end_round);

	}

	if (thisptr->fields.model)
	{

		thisptr->fields.model->fields.globalBloonScale = ui::bloon_size_scale;
		thisptr->fields.model->fields.globalTowerScale = ui::tower_size_scale;

	}

	thisptr->fields.instantCooldowns = ui::instant_ability_cooldown;

	static bool set_speed = false;

	if (ui::faster_forward)
	{

		il2cpp::set_static_field_value<float>(il2cpp::find_class("Assets.Scripts.Utils", "TimeManager"), "networkScale", ui::faster_forward_speed);
		il2cpp::set_static_field_value<float>(il2cpp::find_class("Assets.Scripts.Utils", "TimeManager"), "timeScaleWithoutNetwork", ui::faster_forward_speed);
		il2cpp::set_static_field_value<float>(il2cpp::find_class("Assets.Scripts.Utils", "TimeManager"), "maxSimulationStepsPerUpdate", ui::faster_forward_speed);
		set_speed = true;

	}
	else if (set_speed && !ui::faster_forward)
	{

		il2cpp::set_static_field_value<float>(il2cpp::find_class("Assets.Scripts.Utils", "TimeManager"), "networkScale", 1.f);
		il2cpp::set_static_field_value<float>(il2cpp::find_class("Assets.Scripts.Utils", "TimeManager"), "timeScaleWithoutNetwork", 1.f);
		il2cpp::set_static_field_value<float>(il2cpp::find_class("Assets.Scripts.Utils", "TimeManager"), "maxSimulationStepsPerUpdate", 1.f);
		set_speed = false;

	}

	return reinterpret_cast<f_simulate>(hooks::hooks[0].original_function)(thisptr);

}

using f_tutorial_update = void(__cdecl*)(Assets_Scripts_Unity_UI_New_Tutorial_InGameTutorial_o*);
void __cdecl hk_tutorial_update(Assets_Scripts_Unity_UI_New_Tutorial_InGameTutorial_o* thisptr)
{

	if (ui::tutorial_skip)
	{

		static bool skipped = false;

		if (!skipped)
		{

			skipped = true;
			il2cpp::call_function<void>(il2cpp::get_method("Assets.Scripts.Unity.UI_New.Tutorial", "InGameTutorial", "Round10Over"), thisptr);
			
		}

		return;

	}

	return reinterpret_cast<f_tutorial_update>(hooks::hooks[1].original_function)(thisptr);

}

using f_tower_free_upgrade = bool(__cdecl*)(Assets_Scripts_Simulation_Towers_TowerManager_o*, UnityEngine_Vector3_o*, Assets_Scripts_Simulation_Towers_Tower_o*, int, int);
bool __cdecl hk_tower_free_upgrade(Assets_Scripts_Simulation_Towers_TowerManager_o* thisptr, UnityEngine_Vector3_o* position, Assets_Scripts_Simulation_Towers_Tower_o* tower, int path, int tier)
{

	if (ui::free_upgrades)
		return true;

	return reinterpret_cast<f_tower_free_upgrade>(hooks::hooks[2].original_function)(thisptr, position, tower, path, tier);

}

using f_ingame_update = void(__cdecl*)(Assets_Scripts_Unity_UI_New_InGame_InGame_o*);
void __cdecl hk_ingame_update(Assets_Scripts_Unity_UI_New_InGame_InGame_o* thisptr)
{

	for (auto script : scripting.get_current_scripts())
		for (auto callback : script.callbacks)
			if (callback.callback_name == "OnInGameUpdate")
				scripting.run_script_from_function(callback.function, thisptr);

	return reinterpret_cast<f_ingame_update>(hooks::hooks[3].original_function)(thisptr);

}

using f_has_upgrade = bool(__cdecl*)(Assets_Scripts_Unity_Player_Btd6Player_o*, System_String_o*);
bool __cdecl hk_has_upgrade(Assets_Scripts_Unity_Player_Btd6Player_o* thisptr, System_String_o* upgrade_id)
{

	if (ui::unlock_all_tower_upgrades)
		return true;

	return reinterpret_cast<f_has_upgrade>(hooks::hooks[4].original_function)(thisptr, upgrade_id);

}

using f_has_unlocked_tower = bool(__cdecl*)(Assets_Scripts_Unity_Player_Btd6Player_o*, System_String_o*, bool);
bool __cdecl hk_has_unlocked_tower(Assets_Scripts_Unity_Player_Btd6Player_o* thisptr, System_String_o* tower_id, bool ignore_debug)
{

	if (ui::unlock_all_towers)
		return true;

	return reinterpret_cast<f_has_unlocked_tower>(hooks::hooks[5].original_function)(thisptr, tower_id, ignore_debug);

}

using f_has_unlocked_hero = bool(__cdecl*)(Assets_Scripts_Unity_Player_Btd6Player_o*, System_String_o*);
bool __cdecl hk_has_unlocked_hero(Assets_Scripts_Unity_Player_Btd6Player_o* thisptr, System_String_o* tower_skin_id)
{

	if (ui::unlock_all_heros)
		return true;

	return reinterpret_cast<f_has_unlocked_hero>(hooks::hooks[6].original_function)(thisptr, tower_skin_id);

}

using f_coop_update = void(__cdecl*)(Assets_Scripts_Unity_UI_New_Coop_CoopLobbyScreen_o*);
void __cdecl hk_coop_update(Assets_Scripts_Unity_UI_New_Coop_CoopLobbyScreen_o* thisptr)
{

	if (ui::single_player_coop) 
	{

		thisptr->fields.readyBtn->fields.m_Interactable = true;
		thisptr->fields.readyBtn->fields.m_EnableCalled = true;

	}

	return reinterpret_cast<f_coop_update>(hooks::hooks[7].original_function)(thisptr);

}

using f_has_knowledge = bool(__cdecl*)(Assets_Scripts_Unity_Player_Btd6Player_o*, System_String_o*);
bool __cdecl hk_has_knowledge(Assets_Scripts_Unity_Player_Btd6Player_o* thisptr, System_String_o* knowledge_id)
{

	if (ui::unlock_all_mk)
		return true;

	return reinterpret_cast<f_has_knowledge>(hooks::hooks[8].original_function)(thisptr, knowledge_id);

}

using f_can_get_map = bool(__cdecl*)(Assets_Scripts_Unity_Player_Btd6Player_o*, System_String_o*);
bool __cdecl hk_can_get_map(Assets_Scripts_Unity_Player_Btd6Player_o* thisptr, System_String_o* map)
{

	if (ui::unlock_all_maps)
		return true;

	return reinterpret_cast<f_can_get_map>(hooks::hooks[9].original_function)(thisptr, map);

}

void hooks::init()
{

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Simulation", "Simulation", "Simulate")->methodPointer, hk_simulate))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking simulate.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.UI_New.Tutorial", "InGameTutorial", "Update")->methodPointer, hk_tutorial_update))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking update", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Simulation.Towers", "TowerManager", "GetFreeUpgrade")->methodPointer, hk_tower_free_upgrade))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking getfreeupgrade.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.UI_New.InGame", "InGame", "Update")->methodPointer, hk_ingame_update))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking ingameupdate.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.Player", "Btd6Player", "HasUpgrade")->methodPointer, hk_has_upgrade))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking hasupgrade.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.Player", "Btd6Player", "HasUnlockedTower")->methodPointer, hk_has_unlocked_tower))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking hasunlockedtower.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.Player", "Btd6Player", "HasUnlockedHero")->methodPointer, hk_has_unlocked_hero))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking hasunlockedhero.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.UI_New.Coop", "CoopLobbyScreen", "Update")->methodPointer, hk_coop_update))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking coopupdate.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.Player", "Btd6Player", "HasKnowledge")->methodPointer, hk_has_knowledge))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking hasknowledge.", NULL);
		exit(0);
	}

	if (!hook_function(il2cpp::get_method("Assets.Scripts.Unity.Player", "Btd6Player", "IsMapUnlocked")->methodPointer, hk_can_get_map))
	{
		MessageBoxA(NULL, "btd6 addon - error", "Failed in hooking cangetmap.", NULL);
		exit(0);
	}

}
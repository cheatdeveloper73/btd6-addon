#pragma once

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui_internal.h"
#include "../includes.h"
#include "../imgui/editor.h"

#include "../scripting/scripting.h"
#include "../notifications/font_awesome_5.h"

#include <sstream>
#include <fstream>

namespace ui
{

	inline bool menu_open = true;
	inline ImVec2 screen_res{ 000, 000 };
	inline ImVec2 window_pos{ 0, 0 };
	inline ImVec2 window_size{ 630, 550 };
	inline DWORD  window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
	inline ImFont* code_font = nullptr;
	inline int current_tab = 0;
	inline bool finished_editing_script = false;
	inline bool should_render_editor = false;

	inline int current_mod_index = 0;

	inline bool instant_kill_bloons = false;
	inline bool infinite_cash = false;
	inline bool infinite_health = false;
	inline bool tutorial_skip = false;
	inline bool instant_win_game = false;
	inline float tower_size_scale = 1.f;
	inline float bloon_size_scale = 1.f;
	inline bool free_upgrades = false;

	inline bool faster_forward = false;
	inline float faster_forward_speed = 1.f;

	inline bool instant_ability_cooldown = false;

	inline bool unlock_all_heros = false;
	inline bool unlock_all_towers = false;
	inline bool unlock_all_mk = false;
	inline bool unlock_all_tower_upgrades = false;
	inline bool unlock_all_maps = false;
	inline bool single_player_coop = false;

	void render();

}
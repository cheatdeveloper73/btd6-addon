#include "ui.h"

const TextEditor::Palette& getcustomcolors()
{

	const static TextEditor::Palette p = { {
			ImGui::GetColorU32(ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f)),	// Default
			ImGui::GetColorU32(ImVec4(255 / 255.f, 250 / 255.f, 97 / 255.f, 255 / 255.f)),	// Keyword	
			ImGui::GetColorU32(ImVec4(159 / 255.f, 240 / 255.f, 113 / 255.f, 1.f)),	// Number
			ImGui::GetColorU32(ImVec4(219 / 255.f, 66 / 255.f, 112 / 255.f, 1.f)),	// String
			ImGui::GetColorU32(ImVec4(219 / 255.f, 66 / 255.f, 112 / 255.f, 1.f)), // Char literal
			ImGui::GetColorU32(ImVec4(167 / 255.f, 161 / 255.f, 173 / 255.f, 1.f)), // Punctuation
			ImGui::GetColorU32(ImVec4(0.41f, 0.39f, 0.80f, 1.f)),	// Preprocessor
			ImGui::GetColorU32(ImVec4(135 / 255.f, 214 / 255.f, 209 / 255.f, 1.f)), // Identifier
			ImGui::GetColorU32(ImVec4(0.71f, 0.39f, 0.39f, 1.f)), // Known identifier
			ImGui::GetColorU32(ImVec4(108 / 255.f, 26 / 255.f, 189 / 255.f, 1.f)), // Preproc identifier
			ImGui::GetColorU32(ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f)), // Comment (single line)
			ImGui::GetColorU32(ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f)), // Comment (multi line)
			ImGui::GetColorU32(ImVec4(0.02f, 0.02f, 0.02f, 0.53f)), // Background
			0xffe0e0e0, // Cursor
			0x80a06020, // Selection
			0x800020ff, // ErrorMarker
			0x40f08000, // Breakpoint
			ImGui::GetColorU32(ImVec4(219 / 255.f, 219 / 255.f, 213 / 255.f, 1.f)), // Line number
			0x40000000, // Current line fill
			0x40808080, // Current line fill (inactive)
			0x40a0a0a0, // Current line edge
		} };
	return p;
}

void render_editor(std::string file_path, std::string window_name)
{

	if (ui::finished_editing_script)
		return;

	std::stringstream ss;

	ss << window_name;

	ImGui::SetNextWindowSize(ImVec2(750, 730), ImGuiCond_Once);
	ImGui::Begin(ss.str().c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 3.5f);

	static TextEditor editor;
	static bool setup = false;
	static std::string old_path;

	if (file_path != old_path)
		setup = false;

	if (!setup)
	{

		static auto editor_language = TextEditor::LanguageDefinition::CPlusPlus();

		editor.SetLanguageDefinition(editor_language);
		editor.SetReadOnly(false);

		std::ifstream t(file_path);
		if (t.good()) // does the file exist?
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			editor.SetText(str); // setup script content
		}

		setup = !setup;

	}

	ImGui::PushFont(ui::code_font);

	editor.SetPalette(getcustomcolors());
	editor.Render(ss.str().c_str(), ImGui::GetWindowSize() - ImVec2(0, 66));

	ImGui::PopFont();

	ImGui::Spacing();

	if (ImGui::Button("Save File"))
	{

		std::ofstream out;
		out.open(file_path);
		out << editor.GetText() << std::endl;
		out.close();
		scripting.reload_scripts();

	}

	ImGui::SameLine();

	if (ImGui::Button("Close Editor"))
	{
		ui::finished_editing_script = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Toggle Syntax Highlighting"))
		editor.SetColorizerEnable(!editor.IsColorizerEnabled());

	ImGui::SameLine();

	ImGui::Text("Currently Editing %s", file_path.c_str());

	ImGui::PopStyleVar();
	ImGui::End();

	old_path = file_path;

}

void text_centered(const char* text) {

	auto window_size = ImGui::GetWindowSize();
	auto text_size = ImGui::CalcTextSize(text);

	float y = ImGui::GetCursorPosY();
	ImGui::SetCursorPosX((window_size.x - text_size.x) * 0.5f);
	ImGui::SetCursorPosY((window_size.y - text_size.y) * 0.5f);
	ImGui::Text(text);

}

bool script_chip(AstroScript script)
{

	auto sz = ImGui::GetWindowSize();

	ImVec2 chip_size(sz.x - 20, 80);

	bool ret = false;

	ImGui::BeginChild(script.name.c_str(), chip_size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	if (script.state != ASTRO_SCRIPT_STATE::STATE_RUNNING)
		ImGui::Text(script.name.c_str());
	else
		ImGui::Text(std::string(script.name + " - Running").c_str());
	ImGui::SameLine((sz.x - 20) - 29);

	if (ImGui::Button(ICON_FA_COG))
	{
		ui::current_mod_index = script.id;
		ImGui::OpenPopup("ButtonSettings");
	}

	if (ImGui::BeginPopup("ButtonSettings"))
	{

		if (ImGui::Button("Load Mod"))
			scripting.run_script_from_id(script.id);
		if (ImGui::Button("Unload Mod"))
			scripting.unload_script_from_id(script.id);
		if (ImGui::Button("Delete Mod"))
		{
			scripting.unload_script_from_id(script.id);
			remove(script.path.c_str());
			scripting.reload_scripts();
		}
		if (ImGui::Button("Edit"))
		{
			ui::should_render_editor = true;
			ui::finished_editing_script = false;
		}

		ImGui::EndPopup();

	}

	ImGui::Separator();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(139 / 255.f, 162 / 255.f, 183 / 255.f, 255 / 255.f));
	ImGui::Text(script.description.c_str());
	ImGui::PopStyleColor();

	ImGui::EndChild();

	return ret;

}

#include <string>

bool tab(const char* name, ImVec2 size, bool pressed)
{

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(143 / 255.f, 163 / 255.f, 181 / 255.f, 255 / 255.f));

	if (pressed)
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	else
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(39 / 255.f, 58 / 255.f, 75 / 255.f, 255 / 255.f));

	ImGui::BeginChild(name, size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	bool ret = ImGui::InvisibleButton(name, size);

	text_centered(name);

	ImGui::EndChild();

	ImGui::PopStyleColor(2);

	return ret;

}

struct col
{
	float r, g, b, a;
};

void ui::render()
{

	if (should_render_editor && !finished_editing_script)
	{
		auto scripts = scripting.get_current_scripts();
		render_editor(scripts[current_mod_index].path, scripts[current_mod_index].name);
	}

	ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y), ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints(ImVec2(630, 550), ImVec2(9999, 9999));
	ImGui::SetNextWindowBgAlpha(1.0f);

	auto sz = ImGui::GetWindowSize();

	ImGui::Begin("##btd6addon", &menu_open, window_flags);
	{

		auto draw = ImGui::GetWindowDrawList();
		auto pos = ImGui::GetWindowPos();

		ImGui::BeginChild("tabs", ImVec2(sz.x - 10, 60), true);

		if (tab("Features", ImVec2((620 / 2) - 5, 50), current_tab == 0)) current_tab = 0;
		ImGui::SameLine();
		if (tab("Mod List", ImVec2((620 / 2) - 10, 50), current_tab == 1)) current_tab = 1;

		ImGui::EndChild();

		ImGui::Separator();

		switch (current_tab)
		{

			case 0:
			{

				ImGui::BeginChild("content", ImVec2((620 / 2), 470), true);

				ImGui::Checkbox("Instant Kill Bloons", &instant_kill_bloons);
				ImGui::Checkbox("Infinite Cash", &infinite_cash);
				ImGui::Checkbox("Infinite Health", &infinite_health);
				ImGui::Checkbox("Tutorial Skip", &tutorial_skip);
				ImGui::Checkbox("Instant Win Game", &instant_win_game);
				ImGui::Checkbox("Free Upgrades", &free_upgrades);

				ImGui::SliderFloat("Tower Size Scale", &tower_size_scale, 0.f, 10.f);
				ImGui::SliderFloat("Bloon Size Scale", &bloon_size_scale, 0.f, 10.f);

				ImGui::Checkbox("Unlock All Towers", &unlock_all_towers);
				ImGui::Checkbox("Unlock All Knowledge", &unlock_all_mk);
				ImGui::Checkbox("Unlock All Maps", &unlock_all_maps);
				ImGui::Checkbox("Unlock All Heros", &unlock_all_heros);

				ImGui::Checkbox("Instant Ability Cooldowns", &instant_ability_cooldown);

				ImGui::Checkbox("Unlock All Tower Upgrades", &unlock_all_tower_upgrades);
				ImGui::Checkbox("Player Co-Op With 1 Player", &single_player_coop);

				ImGui::EndChild();

				ImGui::SameLine();

				ImGui::BeginChild("content2", ImVec2((620 / 2) - 5, 470), true);

				ImGui::Checkbox("Super Fast-Forward", &faster_forward);
				ImGui::SliderFloat("Super Fast-Forward Speed", &faster_forward_speed, 0.1f, 100.f);

				ImGui::EndChild();

				break;

			}

			case 1:
			{

				ImGui::BeginChild("content", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);

				ImVec2 child_size(sz.x - 20, 40);

				ImGui::BeginChild("modoptions", child_size, true);

				if (ImGui::Button("Reload Mods"))
					scripting.reload_scripts();
				static bool reeq = false;
				ImGui::SameLine();
				ImGui::Checkbox("Enable Risky Functions", &reeq);

				ImGui::EndChild();

				for (auto script : scripting.get_current_scripts())
				{

					script_chip(script);

				}

				ImGui::EndChild();

				break;

			}

		}

	}
	ImGui::End();

}

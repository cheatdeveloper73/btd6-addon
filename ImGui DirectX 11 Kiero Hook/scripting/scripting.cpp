//#pragma comment(lib, "angelscript64.lib")


#include "scripting.h"
#include <string>

#include "../notifications/imgui_notify.h"
#include "../ui/ui.h"

#include "interface.h"

inline void quicklog(const std::string& text)
{

	ImGui::InsertNotification({ ImGuiToastType_Success, 10000, text.c_str()});

}

 void MessageCallback(const asSMessageInfo* msg, void* param)
{

	const char* type = "ERROR";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARNING";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFORMATION";

	//printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);

	std::stringstream ss;

	ss << "[" << type << "] " << msg->section << " | row / " << msg->row << " | column / " << msg->col << " | message: " << msg->message;
	quicklog(ss.str());

}

 namespace SCRIPTING_FUNCTIONS
 {

	 void sMessageBox(std::string text) { MessageBoxA(NULL, text.c_str(), "addon", 0); }
	 void sLog(std::string text) { quicklog("[SCRIPT] " + text); }
	 void sRegisterCallback(std::string callback, std::string function_name) { scripting.register_callback_to_function(ui::current_mod_index, callback, function_name); };

 }

#include "../imgui/imgui_internal.h"

 bool Astro::Initialize()
 {

	 scriptengine = asCreateScriptEngine();

	 if (!scriptengine)
		 return false; 

	 if (scriptengine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL) < 0)
		 return false;

	 RegisterStdString(scriptengine);

	 return true;

}

#define CHECK(f) if (f < 0) return false;



 bool Astro::AddFunctions()
 {

	 {
		 CHECK(scriptengine->RegisterEnum("ImGuiWindowFlags"));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_None", ImGuiWindowFlags_None));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoTitleBar", ImGuiWindowFlags_NoTitleBar));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoMove", ImGuiWindowFlags_NoMove));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoScrollbar", ImGuiWindowFlags_NoScrollbar));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoCollapse", ImGuiWindowFlags_NoCollapse));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoBackground", ImGuiWindowFlags_NoBackground));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoSavedSettings", ImGuiWindowFlags_NoSavedSettings));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoMouseInputs", ImGuiWindowFlags_NoMouseInputs));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_MenuBar", ImGuiWindowFlags_MenuBar));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_AlwaysVerticalScrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_AlwaysUseWindowPadding", ImGuiWindowFlags_AlwaysUseWindowPadding));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoNavInputs", ImGuiWindowFlags_NoNavInputs));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoNavFocus", ImGuiWindowFlags_NoNavFocus));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_UnsavedDocument", ImGuiWindowFlags_UnsavedDocument));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoNav", ImGuiWindowFlags_NoNav));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoDecoration", ImGuiWindowFlags_NoDecoration));
		 CHECK(scriptengine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoInputs", ImGuiWindowFlags_NoInputs));
	 }

	 CHECK(scriptengine->RegisterGlobalFunction("void MessageBox(string text)", asFUNCTION(SCRIPTING_FUNCTIONS::sMessageBox), asCALL_CDECL));
	 CHECK(scriptengine->RegisterGlobalFunction("void Log(string text)", asFUNCTION(SCRIPTING_FUNCTIONS::sLog), asCALL_CDECL));
	 CHECK(scriptengine->RegisterGlobalFunction("void RegisterCallback(string callback, string function_name)", asFUNCTION(SCRIPTING_FUNCTIONS::sRegisterCallback), asCALL_CDECL));

	 {
		 CHECK(scriptengine->RegisterObjectType("ImVec2", sizeof(ImVec2), asOBJ_VALUE | asOBJ_POD));
		 CHECK(scriptengine->RegisterObjectProperty("ImVec2", "float x", asOFFSET(ImVec2, ImVec2::x)));
		 CHECK(scriptengine->RegisterObjectProperty("ImVec2", "float y", asOFFSET(ImVec2, ImVec2::y)));
	 }

	 CHECK(scriptengine->RegisterObjectType("IL2CPPClass", sizeof(il2cpp::il2cppClass), asOBJ_VALUE | asOBJ_POD));
	 CHECK(scriptengine->RegisterObjectType("IL2CPPMethodInfo", sizeof(il2cpp::il2cppMethodInfo), asOBJ_VALUE | asOBJ_POD));
	 CHECK(scriptengine->RegisterObjectType("System_String_o", sizeof(System_String_o), asOBJ_VALUE | asOBJ_POD));

	 {
		 CHECK(scriptengine->RegisterObjectType("Assets_Scripts_Simulation_Simulation_Fields", sizeof(Assets_Scripts_Simulation_Simulation_Fields), asOBJ_VALUE | asOBJ_POD));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "uint localObjectId", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::localObjectId)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool canEarnXP", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::canEarnXP)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool debugOptions", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::debugOptions)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool instantCooldowns", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::instantCooldowns)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool pauseWeapons", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::pauseWeapons)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "float endOnRoundAdditionalTime", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::endOnRoundAdditionalTime)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool hasCashChanged", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::hasCashChanged)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool hasHealthChanged", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::hasHealthChanged)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool alternateHeliControls", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::alternateHeliControls)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "int showCancel", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::showCancel)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool gameStarted", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::gameStarted)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool _gameWon_k__BackingField", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::_gameWon_k__BackingField)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool matchJustWon", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::matchJustWon)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool gameLost", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::gameLost)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool noLivesLost", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::noLivesLost)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool autoPlay", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::autoPlay)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool blockAutoPlay", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::blockAutoPlay)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "int inputInterest", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::inputInterest)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool hasTakenLeakDamage", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::hasTakenLeakDamage)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool _WasDoubleCashUsed_k__BackingField", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::_WasDoubleCashUsed_k__BackingField)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "bool sandbox", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::sandbox)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "float lastSetHealth", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::lastSetHealth)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "int sandBoxResetHealthTimer", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::sandBoxResetHealthTimer)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "int raceSendRoundCount", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::raceSendRoundCount)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "int raceSendRoundTimer", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::raceSendRoundTimer)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "int roundStartTime", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::roundStartTime)));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "float cachedSimBehaviorCashModifier", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::cachedSimBehaviorCashModifier))); CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_Fields", "int cachedTierCount", asOFFSET(Assets_Scripts_Simulation_Simulation_Fields, Assets_Scripts_Simulation_Simulation_Fields::cachedTierCount)));
		 CHECK(scriptengine->RegisterObjectType("Assets_Scripts_Simulation_Simulation_o", sizeof(Assets_Scripts_Simulation_Simulation_o), asOBJ_VALUE | asOBJ_POD));
		 CHECK(scriptengine->RegisterObjectProperty("Assets_Scripts_Simulation_Simulation_o", "Assets_Scripts_Simulation_Simulation_Fields fields", asOFFSET(Assets_Scripts_Simulation_Simulation_o, Assets_Scripts_Simulation_Simulation_o::fields)));
	 }

	 {
		 CHECK(scriptengine->RegisterObjectType("BoolReference", sizeof(BoolReference), asOBJ_REF));
		 CHECK(scriptengine->RegisterObjectBehaviour("BoolReference", asBEHAVE_FACTORY, "BoolReference@ f(bool value)", asFUNCTION(BoolReferenceFactory), asCALL_CDECL));
		 CHECK(scriptengine->RegisterObjectBehaviour("BoolReference", asBEHAVE_ADDREF, "void f()", asMETHOD(BoolReference, BoolReference::AddReference), asCALL_THISCALL));
		 CHECK(scriptengine->RegisterObjectBehaviour("BoolReference", asBEHAVE_RELEASE, "void f()", asMETHOD(BoolReference, BoolReference::Release), asCALL_THISCALL));
		 CHECK(scriptengine->RegisterObjectProperty("BoolReference", "bool Value", asOFFSET(BoolReference, BoolReference::value)));

		 CHECK(scriptengine->RegisterObjectType("IntReference", sizeof(IntReference), asOBJ_REF));
		 CHECK(scriptengine->RegisterObjectBehaviour("IntReference", asBEHAVE_FACTORY, "IntReference@ f(int value)", asFUNCTION(IntReferenceFactory), asCALL_CDECL));
		 CHECK(scriptengine->RegisterObjectBehaviour("IntReference", asBEHAVE_ADDREF, "void f()", asMETHOD(IntReference, IntReference::AddReference), asCALL_THISCALL));
		 CHECK(scriptengine->RegisterObjectBehaviour("IntReference", asBEHAVE_RELEASE, "void f()", asMETHOD(IntReference, IntReference::Release), asCALL_THISCALL));
		 CHECK(scriptengine->RegisterObjectProperty("IntReference", "int Value", asOFFSET(IntReference, IntReference::value)));

		 CHECK(scriptengine->RegisterObjectType("FloatReference", sizeof(FloatReference), asOBJ_REF));
		 CHECK(scriptengine->RegisterObjectBehaviour("FloatReference", asBEHAVE_FACTORY, "FloatReference@ f(float value)", asFUNCTION(FloatReferenceFactory), asCALL_CDECL));
		 CHECK(scriptengine->RegisterObjectBehaviour("FloatReference", asBEHAVE_ADDREF, "void f()", asMETHOD(FloatReference, FloatReference::AddReference), asCALL_THISCALL));
		 CHECK(scriptengine->RegisterObjectBehaviour("FloatReference", asBEHAVE_RELEASE, "void f()", asMETHOD(FloatReference, FloatReference::Release), asCALL_THISCALL));
		 CHECK(scriptengine->RegisterObjectProperty("FloatReference", "float Value", asOFFSET(FloatReference, FloatReference::value)));
	 }

	 {
		 CHECK(scriptengine->SetDefaultNamespace("IL2CPP"));

		 CHECK(scriptengine->RegisterGlobalFunction("IL2CPPClass& FindClass(string namespxce, string klass)", asFUNCTION(il2cpp_interface::FindClass), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("string ToString(System_String_o&in str)", asFUNCTION(il2cpp_interface::SystemStringToString), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("void GetStaticFieldValue(IL2CPPClass&in klass, string field, ?&out value)", asFUNCTION(il2cpp_interface::GetStaticFieldValue), asCALL_CDECL));
	 }

	 {
		 CHECK(scriptengine->SetDefaultNamespace("Menu"));

		 CHECK(scriptengine->RegisterGlobalProperty("bool MenuOpen", &ui::menu_open));

		 CHECK(scriptengine->RegisterGlobalFunction("void Begin(string name, int window_flags)", asFUNCTION(imgui_interface::Begin), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("void End()", asFUNCTION(imgui_interface::End), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("bool Button(string name)", asFUNCTIONPR(imgui_interface::Button, (std::string), bool), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("bool Button(string name, ImVec2 size)", asFUNCTIONPR(imgui_interface::Button, (std::string, ImVec2), bool), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("void Label(string text)", asFUNCTION(imgui_interface::Label), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("bool Checkbox(string label, BoolReference&inout value)", asFUNCTION(imgui_interface::Checkbox), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("bool Combo(string label, string items, IntReference&inout value)", asFUNCTION(imgui_interface::Combo), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("bool SliderInt(string label, IntReference&inout value, int min_value, int max_value)", asFUNCTION(imgui_interface::SliderInt), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("bool SliderFloat(string label, FloatReference&inout value, float min_value, float max_value)", asFUNCTION(imgui_interface::SliderFloat), asCALL_CDECL));

		 CHECK(scriptengine->RegisterGlobalProperty("bool InstantKillBloons", &ui::instant_kill_bloons));
		 CHECK(scriptengine->RegisterGlobalProperty("bool InfiniteCash", &ui::infinite_cash));
		 CHECK(scriptengine->RegisterGlobalProperty("bool InfiniteHealth", &ui::infinite_health));
		 CHECK(scriptengine->RegisterGlobalProperty("bool TutorialSkip", &ui::tutorial_skip));
		 CHECK(scriptengine->RegisterGlobalProperty("bool InstantWinGame", &ui::instant_win_game));
		 CHECK(scriptengine->RegisterGlobalProperty("float TowerSizeScale", &ui::tower_size_scale));
		 CHECK(scriptengine->RegisterGlobalProperty("float BloonSizeScale", &ui::bloon_size_scale));
		 CHECK(scriptengine->RegisterGlobalProperty("bool FreeUpgrades", &ui::free_upgrades));
		 CHECK(scriptengine->RegisterGlobalProperty("bool FasterForward", &ui::faster_forward));
		 CHECK(scriptengine->RegisterGlobalProperty("float FasterForwardSpeed", &ui::faster_forward_speed));
		 CHECK(scriptengine->RegisterGlobalProperty("bool InstantAbilityCooldowns", &ui::instant_ability_cooldown));

	 }

	 {
		 CHECK(scriptengine->SetDefaultNamespace("Input"));
		 CHECK(scriptengine->RegisterGlobalFunction("void LeftClickDown()", asFUNCTION(winapi_interface::SendLeftClickDown), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("void LeftClickUp()", asFUNCTION(winapi_interface::SendLeftClickUp), asCALL_CDECL));
		 CHECK(scriptengine->RegisterGlobalFunction("int GetKeyState(int key)", asFUNCTION(GetAsyncKeyState), asCALL_CDECL));
	 }



	 return true;

 }

 void CScripting::init_scripting()
 {

	 if (!astro.Initialize())
	 {
		 quicklog("Failed to initialize script engine.");
		 return;
	 }

	 if (!astro.AddFunctions())
	 {

		 quicklog("Failed to add script engine functions.");
		 return;

	 }

	 quicklog("Initialized script engine!");

	 astro.LoadScriptsFromPath();

 }

 void CScripting::run_script_from_function(asIScriptFunction* func, void* args)
 {
	 return astro.RunScriptFromFunction(func, args);
 }

 std::vector<AstroScript> CScripting::get_current_scripts()
 {

	 return astro.current_scripts;

 }

 void CScripting::run_script_from_id(int id)
 {

	 AstroScript script;

	 for (auto currentscript : astro.current_scripts)
		 if (currentscript.id == id)
			 script = currentscript;

	 astro.ExecuteScriptFromPath(script.path);

 }

 void CScripting::reload_scripts()
 {

	 astro.LoadScriptsFromPath();

 }

 bool CScripting::register_callback_to_function(int id, std::string callback_name, std::string function_name)
 {

	 astro.AddCallbackToScript(id, callback_name, function_name);

	 bool found = false;

	 for (auto callback : astro.current_scripts[id].callbacks)
		 if (callback.callback_name == callback_name)
			 found = true;

	 if (!found)
	 {

		 quicklog("Failed to add script callback :(");

	 }

	 return found;

 }

 AstroScript CScripting::get_script_from_id(int id)
 {

	 return astro.current_scripts[id];

 }

 void CScripting::unload_script_from_id(int id)
 {
	 astro.UnloadScriptFromID(id);
 }

 void Astro::LoadScriptsFromPath()
 {

	 auto oldScripts = current_scripts;

	 for (auto script : current_scripts)
		 UnloadScriptFromID(script.id);

	 current_scripts.clear();

	 std::string folder;
	 static TCHAR path[MAX_PATH];

	 if (true)
	 {

		 std::wstring tmp = (std::wstring(L"C:\\addon\\mods"));

		 folder = std::string(tmp.begin(), tmp.end());

		 auto i = 0;

		 for (auto& entry : std::filesystem::directory_iterator(folder))
		 {
			 if (entry.path().extension() == ".as")
			 {
				 auto path = entry.path();
				 auto filename = path.filename().string();

				 AstroScript script = AstroScript{filename, path.string(), "None", i, STATE_NONE};

				 for (auto old_script : oldScripts)
					 if (old_script.id == script.id && old_script.path == script.path)
						 continue;

				 current_scripts.emplace_back(script);

				 ++i;

			 }
		 }
	 }

 }

 void Astro::ExecuteScriptFromPath(std::string path)
 {

	 FILE* file;

	 fopen_s(&file, path.c_str(), "rb");
	 size_t filesize;

	 if (!file)
	 {
		 quicklog("Error loading file! " + path);
		 return;
	 }

	 AstroScript target_script;

	 for (auto script : current_scripts)
	 {
		 if (script.path == path)
			 target_script = script;

	 }

	 if (target_script.state == STATE_RUNNING)
	 {

		 quicklog("Script is already running! " + target_script.name);
		 return;

	 }

	 fseek(file, 0, SEEK_END);
	 filesize = ftell(file);
	 fseek(file, 0, SEEK_SET);

	 std::string scriptcontent;

	 scriptcontent.resize(filesize);
	 size_t c = fread(&scriptcontent[0], filesize, 1, file);

	 fclose(file);

	 if (!c)
	 {

		 quicklog("Failed to load script file! " + target_script.name);
		 return;

	 }

	 asIScriptModule* scriptmodule = scriptengine->GetModule(path.c_str(), asGM_ALWAYS_CREATE);

	 if (!scriptmodule)
	 {

		 quicklog("Failed to create script module! " + target_script.name);
		 return;

	 }

	 int result = scriptmodule->AddScriptSection(target_script.name.c_str(), &scriptcontent[0], filesize);

	 if (result < 0)
	 {

		 quicklog("Failed to create script section! " + target_script.name);
		 return;
		 
	 }
	
	 result = scriptmodule->Build();

	 if (result < 0)
	 {

		 quicklog("Failed to build script! " + target_script.name);
		 return;

	 }

	 asIScriptFunction* main_function = scriptmodule->GetFunctionByDecl("void Init()");
	 asIScriptContext* script_context = scriptengine->CreateContext();

	 if (!main_function)
	 {

		 quicklog("Failed to find Init function, is it defined like \"void Init()\"?");
		 script_context->Release();
		 return;

	 }

	 if (!script_context)
	 {

		 quicklog("Failed to create script context!");
		 return;

	 }

	 result = script_context->Prepare(main_function);

	 if (result < 0)
	 {

		 quicklog("Failed to prepare script context! " + target_script.name);
		 return;

	 }

	 result = script_context->Execute();

	 if (result < 0)
	 {

		 quicklog("Failed to call Init! " + target_script.name);
		 script_context->Release();
		 return;

	 }

	 quicklog("Loaded script! " + target_script.name);
	 current_scripts.at(target_script.id).state = STATE_RUNNING;

 }

 void Astro::UnloadScriptFromID(int id)
 {

	 //current_scripts.erase(current_scripts.begin() + id);
	 //LoadScriptsFromPath();

	 if (id > current_scripts.size())
	 {
		 quicklog("Invalid thing lol");
		 return;
	 }

	 AstroScript* script = &current_scripts[id];

	 if (!script)
		 return;

	 if (script->state != STATE_RUNNING)
		 return;

	 script->callbacks.clear();
	 script->state = STATE_NONE;

	 quicklog("Successfully unloaded script!");

 }

 void Astro::AddCallbackToScript(int id, std::string callback_name, std::string function_name)
 {

	 AstroScript script = current_scripts[id];

	 if (!(callback_name == "OnRender" || callback_name == "OnSimulate"))
	 {

		 quicklog("Invalid callback name! " + script.name);
		 return;

	 }

	 asIScriptModule* module = scriptengine->GetModule(script.path.c_str());

	 if (!module)
	 {

		 quicklog("Failed to get script module! " + script.name);
		 return;

	 }

	 for (auto _script : current_scripts)
		 for (auto callback : _script.callbacks)
			 if (script.id == script.id)
				 if (callback.callback_name == callback_name)
				 {
					 quicklog("Callback for " + callback_name + " already registered! " + script.name);
					 return;
				 }

	 asIScriptFunction* func = module->GetFunctionByName(function_name.c_str());

	 if (callback_name == "OnRender")
	 {

		 if (!func)
		 {

			 quicklog("Failed to find " + function_name + " is it defined? " + script.name);
			 return;

		 }

		 current_scripts[id].callbacks.emplace_back(AstroScriptCallback{ func, "OnRender" });

	 }
	 else if (callback_name == "OnSimulate")
	 {

		 if (!func)
		 {

			 quicklog("Failed to find " + function_name + " is it defined? " + script.name);
			 return;

		 }

		 current_scripts[id].callbacks.emplace_back(AstroScriptCallback{ func, "OnSimulate" });

	 }
	 else
	 {

		 quicklog("Unknown error when adding callback! " + script.name);
		 return;

	 }

	 quicklog("Added script callback! " + script.name);

 }

 void Astro::RunScriptFromFunction(asIScriptFunction* func, void* args)
 {

	asIScriptContext* context = scriptengine->CreateContext();
	int ret = context->Prepare(func);
	if (args != nullptr)
		ret = context->SetArgAddress(0, args);
	context->Execute();

 }
#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <Windows.h>
#include <filesystem>

#include "../angelscript/angelscript.h"

#include "../angelscript/scriptbuilder/scriptbuilder.h"
#include "../angelscript/scriptstdstring/scriptstdstring.h"

enum ASTRO_SCRIPT_STATE
{
	STATE_NONE ,
	STATE_RUNNING
};

struct AstroScriptCallback
{
	asIScriptFunction* function;
	std::string callback_name;
};

struct AstroScript 
{
	
	std::string name = "None";
	std::string path = "None";
	std::string description = "None";
	int id;
	ASTRO_SCRIPT_STATE state;
	std::vector<AstroScriptCallback> callbacks;
	bool risky_features_enabled = false;

	inline bool operator==(AstroScript b)
	{

		return (this->name == b.name) && (this->path == b.path);

	}

};

class Astro
{
	
public:
	std::vector<AstroScript> current_scripts;
	bool Initialize();
	bool AddFunctions();
	void LoadScriptsFromPath();
	void ExecuteScriptFromPath(std::string path);
	void UnloadScriptFromID(int id);
	void AddCallbackToScript(int id, std::string callback_name, std::string function_name);
	void RunScriptFromFunction(asIScriptFunction* func, void* args);
private:
	asIScriptEngine* scriptengine = nullptr;

};

class CScripting
{

private:
	Astro astro;
public:
	bool run_script_from_path(std::string path);
	void init_scripting();
	void run_script_from_id(int id);
	void reload_scripts();
	std::vector<AstroScript> get_current_scripts();
	AstroScript get_script_from_id(int id);
	bool register_callback_to_function(int id, std::string callback_name, std::string function_name);
	void run_script_from_function(asIScriptFunction* func, void* args);
	void unload_script_from_id(int id);

}; inline CScripting scripting;
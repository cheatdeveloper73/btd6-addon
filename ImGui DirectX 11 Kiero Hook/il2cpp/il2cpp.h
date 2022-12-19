#pragma once

#include <iostream>
#include <windows.h>
#include <string>

#include "../il2cppdump/il2cpp.h"
#include "structures.h"

namespace il2cpp
{

	inline uintptr_t gameassembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
	inline il2cppAssembly* assembly = nullptr;
	
	template <class t>
	t resolve_export(const char* name)
	{

		return reinterpret_cast<t>(GetProcAddress((HMODULE)gameassembly, name));

	}

	inline auto resolve_call = resolve_export<void*(__fastcall*)(const char*)>("il2cpp_resolve_icall");
	inline auto get_domain = resolve_export<void*(__fastcall*)(void)>("il2cpp_domain_get");
	inline auto thread_attach = resolve_export<void*(__fastcall*)(void*)>("il2cpp_thread_attach");
	inline auto get_assemblies = resolve_export<il2cppAssembly**(__fastcall*)(void*, size_t*)>("il2cpp_domain_get_assemblies");
	inline auto class_from_name = resolve_export<il2cppClass*(__fastcall*)(il2cppImage*, const char*, const char*)>("il2cpp_class_from_name");
	inline auto method_from_name = resolve_export<MethodInfo*(__fastcall*)(il2cppClass*, const char*, int)>("il2cpp_class_get_method_from_name");
	inline auto field_from_name = resolve_export<il2cppFieldInfo*(__fastcall*)(il2cppClass*, const char*)>("il2cpp_class_get_field_from_name");
	inline auto static_field_get_value = resolve_export<void(__fastcall*)(il2cppFieldInfo*, void*)>("il2cpp_field_static_get_value");
	inline auto static_field_set_value = resolve_export<void(__fastcall*)(il2cppFieldInfo*, void*)>("il2cpp_field_static_set_value");

	template <typename ret, typename... _args>
	inline ret call_function(MethodInfo* method, _args... args)
	{
		typedef ret(*func)(_args...);
		func fn = (func)((void*)method->methodPointer);
		return fn(args...);
	}

	inline il2cppClass* find_class(const char* namespxce, const char* class_name)
	{
		return class_from_name(assembly->m_pImage, namespxce, class_name);
	}

	inline MethodInfo* get_fn_ptr(il2cppClass* klass, const char* _method, int args_count = 0)
	{

		return method_from_name(klass, _method, args_count);

	}

	inline MethodInfo* get_method(const char* namespxce, const char* class_name, const char* method)
	{

		auto klass = find_class(namespxce, class_name);
		
		for (int i = 0; i < 16; i++)
		{

			auto info = method_from_name(klass, method, i);

			if (!info)
				continue;

			return info;

		}

	}

	inline std::string to_string(System_String_o* str)
	{

		if (!str)
			return "NULL";
		else
		{
			auto wstr = std::wstring((wchar_t*)(&str->fields._firstChar));
			return std::string(wstr.begin(), wstr.end());
		}

	}

	template <class type>
	inline type get_static_field_value(il2cppClass* klass, const char* field_name)
	{

		auto field = field_from_name(klass, field_name);

		type buffer;
		static_field_get_value(field, &buffer);
		return buffer;

	}

	template <class type>
	void set_static_field_value(il2cppClass* klass, const char* field_name, type val)
	{

		auto field = field_from_name(klass, field_name);

		static_field_set_value(field, &val);

	}


	bool sanity();

}
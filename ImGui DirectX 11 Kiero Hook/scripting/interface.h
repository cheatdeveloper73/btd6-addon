#pragma once

#include "../il2cpp/il2cpp.h"

#include "../angelscript/angelscript.h"

#include "../imgui/imgui.h"

class BoolReference
{
public:
	int ReferenceCount = 0;
	BoolReference(bool _default)
	{
		ReferenceCount = 1;
		value = _default;

	}
	void AddReference()
	{
		++ReferenceCount;
	}
	void Release()
	{
		if (--ReferenceCount == 0)
			delete this;
	}
	bool value = false;
};

BoolReference* BoolReferenceFactory(bool _default)
{

	return new BoolReference(_default);

}

class IntReference
{
public:
	int ReferenceCount = 0;
	IntReference(int _default)
	{
		ReferenceCount = 1;
		value = _default;

	}
	void AddReference()
	{
		++ReferenceCount;
	}
	void Release()
	{
		if (--ReferenceCount == 0)
			delete this;
	}
	int value = 0;
};

IntReference* IntReferenceFactory(int _default)
{

	return new IntReference(_default);

}

class FloatReference
{
public:
	int ReferenceCount = 0;
	FloatReference(float _default)
	{
		ReferenceCount = 1;
		value = _default;

	}
	void AddReference()
	{
		++ReferenceCount;
	}
	void Release()
	{
		if (--ReferenceCount == 0)
			delete this;
	}
	float value = 0.f;
};

FloatReference* FloatReferenceFactory(float _default)
{

	return new FloatReference(_default);

}

namespace il2cpp_interface
{

	template<typename ret, typename ... args>
	ret CallFunction(il2cpp::il2cppMethodInfo* method, args... arg)
	{

		return il2cpp::call_function<ret>(method, arg);

	}

	il2cpp::il2cppClass* FindClass(std::string namespxce, std::string klass)
	{

		return il2cpp::find_class(namespxce.c_str(), klass.c_str());

	}

	//il2cpp::il2cppMethodInfo* FindFunction(std::string namespxce, std::string klass, std::string function)
	//{

	//	return il2cpp::get_fn_ptr(klass.c_str(), namespxce.c_str(), function.c_str());

	//}

	std::string SystemStringToString(System_String_o* str)
	{

		return il2cpp::to_string(str);

	}

	void GetStaticFieldValue(il2cpp::il2cppClass* klass, std::string field, asIScriptGeneric* gen)
	{

		if (!gen)
			return;

		switch (gen->GetArgTypeId(2))
		{

			case 2:
			{
				gen->SetReturnByte(il2cpp::get_static_field_value<char>(klass, field.c_str()));
				break;
			}
			case 3:
			{
				gen->SetReturnByte(il2cpp::get_static_field_value<short>(klass, field.c_str()));
				break;
			}
			case 4:
			{
				gen->SetReturnDWord(il2cpp::get_static_field_value<int>(klass, field.c_str()));
				break;
			}
			case 5:
			{
				gen->SetReturnDWord(il2cpp::get_static_field_value<long long>(klass, field.c_str()));
				break;
			}
			case 6:
			{
				gen->SetReturnDWord(il2cpp::get_static_field_value<unsigned char>(klass, field.c_str()));
				break;
			}
			case 9:
			{
				gen->SetReturnQWord(il2cpp::get_static_field_value<unsigned long long>(klass, field.c_str()));
				break;
			}
			case 10:
			{
				gen->SetReturnFloat(il2cpp::get_static_field_value<float>(klass, field.c_str()));
				break;
			}
			case 11:
			{
				gen->SetReturnDouble(il2cpp::get_static_field_value<double>(klass, field.c_str()));
				break;
			}
			default:
			{
				gen->SetReturnAddress(il2cpp::get_static_field_value<void*>(klass, field.c_str()));
				break;
			}

		}

	}

}

namespace imgui_interface
{

	void Begin(std::string name, int window_flags)
	{

		ImGui::Begin(name.c_str(), NULL, window_flags);

	}

	void End()
	{

		ImGui::End();

	}

	bool Button(std::string name, ImVec2 size)
	{

		return ImGui::Button(name.c_str(), size);

	}

	bool Button(std::string name)
	{

		return ImGui::Button(name.c_str());

	}

	void Label(std::string text)
	{

		ImGui::Text(text.c_str());

	}

	bool Checkbox(std::string label, BoolReference* value)
	{

		return ImGui::Checkbox(label.c_str(), &value->value);

	}

	bool SliderInt(std::string label, IntReference* value, int min_value, int max_value)
	{

		return ImGui::SliderInt(label.c_str(), &value->value, min_value, max_value);

	}

	bool SliderFloat(std::string label, FloatReference* value, float min_value, float max_value)
	{

		return ImGui::SliderFloat(label.c_str(), &value->value, min_value, max_value);

	}

	bool Combo(std::string label, std::string items, IntReference* value)
	{

		return ImGui::Combo(label.c_str(), &value->value, items.c_str());

	}

}

namespace winapi_interface
{

	void SendLeftClickUp()
	{

		INPUT ipt[1] = {0};

		ipt[0].type = INPUT_MOUSE;
		ipt[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

		SendInput(1, ipt, sizeof(ipt));

	}

	void SendLeftClickDown()
	{

		INPUT ipt[1] = { 0 };

		ipt[0].type = INPUT_MOUSE;
		ipt[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		SendInput(1, ipt, sizeof(ipt));

	}

}
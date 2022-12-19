#include "il2cpp.h"

#define ASSERT(x) { MessageBoxA(NULL, NULL, x, NULL); exit(0); }

bool il2cpp::sanity()
{

	if (!gameassembly)
		ASSERT("Failed to get GameAssembly.dll.")
	if (!resolve_call)
		ASSERT("Failed to get resolve call.")
	if (!get_domain)
		ASSERT("Failed to get get domain.")
	if (!thread_attach)
		ASSERT("Failed to get thread attach")

	size_t assembly_count = 0;
	auto assemblies = get_assemblies(get_domain(), &assembly_count);

	for (int i = 0; i < assembly_count; i++)
	{

		if (std::string(assemblies[i]->m_aName.m_pName) == "Assembly-CSharp")
			assembly = assemblies[i];

	}

	if (!assembly)
		ASSERT("Failed to find assembly.")

	return true;

}
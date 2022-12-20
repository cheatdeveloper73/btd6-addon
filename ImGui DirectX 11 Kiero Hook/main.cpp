#include "includes.h"
#include "imgui/imgui_freetype.h"
#include "notifications/imgui_notify.h"
#include "ui/ui.h"
#include "hooks/hooks.h"
#include "inter.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (ui::menu_open)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			ImGuiIO& io = ImGui::GetIO();
			ImFontConfig cfg;
			cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;
			io.Fonts->AddFontFromMemoryTTF(rawData, sizeof(rawData), 16.f, &cfg);
			ImGui::MergeIconsWithLatestFont(15.f, false);
			ImFontConfig othercfg;
			othercfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome;
			othercfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting;
			ui::code_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 15.f, &othercfg);
			init = true;
			il2cpp::thread_attach(il2cpp::get_domain());
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1)
		ui::menu_open = !ui::menu_open;

	if (ui::menu_open)
		ui::render();

	for (auto script : scripting.get_current_scripts())
		for (auto callback : script.callbacks)
			if (callback.callback_name == "OnRender")
				scripting.run_script_from_function(callback.function, nullptr);

	ImGui::RenderNotifications();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	CreateDirectoryA("C:\\addon\\", NULL);
	CreateDirectoryA("C:\\addon\\mods\\", NULL);
	bool init_hook = false;
	do
	{

		auto status = kiero::init(kiero::RenderType::D3D11);

		if (status == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);

			il2cpp::sanity();
			hooks::init();
			scripting.init_scripting();
			scripting.reload_scripts();

			// woahhh

			init_hook = true;

		}
		else
		{

			MessageBoxA(NULL, "btd6 addon - error", std::to_string(status).c_str(), NULL);
			exit(0);

		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
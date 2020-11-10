#pragma warning(disable : 4530)

#define Use_VMProtect 0
#define AllocCons 0

#include <Windows.h>
#include "sdk/VMProtectSDK.h"
#pragma comment(lib, "VMProtectSDK64.lib")
#include <psapi.h>
#include <d3d11.h>
#include <emmintrin.h>
#include "winternl.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <time.h>
#include <chrono>
#include <sstream>


int isSteam = 0;

#include "sdk/NewUtils.h"
#include "sdk/utils.h"
#include "sdk/safe_mem.h"
#include "sdk_new/math.h"
#include "Config/Config.h"
#include "sdk/game_sdk.h"
#include "NewGui.h"
#include "cheat_funcs/EspFuncs.h"
#include "cheat_funcs/AimFuncs.h"
#include "cheat_funcs/MiscFuncs.h"
#include "cheat_funcs/EntityFuncs.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#pragma comment(lib, "Silent.lib")
namespace DischanSDK
{
	namespace Silent
	{
		void Enable();
		void Disable();
		void SetTarget(DWORD64 pEntity);
	}
}

using namespace GUI;

//#include "FLIC.h"

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static ID3D11DeviceContext* g_pd3dDeviceImmediateContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;


static void ShowExampleMenuFile();

void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
ImVec4 tempColor;
void CreateRenderTarget()
{
	DXGI_SWAP_CHAIN_DESC sd;
	g_pSwapChain->GetDesc(&sd);
	ID3D11Texture2D* pBackBuffer;
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = sd.BufferDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	pBackBuffer->Release();

}
DWORD WINAPI DetachDll(PVOID base)
{
	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 0);
}

#define AllocCons 0

typedef HRESULT(__fastcall* DX11PresentFn) (IDXGISwapChain* pSwapChain, UINT syncintreval, UINT flags);
DX11PresentFn oPresent = nullptr;

typedef HRESULT(__stdcall* DX11ResizeFn)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
DX11ResizeFn oResize = nullptr;

HRESULT __stdcall Resize(IDXGISwapChain* Swap, UINT a2, UINT a3, UINT a4, DXGI_FORMAT a5, UINT a6) {
	GUI::Render.ResetCanvas(); return oResize(Swap, a2, a3, a4, a5, a6);
}

HWND hWnd = NULL;
ID3D11Device* pD11Device = nullptr;
IDXGISwapChain* dxSwapChain = nullptr;
ID3D11DeviceContext* pD11DeviceContext = nullptr;
ID3D11RenderTargetView* pD11RenderTargetView = nullptr;

void ApplyImGuiIO(ImGuiIO& io)
{
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	static ImWchar ranges[] = { 0x0020, 0x00FF, 0x0100, 0x017f, 0 };
	char buffer[MAX_PATH];

	if (GetWindowsDirectoryA(buffer, MAX_PATH))
		io.Fonts->AddFontFromFileTTF(strcat(buffer, "/Fonts/Times.ttf"), 20.0f, nullptr, ranges);


	//if (GetWindowsDirectoryA(buffer, MAX_PATH))
	//{
	//	//io.Fonts->AddFontFromMemoryTTF(strcat(buffer, "/Fonts/Arial.ttf"), 12.0f, 12.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	//	//g_pRenderer->m_pFont = io.Fonts->AddFontFromMemoryTTF(strcat(buffer, "/Fonts/Arial.ttf"), 12.0f, 12.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	//	//g_pRenderer->m_pFont = io.Fonts->AddFontFromFileTTF(strcat(buffer, "/Fonts/Calibri.ttf"), 11.0f, nullptr, ranges);
	//	//g_pRenderer->m_pFont = io.Fonts->AddFontFromFileTTF(strcat(buffer, "/Fonts/Arial.ttf"), 11.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	//}
}
void ImGui::StyleColorsDark(ImGuiStyle* dst)
{
	ImGuiStyle* style = (dst ? dst : &ImGui::GetStyle());
	ImVec4* colors = style->Colors;

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 10.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 12.0f; // Make all elements (checkboxes, etc) circles
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 20.0f; // Make grab a circle
	style->GrabRounding = 12.0f;
	style->PopupRounding = 7.f;
	style->Alpha = 1.0;

	style->Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.95f, 0.95f, 0.30f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.67f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.000f, 0.777f, 0.578f, 0.780f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.987f, 0.611f, 0.600f);
	style->Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.77f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.84f, 0.97f, 0.01f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.31f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
	style->Colors[ImGuiCol_Tab] = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.96f, 0.73f, 0.09f, 0.90f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.97f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
	style->Colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
	style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
	style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

bool InitD3DX11(IDXGISwapChain* dxSwapChain)
{
	if (!pD11Device || !pD11DeviceContext)
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ApplyImGuiIO(ImGui::GetIO());

		if (SUCCEEDED(dxSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pD11Device)))
		{
			dxSwapChain->GetDevice(__uuidof(pD11Device), (void**)&pD11Device);
			pD11Device->GetImmediateContext(&pD11DeviceContext);
		}

		DXGI_SWAP_CHAIN_DESC desc;
		dxSwapChain->GetDesc(&desc);
		hWnd = desc.OutputWindow;
		GUI::NextWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)GUI::WndProc));

		ImGui_ImplWin32_Init(desc.OutputWindow);
		ImGui_ImplDX11_Init(pD11Device, pD11DeviceContext);

		return true;
	}

	return false;
}

class Misk
{
public:
	static Misk& Instance();
	void Functions();

private:
	Misk() {};
	~Misk() {};
};

class Menu
{
public:
	static Menu& Instance();
	void Functions();

private:
	Menu() {};
	~Menu() {};
};

#define MENU Menu::Instance()

Menu& Menu::Instance()
{
	static Menu instance{};
	return instance;
}


static int tabs = 1;
bool init = false;

bool menu_show = false;

bool label_1;
bool label_2;
bool label_3;
bool label_4;
bool label_5;
bool label_6;
bool label_7;
bool label_11;
int label_12 = 90;
float label_13[] = { 225.f, 0.0f, 0.0f, };
float label_14[] = { 0.0f, 255.0f, 0.0f, };
float label_15 = 1.0f;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void ChangeKey(void* blank)
{
	Vars::AimBot::keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				Vars::AimBot::aimkey = i;
				Vars::AimBot::keystatus = 0;
				return;
			}
		}
	}
}


void Menu::Functions()
{
	if (GUI::Visible && ImGui::Begin("Public Cheat"))
	{
			{
				ImGui::Text("DLL Currently running at %.3f ms & (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			using DischanSDK::Silent::Enable;
			using DischanSDK::Silent::Disable;

			Vars::AimBot::salobydischanwork_work ? Enable() : Disable();

			if (ImGui::CollapsingHeader("Aimbot Options"))
			{
				ImGui::Checkbox("Enable Aimbot (M2)", &Vars::AimBot::Activate);
				ImGui::Checkbox("Prediction", &Vars::AimBot::Prediction);
				ImGui::Checkbox("Ignore Team", &Vars::AimBot::IgnoreTeam);
				ImGui::Checkbox("Snap Lines", &Vars::PlayerEsp::SnapLines);
				ImGui::Checkbox("Crosshair", &Vars::AimBot::Crosshair);
				ImGui::Checkbox("Field of View", &Vars::AimBot::DrawFov);
				if (Vars::AimBot::DrawFov)
				{
					ImGui::SliderFloat("Set fov", &Vars::AimBot::Fov, 70.f, 300.f);
				}
				ImGui::Checkbox("Blackout FOV", &Vars::AimBot::FillFov);
				ImGui::Checkbox("Distance", &Vars::AimBot::Distance);
				if (Vars::AimBot::Distance)
				{
					ImGui::SliderFloat("Set Distance", &Vars::AimBot::Range, 0.f, 300.f);
				}
			}


			if (ImGui::CollapsingHeader("Weapon Options"))
			{
				ImGui::Checkbox("No Recoil", &Vars::Weapon::NoRecoil);
				ImGui::Checkbox("No Spread", &Vars::Weapon::AntiSpread);
				ImGui::Checkbox("No Sway", &Vars::Weapon::NoSway);
				ImGui::Checkbox("Aim at Body", &Vars::AimBot::BodyAim);
				ImGui::Checkbox("Visible Check", &Vars::AimBot::VisibleCheck);
				ImGui::Checkbox("Automatic", &Vars::Weapon::SetAutomatic);
				ImGui::Checkbox("Rapid Fire Bow", &Vars::Misc::SuperBow);
				ImGui::Checkbox("Instant Eoka", &Vars::Weapon::SuperEoka);
				ImGui::Checkbox("Super Melee", &Vars::Misc::LongHand);
			}

			if (ImGui::CollapsingHeader("ESP Options"))
			{
				ImGui::Checkbox("Player ESP", &Vars::PlayerEsp::PlayerESP);
				ImGui::Checkbox("Player Box", &Vars::PlayerEsp::ShowPlayerBox);
				ImGui::Checkbox("Player Bones", &Vars::PlayerEsp::SkeletonPlayer);
				ImGui::Checkbox("Player Health", &Vars::PlayerEsp::ShowPlayerHealth);
				ImGui::Checkbox("Player Weapon", &Vars::PlayerEsp::ShowPlayerWeapon);
				ImGui::Checkbox("Player Distance", &Vars::PlayerEsp::ShowPlayerDist);
				ImGui::Checkbox("Ignore NPC", &Vars::BotsEsp::IgnoreNpc);
			    ImGui::Checkbox("Ignore Sleepers", &Vars::PlayerEsp::IgnoreSleepers); 
			}

			if (ImGui::CollapsingHeader("Rage Options"))
			{
				ImGui::Checkbox("Fat Bullet", &Vars::Misc::FatBullet);
				//ImGui::Checkbox("Shoot Through Wall", &Vars::AimBot::ShootThroughWall);
				ImGui::Checkbox("Silent Aim", &Vars::AimBot::salobydischanwork_work);

			}

			if (ImGui::CollapsingHeader("Resource Options"))
			{
				ImGui::Checkbox("Stone", &Vars::Visuals::Stone);
				ImGui::Checkbox("Sulphur", &Vars::Visuals::Sulphur);
				ImGui::Checkbox("Metal", &Vars::Visuals::Metal);
				ImGui::Checkbox("Stash", &Vars::Visuals::Stash);
				ImGui::Checkbox("Hemp", &Vars::Visuals::Hemp);
				ImGui::Checkbox("Minicopter", &Vars::Visuals::Minicopter);
				ImGui::Checkbox("Traps", &Vars::Visuals::Traps);
				ImGui::Checkbox("Corpse", &Vars::Visuals::Corpse);
				ImGui::Checkbox("Mushrooms", &Vars::Visuals::Mushrooms);
				ImGui::Checkbox("ScrapHeli", &Vars::Visuals::ScrapHeli);
				ImGui::Checkbox("Crate", &Vars::Visuals::Crate);
			}

			if (ImGui::CollapsingHeader("Misc Options"))
			{
				ImGui::Checkbox("Fake Admin", &Vars::Misc::FakeAdmin);
				ImGui::Checkbox("Set Swimming", &Vars::Misc::swiming);
				ImGui::Checkbox("Always Day", &Vars::Misc::AlwaysDay);
				ImGui::Checkbox("Spider Man", &Vars::Misc::SpiderMan);
				ImGui::Checkbox("High Jump", &Vars::Misc::HighJump);
				if (Vars::Misc::HighJump) {
					ImGui::SliderFloat("Gravity Percentage", &Vars::Misc::JumpValue, 0.f, 2.5f);
				}
				ImGui::Checkbox("Walk on Water (V)", &Vars::Misc::WalkOnWater);
				ImGui::Checkbox("Speed in Heavy", &Vars::Misc::NoHeavyReduct);
				ImGui::Checkbox("Water Boost", &Vars::Misc::WaterBoost);
				ImGui::Checkbox("Aim in Jump", &Vars::Misc::AimInJump);
				ImGui::Checkbox("Stuck in Air (CAPSLOCK)", &Vars::Misc::AirStuck);

			}

			if (ImGui::CollapsingHeader("Test Options"))
			{
				ImGui::Text("Unload Menu = HOME");
			}

			ImGui::End();
	}
}



#include <Winuser.h>
#include <tchar.h>
#include <iostream>
using namespace std;

HMODULE g_hModule;
PDWORD64 origPresent = 0;
PDWORD64 origResize = 0;
void hookD3D11PresentOld(IDXGISwapChain* pSwapChain);

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (Vars::Other::bUnloading)
	{
		GUI::NextWndProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)GUI::NextWndProc);
		*(uintptr_t*)origPresent = (uintptr_t)oPresent;
		*(uintptr_t*)origResize = (uintptr_t)oResize;

		auto result = oPresent(pSwapChain, SyncInterval, Flags);
		HANDLE handle = CreateThread(nullptr, NULL, DetachDll, g_hModule, NULL, nullptr);
		CloseHandle(handle);
		return result;
	}
	else
	{
		if (GetAsyncKeyState(VK_HOME))
			Vars::Other::bUnloading = true;
	}

	dxSwapChain = pSwapChain;

	if (!InitD3DX11(dxSwapChain))
	{
		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (!pD11RenderTargetView)
		{
			if (SUCCEEDED(dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
			{
				pD11Device->CreateRenderTargetView(renderTargetTexture, nullptr, &pD11RenderTargetView);
				renderTargetTexture->Release();
			}
		}
	}

	static bool show_app_metrics = false;
	static bool show_app_style_editor = false;
	static bool show_app_about = false;

	if (show_app_metrics)
	{
		ImGui::ShowMetricsWindow(&show_app_metrics);
	}
	if (show_app_style_editor)
	{
		ImGui::Begin("Style Editor", &show_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End();
	}
	if (show_app_about)
	{
		ImGui::ShowAboutWindow(&show_app_about);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	MENU.Functions();

	hookD3D11PresentOld(pSwapChain);

	if (GUI::Render.NewFrame(pSwapChain))
	{
		

		if (Vars::AimBot::DrawFov)
			DrawFOV();

		if (Vars::AimBot::Crosshair)
			Crosshair();

		GUI::Render.EndFrame();
	}

	pD11DeviceContext->OMSetRenderTargets(1, &pD11RenderTargetView, nullptr);
	ImGui::Render();

	if (pD11RenderTargetView)
	{
		pD11RenderTargetView->Release();
		pD11RenderTargetView = nullptr;
	}

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

	if (SUCCEEDED(ret))
		(*ppDevice)->GetImmediateContext(ppContext);

	return ret;
}

void hookD3D11PresentOld(IDXGISwapChain* pSwapChain)
{
	if (GUI::Render.NewFrame(pSwapChain))
	{
		GUI::Render.String({ 15.f, 15.f }, L"Welcome, Name", D2D1::ColorF::Red);
		Vector2 kek = GUI::Render.CanvasSize();
		Vars::Global::ScreenWidth = kek.x;
		Vars::Global::ScreenHigh = kek.y;

		if (Vars::AimBot::BodyAim) Vars::Global::BoneToAim = 1;
		else Vars::Global::BoneToAim = BoneList(neck);

		EntityZaLoop();

		if (GUI::Visible)
		{
			if (Vars::AimBot::Fov > (kek.y - 3)) Vars::AimBot::Fov = (kek.y - 3);
			D2D1::ColorF FillClr = { 220, 20, 60 };
			/*GUI::Begin(Vars::Global::MenuPos, Vector2{ 520.f, 290.f }, { 0.01, 0, 0.15, 0.5 }); //light blue
			GUI::Render.StringCenter(GUI::CenterLine({ Vars::Global::MenuPos.x + 40.f, Vars::Global::MenuPos.y + 15.f }), L"EGGHEAD.IO", D2D1::ColorF::White, true);
			GUI::CurWnd.Pos.y += 25.f;
			GUI::Render.Line({ CurWnd.Pos.x, CurWnd.Pos.y - 25 }, { CurWnd.Pos.x + 520.f, CurWnd.Pos.y - 25 }, D2D1::ColorF::Purple); // 0
			GUI::Render.Line({ CurWnd.Pos.x + 110.f, CurWnd.Pos.y - 25 }, { CurWnd.Pos.x + 110.f, CurWnd.Pos.y + 270.f - 30.f }, D2D1::ColorF::Purple); // 0
			GUI::Render.Line({ CurWnd.Pos.x + 0.10f, CurWnd.Pos.y - 25 }, { CurWnd.Pos.x + 0.10f, CurWnd.Pos.y + 269.f - 30.f }, D2D1::ColorF::Purple); // 1
			GUI::Render.Line({ CurWnd.Pos.x, CurWnd.Pos.y + 18 }, { CurWnd.Pos.x + 110.f, CurWnd.Pos.y + 18 }, D2D1::ColorF::Purple); // 1
			GUI::Render.Line({ CurWnd.Pos.x, CurWnd.Pos.y + 58 }, { CurWnd.Pos.x + 110.f, CurWnd.Pos.y + 58 }, D2D1::ColorF::Purple); // 2
			GUI::Render.Line({ CurWnd.Pos.x, CurWnd.Pos.y + 98 }, { CurWnd.Pos.x + 110.f, CurWnd.Pos.y + 98 }, D2D1::ColorF::Purple); // 3
			GUI::Render.Line({ CurWnd.Pos.x, CurWnd.Pos.y + 138 }, { CurWnd.Pos.x + 110.f, CurWnd.Pos.y + 138 }, D2D1::ColorF::Purple); // 4
			*/GUI::CurWnd.Pos.y -= 15.f;
			float CurPos = GUI::CurWnd.Pos.y;
			const wchar_t* pos[5] =
			{
				L" A̲i̲m̲B̲o̲t̲", L" V̲i̲s̲u̲a̲l̲s̲", L"W̲e̲a̲p̲o̲n̲", L"   M̲i̲s̲c̲", L"  Other"
			};
			for (int i = 0; i < 5 && false; i++)
			{
				D2D1::ColorF TextClr = { 99, 0, 0, 1.0f };
				D2D1::ColorF FillClr = { 99, 0, 0, 1.0f };
				if (i == Vars::Global::CurPage)
				{
					Render.FillRectangle({ CurWnd.Pos.x + 15.f, CurPos }, { 85.f, 25.f }, FillClr);
				}
				else
				{
					Render.Rectangle({ CurWnd.Pos.x + 15.f, CurPos }, { 85.f, 25.f }, FillClr); //change clr if selected
				}
				Render.String({ CurWnd.Pos.x + 20.f, CurPos }, pos[i], TextClr, true);
				if (IO.LKM == Clicked && InRect({ CurWnd.Pos.x + 20.f, CurPos }, { 80.f, 30.f }, IO.MousePos) && InputMgr(L"Tab", true)) Vars::Global::CurPage = i;
				CurPos += 40.f;

			}

			GUI::End();
		}

		GUI::Render.EndFrame();
	}
	GUI::Render.EndFrame();
	Vars::Global::PresentDelta = 0;
}

__declspec(noinline) void StartRender()
{
	DWORD64 dwX64PresentOrig = NULL; DWORD64 dwX64ResizeBuffers = NULL;
	// hWnd = FC(user32, FindWindowA, StrA("UnityWndClass"), NULL);
	// GUI::NextWndProc = (WNDPROC)FC(user32, SetWindowLongPtrW, hWnd, GWLP_WNDPROC, (LONG_PTR)GUI::WndProc);
	origPresent = (PDWORD64)RVA(FindPattern((PBYTE)"\xFF\x15\x00\x00\x00\x00\x8B\xD8\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xEB\x10", StrA("xx????xxx????x????xx"), StrW(L"DiscordHook64.dll")), 6);
	dwX64PresentOrig = *origPresent;
	oPresent = (DX11PresentFn)(*origPresent);
	*origPresent = (DWORD_PTR)(&hookD3D11Present);

	origResize = (PDWORD64)RVA(FindPattern((PBYTE)"\x44\x8B\xCB\x44\x8B\xC7", StrA("xxxxxx"), StrW(L"DiscordHook64.dll")) + 0x1F, 7);
	dwX64ResizeBuffers = *origResize;
	oResize = (DX11ResizeFn)(*origResize);
	*origResize = (DWORD_PTR)(&Resize);

	//CreateThreadSafe(EntityHandler);
	//CreateThreadSafe(CheatFunc);
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved)
{
	if (dwCallReason == DLL_PROCESS_ATTACH)
	{
#if AllocCons == 1
		//FC(kernel32, AllocConsole);
		//FILE* pFILE = 0;
		//freopen_s(&pFILE, "CONIN$", "r", stdin);
		//freopen_s(&pFILE, "CONOUT$", "w", stdout);
		//freopen_s(&pFILE, "CONOUT$", "w", stderr);
#endif
		//FLIC::Start();

		//AllocConsole();
		//freopen("CONOUT$", "w", stdout);
		g_hModule = hModule;
		StartRender();
	}

	return TRUE;
}

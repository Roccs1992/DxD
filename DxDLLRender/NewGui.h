#include <d2d1.h>
#include <dwrite_1.h>
#include <intrin.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
class RenderClass
{
private:
	//render context
	ID2D1Factory* Interface;
	ID2D1RenderTarget* Canvas;
	IDWriteFactory1* TextEngine;
	IDWriteTextFormat* TextFormat;
	IDWriteTextFormat* BigTextFormat;
	IDWriteTextFormat* CuTextFormat;
	ID2D1SolidColorBrush* SolidColor;

	//fast get wstring length
	__forceinline UINT wcslen(const wchar_t* Str) {
		const wchar_t* TempStr = Str;
		for (; *TempStr; ++TempStr);
		return (UINT)(TempStr - Str);
	}

	__declspec(noinline) bool InitRender(IDXGISwapChain* SwapChain)
	{
		//prep d2d render & text engine
		static bool initialized; if (!initialized) {
			initialized = true; D2D1_FACTORY_OPTIONS CreateOpt = { D2D1_DEBUG_LEVEL_NONE };
			FC(dwrite, DWriteCreateFactory, DWRITE_FACTORY_TYPE_SHARED, __uuidof(TextEngine), (IUnknown**)&TextEngine);
			FC(d2d1, D2D1CreateFactory, D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &CreateOpt, (void**)&Interface);
			TextEngine->CreateTextFormat(StrW(L"Century Gothic"), NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 22.f, L"", &TextFormat);
			TextEngine->CreateTextFormat(StrW(L"Century Gothic"), NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 30.f, L"", &BigTextFormat);
			TextEngine->CreateTextFormat(StrW(L"Century Gothic"), NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 30.f, L"", &CuTextFormat);
			printf("InitRender %d %d %d\n", Interface != 0, TextEngine != 0, TextFormat != 0);
			if (!Interface || !TextEngine || !TextFormat) return false;
		}

		//get window flags var
		ID3D11Device* d3d_device;
		if (SwapChain->GetDevice(IID_PPV_ARGS(&d3d_device))) return false;
		WORD flagsOffset = *(WORD*)((*(DWORD64**)d3d_device)[38] + 2); //x64
		int& flags = *(INT*)((DWORD64)d3d_device + flagsOffset);
		d3d_device->Release();

		//get d3d backbuffer
		IDXGISurface* d3d_bbuf;
		if (SwapChain->GetBuffer(0, IID_PPV_ARGS(&d3d_bbuf)))
			return false;

		//create canvas & cleanup
		D2D1_RENDER_TARGET_PROPERTIES d2d_prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		flags |= 0x20; HRESULT canvas_state = Interface->CreateDxgiSurfaceRenderTarget(d3d_bbuf, d2d_prop, &Canvas); flags &= ~0x20; d3d_bbuf->Release(); if (canvas_state) return false;
		if (!SolidColor) Canvas->CreateSolidColorBrush({}, &SolidColor); return true;
	}

public:
	//canvas mgr
	__forceinline bool NewFrame(IDXGISwapChain* SwapChain)
	{
		//need prep d2d canvas
		if (!Canvas && !InitRender(SwapChain))
			return false;

		//draw start
		Canvas->BeginDraw();
		return true;
	}

	__forceinline Vector2 CanvasSize()
	{
		D2D1_SIZE_F Size = Canvas->GetSize();
		return Vector2{ Size.width, Size.height };
	}

	__forceinline void ResetCanvas()
	{
		if (Canvas)
		{
			Canvas->Release();
			Canvas = nullptr;
		}
	}

	__forceinline void EndFrame()
	{
		HRESULT state = Canvas->EndDraw();
		if (state == D2DERR_RECREATE_TARGET)
			ResetCanvas();
	}

	void DrawBox(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled)
	{

	}

	void DrawCircle(float x, float y, float radius, float thickness, float r, float g, float b, float a, bool filled)
	{

	}

	void DrawEllipse(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled)
	{

	}

	void DrawString(std::string str, float fontSize, float x, float y, float r, float g, float b, float a = 1)
	{

	}

	void DrawWString(std::wstring str, float fontSize, float x, float y, float r, float g, float b, float a = 1)
	{

	}

	void DrawLine(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a)
	{

	}
	//line
	__forceinline void Line(const Vector2& Start, const Vector2& End, const D2D1::ColorF& Clr, float Thick = 2.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawLine({ Start.x, Start.y }, { End.x, End.y }, SolidColor, Thick);
	}

	//circle
	__forceinline void Ñircle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad, float Thick = 2.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawEllipse({ { Start.x, Start.y }, Rad, Rad }, SolidColor, Thick);
	}

	__forceinline void FillCircle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad)
	{
		SolidColor->SetColor(Clr); Canvas->FillEllipse({ { Start.x, Start.y }, Rad, Rad }, SolidColor);
	}

	//rectangle
	__forceinline void Rectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Thick = 2.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, SolidColor, Thick);
	}
	__forceinline void FillRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr)
	{
		SolidColor->SetColor(Clr); Canvas->FillRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, SolidColor);
	}

	__forceinline void RoundedRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawRoundedRectangle({ {Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, Rad, Rad }, SolidColor, Thick);
	}

	__forceinline void FillRoundedRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Rad)
	{
		SolidColor->SetColor(Clr); Canvas->FillRoundedRectangle({ {Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y}, Rad, Rad }, SolidColor);
	}

	//text
	__forceinline Vector2 StringCenter(const Vector2& Start, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false)
	{
		SolidColor->SetColor(Clr); IDWriteTextLayout* TextLayout;
		if (big) TextEngine->CreateTextLayout(Str, this->wcslen(Str), CuTextFormat, 1200.f, 300.f, &TextLayout);
		else TextEngine->CreateTextLayout(Str, this->wcslen(Str), TextFormat, 300.f, 200.f, &TextLayout);
		DWRITE_TEXT_METRICS TextInfo;
		TextLayout->GetMetrics(&TextInfo);
		Vector2 TextSize = { TextInfo.width / 4.f, TextInfo.height / 4.f };
		Canvas->DrawTextLayout({ Start.x - TextSize.x, Start.y - TextSize.y }, TextLayout, SolidColor); TextLayout->Release();
		return TextSize; //return half text size
	}

	__forceinline void String(const Vector2& Start, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false)
	{
		SolidColor->SetColor(Clr);
		if (!big) Canvas->DrawTextW(Str, this->wcslen(Str), TextFormat, { Start.x, Start.y, FLT_MAX, FLT_MAX }, SolidColor);
		else Canvas->DrawTextW(Str, this->wcslen(Str), BigTextFormat, { Start.x, Start.y, FLT_MAX, FLT_MAX }, SolidColor);
	}
};
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace GUI
{
	bool Visible = true;
	RenderClass Render;
	WNDPROC NextWndProc;
	enum Button {
		NoPress,
		Clicked,
		Pressed
	};
	struct IO {
		Button LKM;
		Vector2 MousePos;
		Vector2 MouseDelta;
		Vector2 OldMousePos;
		USHORT CurElement;
	} IO;
	struct WndData {
		Vector2 WndPos;
		Vector2 Size;
		Vector2 Pos;
	} CurWnd;

	unsigned short __forceinline HashStr(const wchar_t* Str)
	{
		unsigned char i;
		unsigned short crc = 0xFFFF;
		while (wchar_t DChar = *Str++)
		{
			unsigned char Char = (unsigned char)DChar;
			crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
			Char = (unsigned char)(DChar >> 8);
			crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
		} return crc;
	}

	Vector2 __forceinline CenterLine(const Vector2& Pos)
	{
		return { (Pos.x + (CurWnd.Size.x / 2)), Pos.y };
	}

	bool __forceinline InRect(Vector2 Rect, Vector2 Size, Vector2 Dot)
	{
		return (Dot.x > Rect.x && Dot.x < Rect.x + Size.x && Dot.y > Rect.y && Dot.y < Rect.y + Size.y);
	}

	LRESULT WINAPI WndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_LBUTTONDOWN:
			IO.LKM = Pressed;
			break;

		case WM_LBUTTONUP:
			IO.LKM = Clicked;
			IO.CurElement = 0;
			break;

		case WM_KEYUP:
			if (wParam == VK_INSERT || wParam == VK_NEXT)
				Visible = !Visible;
			break;

		case WM_MOUSEMOVE:
			IO.MousePos.x = (signed short)(lParam);
			IO.MousePos.y = (signed short)(lParam >> 16);
			break;
		}

		if (Visible && !ImGui_ImplWin32_WndProcHandler(Wnd, Msg, wParam, lParam))
			return TRUE;

		return FC(user32, CallWindowProcW, NextWndProc, Wnd, Msg, wParam, lParam);
	}

	void ProcessInput(bool End = false)
	{
		if (!End)
		{
			//calc mouse delta
			IO.MouseDelta = IO.MousePos - IO.OldMousePos;
			IO.OldMousePos = IO.MousePos;
		}

		else
		{
			//update LKM button
			if (IO.LKM == Clicked)
				IO.LKM = NoPress;
		}
	}

	bool InputMgr(const wchar_t* Name, bool Reg = false)
	{
		unsigned short StrHash = HashStr(Name);
		if (Reg && !IO.CurElement)
		{
			IO.CurElement = StrHash;
			return true;
		}
		else if (!Reg)
			return (IO.CurElement == StrHash);
		return false;
	}

	//main
	void SliderFloat(const wchar_t* Name, float* Current, float Min, float Max)
	{
		wchar_t Str[64];
		_swprintf(Str, L"%s : %d", Name, (int)*Current);
		Render.String({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, Str);
		CurWnd.Pos.y += 20.f;

		if (IO.LKM == Pressed && InRect({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { CurWnd.Size.x - 20.f, 16.f }, IO.MousePos))
		{
			float Val = ((((IO.MousePos.x - 18.f - CurWnd.Pos.x) / (CurWnd.Size.x - 36.f)) * (Max - Min)) + Min);
			*Current = ((Val > Max) ? Max : ((Val < Min) ? Min : Val)); InputMgr(Name, true);
		}

		float CurOff = (*Current - Min) / (Max - Min);
		Render.FillRoundedRectangle({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { CurWnd.Size.x - 20.f, 16.f }, D2D1::ColorF{ 0.844f, 0.844f, 0.844f, 0.7f }, 7.f);
		Render.FillCircle({ CurWnd.Pos.x + 18.f + ((CurWnd.Size.x - 36.f) * CurOff), CurWnd.Pos.y + 8 }, D2D1::ColorF::White, 7.f);
		CurWnd.Pos.y += 24.f;
	}

	void SliderFloatS(const wchar_t* Name, float* Current, float Min, float Max)
	{
		float adapt = Max / 100; int AdaptCurrent = *Current / adapt;
		wchar_t Str[64];
		_swprintf(Str, L"%s : %d%%/100%%", Name, AdaptCurrent);
		Render.String({ CurWnd.Pos.x + 100.f, CurWnd.Pos.y + 5.f }, Str);
		CurWnd.Pos.y += 20.f;

		if (IO.LKM == Pressed && InRect({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { CurWnd.Size.x - 20.f, 16.f }, IO.MousePos))
		{
			float Val = ((((IO.MousePos.x - 18.f - CurWnd.Pos.x) / (CurWnd.Size.x - 36.f)) * (Max - Min)) + Min);
			*Current = ((Val > Max) ? Max : ((Val < Min) ? Min : Val)); InputMgr(Name, true);
		}

		float CurOff = (*Current - Min) / (Max - Min);
		Render.FillRoundedRectangle({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { CurWnd.Size.x - 20.f, 16.f }, D2D1::ColorF{ 0.844f, 0.844f, 0.844f, 0.7f }, 8.f);
		Render.FillCircle({ CurWnd.Pos.x + 18.f + ((CurWnd.Size.x - 36.f) * CurOff), CurWnd.Pos.y + 8 }, D2D1::ColorF::White, 8.f);
		CurWnd.Pos.y += 24.f;
	}

	void CheckBox(const wchar_t* Name, bool* Switch, bool last = false)
	{
		bool SW = *Switch; Render.String({ CurWnd.Pos.x + 30.f, CurWnd.Pos.y }, (wchar_t*)Name);
		if (IO.LKM == Clicked && InRect({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { 20.f, 20.f }, IO.MousePos) && InputMgr(Name, true)) *Switch = SW = !SW;
		Render.FillRectangle({ CurWnd.Pos.x + 10.f, CurWnd.Pos.y }, { 15.f, 15.f }, D2D1::ColorF::White);
		Render.FillRectangle({ CurWnd.Pos.x + 10.f + 2.5f, CurWnd.Pos.y + 2.5f }, { 10.f, 10.f }, SW ? D2D1::ColorF::Green : D2D1::ColorF::Green);
		CurWnd.Pos.y += 18.f;
		if (last) CurWnd.Pos.y += 2.f;
	}

	void Spacing(const wchar_t* Name) {
		Vector2 CntLine = CenterLine(CurWnd.Pos); Vector2 TextSize = Render.StringCenter(CntLine, Name);
		Render.Line({ CntLine.x - TextSize.x - 2.f, CntLine.y }, { CurWnd.Pos.x + 4.f, CurWnd.Pos.y }, D2D1::ColorF::White);
		Render.Line({ CntLine.x + TextSize.x + 2.f, CntLine.y }, { CurWnd.Pos.x + CurWnd.Size.x - 4.f, CurWnd.Pos.y }, D2D1::ColorF::White);
		CurWnd.Pos.y += 8.f;
	}

	void Begin(Vector2& Pos, const Vector2& Size, const D2D1::ColorF& Clr)
	{
		//base menu
		ProcessInput();
		if (!CurWnd.WndPos.Zero()) Pos = CurWnd.WndPos;
		Render.FillRoundedRectangle(Pos, Size, Clr, 25.f);
		CurWnd.WndPos = { Pos.x, Pos.y }; CurWnd.Size = Size;
		CurWnd.Pos = { Pos.x, Pos.y + 25.f };
	}

	void End()
	{
		//drag window
		ProcessInput(true);
		//if (InputMgr(L"##Drag") || (IO.LKM == Pressed && InRect(CurWnd.WndPos, CurWnd.Size, IO.MousePos) && InputMgr(L"##Drag", true)))
			//CurWnd.WndPos += IO.MouseDelta;
	}
}
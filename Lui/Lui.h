#pragma once

#include <Windows.h>
#include <comdef.h>
#include <string>
#include <vector>
#include <stdexcept>

#ifdef UNICODE
#define MAINDECLARE(hInstance,hPrevInstance,lpCmdLine,nCmdShow)\
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,\
	_In_opt_ HINSTANCE hPrevInstance,\
	_In_ LPWSTR lpCmdLine,\
	_In_ int nCmdShow)
#else
#define MAINDECLARE(hInstance,hPrevInstance,lpCmdLine,nCmdShow)\
int APIENTRY WinMain(_In_ HINSTANCE hInstance,\
	_In_opt_ HINSTANCE hPrevInstance,\
	_In_ LPSTR lpCmdLine,\
	_In_ int nCmdShow)
#endif // UNICODE

constexpr DWORD ID_BTN = 1000;

typedef void(*LFuncType)(void*);
#if UNICODE
typedef std::wstring LTstdstr;
#else
typedef std::string LTstdstr;
#endif // UNICODE


class Lui;
class LWindow;
class LButton;

struct LCrood {
	DWORD x, y;
};

class Lui
{
public:
	const static void DefaultFunction(void*) {};
	static HINSTANCE thisHINSTANCE;
};

class LWindow : Lui
{
public:
	//register & create
	bool create()
	{

		return this->creation;
	}

	bool init(const LTstdstr& classname, const LTstdstr& title)
	{

	}
	void setupUi();
	enum Signal {
		LButtonDown,
		RButtonDown,
		LButtonMove,
		RButtonMove,
		Close,
		__last
	};
	enum Style {
		//Window Styles
		Border = WS_BORDER,					//窗口具有正常的边框
		Caption = WS_CAPTION,				//窗口具有标题栏
		Child = WS_CHILD,
		Clipchildren = WS_CLIPCHILDREN,
		Disabled = WS_DISABLED,
		Dlgframe = WS_DLGFRAME,				//窗口无标题栏
		Group = WS_GROUP,
		HScroll = WS_HSCROLL,				//窗口具有水平滚动条
		Iconic = WS_ICONIC,
		Maximize = WS_MAXIMIZE,				//窗口具有最大化状态
		MaximizeBox = WS_MAXIMIZEBOX,		//窗口具有最大化按钮
		Minimize = WS_MINIMIZE,				//窗口具有最小化状态
		MinimizeBox = WS_MINIMIZEBOX,		//窗口具有最小化按钮
		Overlapped = WS_OVERLAPPED,			//窗口具有标题和边框
		Popup = WS_POPUP,					//作为弹出式窗口来创建
		Sizebox = WS_SIZEBOX,
		Sysmenu = WS_SYSMENU,				//窗口的标题栏具有系统菜单框
		Tabstop = WS_TABSTOP,
		Thickframe = WS_THICKFRAME,			//窗口具有可缩放的厚边框
		Tiled = WS_TILED,
		TiledWindow = WS_TILEDWINDOW,
		VScroll = WS_VSCROLL,				//窗口具有垂直滚动条
		Visible = WS_VISIBLE,
		//Common Window Styles
		OverlappedWindow = WS_OVERLAPPEDWINDOW,
		PopupWindow = WS_POPUPWINDOW,
		ChildWindow = WS_CHILDWINDOW
	};
	void connect(Signal slot, LFuncType func) {
#ifdef _DEBUG
		if (slot >= __last)
		{
			throw std::runtime_error("Error: slot index out of range");
			return;
		}
#endif // _DEBUG
		slotsFunc[slot] = func;
	}
	LButton* newButton();

private:
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool creation = false;
	HWND thisHWND = NULL;
	LFuncType slotsFunc[__last] = { (LFuncType)DefaultFunction };
	//std::vector<LButton> buttons;

	LTstdstr thisclassname, thistitle;
};

class LButton : Lui
{
public:
	enum Signal {
		Click,
		Release,
		__last
	};
	void connect(Signal slot, LFuncType func) {
#ifdef _DEBUG
		if (slot >= __last)
		{
			throw std::runtime_error("Error: slot index out of range");
			return;
		}
#endif // _DEBUG
		slotsFunc[slot] = func;
	}
	//event simulation
	void click()
	{
#ifdef _DEBUG
		this->slotsFunc[Click](NULL);
#endif // _DEBUG
	}
private:
	LFuncType slotsFunc[__last] = { (LFuncType)DefaultFunction };
	HWND thisHWND;
	HMENU thisHMENU;
	static DWORD lastHMENU;

};

//namespace for private global variables
namespace nLui
{
	static HINSTANCE thisHINSTANCE = NULL;
}

//static class variables initialization
DWORD LButton::lastHMENU = ID_BTN;

//handle messages
LRESULT CALLBACK LWindow::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
/*
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
*/
int main(int argc, char** argv);
MAINDECLARE(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	//invoke_main

	return 0;
}
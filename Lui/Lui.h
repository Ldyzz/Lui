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

typedef RECT LRect;

class Lui
{
public:
	const static void DefaultFunction(void*) {};
	static HINSTANCE thisHINSTANCE;
};

class LWindow : Lui
{
public:
	LWindow()
	{
		// ==========TODO: add default slot functions==========
		slotsFunc[Close] = [](void*) {};
	}
	//register & create
	bool create()
	{
		if (this->creation)
		{
			throw std::runtime_error("Error: window class REcreation");
			return false;
		}
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = this->wndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = this->thisHINSTANCE;
		wcex.hIcon = LoadIcon(this->thisHINSTANCE, MAKEINTRESOURCE(107));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(109);
		wcex.lpszClassName = this->thisclassname.c_str();
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));
		this->creation = RegisterClassEx(&wcex);
		this->thisHWND = CreateWindowW(this->thisclassname.c_str(), this->thistitle.c_str(), this->thisstyle,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, this->thisHINSTANCE, nullptr);
		this->creation = this->creation && this->thisHWND;
		return this->creation;
	}
	void init(const LTstdstr& title = TEXT("window title"),
		const INT32 style = OverlappedWindow,
		const LWindow* parent_window = NULL,
		const LRect& rect = { 0,0,800,600 },
		const bool adj = true,
		const bool hasmenu = false,
		const LTstdstr& classname = TEXT("LuiWindowClass"))
	{
		this->thisclassname = classname;
		this->thistitle = title;
		this->thisparent_window = const_cast<LWindow*>(parent_window);
		this->thisstyle = style;
		this->thisrect = rect;
		this->menu = hasmenu;
		if (adj)
			AdjustWindowRectEx(&this->thisrect, this->thisstyle, this->menu, this->thisstyle);
	}
	// ==========TODO: dynamically & statically adding ui elements==========
	//void setupUi();
	int handleMessages()
	{
		MSG msg;
		//main message loop
		while (GetMessage(&msg, this->thisHWND, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}
	void show() const
	{
		ShowWindow(this->thisHWND, SW_SHOW);
		UpdateWindow(this->thisHWND);
	}
	enum Signal {
		LButtonDown,
		RButtonDown,
		LButtonMove,
		RButtonMove,
		Create,
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
		if (slot >= __last || slot < 0)
		{
			throw std::runtime_error("Error: slot index out of range");
			return;
		}
#endif // _DEBUG
		slotsFunc[slot] = func;
	}
	// ==========TODO: generating menus==========
private:
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND thisHWND = NULL;
	DWORD thisstyle = OverlappedWindow;
	LWindow* thisparent_window = NULL;
	LRect thisrect;
	bool creation = false;
	bool menu = false;
	// ==========FIXME: const_cast not available==========
	LFuncType slotsFunc[__last] = { (void(*)(void*))(Lui::DefaultFunction) };
	// ==========TODO: vector of LButton==========
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

//static class variables initialization
DWORD LButton::lastHMENU = ID_BTN;
HINSTANCE Lui::thisHINSTANCE = NULL;
//core: handle messages
LRESULT CALLBACK LWindow::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ========== TODO: button messages ==========
	switch (msg)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// button and menu messages
		switch (wmId)
		{
			
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		// ========== TODO: create buttons ==========

	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;

}

int main(int argc, char** argv);

MAINDECLARE(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	Lui::thisHINSTANCE = hInstance;
	LWindow window;
	window.init();
	auto a = window.create();
	window.show();
	return window.handleMessages();
}
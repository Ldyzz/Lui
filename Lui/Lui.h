#pragma once

#include <Windows.h>
#include <comdef.h>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

//#define LUI_NOT_CHECK

#ifdef UNICODE
#define MAINDECLARE(hInstance,hPrevInstance,lpCmdLine,nCmdShow)\
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,\
	_In_opt_ HINSTANCE hPrevInstance,\
	_In_ LPWSTR lpCmdLine,\
	_In_ int nCmdShow)
typedef std::wstring LTstdstr;
#define lto_tstring std::to_wstring
#else
#define MAINDECLARE(hInstance,hPrevInstance,lpCmdLine,nCmdShow)\
int APIENTRY WinMain(_In_ HINSTANCE hInstance,\
	_In_opt_ HINSTANCE hPrevInstance,\
	_In_ LPSTR lpCmdLine,\
	_In_ int nCmdShow)
typedef std::string LTstdstr;
#define lto_tstring std::to_string
#endif // UNICODE

constexpr DWORD ID_BTN = 1000;

typedef std::function<void(void*)> LFuncType;

class Lui;
class LWindow;
class LWindowClass;
class LButton;

class LCrood {
public:
	LONG x, y;
	LCrood(LONG nx, LONG ny) :x(nx), y(ny) {}
};
typedef RECT LRect;

class Lui
{
	friend class LWindowClass;
protected:
	static HINSTANCE thisHINSTANCE;
	bool bAccessable = true;
	void checkAccess()const
	{
		if (!this->bAccessable)
			throw std::runtime_error("Error: trying to access destroyed window");
	}
	static std::vector<LWindow> vec_windows;
public:
	// ========== FIXME: may need c++ 11 ==========
	enum Style :DWORD;
	bool isAccessable()const { return this->bAccessable; };
	static void DefaultFunction(void*) {};
	static void setHINSTANCE(HINSTANCE nh)
	{
		if (!Lui::thisHINSTANCE)//only set once
			Lui::thisHINSTANCE = nh;
#ifndef LUI_NOT_CHECK
		else
			throw std::runtime_error("Error: hInstance can only be set once");
#endif
	}
};

class LWindowClass : Lui {
	friend class LWindow;
	static LWindowClass default_windowclass;
public:
	//constructor
	LWindowClass(const LTstdstr& classname = TEXT("LuiWindowClass"), const DWORD dwstyle = LWindowClass::Style::Default)
	{
		this->thisclassname = classname;
		this->thisstyle = dwstyle;
	}
	enum Style {
		//copied from https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
		BytealignClient = CS_BYTEALIGNCLIENT,	//Aligns the window's client area on a byte boundary (in the x direction). This style affects the width of the window and its horizontal placement on the display.
		BytealignWindow = CS_BYTEALIGNWINDOW,	//Aligns the window on a byte boundary(in the x direction).This style affects the width of the window and its horizontal placement on the display.
		ClassDC = CS_CLASSDC,					//Allocates one device context to be shared by all windows in the class.Because window classes are process specific, it is possible for multiple threads of an application to create a window of the same class.It is also possible for the threads to attempt to use the device context simultaneously.When this happens, the system allows only one thread to successfully finish its drawing operation.
		DblClks = CS_DBLCLKS,					//Sends a double - click message to the window procedure when the user double - clicks the mouse while the cursor is within a window belonging to the class.
		DropShadow = CS_DROPSHADOW,				//Enables the drop shadow effect on a window.The effect is turned on and off through SPI_SETDROPSHADOW.Typically, this is enabled for small, short - lived windows such as menus to emphasize their Z - order relationship to other windows.Windows created from a class with this style must be top - level windows; they may not be child windows.
		Globalclass = CS_GLOBALCLASS,			//Indicates that the window class is an application global class.For more information, see the "Application Global Classes" section of About Window Classes.
		HRedraw = CS_HREDRAW,					//Redraws the entire window if a movement or size adjustment changes the width of the client area.
		NoClose = CS_NOCLOSE,					//Disables Close on the window menu.
		OwnDC = CS_OWNDC,						//Allocates a unique device context for each window in the class.
		ParentDC = CS_PARENTDC,					//Sets the clipping rectangle of the child window to that of the parent window so that the child can draw on the parent.A window with the CS_PARENTDC style bit receives a regular device context from the system's cache of device contexts. It does not give the child the parent's device context or device context settings.Specifying CS_PARENTDC enhances an application's performance.
		SaveBits = CS_SAVEBITS,					//Saves, as a bitmap, the portion of the screen image obscured by a window of this class.When the window is removed, the system uses the saved bitmap to restore the screen image, including other windows that were obscured.Therefore, the system does not send WM_PAINT messages to windows that were obscured if the memory used by the bitmap has not been discardedand if other screen actions have not invalidated the stored image.
		//This style is useful for small windows(for example, menus or dialog boxes) that are displayed brieflyand then removed before other screen activity takes place.This style increases the time required to display the window, because the system must first allocate memory to store the bitmap.
		VRedraw = CS_VREDRAW,					//Redraws the entire window if a movement or size adjustment changes the height of the client area.
		Default = CS_HREDRAW | CS_VREDRAW		//Default window class: CS_HREDRAW | CS_VREDRAW
	};
private:
	bool registerClass()
	{
		if (this->bRegistration)
		{
#ifndef LUI_NOT_CHECK
			throw std::runtime_error("Error: WindowClass RE registration");
#endif
			return false;
		}
		return this->bRegistration = RegisterClassEx(&this->getWndClassEx());
	}
	WNDCLASSEX getWndClassEx()
	{
		//using default icons
		// ========== TODO: add support for custom icons ==========
		WNDCLASSEX ret;
		ZeroMemory(&ret, sizeof(ret));
		ret.cbSize = sizeof(WNDCLASSEX);
		ret.style = CS_HREDRAW | CS_VREDRAW;
		ret.lpfnWndProc = this->wndProc;
		ret.cbClsExtra = 0;
		ret.cbWndExtra = 0;
		ret.hInstance = this->thisHINSTANCE;
		ret.hIcon = LoadIcon(this->thisHINSTANCE, IDI_APPLICATION);
		ret.hCursor = LoadCursor(nullptr, IDC_ARROW);
		ret.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		ret.lpszMenuName = NULL;
		ret.lpszClassName = this->thisclassname.c_str();
		ret.hIconSm = LoadIcon(ret.hInstance, IDI_APPLICATION);
		return ret;
	}
	bool bRegistration = false;
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LTstdstr thisclassname;
	DWORD thisstyle = Default;
};

class LWindow : Lui
{
	friend class LWindowClass;
public:
	LWindow()
	{
		// ==========TODO: add default slot functions==========
		for (int i = 0; i < LWindow::Signal::__last; i++)
			this->slotsFunc[i] = Lui::DefaultFunction;
	}
	static LWindow* newWindow()
	{
		Lui::vec_windows.emplace_back();
		return &Lui::vec_windows.back();
	}
	//register & create
	bool create()
	{
		if (this->bCreation)
		{
#ifndef LUI_NOT_CHECK
			throw std::runtime_error("Error: window class RE creation");
#endif // !LUI_NOT_CHECK
			return false;
		}
		this->bCreation = this->thiswindowclass.registerClass();
		LONG x, y, w, h;
		if (this->thisrect.left == CW_USEDEFAULT && this->thisrect.right == CW_USEDEFAULT && this->thisrect.bottom == CW_USEDEFAULT && this->thisrect.top == CW_USEDEFAULT)
		{
			x = CW_USEDEFAULT, y = CW_USEDEFAULT, w = CW_USEDEFAULT, h = CW_USEDEFAULT;
		}
		else
		{
			//using custom display position
			x = this->thisrect.left, y = this->thisrect.top;
			w = this->thisrect.right - this->thisrect.left;
			h = this->thisrect.bottom - this->thisrect.top;
		}
		this->thisHWND = CreateWindowW(
			this->thiswindowclass.thisclassname.c_str(), this->thistitle.c_str(), this->thisstyle,
			//nX,nY,nW,nH
			x, y, w, h,
			nullptr, nullptr, this->thisHINSTANCE, nullptr);
		this->bCreation = this->bCreation && this->thisHWND;
		return this->bCreation;
	}
	void init(const LTstdstr& title = TEXT("window title"),
		const LRect& rect = { 100,100,800,600 },
		const INT32 style = OverlappedWindow,
		const LWindow* parent_window = NULL,
		const bool hasmenu = false,
		const LWindowClass& windowclass = LWindowClass::default_windowclass)
	{
		this->thiswindowclass = windowclass;
		this->thistitle = title;
		this->thisparent_window = const_cast<LWindow*>(parent_window);
		this->thisstyle = style;
		this->thisrect = rect;
		this->bMenu = hasmenu;
		AdjustWindowRectEx(&this->thisrect, this->thisstyle, this->bMenu, this->thisstyle);
	}
	// ==========TODO: dynamically & statically adding ui elements==========
	void setupUi()
	{

	}
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
	bool show()
	{

		this->bShow = ShowWindow(this->thisHWND, SW_SHOW);
		return this->bShow = this->bShow && UpdateWindow(this->thisHWND);
	}
	bool destroy()
	{
		this->checkAccess();
		this->bAccessable = !DestroyWindow(this->thisHWND);
		return !this->bAccessable;
	}
	enum Signal {
		LButtonDown,
		RButtonDown,
		LButtonUp,
		RButtonUp,
		MouseMove,
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
	void connect(DWORD slot, const LFuncType& func) {
#ifndef LUI_NOT_CHECK
		if (slot >= __last || slot < 0)
		{
			throw std::runtime_error("Error: slot index out of range");
			return;
		}
#endif // LUI_NOT_CHECK
		//memcpy(slotsFunc[slot], &func, sizeof(func));
		slotsFunc[slot] = (func);
	}
	// ==========TODO: generating menus==========
private:
	//private functions
	//private variables
	HWND thisHWND = NULL;
	DWORD thisstyle = OverlappedWindow;//default style
	LWindow* thisparent_window = NULL;
	LWindowClass thiswindowclass;
	LRect thisrect;
	bool bCreation = false;
	bool bMenu = false;
	bool bShow = false;
	LFuncType slotsFunc[__last] = {};
	// ==========TODO: vector of LButton==========
	//std::vector<LButton> buttons;
	LTstdstr thistitle;
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
LWindowClass LWindowClass::default_windowclass = LWindowClass();
std::vector<LWindow> Lui::vec_windows;

//core: handle messages
LRESULT CALLBACK LWindowClass::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ========== TODO: button messages ==========
	LWindow* window = nullptr;
	for (auto a = Lui::vec_windows.begin(); a != Lui::vec_windows.end(); a++)
	{
		if (a->thisHWND == hwnd)
		{
			window = &*a;
			break;
		}
	}
	if (window == nullptr || window->thisHWND != hwnd)
		goto def;
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
		break;
	}
	case WM_LBUTTONDOWN:
	{
		(window->slotsFunc[LWindow::Signal::LButtonDown])(&LCrood(LOWORD(lParam), HIWORD(lParam)));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		(window->slotsFunc[LWindow::Signal::RButtonDown])(&LCrood(LOWORD(lParam), HIWORD(lParam)));
		break;
	}
	case WM_LBUTTONUP:
	{
		(window->slotsFunc[LWindow::Signal::LButtonUp])(&LCrood(LOWORD(lParam), HIWORD(lParam)));
		break;
	}
	case WM_RBUTTONUP:
	{
		(window->slotsFunc[LWindow::Signal::RButtonUp])(&LCrood(LOWORD(lParam), HIWORD(lParam)));
		break;
	}
	case WM_MOUSEMOVE:
	{
		(window->slotsFunc[LWindow::Signal::MouseMove])(&LCrood(LOWORD(lParam), HIWORD(lParam)));
		break;
	}
	case WM_CREATE:
	{
		// ========== TODO: create ui elements ==========

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
def:
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main(int argc, TCHAR** argv);

MAINDECLARE(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	Lui::setHINSTANCE(hInstance);
	//TODO: generate argc & argv
	return main(0, NULL);
}
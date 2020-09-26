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
		Border = WS_BORDER,					//���ھ��������ı߿�
		Caption = WS_CAPTION,				//���ھ��б�����
		Child = WS_CHILD,
		Clipchildren = WS_CLIPCHILDREN,
		Disabled = WS_DISABLED,
		Dlgframe = WS_DLGFRAME,				//�����ޱ�����
		Group = WS_GROUP,
		HScroll = WS_HSCROLL,				//���ھ���ˮƽ������
		Iconic = WS_ICONIC,
		Maximize = WS_MAXIMIZE,				//���ھ������״̬
		MaximizeBox = WS_MAXIMIZEBOX,		//���ھ�����󻯰�ť
		Minimize = WS_MINIMIZE,				//���ھ�����С��״̬
		MinimizeBox = WS_MINIMIZEBOX,		//���ھ�����С����ť
		Overlapped = WS_OVERLAPPED,			//���ھ��б���ͱ߿�
		Popup = WS_POPUP,					//��Ϊ����ʽ����������
		Sizebox = WS_SIZEBOX,
		Sysmenu = WS_SYSMENU,				//���ڵı���������ϵͳ�˵���
		Tabstop = WS_TABSTOP,
		Thickframe = WS_THICKFRAME,			//���ھ��п����ŵĺ�߿�
		Tiled = WS_TILED,
		TiledWindow = WS_TILEDWINDOW,
		VScroll = WS_VSCROLL,				//���ھ��д�ֱ������
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
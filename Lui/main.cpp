#include "Lui.h"

int main(int argc, TCHAR** argv)
{
	LWindow* window = LWindow::newWindow();
	window->init(TEXT("A Title"));
	window->create();
	window->connect(LWindow::Signal::RButtonDown, [](void* param) {
		LTstdstr context(TEXT("RButtonDown:("));
		context += lto_tstring(((LCrood*)param)->x);
		context += TEXT(",");
		context += lto_tstring(((LCrood*)param)->y);
		context += TEXT(")");
		MessageBox(NULL, context.c_str(), TEXT("RButtonDown"), 0);
		});
	window->show();
	return window->handleMessages();
}
#include "Header.h"
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create a window

	Window window(640, 480, L"Hello World");

	//	Message pump
	
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//	Check if GetMessage call itself borked

	if (gResult == -1)
		return -1;

	return (int)msg.wParam;
}
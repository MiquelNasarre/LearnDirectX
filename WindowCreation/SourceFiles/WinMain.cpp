#include <Windows.h>
#include <WinUser.h>

//	Window procedure with inputs
//	https://learn.microsoft.com/en-us/windows/win32/learnwin32/writing-the-window-procedure
//

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case(WM_CLOSE):
		PostQuitMessage(69);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//	Register Window class
	//	https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	//

	WNDCLASSEX wc = { 0 };
	const LPCWSTR ClassName = L"Hello World";

	wc.cbSize			= sizeof(wc);
	wc.style			= CS_OWNDC;
	wc.lpfnWndProc		= WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= NULL;
	wc.hIconSm			= NULL;
	wc.hCursor			= NULL;
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= ClassName;

	RegisterClassEx(&wc);

	//	Create Window
	//	https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	//

	HWND hWnd = CreateWindowEx(
		0,
		ClassName,
		L"Just a window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200,
		200,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//	Show window
	//	https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	//

	ShowWindow(hWnd, SW_SHOW);

	//	Message pump
	//	https://learn.microsoft.com/es-es/windows/win32/api/winuser/nf-winuser-getmessage
	//

	MSG msg;
	while (GetMessage(&msg, hWnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}
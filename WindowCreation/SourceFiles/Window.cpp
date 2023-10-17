#include "Window.h"

#include "resource.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "atlstr.h"
#include <windowsx.h>
#include <chrono>

// Window Exception Stuff

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:ExceptionClass(line, file),
	hr(hr)
{
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Chili Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

// Window Class Stuff

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:hInst{ GetModuleHandle(nullptr) }
{
	//	Register Window class

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 51, 51, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const LPCWSTR Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

// Window Stuff

void Window::pushMessage()
{
	msgQueue.push(msg);
	if (msgQueue.size() > maxMessages)
		msgQueue.pop();
}

void Window::handleFramerate()
{
	if (noFrameUpdate) {
		noFrameUpdate = false;
		frame = timer.skip();
		return;
	}

	if (timer.check() < Frametime)
		std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * (Frametime - timer.check()))));
	frame = timer.mark();
}

Window::Window(int width, int height, LPCWSTR name)
	: Dimensions{ Vector2i(width,height) }, timer(true)
{
	Name = CW2A(name);

	//	Calculate window size based on desired client region size

	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
		throw CHWND_LAST_EXCEPT();

	//	Create Window & get hWnd

	hWnd = CreateWindow(
		WindowClass::GetName(), 
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		wr.right - wr.left, 
		wr.bottom - wr.top,
		nullptr, 
		nullptr, 
		WindowClass::GetInstance(),
		this
	);

	//	Check for error

	if (!hWnd)
		throw CHWND_LAST_EXCEPT();

	//	Initialize Keyboard & mouse & ...

	Keyboard::init();
	Mouse::init();
	timer.reset();

	//	Create graphics object

	graphics.create(hWnd);
	graphics.setWindowDimensions(Dimensions);

}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_CHAR:
		Keyboard::pushChar((char)wParam);
		break;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if(Keyboard::getAutorepeat() || !Keyboard::isKeyPressed((unsigned char)wParam))
			Keyboard::pushEvent(Keyboard::event::Type::Pressed, (unsigned char)wParam);
		Keyboard::setKeyPressed((unsigned char)wParam);
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		Keyboard::setKeyReleased((unsigned char)wParam);
		Keyboard::pushEvent(Keyboard::event::Type::Released, (unsigned char)wParam);
		break;

	case WM_LBUTTONDOWN:
		Mouse::pushEvent(Mouse::event::Type::Pressed, Mouse::Left, Mouse::Position);
		Mouse::setButtonPressed(Mouse::Left);
		SetCapture(hWnd);
		break;

	case WM_LBUTTONUP:
		Mouse::pushEvent(Mouse::event::Type::Released, Mouse::Left, Mouse::Position);
		Mouse::setButtonReleased(Mouse::Left);
		ReleaseCapture();
		break;

	case WM_RBUTTONDOWN:
		Mouse::pushEvent(Mouse::event::Type::Pressed, Mouse::Right, Mouse::Position);
		Mouse::setButtonPressed(Mouse::Right);
		break;

	case WM_RBUTTONUP:
		Mouse::pushEvent(Mouse::event::Type::Released, Mouse::Right, Mouse::Position);
		Mouse::setButtonReleased(Mouse::Right);
		break;

	case WM_MBUTTONDOWN:
		Mouse::pushEvent(Mouse::event::Type::Pressed, Mouse::Middle, Mouse::Position);
		Mouse::setButtonPressed(Mouse::Middle);
		break;

	case WM_MBUTTONUP:
		Mouse::pushEvent(Mouse::event::Type::Released, Mouse::Middle, Mouse::Position);
		Mouse::setButtonReleased(Mouse::Middle);
		break;

	case WM_MOUSEMOVE:
		Mouse::setPosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		Mouse::setScPosition(GET_X_LPARAM(lParam) + Position.x, GET_Y_LPARAM(lParam) + Position.y);
		Mouse::pushEvent(Mouse::event::Type::Moved, Mouse::None, Mouse::Position);
		break;

	case WM_SIZE:
		Dimensions.x = LOWORD(lParam);
		Dimensions.y = HIWORD(lParam);
		graphics.setWindowDimensions(Dimensions);
		break;

	case WM_MOVE:
		noFrameUpdate = true;
		Position.x = LOWORD(lParam);
		Position.y = HIWORD(lParam);
		break;

	case WM_KILLFOCUS:
		Keyboard::clearKeyStates();
		Mouse::resetWheel();
		Mouse::clearBuffer();
		break;

	case WM_MOUSEWHEEL:
		Mouse::increaseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		Mouse::pushEvent(Mouse::event::Type::Wheel, Mouse::None, Mouse::Position);
		break;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//	Client functions

void Window::setTitle(std::string name)
{
	Name = name;
	if (!SetWindowTextA(hWnd, name.c_str()))
		throw CHWND_LAST_EXCEPT();
}

void Window::setDimensions(int width, int height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
		throw CHWND_LAST_EXCEPT();

	if (!SetWindowPos(hWnd, hWnd, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOZORDER))
		throw CHWND_LAST_EXCEPT();
}

void Window::setDimensions(Vector2i Dim)
{
	setDimensions(Dim.x, Dim.y);
}

void Window::setPosition(int X, int Y)
{
	int eX = -8;
	int eY = -31;
	X += eX;
	Y += eY;

	if (!SetWindowPos(hWnd, hWnd, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER))
		throw CHWND_LAST_EXCEPT();
}

void Window::setPosition(Vector2i Pos)
{
	setPosition(Pos.x, Pos.y);
}

void Window::setFramerateLimit(int fps)
{
	Frametime = 1.f / float(fps);
	timer.setMax(fps);
}

std::string Window::getTitle()
{
	return Name;
}

Vector2i Window::getDimensions()
{
	return Dimensions;
}

Vector2i Window::getPosition()
{
	return Position;
}

float Window::getFramerate()
{
	return 1.f / timer.average();
}

float Window::getFrameTime()
{
	return frame;
}

bool Window::popMessage(MSG& clientMsg)
{
	if (!msgQueue.size())
		return false;
	clientMsg = msgQueue.front();
	msgQueue.pop();
	return true;
}

bool Window::processEvents()
{
	//	Message Pump

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT)
			return false;

		pushMessage();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	handleFramerate();
	return true;
}

void Window::close()
{
	PostQuitMessage(0);
}

#pragma once

#include "Header.h"
#include "resource.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window
{
public:
	class Exception : public ExceptionClass
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

private:
	class WindowClass
	{
	public:
		static const LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCWSTR wndClassName = L"Learn DirectX Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	Vector2i Dimensions;
	Vector2i Position;
	std::string Name;
	HWND hWnd;
	MSG msg;
	BOOL gResult;

public:
	Window(int width, int height, LPCWSTR name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	//	Client Functions

	void setTitle(std::string name);
	void setDimensions(int width, int height);
	void setDimensions(Vector2i Dim);
	void setPosition(int X, int Y);
	void setPosition(Vector2i Pos);

	std::string getTitle();
	Vector2i getDimensions();
	Vector2i getPosition();

	bool pollEvent();
};

// error exception helper macros

#define CHWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )
#pragma once

#include "Header.h"
#include "Exception.h"
#include <d3d11.h>
#include <wrl.h>

#define pCom Microsoft::WRL::ComPtr

class Graphics
{
public:
	class HrException : public ExceptionClass
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
	private:
		HRESULT hr;
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};

public:
	Graphics() {}
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void create(HWND hWnd);

	void pushFrame();
	void clearBuffer(float R = 1.f, float G = 1.f, float B = 1.f, float A = 1.f) noexcept;

	void setWindowDimensions(Vector2i& Dim);
	void drawTestTriangle();

private:
	pCom<ID3D11Device>				pDevice;
	pCom<IDXGISwapChain>			pSwap;
	pCom<ID3D11DeviceContext>		pContext;
	pCom<ID3D11RenderTargetView>	pTarget;

	Vector2i WindowDimensions;
};

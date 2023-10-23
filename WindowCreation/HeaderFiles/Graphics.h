#pragma once

#include "Header.h"
#include "Exception.h"
#include <d3d11.h>
#include <wrl.h>
#include "DxgiInfoManager.h"

#define pCom Microsoft::WRL::ComPtr

class Graphics
{
	friend class Bindable;
public:
	class HrException : public ExceptionClass
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public ExceptionClass
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};

public:
	Graphics() {}
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void create(HWND hWnd);
	bool isInitialized();

	void pushFrame();
	void clearBuffer(float R = 1.f, float G = 1.f, float B = 1.f, float A = 1.f);
	void clearBuffer(Color color);

	Vector2f PixeltoR2(Vector2i MousePos);
	void setWindowDimensions(Vector2i& Dim);
	void initTestTriangle();
	void bindTestTrinagle();
	void drawTestTriangle(Vector2i MousePos, float scale, Vector3f obs);
	void drawIndexed(UINT IndexCount);

private:
	struct Vertex {
		Vector3f vector;
		Color color;

		static const UINT stride = sizeof(Vector3f) + sizeof(Color);
		static const UINT offset = 0u;
	};

	struct {

		struct VSconstBuffer {
			_float4vector translation;
			_float4matrix rotation;
			_float4matrix projection;
		}vscBuff;

		struct PSconstBuffer {
			_float4vector norm4[24];
		}pscBuff;

		pCom<ID3D11Buffer>			pVertexBuffer;
		pCom<ID3D11Buffer>			pIndexBuffer;
		pCom<ID3D11PixelShader>		pPixelShader;
		pCom<ID3D11VertexShader>	pVertexShader;
		pCom<ID3D11InputLayout>		pInputLayout;
		pCom<ID3D11Buffer>			pVSconstBuffer;
		pCom<ID3D11Buffer>			pPSconstBuffer;
		unsigned int				NumIndexes;

		std::vector<Vertex> vertexs;
		std::vector<unsigned short> Indexs;
		std::vector<Vector3f> norms;
	}Triangle;

private:
	HWND HWnd;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	pCom<ID3D11Device>				pDevice;
	pCom<IDXGISwapChain>			pSwap;
	pCom<ID3D11DeviceContext>		pContext;
	pCom<ID3D11RenderTargetView>	pTarget;
	pCom<ID3D11DepthStencilView>	pDSV;

	Vector2i WindowDim;
};

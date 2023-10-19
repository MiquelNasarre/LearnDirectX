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
	void clearBuffer(Color color) noexcept;

	Vector2f PixeltoR2(Vector2i MousePos);
	void setWindowDimensions(Vector2i& Dim);
	void initSettings();
	void initTestTriangle();
	void drawTestTriangle(float angle, Vector2i MousePos);

private:
	struct Vertex {
		Vector3f vector;
		Color color;
	};

	struct indexedStructure {
		pCom<ID3D11Buffer>			pVertexBuffer;
		pCom<ID3D11Buffer>			pIndexBuffer;
		pCom<ID3D11PixelShader>		pPixelShader;
		pCom<ID3D11VertexShader>	pVertexShader;
		pCom<ID3D11InputLayout>		pInputLayout;
		pCom<ID3D11Buffer>			pPSConstantBuffer;
		unsigned int				NumIndexes;

		std::vector<Vertex> vertexs;
		std::vector<unsigned short> Indexs;
		std::vector<Vector3f> norms;
	};

	indexedStructure TestTriangle;


private:
	pCom<ID3D11Device>				pDevice;
	pCom<IDXGISwapChain>			pSwap;
	pCom<ID3D11DeviceContext>		pContext;
	pCom<ID3D11RenderTargetView>	pTarget;

	Vector2i WindowDimensions;
};

#include "Exception/DxgiInfoManager.h"
#include "Window.h"
#include "Graphics.h"
#include <dxgidebug.h>

#include "Exception/ExceptionMacros.h"

DxgiInfoManager::DxgiInfoManager()
{
	// create the COM pointer

	pDxgiInfoQueue = (void*)Microsoft::WRL::ComPtr<IDXGIInfoQueue>().Detach();

	// define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (!hModDxgiDebug)
		throw CHWND_LAST_EXCEPT();

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (!DxgiGetDebugInterface)
		throw CHWND_LAST_EXCEPT();

	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue));
}

DxgiInfoManager::~DxgiInfoManager()
{
	((IDXGIInfoQueue*)pDxgiInfoQueue)->Release();
}

void DxgiInfoManager::Set() noexcept
{
	// set the index (next) so that the next all to GetMessages()
	// will only get errors generated after this call
	next = ((IDXGIInfoQueue*)pDxgiInfoQueue)->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

const char** DxgiInfoManager::GetMessages() const
{
	
	const auto end = ((IDXGIInfoQueue*)pDxgiInfoQueue)->GetNumStoredMessages(DXGI_DEBUG_ALL);
	const char** messages = (const char**)calloc(end+1,sizeof(void*));
	for (unsigned long long i = next; i < end; i++)
	{
		SIZE_T messageLength;
		// get the size of message i in bytes
		GFX_THROW_NOINFO(((IDXGIInfoQueue*)pDxgiInfoQueue)->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description into the vector
		GFX_THROW_NOINFO(((IDXGIInfoQueue*)pDxgiInfoQueue)->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages[i] = std::string(pMessage->pDescription).c_str();
	}
	return messages;
}

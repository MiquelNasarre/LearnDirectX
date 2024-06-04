#pragma once

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager( const DxgiInfoManager& ) = delete;
	DxgiInfoManager& operator=( const DxgiInfoManager& ) = delete;
	void Set() noexcept;
	const char** GetMessages() const;
private:
	unsigned long long next = 0u;
	void* pDxgiInfoQueue;
};
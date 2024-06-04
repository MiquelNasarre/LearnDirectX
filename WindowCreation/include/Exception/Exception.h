#pragma once

class ExceptionClass
{
public:
	ExceptionClass( int line,const char* file ) noexcept;
	virtual const char* what() const noexcept;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const char* GetFile() const noexcept;
	const char* GetOriginString() const noexcept;
private:
	int line;
	const char* file;
protected:
	mutable char* whatBuffer;
};

#include "Exception/Exception.h"
#include <sstream>

ExceptionClass::ExceptionClass( int line,const char* file ) noexcept
	:line( line ),
	file( file )
{

}

const char* ExceptionClass::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = (char*)oss.str().c_str();
	return whatBuffer;
}

const char* ExceptionClass::GetType() const noexcept
{
	return "Chili Exception";
}

int ExceptionClass::GetLine() const noexcept
{
	return line;
}

const char* ExceptionClass::GetFile() const noexcept
{
	return file;
}

const char* ExceptionClass::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str().c_str();
}
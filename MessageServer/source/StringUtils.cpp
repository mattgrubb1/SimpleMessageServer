#include "SimpleServerPCH.h"

std::string StringUtils::GetCommandLineArg(int inIndex)
{
	if (inIndex < __argc)
	{
		return std::string(__argv[inIndex]);
	}

	return std::string();
}

std::string StringUtils::Sprintf(const char* inFormat, ...)
{
	// not thread safe
	static char temp[4096];

	va_list args;
	va_start(args, inFormat);

	_vsnprintf_s(temp, 4096, 4096, inFormat, args);

	return std::string(temp);
}

void StringUtils::Log(const char* inFormat, ...)
{
	// not thread safe
	static char temp[4096];

	va_list args;
	va_start(args, inFormat);

	_vsnprintf_s(temp, 4096, 4096, inFormat, args);

}
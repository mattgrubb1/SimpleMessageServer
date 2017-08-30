#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
class SocketAddressFactory
{
public:
	static SocketAddressPtr CreateIPv4FromString(const std::string& inString);
};
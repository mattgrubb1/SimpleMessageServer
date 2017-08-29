#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>
#include <memory>

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddress
{
public:
	// Constructor for IPv4
	SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	// Constructor for a network API that returns a sockaddr and you
	// want to wrap it in the SocketAddress class
	SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
};
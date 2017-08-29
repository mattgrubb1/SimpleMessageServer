#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>
#include <memory>
#include <string>
#include "SocketAddress.h"

class SocketAddressFactory
{
public:
	// CreateIPv4FromString makes sure that a SocketAddress is never incorrectly
	// initialized. If it returns a non-null pointer than its guaranteed to be
	// a valid SocketAddress.
	static SocketAddressPtr CreateIPv4FromString(const std::string& inString)
	{
		// Find the position of the last ':' character
		auto pos = inString.find_last_of(':');
		std::string host;
		std::string service;

		if (pos != std::string::npos) // std::string::npos is the end position of a string
		{
			host = inString.substr(0, pos); // IP
			service = inString.substr(pos + 1); // Port
		}
		else
		{
			host = inString;
			// use default port
			service = "0";
		}
		
		addrinfo hint;					// hint is used as a blueprint for determining what type of info to get
		memset(&hint, 0, sizeof(hint));	// zero out all members of hint to avoid errors when some platforms don't use the same fields
		hint.ai_family = AF_INET;		// set to AF_INET because we only want IPv4 results
		addrinfo* result;				// a link list of all the results from getaddrinfo

		// getaddrinfo returns 0 if successful
		int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);

		if (error != 0 && result != nullptr)
		{
			// We ran into an error so free up the result memory and return nullptr
			freeaddrinfo(result);
			return nullptr;
		}

		// iterate through result until we find the addrinfo we are looking for
		while (!result->ai_addr && result->ai_next)
		{
			result = result->ai_next;
		}

		// if we didn't find anything in the result linked list then free used memory and return nullptr
		if (!result->ai_addr)
		{
			freeaddrinfo(result);
			return nullptr;
		}

		// Success! Copy the ai_addr from result using make_shared which returns a shared_ptr
		auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);
		// Finally, free up memory used by result
		freeaddrinfo(result);

		return toRet;
	}
};
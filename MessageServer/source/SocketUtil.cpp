#include "SimpleServerPCH.h"

bool SocketUtil::StaticInit()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR)
	{
		ReportError("Starting Up");
		return false;
	}

	return true;
}

void SocketUtil::CleanUp()
{
	WSACleanup();
}

void SocketUtil::ReportError(const char* inOperationDesc)
{
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		/*out*/ (LPTSTR)&lpMsgBuf,
		0,
		NULL);

	LOG("Error: %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);

	if (s != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket(s));
	}
	else
	{
		ReportError("SocketUtil::CreateUDPSocket");
		return nullptr;
	}
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);

	if (s != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(s));
	}
	else
	{
		ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
}

// Notes: fd_set (file descriptor set) is a data structure to store sockets as a set. Has members fd_count and fd_array
fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const std::vector<TCPSocketPtr>* inSockets, int& ioNaxNfds)
{
	if (inSockets)
	{
		// FD_ZERO initializes fd_set to have zero bits for all file descriptors
		FD_ZERO(&outSet);

		for (const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->mSocket, &outSet);
		}

		return &outSet;
	}
	else
	{
		return nullptr;
	}
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets, const std::vector<TCPSocketPtr>* inSockets, const fd_set& inSet)
{
	if (inSockets && outSockets)
	{
		outSockets->clear();

		for (const TCPSocketPtr& socket : *inSockets)
		{
			if (FD_ISSET(socket->mSocket, &inSet))
			{
				outSockets->push_back(socket);
			}
		}
	}
}

int SocketUtil::Select(const std::vector<TCPSocketPtr>* inReadSet,
	std::vector<TCPSocketPtr>* outReadSet,
	const std::vector<TCPSocketPtr>* inWriteSet,
	std::vector<TCPSocketPtr>* outWriteSet,
	const std::vector<TCPSocketPtr>* inExceptSet,
	std::vector<TCPSocketPtr>* outExceptSet)
{
	// build sets from vectors
	fd_set read;
	fd_set write;
	fd_set except;

	int nfds = 0;

	fd_set* readPtr = FillSetFromVector(read, inReadSet, nfds);
	fd_set* writePtr = FillSetFromVector(write, inWriteSet, nfds);
	fd_set* exceptPtr = FillSetFromVector(except, inExceptSet, nfds);

	int toRet = select(nfds + 1, readPtr, writePtr, exceptPtr, nullptr);

	if (toRet > 0)
	{
		FillVectorFromSet(outReadSet, inReadSet, read);
		FillVectorFromSet(outWriteSet, inWriteSet, write);
		FillVectorFromSet(outExceptSet, inExceptSet, except);
	}

	return toRet;
}
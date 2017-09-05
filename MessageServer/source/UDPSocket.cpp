#include "SimpleServerPCH.h"

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
	int error = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());

	if (error != 0)
	{
		SocketUtil::ReportError("UDPSocket::Bind");
		return SocketUtil::GetLastError();
	}

	return NO_ERROR;
}

int UDPSocket::SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress)
{
	int byteSentCount = sendto(mSocket,
		static_cast<const char*>(inToSend),
		inLength,
		0,
		&inToAddress.mSockAddr,
		inToAddress.GetSize());

	if (byteSentCount <= 0)
	{
		// return error as a negative number to indicate less than requested amount of bytes sent.
		SocketUtil::ReportError("UDPSocket::SendTo");
		return -SocketUtil::GetLastError();
	}
	else
	{
		return byteSentCount;
	}
}

int UDPSocket::ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress)
{
	socklen_t fromLength = outFromAddress.GetSize();

	int readByteCount = recvfrom(mSocket,
		static_cast<char*>(inToReceive),
		inMaxLength,
		0,
		&outFromAddress.mSockAddr,
		&fromLength);

	if (readByteCount >= 0)
	{
		return readByteCount;
	}
	else
	{
		int error = SocketUtil::GetLastError();

		if (error == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else if (error == WSAECONNRESET)
		{
			// This can happen if a client closed and we haven't disconnected yet.
			// This is the ICMP message being sent back saying the port on that computer is closed.
			LOG("Connection reset from %s", outFromAddress.ToString().c_str());
			return -WSAECONNRESET;
		}
		else
		{
			SocketUtil::ReportError("UDPSocket::ReceiveFrom");
			return -error;
		}
	}
}

UDPSocket::~UDPSocket()
{
	closesocket(mSocket);
}
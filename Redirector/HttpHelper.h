#pragma once
#ifndef HTTPHELPER_H
#define HTTPHELPER_H
#include "Based.h"

namespace HttpHelper
{
	SOCKET Connect();

	typedef class TCP
	{
	public:
		~TCP();

		bool Connect(PSOCKADDR_IN6 target);

		int Send(const char* buffer, int length);
		int Read(char* buffer, int length);

		SOCKET tcpSocket = INVALID_SOCKET;
	} *PTCP;
};

#endif

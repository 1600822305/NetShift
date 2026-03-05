#include "HttpHelper.h"

#include "Utils.h"

extern wstring tgtHost;
extern wstring tgtPort;
extern string tgtUsername;
extern string tgtPassword;

SOCKET HttpHelper::Connect()
{
	auto client = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET)
	{
		printf("[Redirector][HttpHelper::Connect] Create socket failed: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	{
		int v6only = 0;
		if (setsockopt(client, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&v6only, sizeof(v6only)) == SOCKET_ERROR)
		{
			printf("[Redirector][HttpHelper::Connect] Set socket option failed: %d\n", WSAGetLastError());

			closesocket(client);
			return INVALID_SOCKET;
		}
	}

	timeval timeout{};
	timeout.tv_sec = 4;

	if (!WSAConnectByNameW(client, (LPWSTR)tgtHost.c_str(), (LPWSTR)tgtPort.c_str(), NULL, NULL, NULL, NULL, &timeout, NULL))
	{
		printf("[Redirector][HttpHelper::Connect] Connect to remote server failed: %d\n", WSAGetLastError());

		closesocket(client);
		return INVALID_SOCKET;
	}

	{
		DWORD returned = 0;

		tcp_keepalive data = { 1, 120000, 10000 };
		WSAIoctl(client, SIO_KEEPALIVE_VALS, &data, sizeof(data), NULL, 0, &returned, NULL, NULL);
	}

	return client;
}

HttpHelper::TCP::~TCP()
{
	if (this->tcpSocket != INVALID_SOCKET)
	{
		closesocket(this->tcpSocket);

		this->tcpSocket = INVALID_SOCKET;
	}
}

bool HttpHelper::TCP::Connect(PSOCKADDR_IN6 target)
{
	this->tcpSocket = HttpHelper::Connect();
	if (this->tcpSocket == INVALID_SOCKET)
		return false;

	/* Build target address string for CONNECT request */
	char addrStr[128];
	char portStr[8];

	if (target->sin6_family == AF_INET)
	{
		auto ipv4 = (PSOCKADDR_IN)target;
		inet_ntop(AF_INET, &ipv4->sin_addr, addrStr, sizeof(addrStr));
		sprintf_s(portStr, "%d", ntohs(ipv4->sin_port));
	}
	else
	{
		inet_ntop(AF_INET6, &target->sin6_addr, addrStr, sizeof(addrStr));
		sprintf_s(portStr, "%d", ntohs(target->sin6_port));
	}

	/* Build HTTP CONNECT request */
	char request[512];
	int reqLen;

	if (!tgtUsername.empty() && !tgtPassword.empty())
	{
		/* Build Basic auth: base64(username:password) */
		string credentials = tgtUsername + ":" + tgtPassword;

		/* Simple base64 encode */
		static const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		string encoded;
		int val = 0, valb = -6;
		for (unsigned char c : credentials)
		{
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0)
			{
				encoded.push_back(b64chars[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) encoded.push_back(b64chars[((val << 8) >> (valb + 8)) & 0x3F]);
		while (encoded.size() % 4) encoded.push_back('=');

		reqLen = sprintf_s(request, sizeof(request),
			"CONNECT %s:%s HTTP/1.1\r\n"
			"Host: %s:%s\r\n"
			"Proxy-Authorization: Basic %s\r\n"
			"Proxy-Connection: Keep-Alive\r\n"
			"\r\n",
			addrStr, portStr, addrStr, portStr, encoded.c_str());
	}
	else
	{
		reqLen = sprintf_s(request, sizeof(request),
			"CONNECT %s:%s HTTP/1.1\r\n"
			"Host: %s:%s\r\n"
			"Proxy-Connection: Keep-Alive\r\n"
			"\r\n",
			addrStr, portStr, addrStr, portStr);
	}

	if (reqLen <= 0)
	{
		printf("[Redirector][HttpHelper::TCP::Connect] Build CONNECT request failed\n");
		return false;
	}

	if (send(this->tcpSocket, request, reqLen, 0) != reqLen)
	{
		printf("[Redirector][HttpHelper::TCP::Connect] Send CONNECT request failed: %d\n", WSAGetLastError());
		return false;
	}

	/* Read response - look for "HTTP/1.x 200" */
	char response[1024];
	int totalRead = 0;
	bool headerEnd = false;

	while (totalRead < (int)sizeof(response) - 1 && !headerEnd)
	{
		int n = recv(this->tcpSocket, response + totalRead, 1, 0);
		if (n <= 0)
		{
			printf("[Redirector][HttpHelper::TCP::Connect] Receive CONNECT response failed: %d\n", WSAGetLastError());
			return false;
		}
		totalRead += n;

		/* Check for \r\n\r\n (end of HTTP headers) */
		if (totalRead >= 4 &&
			response[totalRead - 4] == '\r' &&
			response[totalRead - 3] == '\n' &&
			response[totalRead - 2] == '\r' &&
			response[totalRead - 1] == '\n')
		{
			headerEnd = true;
		}
	}

	response[totalRead] = '\0';

	/* Check for 200 status code */
	if (strstr(response, " 200 ") == NULL)
	{
		printf("[Redirector][HttpHelper::TCP::Connect] CONNECT failed: %s\n", response);
		return false;
	}

	/* Tunnel established, socket is now transparent */
	return true;
}

int HttpHelper::TCP::Send(const char* buffer, int length)
{
	if (this->tcpSocket != INVALID_SOCKET)
		return send(this->tcpSocket, buffer, length, 0);

	return SOCKET_ERROR;
}

int HttpHelper::TCP::Read(char* buffer, int length)
{
	if (this->tcpSocket != INVALID_SOCKET)
		return recv(this->tcpSocket, buffer, length, 0);

	return SOCKET_ERROR;
}

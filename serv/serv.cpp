#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>

struct Data {
	char data[10][10];
	int palyer;
	void createfield() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				std::cout << data[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}
};

/// <summary>
/// Server
/// </summary>
/// <returns></returns>
int main() {
	WSADATA d;
	WORD version = MAKEWORD(2, 2);

	int exitCode = WSAStartup(version, &d);

	if (exitCode != 0) {
		return GetLastError();
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		std::cerr << "Could not create socket!\n";
		return WSAGetLastError();
	}

	hostent* host = gethostbyname("");
	char* ip = inet_ntoa(*(in_addr*)(*host->h_addr_list));

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(5150);

	std::cout << "Server address: " << ip << ":" << 5150 << std::endl;

	bind(sock, (sockaddr*)&address, sizeof(address));

	/*if (result1 != 0) {
		std::cerr << "Could not bind!" << std::endl;
		closesocket(sock, 2);
		return WSAGetLastError();
	}*/

	listen(sock, 1);

	/*if (result2 != 0) {
		std::cerr << "Could not listen!" << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}*/

	SOCKET client1 = accept(sock, nullptr, nullptr);

	std::cout << "Client 1 connected\n";
	int player1 = 1;
	send(client1, (char*)&player1, sizeof(int), 0);

	SOCKET client2 = accept(sock, nullptr, nullptr);

	std::cout << "Client 2 connected\n";
	int player2 = 2;
	send(client2, (char*)&player1, sizeof(int), 0);

	{
		Data data{};
		recv(client1, (char*)&data, sizeof(Data), 0);
		//data.createfield();
	}

	closesocket(sock);

	WSACleanup();

	return 0;
}
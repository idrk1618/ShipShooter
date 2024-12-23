#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <conio.h>

struct Data {
	char data[10][10];
	int player;
	// ������� ��������� ���� � �������
	void seefield() {
		std::cout << "  ";
		// ����� ���� ������
		for (int i = 0; i < 10; i++) {
			std::cout << (i) << ' ';
		}
		std::cout << '\n';
		for (int i = 0; i < 10;  i++) // ������
		{
			for (int j = 0; j < 10; j++) // �������
			{
				if (j == 0){
					std::cout << char('A' + i) << ' '; // ����� ���� �����
				}
				// �������� �������� ���������� � �������
				if (data[i][j] == 0)
				{
					switch (abs(j - i) % 2) // ����
					{
					case 0:
						std::cout << "\033[46m  \033[40m";
						break;
					default:
						std::cout << "\033[104m  \033[40m";
					}
				}
				else{
					std::cout << "\033[41m  \033[40m"; // �������
				}
			}
			std::cout << '\n';
		}
		std::cout << std::endl;
	}

	// ������� ����������� ��������� �� ������������ ��������� ������� 
	bool fieldcheck() {
		int count = 0;
		int ship = true;
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				// �������� �������� ���������� � �������
				if (data[i][j] == 1)
				{
					if (coordcheck(i, j) == 0 || ship == 0) {
						ship = 0;
					}
				}
			}
		}
		// �������� �� ������� 
		if (count == 4 && ship) {
			std::cerr << "\033[102mCORRECT!\033[40m";
			Sleep(1500);
			return true;
		}
		else {
			if (count != 4) {
				std::cerr << "\033[41mYOU NEED " << 4 - count << " MORE SHIPS!\033[40m\n";
			}
			if (ship == 0) {
				std::cerr << "\033[41mYOU PLACED YOUR SHIPS WRONG!\033[40m";
			}
			Sleep(1500);
			return false;
		}
	}

	// ������� ����������� ����������� ��������� ������� ����� �����
	int coordcheck(int i, int j) {
		// �������� �� -1 �� 1 �������� � �������
		for (int i2 = -1; i2 <= 1; i2++)
		{
			// �������� �� -1 �� 1 �������� � �������
			for (int j2 = -1; j2 <= 1; j2++)
			{
				// �������� �� ��, ��� �� ������� �� �������
				char ship[8] = { -1 };
				int i3 = i - i2, j3 = j - j2;
				if ((i3 >= 0 && i3 <= 9) &&
					(j3 >= 0 && j3 <= 9) &&
					(i != i3 || j != j3))
				{
					 is_one(i, i3, j, j3);
				}
			}
		}
	}

	int is_one(int i, int i3, int j, int j3) {
		// �������� �� ��, ��� ��� �������� ������
		if ((int)data[i3][j3] == 1)
		{
			is_two(i, i3, j, j3);
		}
		else {
			return 1;
		}
	}

	int is_two(int i, int i3, int j, int j3) {
		// �������� �� ��, ��� ������� �� ��� ���������
		if (!(i != i3 && j != j3)) {
			if ((int)data[i3][j3] == 1)
			{
				return 2;
			}
		}
		return 0;
	}

	// ������� ����������� ��������� ���� ��� ��������� ��������
	void createfield() {
		setlocale(0, "");
		char coord[3];
		while (true) {
			system("cls");
			std::cerr << "    ('~' to escape)\n";
			seefield();
			std::cerr << "ENTER SHIP COORDINATES: ";
			std::cin >> coord;
			int fir_cord = (int)coord[0] - (int)'A'; // ������ ����������
			int sec_cord = (int)coord[1] - (int)'0'; // ������ ����������
			// �������� ����� �� ������������ �����
			if (coord[0] == '~')
			{ 
				if (fieldcheck() == 0) {
					createfield();
				}\
				else {
					break;
				}
			}
			// �������� �� ����������� �������� ������
			else if ((fir_cord >= 0 && fir_cord <= 9) &&
					(sec_cord >= 0 && sec_cord <= 9))
			{
				// �������� ������� �������� �� ���������������
				data[fir_cord][sec_cord] = (data[fir_cord][sec_cord] == 0 ? 1 : 0);
			}
			else{
				std::cerr << "\033[41m!!WRONG SYMBOL ERROR!!\033[40m";
				Sleep(1500);
			}
		}
	}
};

/// <summary>
/// Client
/// </summary>
/// <returns></returns>
int main() {
	
	system("cls");

	Data data{};
	data.data[0][0] = 0;

	data.createfield();
	
	/*WSADATA d;
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

	char ip[16];
	std::cout << "Enter server IP address: ";
	std::cin >> ip;
	short port;
	std::cout << "Enter server port: ";
	std::cin >> port;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(port);

	connect(sock, (sockaddr*)&address, sizeof(address));

	int playerId;
	recv(sock, (char*)&playerId, sizeof(int), 0);
	std::cout << "This is player " << playerId << std::endl;

	{
		system("cls");

		Data data{};
		data.data[0][0] = 0;
		data.player = playerId;
		send(sock, (char*)&data, sizeof(Data), 0);

		data.createfield();
		int keyCode = _getch();
	}

	closesocket(sock);

	WSACleanup();*/

	return 0;
}
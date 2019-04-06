#include <conio.h>
#include <iostream>
#include <io.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main()
{
	int rc, size, symbols, y = 0, i = 1; 
	WSADATA wsd;
	WSAStartup(MAKEWORD(2,2), &wsd);
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(27015);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	FILE *f;
	f = fopen("../../../send.txt", "rb");

	while (!feof(f))
	{
		char bufer[100];
		symbols = fread(bufer, 1, 100, f);
		size = ftell(f);
		printf("read symbols: %d, part: %d, pos: %ld \n", symbols, i, size);
		printf("\n");
		printf("%s", bufer);
		int iResult = sendto(s, bufer, symbols, 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
		if (iResult == SOCKET_ERROR) {
			wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return 1;
		}
		printf("%d OK", iResult);
		i++;
	}
	char buf[4] = "EOF";
	sendto(s, buf, sizeof(buf), 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
	fclose(f);
	_getch();
	closesocket(s);
	WSACleanup();
	return 0;
}
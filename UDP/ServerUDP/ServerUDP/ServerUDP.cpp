#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main()
{
	int iResult = 0;

	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed with error %d\n", iResult);
		return 1;
	}

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		printf("socket failed with error %d\n", WSAGetLastError());
		return 1;
	}
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddr_size = sizeof(clientaddr);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(27015);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	iResult = bind(s, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (iResult != 0) {
		printf("bind failed with error %d\n", WSAGetLastError());
		return 1;
	}

	FILE *f;
	f = fopen("../../../recieve.txt", "ab");
	if (f == NULL) {
		puts("Cannot open file.");
		return 1;
	}

	int part = 1;
	puts("Receiving datagrams...");
	while (true)
	{
		char buf[100];
		iResult = recvfrom(s, buf, sizeof(buf), 0, (sockaddr *)&clientaddr, &clientaddr_size);
		if (iResult == SOCKET_ERROR) {
			printf("recvfrom failed with error %d\n", WSAGetLastError());
		}
		else
		{
			if (strcmp(buf,"EOF")) {
				printf( "Received %d bytes.\n", iResult);
				part++;
				fwrite(buf, 1, iResult, f);
			}
			else {
				puts("File received.\nExiting.");
				break;
			}
		}
	}
	fclose(f);
	iResult = closesocket(s);
	if (iResult == SOCKET_ERROR) {
		printf("closesocket failed with error %d\n", WSAGetLastError());
		return 1;
	}
	WSACleanup();
	return 0;
}
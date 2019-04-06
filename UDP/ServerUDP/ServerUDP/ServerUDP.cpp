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
	int rc, i = 1;
	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);
	FILE *f;

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in serveraddr, clientaddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(&clientaddr, 0, sizeof(clientaddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(27015);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	rc = bind(s, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rc < 0) printf("error");
	listen(s, 5);
	int clientsize = sizeof(clientaddr);
	f = fopen("../../../recieve.txt", "ab");
	while (true)
	{
		puts("Waiting");
		char buf[100];
		int r = recvfrom(s, buf, sizeof(buf), 0, (sockaddr *)&clientaddr, &clientsize);
		if (r > 0)
		{
			if (strcmp(buf,"EOF")) {
				cout << "receive bytes: " << r << ", part: " << i << endl;
				printf("%s\n", buf);
				i++;
			}
			else {
				break;
			}
		}
	}
	fclose(f);
	_getch();
	closesocket(s);
	WSACleanup();
	return 0;
}
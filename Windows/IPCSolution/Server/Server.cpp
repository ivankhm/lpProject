#include "stdafx.h"

#include <Windows.h>
#include <winsock.h>
#include <process.h>
#include <iostream>

using std::cout;

#pragma comment(lib, "ws2_32.lib")

#define SHUT_RDWR 0x02
#define SIZE 20
#define PORT 12345
#define IP_ADDR "127.0.0.1"
#define MAX_CONN 5

int sz;
int SIDD = 0;
struct sockaddr_in addr, cli_addr;

void InitAddr(struct sockaddr_in &addr, int &size){
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(IP_ADDR);
	size = sizeof(addr);
}

UINT WINAPI Client(LPVOID){
	int N_SID, n;
	char str[SIZE];

	if ((N_SID = accept(SIDD, (struct sockaddr*)&cli_addr, &sz)) == INVALID_SOCKET)
		cout << "Error of nth socket!\n";
	if ((n = recv(N_SID, str, SIZE, 0)) != SOCKET_ERROR){
		cout << "IN: " << str << "\n";
		sprintf(str, "%d", strlen(str));
		cout << "OUT: " << str << "\n";
		send(N_SID, str, strlen(str), 0);
	}
	else
		cout << "Disconnect..\n";

	shutdown(N_SID, SHUT_RDWR);
	closesocket(N_SID);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Server\n";

	int N_SID, BIND;

	WSADATA wsad;
	WSAStartup(MAKEWORD(2, 1), &wsad);
	struct sockaddr_in addr;
	InitAddr(addr, sz);

	if ((SIDD = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		cout << "Error of socket!\n";
	if ((BIND = bind(SIDD, (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
		cout << "Error of bind!\n";
	listen(SIDD, MAX_CONN);
	while (TRUE)
		_beginthreadex(NULL, NULL, Client, NULL, NULL, NULL);

	shutdown(SIDD, SHUT_RDWR);
	closesocket(SIDD);
	WSACleanup();
	return 0;
}


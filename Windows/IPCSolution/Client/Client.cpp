#include "stdafx.h"

#include <Windows.h>
#include <winsock.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib");

using std::cout;
using std::cin;

#define SHUT_RDWR 0x02
#define SIZE 512
#define PORT 12345
#define IP_ADDR "127.0.0.1"

void InitAddr(struct sockaddr_in &addr){
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(IP_ADDR);
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Client\n";

	int sid, n;
	char buf[SIZE];
	WSADATA wsad;
	WSAStartup(MAKEWORD(2, 1), &wsad);
	struct sockaddr_in addr;
	InitAddr(addr);

	while (TRUE){
		if ((sid = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
			cout << "Error of socket!\n";
			break;
		}
		if (connect(sid, (struct sockaddr*)&addr, sizeof(addr))){
			cout << "Waiting..\n";
			shutdown(sid, SHUT_RDWR);
			closesocket(sid);
			continue;
		}
		cout << "Ok!\n";
		cout << "OUT: ";
		cin >> buf;
		if (strcmp(buf, "exit")){
			send(sid, buf, strlen(buf) + 1, 0);
			memset(buf, 0, sizeof(char) * SIZE);
			if (recv(sid, _strrev(buf), sizeof(buf), 0) != SOCKET_ERROR){
				cout << "IN: " << buf << "\n";
				memset(buf, 0, sizeof(char) * SIZE);
			}
			else
				send(sid, "Message didn't send\n", 30, 0);
			shutdown(sid, SHUT_RDWR);
			closesocket(sid);
		}
		else{
			shutdown(sid, SHUT_RDWR);
			closesocket(sid);
			break;
		}
	}

	WSACleanup();
	return 0;
}


#include <iostream>
#include <Windows.h>
#include <tchar.h>

#define BUFSIZE 512

using namespace std;

int main(int argc, TCHAR *argv[]) {
	HANDLE hPipe;
	LPCWSTR lpMessage = _T("Hello from Client");
	TCHAR buf[BUFSIZE];
	LPCWSTR szPipeName = _T("\\\\.\\pipe\\name");
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	if (argc > 1)
		lpMessage = argv[1];

	while (true) {
		if ((hPipe = CreateFile(szPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)) != INVALID_HANDLE_VALUE)
			break;
		if (GetLastError() != ERROR_PIPE_BUSY) {
			cout << "Error: " << GetLastError()<<endl;
			return -1;
		}
		cout << "Waiting for pipe... \n";
		if (!WaitNamedPipe(szPipeName, 20000)) {
			cout << "Cant open pipe: 20 sec timeout is over.\n";
			return -1;
		}
	}
	cout << "Conected to the server.\n";
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
	WriteFile(hPipe, lpMessage, (lstrlen(lpMessage) + 1) * sizeof(TCHAR), NULL, NULL);
	cout << "Message Send.\n";

	ReadFile(hPipe, buf, BUFSIZE * sizeof(TCHAR), NULL, NULL);
	wcout << "Server replied: " << buf << endl << "Press any key to terminate connection..."<<endl;
	getchar();
	CloseHandle(hPipe);
	return 0;
}
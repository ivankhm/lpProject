#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <process.h>

#define BUFSIZE 512

using namespace std;
DWORD WINAPI InstanceThread(LPVOID);

int main() {
	bool isConnected = false;
	DWORD iThreadID = 0;
	HANDLE hPipe, hThread;
	LPCWSTR lpszPipeName = _T("\\\\.\\pipe\\name");

	while (true) {
		hPipe = CreateNamedPipe(lpszPipeName, PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES, BUFSIZE, BUFSIZE, 5000, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) {
			cout << "Failed to create a pipe with CreateNamedPipe, GLR = " << GetLastError() << endl;
			return -1;
		}

		if (ConnectNamedPipe(hPipe, NULL)) {
			cout << "Client connected, creating a processing thread.\n";

			if (!(hThread = CreateThread(NULL, 0, InstanceThread, (LPVOID)hPipe, 0, &iThreadID)))
				cout << "Failed to create processing thread with CreateThread.\n";
			else CloseHandle(hThread);
		}
		else
			CloseHandle(hPipe);
	}

	return 0;
}

DWORD WINAPI InstanceThread(LPVOID lpParam) {
	HANDLE hPipe;
	bool isSuccess = false;
	TCHAR buf[BUFSIZE] = { 0 };
	DWORD nRead;
	cout << "InstanceThread created, receiving and processing messages." << endl;

	hPipe = (HANDLE)lpParam;

	while (true) {

		isSuccess = ReadFile(hPipe, buf, BUFSIZE * sizeof(TCHAR), &nRead, NULL);

		if (!(isSuccess&&nRead) && (GetLastError() == ERROR_BROKEN_PIPE)) {
			cout << "InstanceThread: client disconected." << endl;
			break;
		}
		wcout << "Message from Client: " << buf << endl;
		lstrcpy(buf, _T("Message From Server ro Client"));
		WriteFile(hPipe, buf, (lstrlen(buf) + 1) * sizeof(TCHAR), NULL, NULL);
	}

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	cout << "InstanceThread exited." << endl;
	return 1;
}
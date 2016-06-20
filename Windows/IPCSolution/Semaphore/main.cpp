#include <iostream>
#include <Windows.h>
#include <process.h>
#include <ctime>
#include <tchar.h>

#define READERS 1
#define WRITERS 2

using namespace std;

HANDLE ghSemaphore;
HANDLE hMaPipe;
HANDLE hPipe;
LPCWSTR szPipeName = _T("\\\\.\\pipe\\pipename");

UINT WINAPI WriterProc(LPVOID);
UINT WINAPI ReaderProc(LPVOID);


int main() {
	srand(time(NULL));
	hMaPipe = CreateNamedPipe(szPipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT, PIPE_UNLIMITED_INSTANCES, 1000, 1000, 0, NULL);
	
	hPipe = CreateFile(szPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	ghSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

	_beginthreadex(0, 0, ReaderProc, NULL, 0, 0);

	for (int i(0); i < WRITERS; ++i) {
		_beginthreadex(0, 0, WriterProc, NULL, 0, 0);
	}

	getchar();
	CloseHandle(ghSemaphore);

	return 0;
}

UINT WINAPI WriterProc(LPVOID p) {
	char buf[100] = { 0 };
	
	if (hPipe == INVALID_HANDLE_VALUE)
		cout << "Writer error: " << GetLastError() << endl;
	while (true) {
		WaitForSingleObject(ghSemaphore, INFINITE);
		
		WriteFile(hPipe, itoa(rand() % 100, buf, 10), 100, NULL, NULL);
		cout << "Writer " << buf << endl;
		
		
		ReleaseSemaphore(ghSemaphore, 1, NULL);
	}
	
	return 0;
}

UINT WINAPI ReaderProc(LPVOID) {
	char buf[100] = { 0 };
	while (true) {
		WaitForSingleObject(ghSemaphore, INFINITE);

		while (ReadFile(hMaPipe, buf, 100, NULL, NULL)) {
			cout << "Reader: " << buf << endl;
		}
		
		ReleaseSemaphore(ghSemaphore, 1, NULL);
		
	}
	return 0;
}
// Event.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>
#include <Windows.h>
#include <iostream>
#include <ctime>

using std::cout;

UINT WINAPI ThreadOne(LPVOID);
UINT WINAPI ThreadTwo(LPVOID);

HANDLE hEvent;

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));
	if ((hEvent = CreateEvent(NULL, TRUE, FALSE, L"Event")) != NULL){
		_beginthreadex(0, 0, ThreadOne, NULL, 0, 0);
		_beginthreadex(0, 0, ThreadTwo, NULL, 0, 0);
		_endthreadex(0);
	}
	else
		cout << "Error\n";
	return 0;
}

UINT WINAPI ThreadOne(LPVOID)
{
	UINT Id = GetCurrentThreadId();
	while (TRUE){
		if (rand() % 5 == 4){
			cout << Id << ": send\n";
			PulseEvent(hEvent);//SetEvent(hEvent);
		}
	}
}

UINT WINAPI ThreadTwo(LPVOID)
{
	UINT Id = GetCurrentThreadId();
	while (TRUE){
		WaitForSingleObject(hEvent, INFINITE);
		cout << Id << ": received\n";
		//ResetEvent(hEvent);
	}
}


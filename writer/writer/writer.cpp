#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>

using namespace std;
const int PAGESIZE = 4096;
const int PAGE_COUNT = 19;
const string WLOG_FILE = "D:\\NIR_dir\\os\\os\\creator\\creator\\writer.txt";

int main()
{
	srand(GetCurrentProcessId());
	HANDLE WMutex[PAGE_COUNT];
	HANDLE RMutex[PAGE_COUNT];
	for (int i = 0; i < PAGE_COUNT; i++) {
		WMutex[i] = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("WMutex_" + to_string(i)).c_str());
		RMutex[i] = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("RMutex_" + to_string(i)).c_str());
	}

	HANDLE WLogMutex = OpenMutexA(MUTEX_MODIFY_STATE | SYNCHRONIZE, false, "WLogMutex");
	HANDLE Mapping = OpenFileMappingA(GENERIC_WRITE, false, "map_file");
	void* OwnMap = MapViewOfFile(Mapping, FILE_MAP_WRITE, 0, 0, PAGESIZE * PAGE_COUNT);
	VirtualLock(OwnMap, PAGESIZE * PAGE_COUNT);
	ofstream Log;
	Log.open(WLOG_FILE, fstream::out | fstream::app);

	while (1)
	{
		char msg[PAGESIZE];
		for (int i = 0; i < PAGESIZE; i++)
			msg[i] = 'a' + rand() % 19;

		DWORD PageToWrite = WaitForMultipleObjects(PAGE_COUNT, WMutex, false, INFINITE) - WAIT_OBJECT_0;
		WaitForSingleObject(WLogMutex, INFINITE);
		Log << "TICK " << (GetTickCount() % 1000000) << " PID " << GetCurrentProcessId() << " WRITE START TO PAGE " << PageToWrite << ", MSG: " << msg[0] << msg[1] << msg[2] << endl;
		ReleaseMutex(WLogMutex);
		memcpy((void*)((intptr_t)OwnMap + (PageToWrite * PAGESIZE)), msg, PAGESIZE);
		Sleep(500 + (rand() % 1000));
		WaitForSingleObject(WLogMutex, INFINITE);
		Log << "TICK " << (GetTickCount() % 1000000) << " PID " << GetCurrentProcessId() << " WRITE STOP TO PAGE " << PageToWrite << endl;
		ReleaseMutex(WLogMutex);
		ReleaseSemaphore(RMutex[PageToWrite], 1, NULL);
	}
	system("pause");
	return 0;
}

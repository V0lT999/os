#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>

using namespace std;
const int PAGESIZE = 4096;
const int PAGE_COUNT = 26;

const std::string RLOG_FILE = "D:\NIR_dir\os\os\6lab\6lab\reader.txt";
int main()
{
	srand(GetCurrentProcessId());
	HANDLE WMutex[PAGE_COUNT];
	HANDLE RMutex[PAGE_COUNT];
	for (int i = 0; i < PAGE_COUNT; i++) {
		WMutex[i] = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("WMutex_" + to_string(i)).c_str());
		RMutex[i] = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("RMutex_" + to_string(i)).c_str());

	}

	HANDLE RLogMutex = OpenMutexA(MUTEX_MODIFY_STATE | SYNCHRONIZE, false, "RLogMutex");
	ofstream Log;
	Log.open(RLOG_FILE, fstream::out | fstream::app);
	HANDLE Mapping = OpenFileMappingA(GENERIC_READ, false, "map_file");
	void* OwnMap = MapViewOfFile(Mapping, FILE_MAP_READ, 0, 0, PAGESIZE * PAGE_COUNT);
	VirtualLock(OwnMap, PAGESIZE * PAGE_COUNT);

	while (1) {
		char msg[PAGESIZE];

		DWORD PageToRead = WaitForMultipleObjects(PAGE_COUNT, RMutex, false, INFINITE) - WAIT_OBJECT_0;
		WaitForSingleObject(RLogMutex, INFINITE);
		Log << "TICK " << (GetTickCount() % 1000000) << " PID " << GetCurrentProcessId() << " READ START FROM PAGE " << PageToRead << endl;
		ReleaseMutex(RLogMutex);
		memcpy(msg, (void*)((intptr_t)OwnMap + (PageToRead * PAGESIZE)), PAGESIZE);
		Sleep(500 + (rand() % 1000));
		WaitForSingleObject(RLogMutex, INFINITE);
		Log << "TICK " << (GetTickCount() % 1000000) << " PID " << GetCurrentProcessId() << " READ STOP FROM PAGE " << PageToRead << ", MSG: " << msg[0] << msg[1] << msg[2] << endl;
		ReleaseMutex(RLogMutex);
		ReleaseSemaphore(WMutex[PageToRead], 1, NULL);

	}
	system("pause");
	return 0;
}

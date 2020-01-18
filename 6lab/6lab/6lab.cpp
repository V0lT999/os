#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>

using namespace std;
const int PAGESIZE = 4096;
const int PAGE_COUNT = 19;
const LPCSTR MAP_FILE = "D:\NIR_dir\os\os\6lab\6lab\map_file.txt";
const string WLOG_FILE = "D:\NIR_dir\os\os\6lab\6lab\writer.txt";
const string RLOG_FILE = "D:\NIR_dir\os\os\6lab\6lab\reader.txt";

const string WPATH = "D:\NIR_dir\os\os\6lab\6lab\Debug\\writer.exe"; //writer
const string RPATH = "D:\NIR_dir\os\os\6lab\6lab\Debug\\reader.exe"; //reader


const int PROCESS_COUNT = 5;
int main()
{

	HANDLE WMutex[PAGE_COUNT];
	HANDLE RMutex[PAGE_COUNT];
	for (int i = 0; i < PAGE_COUNT; i++)
	{
		WMutex[i] = CreateSemaphoreA(NULL, 1, 1, ("WMutex_" + to_string(i)).c_str());
		RMutex[i] = CreateSemaphoreA(NULL, 0, 1, ("RMutex_" + to_string(i)).c_str());
		WaitForSingleObject(RMutex[i], INFINITY);
		if (WMutex[i] == NULL || RMutex[i] == NULL) {
			cerr << "Error in Creating Mutex" << endl;
			return -1;
		}
	}
	HANDLE WLogMutex = CreateMutexA(NULL, false, "WLogMutex");
	HANDLE RLogMutex = CreateMutexA(NULL, false, "RLogMutex");

	HANDLE MapFileHandle = CreateFileA(MAP_FILE, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE MapFileMapping = CreateFileMappingA(MapFileHandle, NULL, PAGE_READWRITE, 0, PAGESIZE * PAGE_COUNT, "map_file");

	ofstream ofs;
	ofs.open(WLOG_FILE, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open(RLOG_FILE, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	cout << "Clear to execute, press to launch the connection" << endl;
	system("pause");

	PROCESS_INFORMATION piW[PROCESS_COUNT];
	PROCESS_INFORMATION piR[PROCESS_COUNT];
	BOOL t, p;
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		STARTUPINFOA siW;
		ZeroMemory(&siW, sizeof(siW));
		siW.cb = sizeof(siW);
		ZeroMemory(&(piW[i]), sizeof(piW[i]));

		STARTUPINFOA siR;
		ZeroMemory(&siR, sizeof(siR));
		siR.cb = sizeof(siR);
		ZeroMemory(&(piR[i]), sizeof(piR[i]));

		t = CreateProcessA(WPATH.c_str(), NULL, NULL, NULL, false,
			0, NULL, NULL, &siW, &(piW[i]));
		if (!t) {
			cerr << "Error in CreateProcessA(WPATH)" << GetLastError() << endl;
			return -1;
		}
		p = CreateProcessA(RPATH.c_str(), NULL, NULL, NULL, false,
			0, NULL, NULL, &siR, &(piR[i]));
		if (!p) {
			cerr << "Error in CreateProcessA(RPATH)" << endl;
			return -1;
		}
	}

	cout << "Connection established, press something to end process of reading/writting" << endl;
	system("pause");

	CloseHandle(MapFileHandle);
	CloseHandle(MapFileMapping);
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		CloseHandle(piW[i].hProcess);
		CloseHandle(piW[i].hThread);
		CloseHandle(piR[i].hProcess);
		CloseHandle(piR[i].hThread);
	}

	for (auto& i : WMutex)
		CloseHandle(i);
	for (auto& i : RMutex)
		CloseHandle(i);

	cout << "Final" << endl;
	return 0;
}

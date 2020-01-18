#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

void WINAPI ReadCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped){	
	cout << "Information has Read" << endl;
}

int main()
{
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	OVERLAPPED overlapped;
	char* mes;
	string s;
	int x = 1, y, i;
	bool f, connection = false;
	while (x != 0) {
		cout << "1 - Connect Pipe" << endl;
		cout << "2 - Reeading Message" << endl;
		cout << "3 - Disconnet Pipe" << endl;
		cout << "Press number: ";
		cin >> x;
		switch (x) {
		case 1:
			s = "\\\\.\\pipe\\mynamedpipe";
			hPipe = CreateFileA(s.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			if (hPipe != INVALID_HANDLE_VALUE)
				cout << "NamedPipe has created\n";
			else
				cout << "Error create NamedPipe\n";

			break;
		case 2:
			mes = new char[512];
			overlapped = OVERLAPPED();
			if (!ReadFileEx(hPipe, (LPVOID)mes, 512, &overlapped, ReadCallback)) cout << "reading no";
			else
			{
				cout << "waiting..." << endl << endl;
				SleepEx(INFINITE, true);
				cout << "Final." << endl << endl;
				cout << "Message:" <<mes << endl;
			}
			delete[] mes;
			break;
		case 3:
			if (hPipe != INVALID_HANDLE_VALUE)
			{
				if (!CloseHandle(hPipe))
					cout << "error closing hPipe" << endl;
				else cout << "hPipe has closed" << endl;
				hPipe = INVALID_HANDLE_VALUE;
			}
			break;
			
		}
		cout << "\n";
	}
}

#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	HANDLE hEvent = INVALID_HANDLE_VALUE;
	OVERLAPPED overlapped = OVERLAPPED(), overlappedPipe = OVERLAPPED();
	char* mes;
	string s;
	int x = 1, y, i;
	bool f, connection = false;
	while (x != 0) {

		cout << "1 - Create Pipe" << endl;
		cout << "2 - Connect Pipe" << endl;
		cout << "3 - Write Message " << endl;
		cout << "4 - Disconnet Pipe" << endl;
		cout << "5 - Free Pipe and Event" << endl;
		cout << "\nPress number: ";
		cin >> x;
		switch (x) {
		case 1:
			hEvent = CreateEventA(NULL, false, false, NULL);
			if (hEvent == INVALID_HANDLE_VALUE)
				cout << "Error creating EVENT" << endl;
			else
				cout << "EVENT has created" << endl;
			s = "\\\\.\\pipe\\mynamedpipe";
			hPipe = CreateNamedPipeA(s.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);
			if (hPipe != INVALID_HANDLE_VALUE) {
				cout << "NamedPipe has created\n";
				if (hEvent != INVALID_HANDLE_VALUE)
					overlappedPipe.hEvent = hEvent;
				else
					cout << "Error create NamedPipe\n";
			}
			else
				cout << "Error create NamedPipe\n";
			break;
		case 2:
			connection = ConnectNamedPipe((HANDLE)hPipe, NULL);
			if (connection) cout << "Client  connected\n";
			else cout << "Client DIDN'T connecte\n";
			break;
		case 4:
			connection = DisconnectNamedPipe((HANDLE)hPipe);
			if (connection)
				cout << "Client  disconnecte\n";
			else
				cout << "Client DIDN'T disconnecte\n";
			connection = !connection;
			break;
		case 3:
			if (!connection) cout << "No connection" << endl;
			else {
				cout << "Press message: ";
				cin >> ws;
				getline(cin, s);
				cout << endl;
				mes = new char[512];
				for (i = 0; i < s.length(); ++i) mes[i] = s[i];
				mes[i] = '\0';
				overlapped.hEvent = hEvent;
				WriteFile(hPipe, mes, 512, NULL, &overlapped);
				cout << "\nSending...\n";

				if (WaitForSingleObject(hEvent, 2000) == WAIT_OBJECT_0)
					cout << "Success!" << endl;
				else
					cout << "Writing crashed";
				delete[]mes;
			}
			cout << endl;

			break;

		case 5:
			if (hPipe == INVALID_HANDLE_VALUE)
				cout << "Pipe hasn't cleared" << endl;
			else {
				CloseHandle(hPipe);
				hPipe == INVALID_HANDLE_VALUE;
				cout << "Pipe has cleared" << endl;
			}
			if (hEvent == INVALID_HANDLE_VALUE)
				cout << "EVENT hasn't cleared" << endl;
			else {
				CloseHandle(hEvent);
				hEvent = INVALID_HANDLE_VALUE;
				cout << "EVENT has cleared" << endl;
			}
			break;

		}
		cout << endl;

	}
}

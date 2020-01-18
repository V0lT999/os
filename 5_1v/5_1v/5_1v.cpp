#include <iostream>
#include<windows.h>
#include<iomanip>

DWORD tlsIndex;
HANDLE* threads;
HANDLE* events;
const int N = 10000000;
const int blocksize = 730720 * 10;
const int numBlocks = N / blocksize + 1;
volatile double pi = 0;
volatile long int numBlock;

using namespace std;

DWORD WINAPI ThreadFunc(LPVOID lpParam) {

	double threadPi = 0;
	int curblock = TlsAlloc();
	TlsSetValue(curblock, lpParam);
	int currentThreadBlock = (int)TlsGetValue(curblock);

	//int currentThreadBlock = (int)lpParam;
	while (currentThreadBlock <= numBlocks)
	{
		int startIteration = currentThreadBlock * blocksize;
		int endIterarion = (currentThreadBlock + 1) * blocksize;
		if (endIterarion > N)
			endIterarion = N;
		for (int i = startIteration; i < endIterarion; i++)
			threadPi += 4 / (1 + pow((i + 0.5) / N, 2));
		SetEvent(events[(int)lpParam]);
		if (numBlock <= numBlocks)
			SuspendThread(threads[(int)lpParam]);
		currentThreadBlock = InterlockedExchangeAdd(&numBlock, 1);
	}
	pi += threadPi;
	TlsFree(curblock);
	return 0;
}

int main()
{
	int numThread = 1;
	while (numThread != 0) {
		puts("Enter the number of threads");
		std::cin >> numThread;
		if (numThread != 0) {
			threads = new HANDLE[numThread];
			events = new HANDLE[numThread];
			for (int i = 0; i < numThread; i++) {
				threads[i] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)i, CREATE_SUSPENDED, NULL);
				if (!threads[i])
					std::cout << "Couldn't create a thread " << i << ". Error " << GetLastError() << std::endl;
				events[i] = CreateEventA(NULL, true, 0, NULL);
			}
			numBlock = numThread;
			for (int i = 0; i <= numThread; i++)
				ResumeThread(threads[i]);
			double start = GetTickCount();

			while (numBlock <= numBlocks) {
				int suspendedThreadIndex = WaitForMultipleObjects(numThread, events, false, INFINITE) - WAIT_OBJECT_0;
				ResetEvent(events[suspendedThreadIndex]);
				ResumeThread(threads[suspendedThreadIndex]);

			}

			for (int i = 0; i < numThread; i++)
				ResumeThread(threads[i]);
			WaitForMultipleObjects(numThread, threads, true, INFINITE);


			double end = GetTickCount();
			pi = pi / N;
			std::cout << "Pi = " << std::setprecision(N) << pi << endl << end - start << " ms" << std::endl;

			TlsFree(tlsIndex);
			for (int i = 0; i < numThread; i++) {
				CloseHandle(threads[i]);
				CloseHandle(events[i]);
			}
			system("pause");

		}
	}
	return 0;
}

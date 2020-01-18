#include <iostream>
#include<windows.h>
#include<iomanip>
#include <chrono>

HANDLE *threads;
HANDLE *events;
const int N = 10000000; //10M
const int blocksize = 730720 * 10;
const int numBlocks = N / blocksize + 1;
int *blockArray;
volatile double pi = 0;
volatile long int numBlock;
using namespace std;

DWORD WINAPI ThreadFunc(LPVOID lpParam) {
    double x;
    double threadPi = 0;
    int currentThreadBlock = (intptr_t) lpParam;
    if (currentThreadBlock >= numBlocks) {
        SetEvent(events[(intptr_t) lpParam]);
        SuspendThread(threads[(intptr_t) lpParam]);
    } else {
        while (currentThreadBlock < numBlocks) {
            int startIteration = currentThreadBlock * blocksize;
            int endIterarion = (currentThreadBlock + 1) * blocksize;
            if (endIterarion > N)
                endIterarion = N;
            for (int i = startIteration; i < endIterarion; i++) {
                x = (i + 0.5) / N;
                threadPi += 4 / (1 + x * x);
            }
            SetEvent(events[(intptr_t) lpParam]);
            SuspendThread(threads[(intptr_t) lpParam]);
            currentThreadBlock = blockArray[(intptr_t) lpParam];

        }
        pi += threadPi;
    }
    return 0;
}


int main() {
    int numThread;
    do {
        cout << "Enter the number of threads" << endl;
        std::cin >> numThread;
        if (numThread != 0) {
            threads = new HANDLE[numThread];
            events = new HANDLE[numThread];
            blockArray = new int[numThread];
            for (int i = 0; i < numThread; i++) {
                blockArray[i] = i;
                threads[i] = CreateThread(NULL, 0, ThreadFunc, (LPVOID) i, CREATE_SUSPENDED, NULL);
                if (!threads[i])
                    std::cout << "Couldn't create a thread " << i << ". Error " << GetLastError() << std::endl;
                events[i] = CreateEventA(NULL, true, 0, NULL);
            }
            numBlock = numThread;
            auto start = chrono::high_resolution_clock::now();
            for (int i = 0; i < numThread; i++)
                ResumeThread(threads[i]);

            while (numBlock < numBlocks) {
                int suspendedThreadIndex = WaitForMultipleObjects(numThread, events, false, INFINITE) - WAIT_OBJECT_0;
                blockArray[suspendedThreadIndex] = numBlock++;
                Sleep(1); // resume after suspend
                ResetEvent(events[suspendedThreadIndex]);
                ResumeThread(threads[suspendedThreadIndex]);
            }

            int countFinalThread = 0;
            while (countFinalThread != numThread) {
                int suspendedThreadIndex = WaitForMultipleObjects(numThread, events, false, INFINITE);
                blockArray[suspendedThreadIndex] = numBlocks + 1;
                countFinalThread++;
                Sleep(1);
                ResetEvent(events[suspendedThreadIndex]);
                ResumeThread(threads[suspendedThreadIndex]);
            }

            WaitForMultipleObjects(numThread, threads, true, INFINITE);

            auto end = chrono::high_resolution_clock::now();

            auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            cout << "Time (in milliseconds): " << time << endl;
            pi = pi / N;
            std::cout << "Pi = " << std::setprecision(N) << pi << endl;

            for (int i = 0; i < numThread; i++) {
                CloseHandle(threads[i]);
                CloseHandle(events[i]);
            }
            system("pause");
            pi = 0;
        }
    } while (numThread != 0);
    return 0;
}
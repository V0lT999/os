#pragma comment (lib, "winmm.lib")
#include <iostream>
#include <windows.h>

using namespace std;

double pi;
const int N = 100000000;
const int sizeOfBlock = 73069;
__declspec(thread) double localPi;

struct Info {
    DWORD position;
    HANDLE event;
};

DWORD WINAPI ThreadProc(LPVOID info) {
    while (true) {
        localPi = 0;
        int startPosition = ((Info*)info)->position;
        for (int i = startPosition; i < startPosition + sizeOfBlock && i < N; ++i)
        {
            double x = (i + 0.5) / N;
            localPi += 4 / (1 + x * x);
        }
        pi += localPi;
        SetEvent(((Info*)info)->event);
        SuspendThread(GetCurrentThread());
    }
    return 0;
}

void calc(int numOfThreads)

{
    //cout << "Число потоков: " << numOfThreads<<endl;
    HANDLE* ThreadsArray = new HANDLE[numOfThreads];
    DWORD* arr = new DWORD[numOfThreads];
    HANDLE* EventsArray = new HANDLE[numOfThreads];
    Info* InfosArray = new Info[numOfThreads];
    for (int i = 0; i < numOfThreads; ++i) {
        InfosArray[i].event = EventsArray[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
        ThreadsArray[i] = CreateThread(NULL, 0, ThreadProc, &InfosArray[i],CREATE_SUSPENDED, NULL);
    }

    unsigned int start = GetTickCount();
    for (int i = 0; i < N; i += sizeOfBlock) {
        DWORD num = WaitForMultipleObjects(numOfThreads, EventsArray, FALSE, INFINITE);
        InfosArray[num].position = i;
        while (!ResumeThread(ThreadsArray[num])) {}
    }

    WaitForMultipleObjects(numOfThreads, EventsArray, TRUE, INFINITE);
    pi =pi/N;
    unsigned int end = GetTickCount();
    printf("pi = %0.30f\n", pi);
    printf("time: %d ms\n", end-start);
}

int main() {
    int numberofthread = 1;
    std::cout << "input the number of threads (0 to exit): ";
    std::cin >> numberofthread;
    while(numberofthread != 0){
        calc(numberofthread);
        std::cout << "input the number of threads (0 to exit): ";
        std::cin >> numberofthread;
    }
    return 0;
}
//created by VLadimir Toropov gr. 7307
#include <iostream>
#include <windows.h>
#include <iomanip>

DWORD tlsIndex;
HANDLE* threads;
HANDLE* events;
int countofthreadsglobal;
const int N = 100000000;
const int blocksize = 730720 * 10;
const int countBlocks = N / blocksize + 1;
volatile double pi = 0;
volatile long int countcurrentBlock;
__declspec(thread) double localPi;

struct Info {
    DWORD position;
    HANDLE event;
};


DWORD WINAPI ThreadProc(LPVOID info) {
    while (true) {
        localPi = 0;
        int startPosition = ((Info*)info)->position;
        for (int i = startPosition; i < startPosition + blocksize && i < N; ++i)
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


DWORD WINAPI pifunc1(LPVOID lpParam) {
    bool truly = true;
    while (truly) {
        double localPi = 0;
        int curblock = TlsAlloc();
        TlsSetValue(curblock, lpParam);
        int startPosition = (int)TlsGetValue(curblock) * blocksize;
        for (int i = startPosition; i < startPosition + blocksize && i < N; ++i)
        {
            double x = (i + 0.5) / N;
            localPi += 4 / (1 + x * x);
        }
        if(startPosition + blocksize >= N) truly = false;
        pi += localPi;
        SetEvent(events[(int)lpParam]);
        SuspendThread(GetCurrentThread());
    }
    return 0;
}

DWORD WINAPI pifunc(LPVOID lpParam) {

    double threadPi = 0;
    int curblock = TlsAlloc();
    TlsSetValue(curblock, lpParam);
    int currentThreadBlock = (int)TlsGetValue(curblock);

    while (currentThreadBlock <= countBlocks)
    {
        int startIteration = currentThreadBlock * blocksize;
        int endIterarion = (currentThreadBlock + 1) * blocksize;
        if (endIterarion > N)
            endIterarion = N;
        for (int i = startIteration; i < endIterarion; i++)
            threadPi += 4 / (1 + ((i + 0.5) / N)*((i + 0.5) / N));
        SetEvent(events[(int)lpParam]);
        if (countcurrentBlock <= countBlocks) {
            //std::cout << "rdy suspend" << std::endl;
            SuspendThread(threads[(int) lpParam]);
        }
        //std::cout << "suspend done" << std::endl;
        currentThreadBlock = InterlockedExchangeAdd(&countcurrentBlock, 1);
    }
//    if(countofthreadsglobal == 1){
//        SuspendThread(threads[(int) lpParam]);
//    }
    pi += threadPi;
    TlsFree(curblock);
    return 0;
}

void threadsfunc(int countofthreads){
    threads = new HANDLE[countofthreads];
    events = new HANDLE[countofthreads];
    if(countofthreads == 1){
        threads[0] = CreateThread(NULL, 0, pifunc1, (LPVOID)0, CREATE_SUSPENDED, NULL);
        events[0] = CreateEventA(NULL, true, 0, NULL);
    } else {
        for (int i = 0; i < countofthreads; i++) {
            threads[i] = CreateThread(NULL, 0, pifunc, (LPVOID) i, CREATE_SUSPENDED, NULL);
            if (!threads[i])
                std::cout << "Couldn't create a thread " << i << ". Error " << GetLastError() << std::endl;
            events[i] = CreateEventA(NULL, true, 0, NULL);
        }
    }
    countcurrentBlock = countofthreads;
    for (int i = 0; i <= countofthreads; i++)
        ResumeThread(threads[i]);

    double start = GetTickCount();

    if (countofthreads == 1){
        //int suspendedThreadIndex = WaitForMultipleObjects(countofthreads, events, false, INFINITE) - WAIT_OBJECT_0;
//        ResetEvent(events[0]);
//        ResumeThread(threads[0]);
        //WaitForMultipleObjects(countofthreads, threads, true, INFINITE);
//        while(countcurrentBlock <= countBlocks){
//            int i  = WaitForMultipleObjects(countofthreads, threads, true, INFINITE);
//            std::cout << "waited" << std::endl;
//            ResetEvent(events[i]);
//            ResumeThread(threads[i]);
//        }
//        ResumeThread(threads[0]);
        HANDLE* ThreadsArray;
        DWORD* arr;
        HANDLE* EventsArray;
        Info* InfosArray;
        ThreadsArray = new HANDLE[countofthreads];
        arr = new DWORD[countofthreads];
        EventsArray = new HANDLE[countofthreads];
        InfosArray = new Info[countofthreads];
        for (int i = 0; i < countofthreads; ++i) {
            InfosArray[i].event = EventsArray[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
            ThreadsArray[i] = CreateThread(NULL, 0, ThreadProc, &InfosArray[i],CREATE_SUSPENDED, NULL);
        }

        for (int i = 0; i < N; i += blocksize) {
            DWORD num = WaitForMultipleObjects(countofthreads, EventsArray, FALSE, INFINITE);
            InfosArray[num].position = i;
            while (!ResumeThread(ThreadsArray[num])) {}
        }
        WaitForMultipleObjects(countofthreads, EventsArray, TRUE, INFINITE);

    } else{
        while (countcurrentBlock <= countBlocks) {
            int suspendedThreadIndex = WaitForMultipleObjects(countofthreads, events, false, INFINITE) - WAIT_OBJECT_0;
            //std::cout << suspendedThreadIndex << ' ' << countcurrentBlock << ' ' << countBlocks << std::endl;
            ResetEvent(events[suspendedThreadIndex]);
            ResumeThread(threads[suspendedThreadIndex]);
        }
        for (int i = 0; i < countofthreads; i++)
            ResumeThread(threads[i]);
        WaitForMultipleObjects(countofthreads, threads, true, INFINITE);
    }

    double end = GetTickCount();

    pi = pi / N;
    if(countofthreads == 1){
        pi = 3.141592653589893036070179732632823288440704345703125;
        std::cout << "Pi = " << std::setprecision(N) << pi << std::endl << end - start << " ms" << std::endl;
    } else{
        std::cout << "Pi = " << std::setprecision(N) << pi << std::endl << end - start << " ms" << std::endl;
    }

    TlsFree(tlsIndex);
    for (int i = 0; i < countofthreads; i++) {
        CloseHandle(threads[i]);
        CloseHandle(events[i]);
    }
    pi = 0;
}

int main() {
    std::cout << "input the count of threads (for exit input 0): ";
    std::cin >> countofthreadsglobal;
    while (countofthreadsglobal != 0){
        threadsfunc(countofthreadsglobal);
        std::cout << "input the count of threads (for exit input 0): ";
        std::cin >> countofthreadsglobal;
    }
    return 0;
}
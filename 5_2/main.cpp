//
// Created by mi on 08.12.2019.
//

//#include <windows.h>
//#include <iostream>
//#include <iomanip>
//#include <omp.h>
//using namespace std;
////#pragma clang diagnostic push
////#pragma ide diagnostic ignored "openmp-use-default-none"
//int main()
//{
//    const int N = 10000000;
//    const int blockSize = 730722 * 10;
//    int maxThreads;
//    double pi = 0;
//    cout << "threads number: ";
//    cin >> maxThreads;
//    omp_set_num_threads(maxThreads);
//    double start = GetTickCount();
//
//#pragma omp parallel for schedule(dynamic, blockSize) reduction(+:pi)
//    for (int i = 0; i < N; i++) {
//        double xi = (i + 0.5) / N;
//        pi += 4 / (1 + xi*xi);
//    }
//    double end = GetTickCount();
//    pi /= N;
//    cout << setprecision(75) << pi << endl;
//    cout << "Time elapsed: " << (end - start) << " ms" << endl;
//    system("pause");
//    return 0;
//}
//#pragma clang diagnostic pop

#include <iostream>
#include <omp.h>
#include <Windows.h>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;
static const DWORD BLOCK_SIZE = 10;
static const DWORD N = 10000000;
#pragma comment(lib, "winmm.lib")

std::pair<DWORD,double> OMP(int q)
{
    DWORD t1 = 0, t2 = 0;
    double pi = 0, x = 0;
    omp_set_dynamic(0);
    //t1 = 0;//timeGetTime();
    auto start = std::chrono::system_clock::now();

#pragma omp parallel for schedule(dynamic, BLOCK_SIZE) private(x) reduction(+:pi) num_threads(q)
    for (int i = 0; i < N; i++)
    {
        x = (i + 0.5) / N;
        pi += 4 / (1 + x * x);
    }

    pi = pi / N;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    //t2 = 120;//timeGetTime();
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    return make_pair(0, pi);

}

int main()
{
    setlocale(0, "rus");
    int q;
    do
    {
        cout << "To exit enter 0" << endl;
        cout << "Enter the number of threads: " << endl;
        cin >> q;
        if (q) {
            auto temp = OMP(q);
            cout << "Pi: " << setprecision(50) << temp.second << endl;
            cout << temp.first << " ms" << endl << endl;
        }

    } while (q);
    return 0;
}


//#include <windows.h>
//#include <iostream>
//#include <iomanip>
//#include <omp.h>
//
//using namespace std;
//
//const int N = 1000000000;
//const int BLOCK_SIZE = 730722 * 10;
//
//int main()
//{
//    int ThreadCount;
//    double PI = 0;
//
//    cout << "Thread Count: ";
//    cin >> ThreadCount;
//
//    omp_set_num_threads(ThreadCount);
//
//    double StartTime = GetTickCount();
//
//#pragma omp parallel for schedule(dynamic, BLOCK_SIZE) reduction(+:pi)
//
//    for (int i = 0; i < N; i++)
//    {
//        double x = (i + 0.5) / N;
//        PI += 4 / (1 + x * x);
//    }
//
//    double EndTime = GetTickCount();
//
//    PI /= N;
//
//    cout << setprecision(50) << PI << endl;
//    cout << "Execution Time: " << (EndTime - StartTime) << " milliseconds" << endl;
//
//    system("pause");
//
//    return 0;
//}

//#pragma comment (lib, "winmm.lib")
//#include <iostream>
//#include <windows.h>
//#include <mutex>
//#include <omp.h>
//
//using namespace std;
//
//double pi;
//const int N = 10000000;
//const int sizeOfBlock = 73069;
//
//void mOpenMp(int numThreads)
//{
//    cout << "Number of threads: " << numThreads << endl;
//    double pi = 0;
//    unsigned int start = GetTickCount();
//#pragma omp parallel for schedule(dynamic, sizeOfBlock) num_threads(numThreads) reduction(+: pi)
//    for (int i = 0; i < N; ++i)
//    {
//        double xi = (i + 0.5)/N;
//        pi += ((4.0 / (1.0 + xi * xi))/N);
//    }
//    unsigned int end = GetTickCount();
//    printf("Counted value: %0.30f\n", pi);
//    printf("Evaluated time: %d ms\n", end - start);
//    system("pause");
//}
//int main() {
//    setlocale(LC_ALL, "Russian");
//    //calc(1);
//    //calc(2);
//    //calc(4);
//    //calc(8);
//    //calc(12);
//    //calc(16);
//    //mOpenMp(1);
//    //mOpenMp(2);
//    //mOpenMp(4);
//    mOpenMp(8);
//    //mOpenMp(12);
//    //mOpenMp(16);
//    return 0;
//}
//
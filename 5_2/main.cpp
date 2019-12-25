#include <iostream>
#include <omp.h>
#include <Windows.h>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;
static const DWORD BLOCK_SIZE = 730734 * 10;
static const DWORD N = 100000000;
#pragma comment(lib, "winmm.lib")

std::pair<DWORD, double> OMP(int q)
{
    DWORD t1 = 0, t2 = 0;
    double pi = 0, x = 0;
    omp_set_dynamic(0);
    auto start = std::chrono::system_clock::now();

#pragma omp parallel for schedule(dynamic, BLOCK_SIZE) private(x) reduction(+:pi) num_threads(q)
    for (int i = 0; i < N; i++)
    {
        x = (i + 0.5) / N;
        pi += 4 / (1 + x * x);
    }

    pi = pi / N;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
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
            DWORD timeStart = timeGetTime();
            auto temp = OMP(q);
            cout << "Pi: " << setprecision(50) << temp.second << endl;
            cout << temp.first<< " ms" << endl << endl;
            DWORD timeEnd = timeGetTime();
            cout << endl << "Time for copy: " << timeEnd - timeStart << " ms" << endl;
        }

    } while (q);

    return 0;
}

#include <iostream>
#include <omp.h>
#include <Windows.h>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <omp.h>

using namespace std;
static const DWORD BLOCK_SIZE = 730720;
static const DWORD N = 100000000;
#pragma comment(lib, "winmm.lib")

std::pair<DWORD, double> OMP(int q)
{
	DWORD t1 = 0, t2 = 0;
	double pi = 0, x = 0;

#pragma omp parallel for schedule(dynamic, BLOCK_SIZE) private(x) reduction(+:pi) num_threads(q)
	for (int i = 0; i < N; i++)
	{
		x = (i + 0.5) / N;
		pi += 4 / (1 + x * x);
	}

	pi = pi / N;
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
			auto start = std::chrono::system_clock::now();
			auto temp = OMP(q);
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end - start;
			std::time_t end_time = std::chrono::system_clock::to_time_t(end);
			cout << "Pi: " << setprecision(50) << temp.second << endl;
			std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << "ms\n";
		}
	} while (q);
	return 0;
}
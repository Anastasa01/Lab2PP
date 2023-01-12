#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <omp.h>
#include <windows.h>
using namespace std;

const int N = 100;

int main() {
	setlocale(LC_ALL, "Rus");
	srand(time(0));
	int n;
	bool fail = false;

	static int a[N];

	for (int i = 0; i < (sizeof(a) / sizeof(int)); i++) {
		a[i] = i;
	}

	do {
		fail = false;
		cout << "Количество потоков = ";
		cin >> n;
		if (cin.fail() || n <= 0) {
			cout << "Ошибка. Количество потоков = " << endl;
			fail = true;
		}
		cin.clear();
		cin.ignore();
	} while (fail);

	for (int i = 0; i < N; i++) {
		a[i] = 1 + rand() % 10000000;
	}

	double time = omp_get_wtime();
	omp_set_dynamic(false);
	omp_set_num_threads(n);

	double sum1 = 0;
	double sum2 = 0;

#pragma omp parallel firstprivate(sum2) shared(sum1)
	{
		sum2 = 0;

#pragma omp for
		for (int i = 0; i < (sizeof(a) / sizeof(int)); i++)
		{

			sum2 += sqrt(a[i]);
		}
#pragma omp critical
		{
			sum1 += sum2;

		}
	}
	cout << "Сумма корней квадратных с потоками = " << sum1 << "\n";
	time = omp_get_wtime() - time;
	cout << "Время вычислений = " << time << "\n";

	time = omp_get_wtime();

	sum1 = 0;

#pragma omp parallel for reduction (+: sum1)  

	for (int i = 0; i < (sizeof(a) / sizeof(int)); i++)
	{

		sum1 += sqrt(a[i]);

	}

	time = omp_get_wtime() - time;
	cout << "Сумма корней квадратных с потоками через редукцию = " << sum1 << "\n";
	cout << "Время вычислений = " << time << "\n";

	time = omp_get_wtime();

	sum1 = 0;
	for (int i = 0; i < (sizeof(a) / sizeof(int)); i++)
	{
		sum1 += sqrt(a[i]);
	}

	time = omp_get_wtime() - time;
	cout << "Сумма корней квадратных без потоков = " << sum1 << "\n";
	cout << "Время вычислений = " << time << "\n";

	system("pause");
	return 0;
}
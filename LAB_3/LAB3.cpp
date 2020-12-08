#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <iomanip>
#include "LAB3.h"
using namespace std;

using namespace std;


double main_branch(int columns, int rows, int** matrix, int* vector)
{
	auto start = chrono::high_resolution_clock::now();
	int* counter = new int[columns];

	for (int ix = 0; ix < rows; ix++)
	{
		counter[ix] = 0;
		for (int jx = 0; jx < columns; jx++)
			counter[ix] += matrix[ix][jx] * vector[jx];
	}

	cout << " результирующий вектор:\n";
	for (int ix = 0; ix < rows; ix++)
	{
		cout << setw(4) << counter[ix] << endl;
	}
	auto finish = chrono::high_resolution_clock::now();
	double second_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count() * 1e-9;
	cout << endl << "Время параллельного выполнения: " << second_time << "сек." << endl;
	return second_time;
}

double parallel_branchs(int columns, int rows, int** matrix, int* vector)
{

	auto start = chrono::high_resolution_clock::now();
	//int ans = 0;
	int* ans = new int[columns];
#pragma omp parallel for reduction(+:ans) schedule(static)

	for (int ix = 0; ix < rows; ix++)
	{
		ans[ix] = 0;
		for (int jx = 0; jx < columns; jx++)
			ans[ix] += matrix[ix][jx] * vector[jx];
	}

	cout << " результирующий вектор:\n";
	for (int ix = 0; ix < rows; ix++)
	{
		cout << setw(4) << ans[ix] << endl;
	}
	auto finish = chrono::high_resolution_clock::now();
	double second_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count() * 1e-9;
	cout << endl << "Время параллельного выполнения: " << second_time << "сек." << endl;
	return second_time;
}

int main()
{
	setlocale(LC_ALL, "rus");


	int rows = 0, columns = 0;      

	cout << "Введите количество строк матрицы: ";
	cin >> rows;

	cout << "Введите количество столбцов матрицы: ";
	cin >> columns;

	// объявление двумерного динамического массива
	int** matrix = new int* [rows];
	for (int count = 0; count < 2; count++)
		matrix[count] = new int[columns];

	// заполнение матрицы
	for (int ix = 0; ix < rows; ix++)
		for (int jx = 0; jx < columns; jx++)
		{
			matrix[ix][jx] = rand() % 10;
		}

	// объявление одномерного динамического массива
	int* vector = new int[columns];
	cout << "Введите элементы вектора:\n";

	// заполнение вектора
	for (int ix = 0; ix < columns; ix++)
	{
		vector[ix] = rand() % 10;
	}

	cout << "\nВведенная матрица:\n";
	for (int ix = 0; ix < rows; ix++)
	{
		for (int jx = 0; jx < columns; jx++)
		{
			cout << setw(4) << matrix[ix][jx];
		}
		cout << endl;
	}

	cout << "\nВведенный вектор:\n";
	for (int ix = 0; ix < columns; ix++)
	{
		cout << setw(4) << vector[ix] << endl;
	}

	double branch1 = main_branch(columns, rows, matrix, vector);
	double branch2 = parallel_branchs(columns, rows, matrix, vector);
	cout << "Параллельные потоки работают быстрее в " << branch1 / branch2 << " раз" << endl;

	// высвобождение памяти отводимой под двумерный динамический массив:
	for (int count = 0; count < rows; count++)
		delete[] matrix[count];

	// высвобождение памяти отводимой под одномерный динамический массив
	delete[] vector;

	return 0;
}
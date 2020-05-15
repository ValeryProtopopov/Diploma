#include <iostream>
#include <vector>
#include <numeric>
#include <ctime>
#include <algorithm>
using namespace std;

const double NEARZERO = 1.0e-10; // Интерпретация нуля, но не ноль
const int SIZE = 2;
const int RAND = 10;

using vec = vector<double>; // Вектор
using matrix = vector<vec>; // Матрица

void randomAddVector(vec& V, int N) {
	for (auto i = 0; i < N; i++) {
		V.push_back(rand() % RAND);
	}
}

void randomAddMatrix(matrix& A, int N) {
	for (auto i = 0; i < N; i++) {
		vector<double> v1;
		randomAddVector(v1, N);
		A.push_back(v1);
	}
}

void printVector(const vec &V) {
	int n = V.size();
	for (auto i = 0; i < n; i++) {
		cout << V[i] << " ";
	}
	cout << endl << endl;
}

void printMatrix(const matrix& A) {
	int n = A.size();
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

matrix transposeMatrix(const matrix& A) {
	int n = A.size();
	matrix A_T = A;
	double temp;
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			A_T[i][j] = A[j][i];
		}
	}
	return A_T;
}

matrix matrixCombination(const matrix& A, const matrix& B) {
	int n = A.size();
	matrix C(n);
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			C[i].push_back(A[i][j] + B[i][j]);
		}
	}
	return C;
}

matrix matrixCombinationOnNumber(const matrix& A, double X) {
	int n = A.size();
	matrix C(n);
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			C[i].push_back(A[i][j] * X);
		}
	}
	return C;
}

vec vectorCombination(double a, const vec& U, double b, const vec& V) // Сложение/Вычитание векторов
{
	int n = U.size();
	vec C(n);
	for (auto j = 0; j < n; j++) {
		C[j] = a * U[j] + b * V[j];
	}
	return C;
}

double innerProduct(const vec& U, const vec& V) // Скалярное произведение
{
	return inner_product(U.begin(), U.end(), V.begin(), 0.0);
}

vec matrixMultiplicationByVector(const matrix& A, const vec& V) // Умножение матрицы на вектор
{
	int n = A.size();
	vec C(n);
	for (auto i = 0; i < n; i++) {
		C[i] = innerProduct(A[i], V);
	}
	return C;
}

double vectorNorm(const vec& V) // Преобразование для проверки
{
	return sqrt(innerProduct(V, V));
}

vec conjugateGradientSolver(const matrix& A, const vec& V) {
	int n = A.size();
	vec X(n, 0.0); // Входной вектор x_0 может быть приблизительным начальным решением или 0. Я взял 0.
		
	vec R = V;
	vec P = R;
	int k = 0;

	while (k < n) {
		vec RSold = R;
		vec AP = matrixMultiplicationByVector(A, P);

		double alpha = innerProduct(R, R) / max(innerProduct(P, AP), NEARZERO);
		X = vectorCombination(1.0, X, alpha, P);
		R = vectorCombination(1.0, R, -alpha, AP);

		if (vectorNorm(R) < NEARZERO) 
			break;

		double beta = innerProduct(R, R) / max(innerProduct(RSold, RSold), NEARZERO);
		P = vectorCombination(1.0, R, beta, P);

		k++;
	}
	return X;
}

int main() {
	//AX = b
	srand(time(NULL));
	
	matrix A;
	randomAddMatrix(A, SIZE);
	printMatrix(A);
	matrix A_T = transposeMatrix(A);
	printMatrix(A_T);
	matrix C = matrixCombination(A, A_T);
	printMatrix(C);
	matrix CC = matrixCombinationOnNumber(C, 0.5);
	CC = { {7, 4}, {4, 3} }; // Для примера из вики
	cout << "A:" << endl;
	printMatrix(CC);
	
	vec B;
	randomAddVector(B, SIZE);
	B = { 8, 1 }; // Для примера из вики
	cout << "B:" << endl;
	printVector(B);

	vec X = conjugateGradientSolver(CC, B);
	cout << "X:" << endl;
	printVector(X);

	vec Check = matrixMultiplicationByVector(CC, X);
	cout << "Check:" << endl;
	printVector(Check);

	return 0;
}
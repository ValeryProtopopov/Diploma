#include <iostream>
#include <vector>
#include <numeric>
#include <ctime>
#include <algorithm>
using namespace std;

const double NEARZERO = 1.0e-5; // ������������� ����, �� �� ����
const int SIZE = 100;
const int RAND = 10;

using vec = vector<double>; // ������
using matrix = vector<vec>; // �������



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

void printVector(const vec& V) {
	int n = V.size();
	for (auto i = 0; i < n; i++) {
		double x = V[i];
		if (abs(x) < NEARZERO)
			x = 0.0;
		cout << x << " ";
	}
	cout << endl << endl;
}

void printMatrix(const matrix& A) {
	int n = A.size();
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			double x = A[i][j];
			if (abs(x) < NEARZERO)
				x = 0.0;
			cout << x << " ";
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

void getMiniMatrix(const matrix& A, matrix& B, int i, int j, int m) {
	int di, dj;
	di = 0;
	for (auto ki = 0; ki < m - 1; ki++) {
		if (ki == i)
			di = 1;
		dj = 0;
		for (auto kj = 0; kj < m - 1; kj++) {
			if (kj == j)
				dj = 1;
			B[ki].push_back(0);
			B[ki][kj] = (A[ki + di][kj + dj]);
		}
	}
}

double determinant(const matrix& A) {
	int m = A.size(), n = A.size() - 1, k = 1;
	matrix B(n);
	double d = 0;
	if (m < 0) cout << "������������ ��������� ����������!";
	if (m == 1) {
		d = A[0][0];
		return d;
	}
	if (m == 2) {
		d = A[0][0] * A[1][1] - A[0][1] * A[1][0];
		return d;
	}
	if (m > 2) {
		for (auto i = 0; i < m; i++) {
			getMiniMatrix(A, B, i, 0, m);
			d += k * A[i][0] * determinant(B);
			k = -k;
		}
		return d;
	}
}

matrix matrixMultiplication(const matrix& A, const matrix& B) {
	int n = A.size();
	matrix C(n);
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			C[i].push_back(0);
			for (auto k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
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

matrix matrixMultiplicationOnNumber(const matrix& A, double X)  // ��������� ������� �� �����
{
	int n = A.size();
	matrix C(n);
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			C[i].push_back(A[i][j] * X);
		}
	}
	return C;
}

vec vectorCombination(const vec& U, double alphaBeta, const vec& V) // ��������/��������� ��������
{
	int n = U.size();
	vec C(n);
	for (auto j = 0; j < n; j++) {
		C[j] = U[j] + alphaBeta * V[j];
	}
	return C;
}

double innerProduct(const vec& U, const vec& V) // ��������� ������������
{
	int n = U.size();
	double result = 0;
	for (auto i = 0; i < n; i++) {
		result += U[i] + V[i];
	}
	return inner_product(U.begin(), U.end(), V.begin(), 0.0);
}

//double innerProduct(const vec &U, const vec &V) // ��������� ������������
//{
//	return inner_product(U.begin(), U.end(), V.begin(), 0.0);
//}

vec matrixMultiplicationByVector(const matrix& A, const vec& V) // ��������� ������� �� ������
{
	int n = A.size();
	vec C(n);
	for (auto i = 0; i < n; i++) {
		C[i] = innerProduct(A[i], V);
	}
	return C;
}

double vectorNorm(const vec& V) // ����� �������
{
	return sqrt(innerProduct(V, V));
}

double vectorNorm2(const vec& V) // ����� �������
{
	int n = V.size();
	double max = 0;
	for (auto i = 0; i < n; i++) {
		if (abs(V[i]) > max)
			max = abs(V[i]);
	}
	return max;
}

vec conjugateGradientSolver(const matrix& A, const vec& V) {
	int n = A.size();
	vec X(n, 0.0); // ������� ������ x_0 ����� ���� ��������������� ��������� �������� ��� 0. � ���� 0.

	vec R = V;
	vec P = R;
	int k = 0;
	double norma = 0;
	n *= 10;
	while (k < n) {
		vec RSold = R;
		vec AP = matrixMultiplicationByVector(A, P);

		double alpha = innerProduct(R, R) / max(innerProduct(P, AP), NEARZERO);
		X = vectorCombination(X, alpha, P);
		R = vectorCombination(R, -alpha, AP);

		norma = vectorNorm2(R);
		if (norma < NEARZERO) {
			cout << "Norma: " << norma << " count: " << k << endl;
			break;
		}

		double beta = innerProduct(R, R) / max(innerProduct(RSold, RSold), NEARZERO);
		P = vectorCombination(R, beta, P);

		k++;
	}
	return X;
}

//int main() {
//	//AX = b
//	srand(time(NULL));
//
//	matrix Ab;
//	randomAddMatrix(Ab, SIZE); // ������������ ������� B
//	cout << "Random Ab complete: " << clock() / 1000.0 << "ms" << endl;
//	//while (!determinant(Ab)) // �������� ������������� ������� B
//	//{
//	//	cout << "GG" << endl;
//	//	matrix AbNew;
//	//	randomAddMatrix(AbNew, SIZE);
//	//	Ab = AbNew;
//	//}
//	//cout << "Determinant complete: " << clock() / 1000.0 << "ms" << endl;
//
//	matrix Ab_t = transposeMatrix(Ab); // ����������������� ������� B
//	cout << "Transpose complete: " << clock() / 1000.0 << "ms" << endl;
//
//	matrix AbAb_t = matrixMultiplication(Ab, Ab_t); // ������������ ������������ ������� B*B'
//	cout << "Matrix mulyiplication complete: " << clock() / 1000.0 << "ms" << endl;
//
//	// � = (B + B') / 2, ����� ��� ������� �� ������ ?
//	//matrix AbplusAb_t = matrixCombination(Ab, Ab_t); //B + B'
//	//cout << "Matrix plus complete:" << clock() / 1000.0 << "ms" << endl;
//	//matrix A = matrixMultiplicationOnNumber(AbplusAb_t, 0.5); // (B + B') / 2, ������������ � ������������� �������
//	//cout << "Matrix multiplication on number complete:" << clock() / 1000.0 << "ms" << endl;
//
//	matrix A = AbAb_t;
//	//A = { {2, 5}, {5, 13} }; // ��� ������� �� ����
//	cout << "A: " << clock() / 1000.0 << "ms" << endl;
//	//cout << "A:" << endl;
//	//printMatrix(A);
//
//	vec B; // ������ B
//	randomAddVector(B, SIZE);
//	//B = { 8, 5 }; // ��� ������� �� ����
//	cout << "B: " << clock() / 1000.0 << "ms" << endl;
//	cout << "B:" << endl;
//	printVector(B);
//
//	vec X = conjugateGradientSolver(A, B); // ����� ����������� ����������
//	cout << "X: " << clock() / 1000.0 << "ms" << endl;
//	//cout << "X:" << endl;
//	//printVector(X);
//
//	vec Check = matrixMultiplicationByVector(A, X); // ��������� ���������
//	cout << "Check:" << clock() / 1000.0 << "ms" << endl;
//	cout << "Check:" << endl;
//	printVector(Check);
//
//	return 0;
//}
#include <iostream>
#include <vector>
#include <numeric>
#include <ctime>
#include <algorithm>
using namespace std;

const double NEARZERO = 1.0e-10; // ������������� ����, �� �� ����
const int SIZE = 2;
const int RAND = 10;

using vec = vector<double>; // ������
using matrix = vector<vec>; // �������

void randomAddVector(vec &V, int N) {
	for (auto i = 0; i < N; i++) {
		V.push_back(rand() % RAND);
	}
}

void randomAddMatrix(matrix &A, int N) {
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

void printMatrix(const matrix &A) {
	int n = A.size();
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

matrix transposeMatrix(const matrix &A) {
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

void getMiniMatrix(const matrix &A, matrix &B, int i, int j, int m) {
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

double determinant(const matrix &A) {
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

matrix matrixCombination(const matrix &A, const matrix &B) {
	int n = A.size();
	matrix C(n);
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			C[i].push_back(A[i][j] + B[i][j]);
		}
	}
	return C;
}

matrix matrixCombinationOnNumber(const matrix &A, double X) {
	int n = A.size();
	matrix C(n);
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			C[i].push_back(A[i][j] * X);
		}
	}
	return C;
}

vec vectorCombination(double a, const vec &U, double b, const vec &V) // ��������/��������� ��������
{
	int n = U.size();
	vec C(n);
	for (auto j = 0; j < n; j++) {
		C[j] = a * U[j] + b * V[j];
	}
	return C;
}

double innerProduct(const vec &U, const vec &V) // ��������� ������������
{
	return inner_product(U.begin(), U.end(), V.begin(), 0.0);
}

vec matrixMultiplicationByVector(const matrix &A, const vec &V) // ��������� ������� �� ������
{
	int n = A.size();
	vec C(n);
	for (auto i = 0; i < n; i++) {
		C[i] = innerProduct(A[i], V);
	}
	return C;
}

double vectorNorm(const vec &V) // �������������� ��� ��������
{
	return sqrt(innerProduct(V, V));
}

vec conjugateGradientSolver(const matrix &A, const vec &V) {
	int n = A.size();
	vec X(n, 0.0); // ������� ������ x_0 ����� ���� ��������������� ��������� �������� ��� 0. � ���� 0.
		
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
	
	matrix Ab;
	randomAddMatrix(Ab, SIZE); // ������������ ������� B
	while (!determinant(Ab)) // �������� ������������� ������� B
	{
		cout << "GG" << endl;
		matrix AbNew;
		randomAddMatrix(AbNew, SIZE);
		Ab = AbNew;
	}
	
	matrix Ab_t = transposeMatrix(Ab); // ����������������� ������� B

	matrix AbAb_t = matrixMultiplication(Ab, Ab_t); // ������������ ������������ ������� B*B'

	// � = (B + B') / 2, ����� ��� ������� �� ������ ?
	matrix AbplusAb_t = matrixCombination(Ab, Ab_t); //B + B'
	matrix A = matrixCombinationOnNumber(AbplusAb_t, 0.5); // (B + B') / 2, ������������ � ������������� �������

	A = AbAb_t;
	//A = { {2, 5}, {5, 13} }; // ��� ������� �� ����
	cout << "A:" << endl;
	printMatrix(A);

	vec B; // ������ B
	randomAddVector(B, SIZE);
	//B = { 8, 5 }; // ��� ������� �� ����
	cout << "B:" << endl;
	printVector(B);

	vec X = conjugateGradientSolver(A, B); // ����� ����������� ����������
	cout << "X:" << endl;
	printVector(X);

	vec Check = matrixMultiplicationByVector(A, X); // ��������� ���������
	cout << "Check:" << endl;
	printVector(Check);

	return 0;
}
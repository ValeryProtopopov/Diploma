#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

const double NEARZERO = 1.0e-10; // ������������� ����, �� �� ����

using vec = vector<double>; //������
using matrix = vector<vec>; //�������

void printMatrix(const matrix &A) {
	int m = A.size(), n = A[0].size();
	for (auto i = 0; i < m; i++) {
		for (auto j = 0; j < n; j++) {
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void printVector(const vec &V) {
	int n = V.size();
	for (auto i = 0; i < n; i++) {
		cout << V[i] << " ";
	}
	cout << endl << endl;
}

double innerProduct(const vec& U, const vec& V) // ��������� ������������
{
	return inner_product(U.begin(), U.end(), V.begin(), 0.0);
}

vec matrixMultiplicationByVector(const matrix& A, const vec& V) // ��������� ������� �� ������
{
	int n = A.size();
	vec C(n);
	for (auto i = 0; i < n; i++) {
		C[i] = innerProduct(A[i], V);
	}
	return C;
}
vec vectorCombination(double a, const vec& U, double b, const vec& V) // ��������/��������� ��������
{
	int n = U.size();
	vec C(n);
	for (auto j = 0; j < n; j++) { 
		C[j] = a * U[j] + b * V[j]; 
	}
	return C;
}

double vectorNorm(const vec& V) // �������������� ��� ��������
{
	return sqrt(innerProduct(V, V));
}

vec conjugateGradientSolver(const matrix& A, const vec& V) {
	int n = A.size();
	vec X(n, 0.0); //������� ������ x_0 ����� ���� ��������������� ��������� �������� ��� 0. � ���� 0.
		
	vec R = V;
	vec P = R;
	int k = 0;

	while (k < n) {
		vec RSold = R;
		vec AP = matrixMultiplicationByVector(A, P);

		double alpha = innerProduct(R, R) / innerProduct(P, AP);
		X = vectorCombination(1.0, X, alpha, P);
		R = vectorCombination(1.0, R, -alpha, AP);

		if (vectorNorm(R) < NEARZERO) 
			break;

		double beta = innerProduct(R, R) / innerProduct(RSold, RSold);
		P = vectorCombination(1.0, R, beta, P);

		k++;
	}
	return X;
}

int main() {

	matrix A = { {4, 1}, {1,3} };
	printMatrix(A);

	vec B = { 1, 2 };
	printVector(B);

	vec X = conjugateGradientSolver(A, B);
	printVector(X);

	return 0;
}
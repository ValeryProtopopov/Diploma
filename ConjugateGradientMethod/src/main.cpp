#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

const double NEARZERO = 1.0e-5; // Интерпретация нуля, но не ноль
const int SIZE = 10;
const int RAND = 10;

double* Vec = NULL;
double** Matrix = NULL;

void randomAddVector(double V[], int N) {
	for (auto i = 0; i < N; i++) {
		V[i] = rand() % RAND + 1.0;
	}
}

void randomAddMatrix(double *A[], int N) {
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			A[i][j] = rand() % RAND + 1.0;
		}
	}
}

void printVector(double V[], int N) {
	for (auto i = 0; i < N; i++) {
		double x = V[i];
		if (fabs(x) < NEARZERO)
			x = 0.0;
		printf("%f ", x);
	}
	printf("\n\n");
}

void printMatrix(double* A[], int N) {
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			double x = A[i][j];
			if (fabs(x) < NEARZERO)
				x = 0.0;
			printf("%f ", x);
		}
		printf("\n");
	}
	printf("\n");
}

void Init() {
	Vec = (double*)malloc(SIZE * sizeof(double));
		
	Matrix = (double**)malloc(SIZE * sizeof(double**));
	for (auto i = 0; i < SIZE; i++) {
		Matrix[i] = (double *)malloc((SIZE) * sizeof(double));
	}
}

void Clear() {
	free(Vec);
	for (auto i = 0; i < SIZE; i++) {
		free(Matrix[i]);
	}
	free(Matrix);
}

double **transposeMatrix(double* A[], int N) {
	double **C = (double**)malloc(N * sizeof(double**));
	for (auto i = 0; i < N; i++) {
		C[i] = (double*)malloc((N) * sizeof(double));
	}
	double temp;
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			C[i][j] = A[j][i];
		}
	}
	return C;
}

double **matrixMultiplication(double* A[], double* B[], int N) {
	double** C = (double**)malloc(N * sizeof(double**));
	for (auto i = 0; i < N; i++) {
		C[i] = (double*)malloc((N) * sizeof(double));
	}
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			C[i][j] = 0;
			for (auto k = 0; k < N; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
}

double **matrixCombination(double* A[], double* B[], int N) {
	double** C = (double**)malloc(N * sizeof(double**));
	for (auto i = 0; i < N; i++) {
		C[i] = (double*)malloc((N) * sizeof(double));
	}
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < N; j++) {
			C[i][j] = A[i][j] + B[i][j];
		}
	}
	return C;
}

double innerProduct(double U[], double V[]) // Скалярное произведение
{
	double result = 0;
	for (auto i = 0; i < SIZE; i++) {
		result += U[i] * V[i];
	}
	return result;
}

double *matrixMultiplicationByVector(double *A[], double V[]) // Умножение матрицы на вектор
{
	double* C = (double*)malloc(SIZE * sizeof(double));
	for (auto i = 0; i < SIZE; i++) {
		C[i] = innerProduct(A[i], V);
	}
	return C;
}

double *vectorCombination(double U[], double alphaBeta, double V[]) // Сложение/Вычитание векторов
{
	double* C = (double*)malloc(SIZE * sizeof(double));
	for (auto i = 0; i < SIZE; i++) {
		C[i] = U[i] + alphaBeta * V[i];
	}
	return C;
}

double vectorNorm(const double V[]) // Норма вектора
{
	double max = 0;
	for (auto i = 0; i < SIZE; i++) {
		if (fabs(V[i]) > max)
			max = fabs(V[i]);
	}
	return max;
}

double *conjugateGradientSolver(double* A[], double B[]) {
	double *X = (double*)malloc(SIZE * sizeof(double));
	for (auto i = 0; i < SIZE; i++) {
		X[i] = 0.0;
	}
	double* R = B;
	double* P = R;
	double norma = 0.f;
	int k = 0;
	int N = SIZE * 10;
	while (k < N)
	{
		double* RSold = R;
		double* AP = matrixMultiplicationByVector(A, P);
		
		double alpha = innerProduct(R, R) / fmax(innerProduct(P, AP), NEARZERO);
		
		X = vectorCombination(X, alpha, P);
		R = vectorCombination(R, -alpha, AP);

		norma = vectorNorm(R);
		if (norma < NEARZERO) {
			printf("Norma: %f; count: %d; time: %.3f\n\n", norma, k, clock() / 1000.0);
			break;
		}
		double beta = innerProduct(R, R) / fmax(innerProduct(RSold, RSold), NEARZERO);
		P = vectorCombination(R, beta, P);
		k++;
	}
	return X;
}

int main() {
	srand(time(NULL));
	Init();
	printf("Init vector and matrix complete: %.3f ms\n", clock() / 1000.0);

	randomAddMatrix(Matrix, SIZE); // Произвольная матрица
	printf("Random matrix complete: %.3f ms\n", clock() / 1000.0);
	//printMatrix(Matrix, SIZE);

	double** Ab_t = NULL;
	Ab_t = transposeMatrix(Matrix, SIZE); // Транспонированная матрица
	printf("Transpose matrix complete: %.3f ms\n", clock() / 1000.0);
	//printMatrix(Ab_t, SIZE);

	double** AbAb_t = NULL;
	AbAb_t = matrixMultiplication(Matrix, Ab_t, SIZE); // Положительно определенная матрица B*B'
	printf("Matrix mulyiplication complete: %.3f ms\n", clock() / 1000.0);
	//printMatrix(AbAb_t, SIZE);

	double** A = AbAb_t; // Матрица А

	randomAddVector(Vec, SIZE); // Произвольный вектор
	printf("Random vector complete: %.3f ms\n", clock() / 1000.0);
	printVector(Vec, SIZE);

	double* X = conjugateGradientSolver(A, Vec);
	printf("X time: %.3f ms\n", clock() / 1000.0);
	printVector(X, SIZE);

	double* Check = matrixMultiplicationByVector(A, X);
	printf("Check time: %.3f ms\n", clock() / 1000.0);
	printVector(Check, SIZE);

	Clear();
	system("pause");
	return 0; 
}

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

void randomAddVector(double *V) {
	for (auto i = 0; i < SIZE; i++) {
		V[i] = rand() % RAND + 1.0;
	}
}

void randomAddMatrix(double **A) {
	for (auto i = 0; i < SIZE; i++) {
		for (auto j = 0; j < SIZE; j++) {
			A[i][j] = rand() % RAND + 1.0;
		}
	}
}

void printVector(double *V) {
	for (auto i = 0; i < SIZE; i++) {
		double x = V[i];
		if (fabs(x) < NEARZERO)
			x = 0.0;
		printf("%f ", x);
	}
	printf("\n\n");
}

void printMatrix(double **A) {
	for (auto i = 0; i < SIZE; i++) {
		for (auto j = 0; j < SIZE; j++) {
			double x = A[i][j];
			if (fabs(x) < NEARZERO)
				x = 0.0;
			printf("%f ", x);
		}
		printf("\n");
	}
	printf("\n\n");
}

void init() {
	Vec = (double*)malloc(SIZE * sizeof(double));
	Matrix = (double**)malloc(SIZE * sizeof(double*));
	for (auto i = 0; i < SIZE; i++) {
		Matrix[i] = (double *)malloc(SIZE * sizeof(double));
	}
}

void clear() {
	free(Vec);
	for (auto i = 0; i < SIZE; i++) {
		free(Matrix[i]);
	}
	free(Matrix);
}

double **transposedMatrix(double **A) {
	double **C = (double**)malloc(SIZE * sizeof(double*));
	for (auto i = 0; i < SIZE; i++) {
		C[i] = (double*)malloc(SIZE * sizeof(double));
		for (auto j = 0; j < SIZE; j++) {
			C[i][j] = A[j][i];
		}
	}
	return C;
}

double **matrixMultiplication(double* A[], double* B[]) {
	double** C = (double**)malloc(SIZE * sizeof(double*));
	for (auto i = 0; i < SIZE; i++) {
		C[i] = (double*)malloc(SIZE * sizeof(double));
		for (auto j = 0; j < SIZE; j++) {
			C[i][j] = 0;
			for (auto k = 0; k < SIZE; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
}

double **matrixCombination(double **A, double **B) {
	double** C = (double**)malloc(SIZE * sizeof(double*));
	for (auto i = 0; i < SIZE; i++) {
		C[i] = (double*)malloc(SIZE * sizeof(double));
		for (auto j = 0; j < SIZE; j++) {
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
	double norma = 0.f, normaOld = 0.f;
	int k = 0;
	int N = SIZE * 10;
	while (1)
	{
		double* RSold = R;
		double* AP = matrixMultiplicationByVector(A, P);
		
		double alpha = innerProduct(R, R) / fmax(innerProduct(P, AP), NEARZERO);
		
		X = vectorCombination(X, alpha, P);
		R = vectorCombination(R, -alpha, AP);

		normaOld = norma;
		norma = vectorNorm(R);
		if ((normaOld == 0) || (norma > normaOld) || ((1 - norma / normaOld) < NEARZERO)) {
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
	init();
	printf("Init vector and matrix complete: %.3f ms\n", clock() / 1000.0);

	randomAddMatrix(Matrix); // Произвольная матрица
	printf("Random matrix complete: %.3f ms\n", clock() / 1000.0);
	//printMatrix(Matrix, SIZE);

	double **Ab_t = NULL;
	Ab_t = transposedMatrix(Matrix); // Транспонированная матрица
	printf("Transpose matrix complete: %.3f ms\n", clock() / 1000.0);
	//printMatrix(Ab_t, SIZE);

	double **AbAb_t = NULL;
	AbAb_t = matrixMultiplication(Matrix, Ab_t); // Положительно определенная матрица B*B'
	printf("Matrix multiplication complete: %.3f ms\n", clock() / 1000.0);
	//printMatrix(AbAb_t, SIZE);

	double **A = AbAb_t; // Матрица А

	randomAddVector(Vec); // Произвольный вектор
	printf("Random vector complete: %.3f ms\n", clock() / 1000.0);
	printVector(Vec);

	double *X = conjugateGradientSolver(A, Vec);
	printf("X time: %.3f ms\n", clock() / 1000.0);
	printVector(X);

	double *Check = matrixMultiplicationByVector(A, X);
	printf("Check time: %.3f ms\n", clock() / 1000.0);
	printVector(Check);

	clear();
	system("pause");
	return 0; 
}

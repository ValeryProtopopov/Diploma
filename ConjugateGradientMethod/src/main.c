#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define SIZE 10
#define RAND 10
const double NEARZERO = 1.0e-5; // Интерпретация нуля, но не ноль

double* Vec = NULL;
double** Matrix = NULL;

union converter {
	double number;
	char bytes[sizeof(double)];
};

void randomAddVector(double *V) { // Сделать вещ, -10 : +10
	for (auto i = 0; i < SIZE; i++) {
		V[i] = (2.0 * rand() / RAND_MAX - 1.0) * RAND;
	}
}

void randomAddMatrix(double **A) {
	for (auto i = 0; i < SIZE; i++) {
		for (auto j = 0; j < SIZE; j++) {
			A[i][j] = (2.0 * rand() / RAND_MAX - 1.0) * RAND;
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

double **matrixMultiplication(double *A[], double *B[]) {
	double **C = (double**)malloc(SIZE * sizeof(double*));
	for (auto i = 0; i < SIZE; i++) {
		C[i] = (double*)malloc(SIZE * sizeof(double));
		for (auto j = 0; j < SIZE; j++) {
			C[i][j] = 0;
			for (auto k = 0; k < SIZE; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
		if (i % 100 == 0) {
			printf("count: %d, time: %.3f sec\n", i , clock() / 1000.0);
		}
	}
	return C;
}

double **matrixCombination(double **A, double **B) {
	double **C = (double**)malloc(SIZE * sizeof(double*));
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
	double *C = (double*)malloc(SIZE * sizeof(double));
	for (auto i = 0; i < SIZE; i++) {
		C[i] = innerProduct(A[i], V);
	}
	return C;
}

double *vectorCombination(double U[], double alphaBeta, double V[]) // Сложение/Вычитание векторов
{
	double *C = (double*)malloc(SIZE * sizeof(double));
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

double *conjugateGradientSolver(double *A[], double B[]) {
	double *X = (double*)malloc(SIZE * sizeof(double));
	for (auto i = 0; i < SIZE; i++) {
		X[i] = 0.0;
	}
	double *R = B;
	double *P = R;
	double norma = 0.f, normaOld = 0.f;
	int k = 0;
	int N = SIZE * 10;
	while (1)
	{
		double *RSold = R;
		double *AP = matrixMultiplicationByVector(A, P);
		
		double alpha = innerProduct(R, R) / fmax(innerProduct(P, AP), NEARZERO);
		
		X = vectorCombination(X, alpha, P);
		R = vectorCombination(R, -alpha, AP);

		if (k == 0) {
			norma = vectorNorm(R);
			if (norma < NEARZERO) {
				//printf("Norma: %f; count: %d; time: %.3f\n\n", norma, k, clock() / 1000.0);
				break;
			}
		}
		else
		{
			normaOld = norma;
			norma = vectorNorm(R);
			if ((normaOld == 0) || /*(norma > normaOld) ||*/ (fabs(1 - norma / normaOld) < NEARZERO)) {
				printf("Norma: %f; count: %d; time: %.3f\n\n", norma, k, clock() / 1000.0);
				break;
			}
		}
		double beta = innerProduct(R, R) / fmax(innerProduct(RSold, RSold), NEARZERO);
		P = vectorCombination(R, beta, P);

		if (k > 0 && k % 1000 == 0) {
			//printf("count: %dk, time: %.3f sec. Norma: %f\n", k / 1000, clock() / 1000.0, norma);
		}

		k++;
	}
	return X;
}

void writeToBin(double **A, double *B, int size, char *vectorFileName, char *matrixFileName) {
	FILE *outputVector, *outputMatrix;
	if ((fopen_s(&outputVector, vectorFileName, "wb")) != 0) {
		printf("The file 'vector.bin' was not opened\n");
	}
	else
	{
		union converter itemVector;
		itemVector.number = (double)size;
		fwrite(itemVector.bytes, sizeof(union converter), 1, outputVector);
		
		// Запись вектора
		for (int i = 0; i < size; i++) {
			itemVector.number = B[i];
			fwrite(itemVector.bytes, sizeof(union converter), 1, outputVector);
		}
		fclose(outputVector);
	}

	if ((fopen_s(&outputMatrix, matrixFileName, "wb")) != 0) {
		printf("The file 'matrix.bin' was not opened\n");
	}
	else
	{
		union converter itemMatrix;
		itemMatrix.number = (double)size;
		fwrite(itemMatrix.bytes, sizeof(union converter), 1, outputMatrix);

		// Запись матрицы
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				itemMatrix.number = A[i][j];
				fwrite(itemMatrix.bytes, sizeof(union converter), 1, outputMatrix);
			}
		}
		fclose(outputMatrix);
	}
}

void readFromBin(char *vectorFileName, char *matrixFileName) {
	FILE *inputVector, *inputMatrix;
	double *vector, **matrix;
	int size;
	inputVector = fopen(vectorFileName, "rb");
	if (inputVector == NULL) {
		printf("The file 'vector.bin' was not opened\n");
	}
	else
	{
		union converter itemVector;
		fread(itemVector.bytes, sizeof(union converter), 1, inputVector);
		size = (int)itemVector.number;
		vector = (double*)malloc((size) * sizeof(double));
		for (int i = 0; i < size; i++) {
			fread(itemVector.bytes, sizeof(union converter), 1, inputVector);
			vector[i] = itemVector.number;
		}
		Vec = vector;
		fclose(inputVector);
	}

	inputMatrix = fopen(matrixFileName, "rb");
	if (inputMatrix == NULL) {
		printf("The file 'matrix.bin' was not opened\n");
	}
	else
	{
		union converter itemMatrix;
		fread(itemMatrix.bytes, sizeof(union converter), 1, inputMatrix);
		size = (int)itemMatrix.number;		
		matrix = (double**)malloc((size) * sizeof(double*));
		for (auto i = 0; i < size; i++) {
			matrix[i] = (double*)malloc((size) * sizeof(double));
			for (int j = 0; j < size; j++) {
				fread(itemMatrix.bytes, sizeof(union converter), 1, inputMatrix);
				matrix[i][j] = itemMatrix.number;
			}
		}
		//printMatrix(matrix);
		Matrix = matrix;
		fclose(inputMatrix);
	}
}

char *getFileName(char *name) {
	char *fileWay = "bin/res/", fileSize[6], *slash = "/", *fileFormat = ".bin";
	sprintf(fileSize, "%d", SIZE);
	char *fileName = (char*)calloc(30, sizeof(char));
	if (!fileName) {
		printf("Error");
		return 0;
	}
	strcat(fileName, fileWay);
	strcat(fileName, name);
	strcat(fileName, slash);
	strcat(fileName, fileSize);
	strcat(fileName, fileFormat);
	return fileName;
}

int main() {
	srand(time(NULL));
	printf("Size: %i\n", SIZE);
	init();
	//printf("Init vector and matrix complete: %.3f sec\n", clock() / 1000.0);

	//randomAddMatrix(Matrix); // Произвольная матрица
	//printf("Random matrix complete: %.3f sec\n", clock() / 1000.0);
	//printMatrix(Matrix);

	//double **Ab_t = NULL;
	//Ab_t = transposedMatrix(Matrix); // Транспонированная матрица
	//printf("Transpose matrix complete: %.3f sec\n", clock() / 1000.0);
	//printMatrix(Ab_t);

	//double **AbAb_t = NULL;
	//AbAb_t = matrixMultiplication(Matrix, Ab_t); // Положительно определенная матрица B*B'
	//printf("Matrix multiplication complete: %.3f sec\n", clock() / 1000.0);
	//printMatrix(AbAb_t);

	//double **A = AbAb_t; // Матрица А
	//printMatrix(A);

	//randomAddVector(Vec); // Произвольный вектор
	//printf("Random vector complete: %.3f sec\n", clock() / 1000.0);
	//printVector(Vec);

	char *vectorFileName = getFileName("vector");
	char *matrixFileName = getFileName("matrix");
	//writeToBin(A, Vec, (int)SIZE, vectorFileName, matrixFileName);
	readFromBin(vectorFileName, matrixFileName);
	//printVector(Vec);
	//printMatrix(Matrix);
	
	double *X = conjugateGradientSolver(Matrix, Vec);
	printf("X time: %.3f sec\n", clock() / 1000.0);
	//printVector(X);

	double *Check = matrixMultiplicationByVector(Matrix, X);
	printf("Check time: %.3f sec\n", clock() / 1000.0);
	//printVector(Check);

	clear();
	system("pause");
	return 0; 
}

#define N 10
#define MAX 10
#define i_j N * i + j
#define j_i N * j + i

#include <ctime>
#include <fstream>

union converter {
	double number;
	char bytes[sizeof(double)];
};

using namespace std;

double get_rand(void) {
	return 2.0 * MAX * rand() / RAND_MAX - MAX;
}

void fill_block(double* block, const int n) {
	for (unsigned int i = 0; i < n; ++i) {
		block[i] = get_rand();
	}
}

int main(void) {
	// Allocate memory in heap
	double* matrix = new double[N * N];
	double* solution = new double[N];
	double* result = new double[N];

	// Open output files
	ofstream out_matrix("a.bin", ofstream::binary | ofstream::out);
	ofstream out_solution("x.bin", ofstream::binary | ofstream::out);
	ofstream out_result("b.bin", ofstream::binary | ofstream::out);
	
	if (out_matrix.is_open()  && out_solution.is_open() && out_result.is_open()) {
		converter item_matrix, item_solution, item_result;

		// Random data generation
		srand(time(NULL));
		fill_block(matrix, N);
		fill_block(solution, N);
		
		for (unsigned int i = 0; i < N; ++i) {
			item_solution.number = solution[i];
			out_solution.write(item_solution.bytes, sizeof(converter));
			
			item_result.number = 0;
			for (unsigned int j = 0; j < N; ++j) {
				item_matrix.number = 0;
				for (unsigned int k = 0; k < N; ++k) {
					// Make matrix positive
					item_matrix.number += matrix[i_j] * matrix[j_i];
				}
				out_matrix.write(item_matrix.bytes, sizeof(converter));

				// Matrix * solution => result
				item_result.number += item_matrix.number * solution[j];
			}
			out_result.write(item_result.bytes, sizeof(converter));
		}

		out_matrix.close();
		out_solution.close();
		out_result.close();
	}

	// Free memory in heap
	delete[] matrix;
	delete[] solution;
	delete[] result;
	
	return 0;
}

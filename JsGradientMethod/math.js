const { PerformanceObserver, performance } = require('perf_hooks');
const fs = require('fs');
const readline = require('readline');

const SIZE = 100;
const BUFFER_SIZE = 8 * SIZE;

const NEARZERO = 1.0e-5;
let Vec = null;
let Matrix = null;

function randomAddVector(vector) {
    for (let i = 0; i < SIZE; i++) {
        vector[i] = (Math.random() * 10).toFixed(6);
    }
}

function randomAddMatrix(matrix) {
    for (let i = 0; i < SIZE; i++) {
        for (let j = 0; j < SIZE; j++) {
            matrix[i][j] = (Math.random() * 10).toFixed(6);
        }
    }
}

function printVector(vector) {
    for (let i = 0; i < SIZE; i++) {
        let x = vector[i];
        if (Math.abs(x) < NEARZERO)
            x = 0.0;
        process.stdout.write(x + " ");
    }
    console.log("\n");
}

function printMatrix(matrix) {
    for (let i = 0; i < SIZE; i++) {
        for (let j = 0; j < SIZE; j++) {
            let x = matrix[i][j];
            if (Math.abs(x) < NEARZERO)
                x = 0.0;
            process.stdout.write(x + " ");
        }
        console.log("");
    }
    console.log("\n");
}

function init() {
    Vec = new Float64Array(SIZE);
    Matrix = new Array(SIZE);
    for (let i = 0; i < SIZE; i++) {
        Matrix[i] = new Float64Array(SIZE);
    }
}

function clear() {
    Vec = [];
    Matrix = [];
}

function transposedMatrix(A) {
    let C = new Array(SIZE);
    for (let i = 0; i < SIZE; i++) {
        C[i] = new Float64Array(SIZE);
        for (let j = 0; j < SIZE; j++) {
            C[i][j] = A[j][i];
        }
    }
    return C;
}

function matrixMultiplication(A, B) {
    let C = new Array(SIZE);
    for (let i = 0; i < SIZE; i++) {
        C[i] = new Float64Array(SIZE);
        for (let j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (let k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

function innerProduct(U, V) // Скалярное произведение
{
    let result = 0;
    for (let i = 0; i < SIZE; i++) {
        result = result +  U[i] * V[i];
    }
    return result;
}

function matrixMultiplicationByVector(A, V) // Умножение матрицы на вектор
{
    let C = new Array(SIZE);
    for (let i = 0; i < SIZE; i++) {
        C[i] = innerProduct(A[i], V);
    }
    return C;
}

function vectorCombination(U, alphaBeta, V) // Сложение/Вычитание векторов
{
    let C = new Array(SIZE);
    for (let i = 0; i < SIZE; i++) {
        C[i] = U[i] + alphaBeta * V[i];
    }
    return C;
}

function vectorNorm(V) { // Норма вектора
    let max = 0;
    for (let i = 0; i < SIZE; i++) {
        if (Math.abs(V[i]) > max) {
            max = Math.abs(V[i]);
        }
    }
    return max;
}

function conjugateGradientSolver(A, B) {
    let X = new Float64Array(SIZE);
    for (let i = 0; i < SIZE; i++) {
        X[i] = 0.0;
    }
    let R = B;
    let P = R;
    let norma = 0.0, normaOld = 0.0;
    let k = 0;
    let N = SIZE * 10;
    while (1)
    {
        let RSold = R;
        let AP = matrixMultiplicationByVector(A, P);

        let alpha = innerProduct(R, R) / Math.max(innerProduct(P, AP), NEARZERO);

        X = vectorCombination(X, alpha, P);
        R = vectorCombination(R, -alpha, AP);

        if (k === 0) {
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
            if ((normaOld === 0) || /*(norma > normaOld) ||*/ (Math.abs(1 - norma / normaOld) < NEARZERO)) {
                let t1 = performance.now();
                console.log(`Norma: ${norma}, count: ${k}, time: ${t1} ms`);
                break;
            }
        }
        let beta = innerProduct(R, R) / Math.max(innerProduct(RSold, RSold), NEARZERO);
        P = vectorCombination(R, beta, P);

        if (k % 1000 === 0) {
            let t1 = performance.now();
            console.log(`count: ${k}, time: ${t1} ms`);
        }

        k++;
    }
    return X;
}

// function readFromBin(filename) {
//     let vector, matrix;
//     let firstItemSize = 8;
//
//     fs.open(filename, 'r', function(err, fd) {
//         if(err) {
//             return console.error(err);
//         }
//         let firstItem = new Buffer.alloc(firstItemSize);
//         fs.read(fd, firstItem, 0, firstItemSize, 0, function(err, bytesRead) {
//             if(err) {
//                 return console.error(err);
//             }
//             let size = firstItem.readDoubleLE(0);
//             let position = 0;
//             let length = 16;
//             vector = new Float64Array(size);
//             console.log(firstItem.readDoubleLE(0));
//             for (let i = 0; i < size; i++) {
//                 position += 8;
//                 let buffer = new Buffer.alloc(length);
//                 fs.read(fd, buffer, 0, length, position, function(err, bytesRead) {
//                     if(err) {
//                         return console.error(err);
//                     }
//                     vector[i] = buffer.readDoubleLE(0);
//                     // console.log(firstItem.readDoubleLE(8));
//                 });
//             }
//         });
//     });
// }

function readVectorFromBinSync(filename) {
    let vector;
    let firstItemSize = 8;

    const file = fs.openSync(filename, 'r');
    if (!file) {
        console.log('cannot open file');
    } else {
        let firstItem = new Buffer.alloc(firstItemSize);
        fs.readSync(file, firstItem, 0, 8, 0);
        console.log(firstItem.readDoubleLE(0));
        let size = firstItem.readDoubleLE(0);
        let position = 0;
        let length = 16;
        let buffer = new Buffer.alloc(length);
        vector = new Float64Array(size);
        for (let i = 0; i < size; i++) {
            position += 8;
            fs.readSync(file, buffer, 0, length, position);
            vector[i] = buffer.readDoubleLE(0);
        }
        // printVector(vector);
        Vec = vector;
    }
}

function readMatrixFromBinSync(filename) {
    let matrix;
    let firstItemSize = 8;

    const file = fs.openSync(filename, 'r');
    if (!file) {
        console.log('cannot open file');
    } else {
        let firstItem = new Buffer.alloc(firstItemSize);
        fs.readSync(file, firstItem, 0, 8, 0);
        // console.log(firstItem.readDoubleLE(0));
        let size = firstItem.readDoubleLE(0);
        let position = 0;
        let length = 16;
        let buffer = new Buffer.alloc(length);
        matrix = new ArrayBuffer(size);
        for (let i = 0; i < size; i++) {
            matrix[i] = new Float64Array(size);
            for (let j = 0; j < size; j++) {
                position += 8;
                fs.readSync(file, buffer, 0, length, position);
                matrix[i][j] = buffer.readDoubleLE(0);
            }
        }
        // printMatrix(matrix);
        Matrix = matrix
    }
}


function main() {
    init();
    readVectorFromBinSync('100.bin');
    readMatrixFromBinSync('100 (2).bin');
    console.log("Size: ", SIZE);
    // let t0 = performance.now();
    // init();
    let t1 = performance.now();
    // console.log("Init vector and matrix complete: ", t1 , " ms");
    // console.log("SIZE: ", SIZE);
    //
    // // t0 = performance.now();
    // randomAddMatrix(Matrix); // Произвольная матрица
    // t1 = performance.now();
    // console.log("Random matrix complete: ", t1 , " ms");
    // // printMatrix(Matrix);
    //
    // let Ab_t;
    // // t0 = performance.now();
    // Ab_t = transposedMatrix(Matrix);
    // t1 = performance.now();
    // console.log("Transpose matrix complete: ", t1 , " ms");
    // // printMatrix(Ab_t);
    //
    // let AbAb_t;
    // // t0 = performance.now();
    // AbAb_t = matrixMultiplication(Matrix, Ab_t);
    // t1 = performance.now();
    // console.log("Matrix multiplication complete: ", t1 , " ms");
    // // printMatrix(AbAb_t);
    //
    // let A = AbAb_t;
    // // t0 = performance.now();
    // randomAddVector(Vec); // Произвольный вектор
    // t1 = performance.now();
    // console.log("Random vector complete: ", t1 , " ms");
    // printVector(Vec);
    // setTimeout(printVector, 10000, Vec);
    //
    // // t0 = performance.now();
    let X = conjugateGradientSolver(Matrix, Vec);
    t1 = performance.now();
    console.log("X time: ", t1 , " ms");
    // printVector(X);

    // t0 = performance.now();
    let Check = matrixMultiplicationByVector(Matrix, X);
    t1 = performance.now();
    console.log("Check time: ", t1 , " ms");
    // printVector(Check);

    clear();
}

main();
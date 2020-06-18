let SIZE = 0;
const NEARZERO = 1.0e-5;

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

export function conjugateGradientSolver(A, B, size) {
    console.log('A: ', A);
    console.log('B: ', B);
    SIZE = size;
    console.log('size: ', SIZE);
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
                console.log(`Norma: ${norma}, count: ${k}`);
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
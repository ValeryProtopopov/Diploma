import { conjugateGradientSolver } from './gradient.js'

function arrayToPtr(array, nByte, size) {
    let ptr = _malloc(size * nByte);
    HEAPF64.set(array, ptr / nByte);
    return ptr;
}

export function ptrToArray(ptr, length, nByte) {
    let array = new Float64Array(length);
    let pos = ptr / nByte;
    array.set(HEAPF64.subarray(pos, pos + length));
    return array;
}

export function calcWasm(subtask, socket) {
    console.log('subtask', subtask);
    let nByte = 8;
    console.log('nByte', nByte);
    let innerProduct = cwrap('innerProduct', 'number', ['Float64Array', 'Float64Array', 'number']);

    console.log('subtask ',subtask);
    let ptrVector = arrayToPtr(subtask.vector, nByte, subtask.size);
    console.log('ptrVector', ptrVector);
    let ptrMtrixLine = arrayToPtr(subtask.matrixLine, nByte, subtask.size);
    console.log('ptrMtrixLine', ptrMtrixLine);

    let result = innerProduct(ptrVector, ptrMtrixLine, subtask.size);
    _free(ptrVector);
    _free(ptrMtrixLine);
    socket.emit('send result',{result: result} );
}

export function calcJs(subtask) {
    let t0 = performance.now();
    let result = conjugateGradientSolver(subtask.matrix, subtask.vector, subtask.size);
    let t1 = performance.now();
    console.log('Time of task: ', t1-t0 , " ms");
    console.log(result);
}
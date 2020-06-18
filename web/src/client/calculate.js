function arrayToPtr(array, nByte) {
    let ptr = _malloc(array.length * nByte);
    HEAPF64.set(array, ptr / nByte);
    return ptr;
}

export function ptrToArray(ptr, length, nByte) {
    let array = new Float64Array(length);
    let pos = ptr / nByte;
    array.set(HEAPF64.subarray(pos, pos + length));
    return array;
}

export function calc(subtask) {
    console.log('subtask', subtask);
    let nByte = 16;
    let innerProduct = cwrap('innerProduct', 'Float64Array', ['Float64Array', 'Float64Array', 'number']);

    let ptrVector = arrayToPtr(subtask.vector, nByte);
    console.log('ptrVector', ptrVector);
    let ptrMtrixLine = arrayToPtr(subtask.matrixLine, nByte);
    console.log('ptrMtrixLine', ptrMtrixLine);
    let result = innerProduct(ptrVector, ptrMtrixLine, subtask.vector.length);
    console.log(result);
}
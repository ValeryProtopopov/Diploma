import fs from 'fs';
import { Pool, Vector, Matrix } from "./models/Pool.js";

export function readVectorFromBin(filename) {
    let vector;
    let firstItemSize = 8;

    fs.open(filename, 'r', function(err, fd) {
        if(err) {
            return console.error(err);
        }
        let firstItem = new Buffer.alloc(firstItemSize);
        fs.read(fd, firstItem, 0, firstItemSize, 0, function(err, bytesRead) {
            if(err) {
                return console.error(err);
            }
            let size = firstItem.readDoubleLE(0);
            let position = 0;
            let length = 16;
            vector = new Float64Array(size);
            console.log(firstItem.readDoubleLE(0));
            for (let i = 0; i < size; i++) {
                position += 8;
                let buffer = new Buffer.alloc(length);
                fs.read(fd, buffer, 0, length, position, function(err, bytesRead) {
                    if(err) {
                        return console.error(err);
                    }
                    vector[i] = buffer.readDoubleLE(0);
                    // console.log(firstItem.readDoubleLE(8));
                });
            }
            Vector.push(vector);
        });
    });
}

export function readMatrixFromBin(filename, page) {
    let vector;
    let firstItemSize = 8;
    let position = page === 0 ? 0 : page * 8 * 10;

    fs.open(filename, 'r', function(err, fd) {
        if(err) {
            return console.error(err);
        }
        let firstItem = new Buffer.alloc(firstItemSize);
        fs.read(fd, firstItem, 0, firstItemSize, 0, function(err, bytesRead) {
            if(err) {
                return console.error(err);
            }
            let size = firstItem.readDoubleLE(0);
            let length = 16;
            vector = new Float64Array(size);
            for (let i = 0; i < size; i++) {
                position += 8;
                let buffer = new Buffer.alloc(length);
                fs.read(fd, buffer, 0, length, position, function(err, bytesRead) {
                    if(err) {
                        return console.error(err);
                    }
                    vector[i] = buffer.readDoubleLE(0);
                    // console.log(firstItem.readDoubleLE(8));
                });
            }
            Matrix.push(vector);
        });
    });
}
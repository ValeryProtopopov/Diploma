import _ from 'lodash'
import Server from 'socket.io';
import server from './server.js'

import { Pool, Vector, Matrix, Result } from "./models/Pool.js";
import * as Master from "./master.js"
import config from './config.js'

const io = new Server(server);
const size = 10;
Master.readVectorFromBin(config.binaryFileDir +`vector/${size}.bin`);
// for (let i = 0; i < size; i++) {
//     Master.readMatrixFromBin(config.binaryFileDir +`vector/${size}.bin`, i, size);
// }
let test = new Array(size);
for (let i = 0; i < size; i++) {
    test[i] = new Float64Array(size);
}
test = Master.readMatrixFromBinSync(config.binaryFileDir + `matrix/${size}.bin`);



let connections = [];

io.on('connection', (socket) => {
    console.log("Успешное соединение", socket.id);
    connections.push(socket);

    socket.on('disconnect', function(data) {
        connections.splice(connections.indexOf(socket), 1);
        console.log("Отключились");
    });

    socket.on('get subtask', function (data) {
        let msg;
        let subTask = {};
        if (!_.isEmpty(Pool) ) {
            subTask.vector = Pool.vector;
            subTask.matrixLine = Pool.matrix;
            subTask.size = size;
        } else {
            msg = "Пока нет задач для решения"
        }

        socket.emit('send subtask', {subtask: subTask, msg: msg});
    });

    socket.on('get subtask js', function (data) {
        let msg;
        let subTask = {};
        if (!_.isEmpty(Vector) && !_.isEmpty(test)) {
            subTask.vector = Vector[0];
            subTask.matrix = test;
            subTask.size = size;
            console.log(subTask);
        } else {
            msg = "Пока нет задач для решения"
        }

        socket.emit('send subtask js', {subtask: subTask, msg: msg});
    });

    socket.on('get matrix line', function (data) {
        const matrixLine = [3,4];
        socket.emit('send matrix line', {matrixLine: matrixLine});
    });

    socket.on('get vector', function (data) {
        const vector = [1,2];
        socket.emit('send vector', {vector: vector});
    });

    socket.on('send result', function (data) {
        console.log(data.result);
        if (data.result)
            Result.push(data.result);
    });
});

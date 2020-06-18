import _ from 'lodash'
import Server from 'socket.io';
import server from './server.js'

import { Pool, Vector, Matrix } from "./models/Pool.js";
import * as Master from "./master.js"
import config from 'config.js'

const io = new Server(server);

Master.readVectorFromBin(config.binaryFileDir +'vector/10.bin');
for (let i = 0; i < 10; i++) {
    Master.readMatrixFromBin(config.binaryFileDir + 'matrix/10.bin', i);
}


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
        if (!_.isEmpty(Vector) && !_.isEmpty(Matrix)) {
            subTask.vector = Vector[0];
            subTask.matrixLine = Matrix[0];
            console.log(subTask);
        } else {
            msg = "Пока нет задач для решения"
        }

        socket.emit('send subtask', {subtask: subTask, msg: msg});
    });

    socket.on('get matrix line', function (data) {
        const matrixLine = [3,4];
        socket.emit('send matrix line', {matrixLine: matrixLine});
    });

    socket.on('get vector', function (data) {
        const vector = [1,2];
        socket.emit('send vector', {vector: vector});
    });
});

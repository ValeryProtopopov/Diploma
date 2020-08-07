import Express from 'express';
import http from 'http';
import io from 'socket.io';
import path from 'path';
import { SubTask } from "./models/SubTask.js";

// import { sumElementsOfArray, processLineByLine, splitTask } from './util.js'
// import { Task } from "./models/Task.js";
// import Queue from "./models/Queue.js";

const app = new Express();
const server = http.createServer(app);
const sockets = io(server);

const port = 3000;
const __dirname = path.resolve();

app.use(Express.static(__dirname + '/views'));

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/views/index.html');
});

app.get('/contributors', (req, res) => {
    res.sendFile(__dirname + '/views/contributors.html');
});

app.get('/about', (req, res) => {
    res.sendFile(__dirname + '/views/about.html');
});

// app.get('/download', (req, res) => {
//     const file = __dirname + '/views/tasks/matrix.txt';
//     res.download(file); // Set disposition and send it.
// });

let connections = [];

// let task = new Task(1, 'Скалярное произведение матриц 3 на 3.', 0, './upload/tasks/1/');
// let subTasks = splitTask(task);
// let queue = Queue.push(subTasks);



sockets.on('connection', (socket) => {
    console.log("Успешное соединение", socket.id);
    connections.push(socket);

    socket.on('disconnect', function(data) {
        connections.splice(connections.indexOf(socket), 1);
        console.log("Отключились");
    });

    socket.on('get subtask', function (data) {
        let subTask = new SubTask(1, 1, 'Скалярное прозведение 1 строки матрицы и ветора', 1, 1, 0)
        sockets.emit('send subtask', {subtask: subTask});
    });

    // socket.on('get matrix line', function (data) {
    //     const matrixLine = [1,2];
    //     sockets.emit('send matrix line', {matrixLine: matrixLine});
    // });
    //
    // socket.on('get vector line', function (data) {
    //     const vector = [1,2];
    //     sockets.emit('send matrix line', {vector: vector});
    // });
    //
    // socket.emit('send result', function (data) {
    //     console.log(data);
    // });
});

server.listen(port, () => {
    console.log('Server start at port: ' + port);
});

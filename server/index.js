import Express from 'express';
import http from 'http';
import io from 'socket.io';
import path from 'path';

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

let connections = [];

sockets.on('connection', (socket) => {
    console.log("Успешное соединение");
    connections.push(socket);

    socket.on('disconnect', function(data) {
        connections.splice(connections.indexOf(socket), 1);
        console.log("Отключились");
    });
});

server.listen(port, () => {
    console.log('Server start at port: ' + port);
});

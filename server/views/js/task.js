import _ from 'lodash';

export function getSubTask(socket) {
    socket.emit('get subtask');

    socket.on('send subtask', function (data) {
        if (!_.isEmpty(data.subtask)) {
            $('#answer').append('<div>' + data.subtask + '</div>');
            getMatixLine(socket, data.subtask.getId());
            getVector(socket, data.subtask.getId());
        } else {
            console.log('data is empty');
        }
    });
}

export function getMatixLine(socket, subTaskId) {
    socket.emit('get matrix line', {subTaskId: subTaskId});

    socket.on('send matrix line', function (data) {
        $('#answer').append('<div>' + data.matrixLine + '</div>')
    });
}

export function getVector(socket, subTaskId) {
    socket.emit('get vector line', {subTaskId: subTaskId});

    socket.on('send vector line', function (data) {
        $('#answer').append('<div>' + data.vector + '</div>')
    })
}

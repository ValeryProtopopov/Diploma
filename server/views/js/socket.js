import { getSubTask } from './task.js'

function getRandomFloat(max) {
    return (Math.random() * 10).toFixed(4);
}

$(function () {
    let socket = io();
    // setTimeout(() => {
    //     socket.emit('send mess', getRandomFloat(3))
    // }, 1000);

    // socket.on('scalar product', function (data) {
    //     $('#answer').append('<div>' + data.answer + '</div>')
    // })

    setTimeout(() => {
        getSubTask(socket)
    }, 1000);

    // socket.on('send subtask', function (data) {
    //     if (Object.keys(data.subtask).length === 0 && data.task.constructor === Object) {
    //         $('#matrix').append('<div>' + data.task.line + '</div>');
    //         $('#vector').append('<div>' + data.vector + '</div>')
    //     } else {
    //         $('#matrix').append('<div>task empty</div>');
    //     }
    // })
});
import _ from 'lodash';
import * as calculate from './calculate.js'

export let subTask = {};
export let matrixLine = {};
export let vector = {};


export function getSubTask(socket) {
    socket.emit('get subtask');

    socket.on('send subtask', function (data) {
        if (!_.isEmpty(data.subtask)) {
            calculate.calcWasm(data.subtask);

        } else {
            console.log(data.msg);
        }
    });
}

export function getSubTaskJS(socket) {
    socket.emit('get subtask js');

    socket.on('send subtask js', function (data) {
        console.log('data', data);
        if (!_.isEmpty(data.subtask)) {
            calculate.calcJs(data.subtask);
        } else {
            console.log(data.msg);
        }
    });
}
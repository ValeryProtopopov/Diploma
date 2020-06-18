import * as controller from './controller.js'
import * as calculate from './calculate.js'

$(function () {
    let socket = io();

    const startFunc =  $('#start');
    if (!startFunc) {
        throw new Error("no found start button object!");
    }
    startFunc.on('click', function(){
        // let inArray = new Float64Array([2.11111111, 2.1111111]);
        controller.getSubTask(socket);
    });

});
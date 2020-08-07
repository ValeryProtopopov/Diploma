import * as controller from './controller.js'

$(function () {
    let socket = io();

    const startWasm =  $('#startWasm');
    if (!startWasm) {
        throw new Error("no found start wasm button object!");
    }
    startWasm.on('click', function(){
        controller.getSubTask(socket);
    });


    const startJs =  $('#startJs');
    if (!startJs) {
        throw new Error("no found start js button object!");
    }
    startJs.on('click', function(){
        controller.getSubTaskJS(socket);
    });
});
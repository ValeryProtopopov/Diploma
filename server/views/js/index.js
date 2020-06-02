async function start() {
    WebAssembly.instantiateStreaming(fetch('wasm/program.wasm'), [])
        .then(obj => {
            console.log(obj.instance.exports.fact(4));
        });
}
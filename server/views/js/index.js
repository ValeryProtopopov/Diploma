async function start() {
    const wasmImports = {
       
    };
    WebAssembly.instantiateStreaming(fetch('wasm/test.wasm'), wasmImports)
        .then(obj => {
        	var SIZE = 1000;
			var Vec = new Float64Array(obj.instance.exports.memory.buffer, 0, SIZE);
			const Matrix = [];
			for(i = 0; i < SIZE; i++){
				Matrix[i] = new Float64Array(obj.instance.exports.memory.buffer, 0, SIZE);
			}
			function rand(min,max){
			    return Math.floor(Math.random() * (max + 1 - min) + min);
			}

			function randomAddVector(V){
				for(i = 0; i < SIZE; i++){
			    	V[i] = rand(0, 10);
			  	}
			}

			function randomAddMatrix(A){
			  	for(i = 0; i < SIZE; i++){
			    	A[i] = [];
			    	for(j = 0; j < SIZE; j++){
			      		A[i][j] = rand(0, 10);
			    	}
			  	}
			}
			randomAddVector(Vec);
			randomAddMatrix(Matrix);
			var t0 = performance.now();
            console.log(obj.instance.exports.innerProduct(Vec, Vec));
            var t1 = performance.now();
            console.log('Execution time: ' + (t1 - t0).toFixed(4) + " ms");
        });}
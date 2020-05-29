function fib(n) {
	var startTime = getTime();

	var a = 1, b = 1;
	if (n <= 1) {
		document.forms[0].result.value = n;
	}
	else{
		for (var i = 3; i <= n; i++) {
			var c = a + b;
	    	a = b;
	   		b = c;
	 	}         
		document.forms[0].result.value = b;
	}

	var endTime = getTime();
	document.forms[0].timer.value = (endTime - startTime)/1000 + " ms";	
}

function getTime(){
	const time = new Date().getTime();
	return time;
}
// This script contains set of functions used in other scripts.

//a function to compute the data for a line
var line = function(f, a, deltax) {
    var deltay = f(a + deltax) - f(a);
    return [a, f(a), deltax, deltay];
};


//a function for returning the zero
//apply the output of line to this
var xinter = function(a, c, deltax, deltay) {
    return (-c * deltax) / deltay + a;
};

//core Newton's method
var rawNewton = function(guess, f, leftBound, rightBound, dx, minXdist, minYdist, exit) {
    //paste default: 1e-10, 1e-10, 1e-10, 10
    var sequencePoints = [[guess, f(guess)]];
    var lineData, prevGuess, y;
    y = f(guess);
    //newton loop
    for (exit; exit > 0; exit -= 1) {
        lineData = line(f, guess, dx);
        prevGuess = guess;
        y = lineData[1];
        guess = xinter.apply(this, lineData);

	if ((guess < leftBound - dx) || (guess > rightBound + dx)) {
            return [false, "Out of Bounds:" + guess, sequencePoints];
        }
        //add (a,f(a))
        sequencePoints.push([guess, f(guess)]);
        if ((Math.abs(guess - prevGuess) < minXdist) && (Math.abs(y) < minYdist)) {
            return [true, guess, sequencePoints];
        }
    }
    //did not meet the required distances
    return [false, "Did not converge", sequencePoints];
};

//a wrapper for list of defaults
var newton = function(guess, f, leftBound, rightBound, dx, minXdist, minYdist, exit) {
    if (!!!dx) {
      dx = 1e-10;
    }
    if (!!!minXdist) {
      minXdist = 1e-10;
    }
    if (!!!minYdist) {
      minYdist = 1e-10;
    }
    if (!!!exit) {
      exit = 10;
    }
  
    return rawNewton.call(this, guess, f, leftBound, rightBound, dx, minXdist, minYdist, exit);
};

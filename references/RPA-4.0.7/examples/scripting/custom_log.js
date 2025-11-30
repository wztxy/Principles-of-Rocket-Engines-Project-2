/***************************************************
 RPA - tool for rocket propulsion analysis.

 This script demonstrates how to write results in required
 format into the file "log.txt".
****************************************************/

// Open the  file "log.txt" in the mode "w" ("write")
var f = new File("log.txt", "w"); 

load("newton.js");

// Returns mixture with products of hydrazine decomposition
// at given decomposition rate x.
function getDecompositionMixture(x) {
	n_NH3 = 4*(1-x);
	n_N2 = 1+2*x;
	n_H2 = 6*x;
	mix = new Mixture("mole");
	mix.addSpecies("NH3", n_NH3);
	mix.addSpecies("N2", n_N2);
	mix.addSpecies("H2", n_H2);
	mix.checkFractions(true);
	return mix;
}

// Estimates the temperature of hydrazine decomposition at given decomposition rate.
// x - decomposition rate of hydrazine
// returns an array:
// 	index 0 - mixture with products of hydrazine decomposition
// 	index 1 - temperature of products of hydrazine decomposition
function getDecompositionT(H_N2H4L, x, p, p_units) {
	// Get supposed decomposition mixture at decomposition rate x
	mix = getDecompositionMixture(x);
	mix.setP(p, p_units);

	// Find the temperature of decomposition products
	Tmix = 1000;
	results = newton(
		1000, 					// Initial guess = 1000 K
		function(T) {			// Control function
			Tmix = T;
			mix.setT(T, "K");
			return mix.getH("kJ/kg") - H_N2H4L;
		},
		300, 						// leftBound = 400 K
		1600, 					// rightBound = 1600 K
		1e-3, 					// step
		1e-3, 					// precision
		1e-3						// precision
	);

	return [mix, Tmix];
}


// Get enthalpy of N2H4(L)
N2H4L = new Mixture();
N2H4L.addSpecies("N2H4(L)", 1);
H_N2H4L = N2H4L.getH("kJ/kg");

p = 2.0;
p_units = "MPa";

// Test decomposition rates
x = [0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 0.99, 1.00];

f.printf("------------------------------------------\n");
f.printf("Estimates the temperature of hydrazine decomposition at given decomposition rates x\n");
f.printf("H_N2H4L=%8.2f kJ/kg\n", H_N2H4L);
f.printf("------------------------------------------\n");
f.printf("x     T [K]    H [kJ/kg] R [J/(kg K)] k\n");
f.printf("------------------------------------------\n");

for (i=0; i<x.length; ++i) {
	result = getDecompositionT(H_N2H4L, x[i], p, p_units);

	mix = result[0];
	T = result[1];
	H = result[0].getH("kJ/kg");
	R = 8314.472/mix.getM();
	Cp	= 0;
	for (j=0; j<result[0].size(); ++j) {
		s = mix.getSpecies(j);
		Cp += mix.getFraction(j, "mole") * s.getCp(T, "K");
	}
	Cv = Cp - 8.314472;
	k = Cp/Cv;

	f.printf("%4.2f %8.2f %8.2f %8.2f       %4.2f\n", x[i], T, H, R, k);
}

f.printf("------------------------------------------\n");

// Close the file
f.close();

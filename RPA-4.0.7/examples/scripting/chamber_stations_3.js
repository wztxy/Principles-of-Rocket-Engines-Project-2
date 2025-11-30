// Load functions to print out nozzle station parameters
load("chamber_station_functions.js");

pc = new Object();
pc.propellant = new Propellant(6);
pc.propellant.addOxidizer("O2(L)", 1.0);
pc.propellant.addFuel("H2(L)", 1.0);
pc.optimize = false;

cc = new Object();
cc.pc = 10;
cc.pc_unit = "MPa"
cc.ions = true;
cc.multiphase = false;
cc.condensed = false;
cc.solveNozzleFlow = true;
cc.nozzleInlet_Ar = 4;
cc.freezing_Ar = 2;
cc.nozzleExit_Ar = 60;
cc.solveOverexpation = true;
cc.pa = 1;
cc.pa_unit = "atm";

tp = new TheoreticalPerformance(pc, cc);
tp.solve();

ch = tp.getChamber();
ch.assignCorrectionFactors(0.99, 0.98, 0.99, 1.0);

UPSTREAM_OF_THE_THROAT = false;
DOWNSTREAM_OF_THE_THROAT = true;

printNozzleStation(ch.getNozzleStation(4, "Ar", UPSTREAM_OF_THE_THROAT, true, true));    // Nozzle inlet

printNozzleStation(ch.getNozzleStation(2, "Ar", UPSTREAM_OF_THE_THROAT, true, true));    // Station between nozzle inlet and throwt

printNozzleStation(ch.getNozzleStation(1, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true));     // Nozzle throat

printNozzleStation(ch.getNozzleStation(4, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true));     // Station downstream nozzle throat - last equilibrium

printNozzleStation(ch.getNozzleStation(10, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true));   // Station downstream nozzle throat - frozen

printNozzleStation(ch.getNozzleStation(60, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true), true);  // Nozzle exit, print out the performance

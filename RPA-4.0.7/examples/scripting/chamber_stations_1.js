// Load functions to print out nozzle station parameters
load("chamber_station_functions.js");

propellant = new Propellant(6);
propellant.addOxidizer("O2(L)", 1.0);
propellant.addFuel("H2(L)", 1.0);

ch = new Chamber(propellant, 2, "Ar", true, false);
ch.setP(10, "MPa");
ch.setAcr(4);
ch.solve();

UPSTREAM_OF_THE_THROAT = false;
DOWNSTREAM_OF_THE_THROAT = true;

printNozzleStation(ch.getNozzleStation(4, "Ar", UPSTREAM_OF_THE_THROAT, true, true));    // Nozzle inlet

printNozzleStation(ch.getNozzleStation(2, "Ar", UPSTREAM_OF_THE_THROAT, true, true));    // Station between nozzle inlet and throwt

printNozzleStation(ch.getNozzleStation(1, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true));     // Nozzle throat

printNozzleStation(ch.getNozzleStation(4, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true));     // Station downstream nozzle throat - last equilibrium

printNozzleStation(ch.getNozzleStation(10, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true));   // Station downstream nozzle throat - frozen

printNozzleStation(ch.getNozzleStation(20, "Ar", DOWNSTREAM_OF_THE_THROAT, true, true), true);  // Nozzle exit, print out the performance

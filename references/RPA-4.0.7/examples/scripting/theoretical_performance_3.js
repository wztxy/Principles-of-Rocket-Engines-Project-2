pc = new Object();
pc.propellant = new Propellant(5);
pc.propellant.addOxidizer("O2(L)", 1.0);
pc.propellant.addFuel("H2(L)", 1.0);
pc.optimize = false;

// BLC (Boundary Layer Coolant) composition specified as a propellant with low O/F
// In a practice, this can be done by configuring the injector to build the layer with lower O/F ratio
pc.blc_propellant = new Propellant(2);
pc.blc_propellant.addOxidizer("O2(L)", 1.0);
pc.blc_propellant.addFuel("H2(L)", 1.0);
pc.blc_m = 0.05;

cc = new Object();
cc.pc = 1;
cc.pc_unit = "MPa"
cc.ions = true;
cc.multiphase = false;
cc.condensed = false;
cc.solveNozzleFlow = true;
cc.nozzleInlet_Ar = 2.5;
cc.freezing_Ar = 2;
cc.nozzleExit_Ar = 20;

tp = new TheoreticalPerformance(pc, cc);
tp.solve();
tp.printResults();

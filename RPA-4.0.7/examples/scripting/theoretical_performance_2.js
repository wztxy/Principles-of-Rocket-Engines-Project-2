# Thermal propulsion system (e.g. NTP - Nuclear Thermal Propulsion System)

pc = new Object();
pc.mixture = new Mixture();
pc.mixture.addSpecies("H2(L)", 1.0);

cc = new Object();
cc.pc = 1;
cc.pc_unit = "MPa"
cc.Tc = 2000;
cc.Tc_unit = "K"
cc.ions = true;
cc.multiphase = false;
cc.condensed = false;
cc.solveNozzleFlow = true;
cc.nozzleInlet_Ar = 4;
cc.freezing_Ar = 2;
cc.nozzleExit_Ar = 20;

tp = new TheoreticalPerformance(pc, cc);
tp.solve();
tp.printResults();

pc = new Object();
pc.propellant = new Propellant(5);
pc.propellant.addOxidizer("O2(L)", 1.0);
pc.propellant.addFuel("H2(L)", 1.0);
pc.optimize = false;

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

ttp08 = new TheoreticalThrottlingPerformance(tp, 0.8)
ttp08.solve();
ttp08.printResults();

ttp13 = new TheoreticalThrottlingPerformance(tp, 1.3)
ttp13.solve();
ttp13.printResults();

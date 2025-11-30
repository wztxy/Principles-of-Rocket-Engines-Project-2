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
cc.nozzleExit_Ar = 40;
cc.solveOverexpation = true;
cc.pa = 1;
cc.pa_unit = "atm";

tp = new TheoreticalPerformance(pc, cc);

printf("Solved: %s\n", tp.isSolved()?"yes":"no");
printf("Valid: %s\n", tp.isValid()?"yes":"no");

printf("Solving...\n");
tp.solve();
printf("Solved: %s\n", tp.isSolved()?"yes":"no");

printf("Optimized: %s\n", tp.isOptimized()?"yes":"no");

exit = tp.getNozzleExit();
if (!!exit) {
    exit.printResults();
}

pc_crit = tp.getPaCrit("atm");
if (pc_crit>0 && pc_crit<1) {
    printf("\nIdeal overexpanded performance (SL):\n");
    printf("Specific impulse (SL): %10.5f m/s\n", tp.getIs_H(1, "atm", "m/s", false));
    printf("Thrust coeff. (SL): %10.5f\n", tp.getCf_H(1, "atm", false));
    printf("\nDelivered overexpanded performance (SL):\n");
    printf("Specific impulse (SL): %10.5f m/s\n", tp.getIs_H(1, "atm", "m/s", true));
    printf("Thrust coeff. (SL): %10.5f\n", tp.getCf_H(1, "atm", true));
}


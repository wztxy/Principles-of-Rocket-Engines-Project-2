propellant = new Propellant(2.6);
propellant.addOxidizer("O2(L)", 1.0);
propellant.addFuel("H2(L)", 1.0);

printf("Combustor:\n");

c = new Combustor(propellant, true, true);
c.setP(1, "MPa");
c.solve(true, false);
c.getEquilibrium().print("SI");
c.getDerivatives().print("SI");

printf("Chamber:\n");

ch = new Chamber(propellant, 4, "Ar", true, false);
ch.setP(10, "MPa");
ch.setAcr(4);
ch.solve();

printf("pc_0=%f MPa\n", ch.getPc_0("MPa"));
printf("sigma_c=%f\n", ch.getSigmaC());
printf("wc=%f m/s\n", ch.getWc());

ch.getEquilibrium("injector").print();
ch.getEquilibrium("inlet").print();
ch.getEquilibrium("throat").print();

exit = ch.getNozzleStation(20, "Ar", true, true, false);

exit.printResults();

printf("Is   =%f m/s\n", exit.getIs("m/s"));
printf("Is_v =%f m/s\n", exit.getIs_v("m/s"));
printf("Is_H =%f m/s\n", exit.getIs_H(1, "atm", "m/s"));

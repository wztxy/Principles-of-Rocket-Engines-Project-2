propellant = new Propellant(6);
propellant.addOxidizer("O2(L)", 1.0);
propellant.addFuel("H2(L)", 1.0);

ch1 = new Chamber(propellant, 2, "Ar", true, false);
ch1.setP(10, "MPa");
ch1.setAcr(4);
ch1.solve();
printChamberResults("Chamber 1 - chemical propulsion", ch1, 20);

ch2 = new Chamber(propellant);
ch2.setPH(10, "MPa", propellant.getH("J/mol"), "J/mol");
ch2.setAcr(4);
ch2.solve();
printChamberResults("Chamber 2 - chemical propulsion", ch2, 40);

mix = new Mixture();
mix.addSpecies("H2(L)", 1.0);

ch3 = new Chamber(mix, 2, "Ar", true, false);
ch3.setPT(10, "MPa", 2000, "K");
ch3.setAcr(4);
ch3.solve();
printChamberResults("Chamber 3 - thermal propulsion", ch3, 40);

function printChamberResults(name, ch, Aer) {
    printf("**************************************************************************\n");
    printf("Chamber %s\n", name);

    printf("Acr     = %f\n", ch.getAcr());
    printf("Mr      = %f kg/(m2-s)\n", ch.getMr("kg/(m2-s)"));
    printf("pc_0    = %f MPa\n", ch.getPc_0("MPa"));
    printf("sigma_c = %f\n",     ch.getSigmaC());
    printf("wc      = %f m/s\n", ch.getWc("m/s"));
    printf("c*      = %f m/s\n", ch.getCstar("m/s"));

    ch.getEquilibrium("injector").print();
    ch.getEquilibrium("inlet").print();
    ch.getEquilibrium("throat").print();

    if (ch.isFrozenConditionSet()) {
        eq = ch.getEquilibriumStation();
        if (!!eq) {
            printf("Frozen flow\n");
            printf("Last equilibrium station:\n");
            eq.printResults();
        }
    }

    exit = ch.getNozzleStation(Aer, "Ar", true, true, false);

    exit.printResults();

    printf("**************************************************************************\n");
}

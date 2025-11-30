printf("\n\n");
printf("###########################################################################\n");

mix = new Mixture();
mix.addSpecies("NH4CLO4(cr)", 0.7);
mix.addSpecies("AL(cr)", 0.2);
mix.addSpecies("HTPB+Curative", 0.1);


mix.print();

r = new Equilibrium(mix);
r.setPH(20.7, "MPa", mix.getH("J/mol"), "J/mol");
d = r.solve(true, false);
r.print("SI");
d.printResults("SI");

printf("###########################################################################\n");

r = new Equilibrium(mix);
r.setPT(0.1, "MPa", 1000, "K", true);
d = r.solve(true, false);
r.print("SI");
d.printResults("SI");




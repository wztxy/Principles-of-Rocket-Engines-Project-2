propellant = new Propellant(5);
propellant.addOxidizer("O2(L)", 1.0);
propellant.addFuel("H2(L)", 1.0);

c1 = new Combustor(propellant, true, true);
c1.setP(1, "MPa");
c1.solve(true, false);
c1.getEquilibrium().print("SI");
c1.getDerivatives().print("SI");

c2 = new Combustor(propellant, true, true);
c2.setPH(1, "MPa", propellant.getH("J/mol"), "J/mol");
c2.solve(true, false);
c2.getEquilibrium().print("SI");
c2.getDerivatives().print("SI");

c3 = new Combustor(propellant, true, true);
c3.setPT(1, "MPa", 2000, "K");
c3.solve(true, false);
c3.getEquilibrium().print("SI");
c3.getDerivatives().print("SI");

mixture = new Mixture();
mixture.addSpecies("H2(L)", 1.0);

c4 = new Combustor(mixture, true, true);
c4.setPT(1, "MPa", 2000, "K");
c4.solve(true, false);
c4.getEquilibrium().print("SI");
c4.getDerivatives().print("SI");


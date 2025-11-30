// Load and run unmodified configuration file
// Print out the results using custom code

cfg = new ConfigFile("examples/A-1.cfg");

a = new PropulsionAnalysis(cfg, false); // false - do not print out the results


printf("***************************************\n");
printf("      Combustion chamber\n");
printf("***************************************\n");

e = a.getPerformanceAnalysis().getChamber().getEquilibrium("inj");
d = a.getPerformanceAnalysis().getChamber().getDerivatives("inj");
printProducts(e);
printCombustionParameters(e);
printDerivativeParameters(d)

printf("***************************************\n");
printf("      Combustion chamber end (nozzle inlet)\n");
printf("***************************************\n");

e = a.getPerformanceAnalysis().getChamber().getEquilibrium("inl");
d = a.getPerformanceAnalysis().getChamber().getDerivatives("inl");
printProducts(e);
printCombustionParameters(e);
printDerivativeParameters(d)

printf("***************************************\n");
printf("      Throat\n");
printf("***************************************\n");

e = a.getPerformanceAnalysis().getChamber().getEquilibrium("thr");
d = a.getPerformanceAnalysis().getChamber().getDerivatives("thr");
printProducts(e);
printCombustionParameters(e);
printDerivativeParameters(d)

printf("***************************************\n");
printf("      Nozzle exit\n");
printf("***************************************\n");

ex = a.getPerformanceAnalysis().getNozzleExit();

e = ex.getEquilibrium();
d = ex.getDerivatives();
if (!!e && !!d) {
    // For equilibrium inly; Undefined for frozen conditions
    printProducts(e);
    printCombustionParameters(e);
    printDerivativeParameters(d)
} else {
    printFrozenCombustionParameters(ex);
}

printf("***************************************\n");
printf("      Performance\n");
printf("***************************************\n");

printPerformance(ex);

printf("***************************************\n");
printf("      Thrust parameters\n");
printf("***************************************\n");

printChamberParameters(a);

printf("***************************************\n");
printf("      Chamber size\n");
printf("***************************************\n");

printChamberSize(a);

printf("***************************************\n");


//***************************************************
// Functions
//***************************************************

function printProducts(e) {

    products = e.getResultingMixture();

    printf("\nCombustion composition:\n");
    printf("%20s %10s %10s %4s\n", "Name", "Mass Frac", "Mole Frac", "Cond");

    sum1 = 0;
    sum2 = 0;

    for (i=0; i<products.size(); ++i) {
        // Reaction product
        s = products.getSpecies(i);

        massFraction = products.getFraction(i, "mass");
        moleFraction = products.getFraction(i, "mole");;

        sum1 += massFraction;
        sum2 += moleFraction;

        // We are printing out mass fraction in  format "%10.7f",
        // so skip all products with massFraction<1e-7
        if (massFraction<1e-7) {
            continue;
        }

        printf(
            "%20s %10.7f %10.7f %4d\n",
            s.getName(),
            massFraction,
            moleFraction,
            s.getCondensed()
        );

    }

    printf(
        "%15s %9.7f %9.7f\n",
        "Summ:",
        sum1,
        sum2
    );

}

function printCombustionParameters(e) {
    printf("\nCombustion parameters:\n");
    printf("%20s %10.5f %10s\n", "Temperature:", e.getT("K"), "K");
    printf("%20s %10.5f %10s\n", "Pressure:", e.getP("MPa"), "MPa");

    printf("%20s %10.3f %10s\n", "Enthalpy:", e.getH("J/mol"), "J/mol");
    printf("%20s %10.3f %10s\n", "", e.getH("kJ/kg"), "kJ/kg");

    printf("%20s %10.3f %10s\n", "Entropy:", e.getS("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.3f %10s\n", "", e.getS("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.3f %10s\n", "Internal energy:", e.getU("J/mol"), "J/mol");
    printf("%20s %10.3f %10s\n", "", e.getU("kJ/kg"), "kJ/kg");
}

function printDerivativeParameters(d) {
    printf("%20s %10.5f %10s\n", "Cp (eq):", d.getCp("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.5f %10s\n", "", d.getCp("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "Cv (eq):", d.getCv("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.5f %10s\n", "", d.getCv("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "gamma", d.getGamma(), "");
    printf("%20s %10.5f %10s\n", "k", d.getK(), "");
    printf("%20s %10.5f %10s\n", "R", d.getR("J/(kg K)"), "J/(kg K)");
    printf("%20s %10.5f %10s\n", "a", d.getA("m/s"), "m/s");
    printf("%20s %10.5f %10s\n", "rho", d.getRho("kg/m^3"), "kg/m^3");
    printf("%20s %10.5f %10s\n", "C-phase", d.getZ(), "Condensed phase mass fraction");

    printf("Thermal transport properties:\n");

    printf("%20s %10.5f %10s\n", "Viscosity:", d.getV("x10^-4 kg/(m s)"), "x10^-4 kg/(m s)");

    printf("Frozen parameters:\n");
    printf("%20s %10.5f %10s\n", "Conductivity:", d.getCfr("W/(m K)"), "W/(m K)");
    printf("%20s %10.5f %10s\n", "Cp:", d.getCpfr("kJ/(kg K)"), "kJ/(kg K)");
    printf("%20s %10.5f %10s\n", "Pr:", d.getPrfr(), "");

    printf("Effective parameters with equilibrium:\n");
    printf("%20s %10.5f %10s\n", "Conductivity:", d.getCeql("W/(m K)"), "W/(m K)");
    printf("%20s %10.5f %10s\n", "Cp:", d.getCpeql("kJ/(kg K)"), "kJ/(kg K)");
    printf("%20s %10.5f %10s\n", "Pr:", d.getPreql(), "");
}

function printFrozenCombustionParameters(ex) {
    printf("\nCombustion parameters:\n");
    printf("%20s %10.5f %10s\n", "Temperature:", ex.getT("K"), "K");
    printf("%20s %10.5f %10s\n", "Pressure:", ex.getP("MPa"), "MPa");

    printf("%20s %10.3f %10s\n", "Enthalpy:", ex.getH("J/mol"), "J/mol");
    printf("%20s %10.3f %10s\n", "", ex.getH("kJ/kg"), "kJ/kg");

    printf("%20s %10.3f %10s\n", "Entropy:", ex.getS("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.3f %10s\n", "", ex.getS("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "Cp (eq):", ex.getCp("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.5f %10s\n", "", ex.getCp("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "Cv (eq):", ex.getCv("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.5f %10s\n", "", ex.getCv("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "k", ex.getK(), "");
    printf("%20s %10.5f %10s\n", "R", ex.getR("J/(kg K)"), "J/(kg K)");
    printf("%20s %10.5f %10s\n", "a", ex.getA("m/s"), "m/s");
    printf("%20s %10.5f %10s\n", "rho", ex.getRho("kg/m^3"), "kg/m^3");
}

function printPerformance(ex) {
    printf("%20s %10.5f %10s\n", "Velocity:", ex.getW("m/s"), "m/s");
    printf("%20s %10.5f %10s\n", "Mach number:", ex.getMach(), "");
    printf("%20s %10.5f %10s\n", "Pressure ratio:", ex.getPi(), "");
    printf("%20s %10.5f %10s\n", "Area ratio:", ex.getFr(), "");

    printf("\nIdeal performance:\n");
    printf("%20s %10.5f %10s\n", "Specific impulse (vac):", ex.getIs_v("m/s", false), "m/s");
    printf("%20s %10.5f %10s\n", "Specific impulse (opt):", ex.getIs("m/s", false), "m/s");
    printf("%20s %10.5f %10s\n", "Specific impulse (SL):", ex.getIs_H(1, "atm", "m/s", false), "m/s");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (vac):", ex.getCf_v(false), "");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (opt):", ex.getCf(false), "");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (SL):", ex.getCf_H(1, "atm", false), "");

    printf("\nDelivered performance:\n");
    printf("%20s %10.5f %10s\n", "Specific impulse (vac):", ex.getIs_v("m/s", true), "m/s");
    printf("%20s %10.5f %10s\n", "Specific impulse (opt):", ex.getIs("m/s", true), "m/s");
    printf("%20s %10.5f %10s\n", "Specific impulse (SL):", ex.getIs_H(1, "atm", "m/s", true), "m/s");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (vac):", ex.getCf_v(true), "");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (opt):", ex.getCf(true), "");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (SL):", ex.getCf_H(1, "atm", true), "");

    printf("\nAmbient conditions for optimum expansion (opt):\n");
    printf("%20s %10.5f %10s\n", "Pressure:", ex.getP("atm"), "atm");
}

function printChamberParameters(a) {
    cconf = a.getChamberDesign().getConfiguration();
    csize = a.getChamberDesign().getSize(50); // Can use as well: a.getChamberSize(50)
    ex = a.getPerformanceAnalysis().getNozzleExit();

    if (!!csize) {
        device = "Chamber";
        mdot = csize.mdot;
        if (cconf.n>1) {
            device = "Engine";
            mdot *= cconf.n;
        }
        thrust_vac = mdot * ex.getIs_v("m/s", true); // "true" - apply correction factors
        thrust_opt = mdot * ex.getIs("m/s", true); // "true" - apply correction factors
        thrust_SL = mdot * ex.getIs_H(1, "atm", "m/s", true); // "true" - apply correction factors

        printf("%20s %10d\n", "Number of chambers:", cconf.n);
        printf("%20s %10.5f %10s\n", device+" thrust (vac):", thrust_vac/1000., "kN");
        printf("%20s %10.5f %10s\n", device+" thrust (opt):", thrust_opt/1000., "kN");
        printf("%20s %10.5f %10s\n", device+" thrust (SL):", thrust_SL/1000., "kN");
        printf("%20s %10.5f %10s\n", device+" mass flow rate:", mdot, "kg/s");

    } else {
        printf("Thrust chamber parameters are not available\n");
    }

}

function printChamberSize(a) {
    csize = a.getChamberSize(50);
    nsize = a.getNozzleSize(false, 50);

    if (!!csize && !!nsize) {
        printf("Combustion chamber:\n");
        printf("    Dc = %7.2f mm         b  = %7.2f deg\n", csize.Dc*1000., 57.296*csize.b);
        printf("    R2 = %7.2f mm         R1 = %7.2f mm\n", csize.R2*1000., csize.R1*1000.);
        printf("    L* = %7.2f mm\n", csize.Lstar*1000.);
        printf("    Lc = %7.2f mm       Lcyl = %7.2f deg\n", csize.Lc*1000., csize.Lcyl*1000.);
        printf("    Dt = %7.2f mm\n", csize.Dt*1000.);
        printf("Nozzle:\n");
        printf("    Rn = %7.2f mm        Tn  = %7.2f deg\n", nsize.R1*1000., 57.296*nsize.theta_1);
        printf("    Le = %7.2f mm        Te  = %7.2f deg\n", nsize.L*1000., 57.296*nsize.theta_e);
        printf("    De = %7.2f mm\n", nsize.De*1000.);
        printf(" Ae/At = %7.2f\n", nsize.Ae/nsize.At);
        printf(" Le/Dt = %7.2f\n", nsize.L/csize.Dt);
    } else {
        printf("Combustion chamber size is not available\n");
    }
}


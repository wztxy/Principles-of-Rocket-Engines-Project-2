// This script contains set of functions used in other scripts.

function printNozzleStation(station, isNozzleExitStation) {
    printf("\n");
    printf("**************************************************************************\n");
    printf("Chamber Station e=%7.4f %s\n", station.getAr(), !!isNozzleExitStation?"- Nozzle Exit":"");
    printf("**************************************************************************\n");

    var e = station.getEquilibrium();
    var d = station.getDerivatives();

    if (!!e && !!d) {
        printf("\nShifting equilibrium flow\n");
    } else {
        printf("\nFrozen equilibrium flow\n");
    }

    printProducts(station.getResultingMixture());

    if (!!e && !!d) {
        // For equilibrium inly; Undefined for frozen conditions
        printCombustionParameters(e);
        printDerivativeParameters(d)
    } else {
        printFrozenCombustionParameters(station);
    }

    if (!!isNozzleExitStation) {
        printPerformance(station);
    } else {
        printFlowParameters(station);
    }
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

function printFrozenCombustionParameters(station) {
    printf("\nCombustion parameters:\n");
    printf("%20s %10.5f %10s\n", "Temperature:", station.getT("K"), "K");
    printf("%20s %10.5f %10s\n", "Pressure:", station.getP("MPa"), "MPa");

    printf("%20s %10.3f %10s\n", "Enthalpy:", station.getH("J/mol"), "J/mol");
    printf("%20s %10.3f %10s\n", "", station.getH("kJ/kg"), "kJ/kg");

    printf("%20s %10.3f %10s\n", "Entropy:", station.getS("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.3f %10s\n", "", station.getS("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "Cp (eq):", station.getCp("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.5f %10s\n", "", station.getCp("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "Cv (eq):", station.getCv("J/(mol K)"), "J/(mol K)");
    printf("%20s %10.5f %10s\n", "", station.getCv("kJ/(kg K)"), "kJ/(kg K)");

    printf("%20s %10.5f %10s\n", "k", station.getK(), "");
    printf("%20s %10.5f %10s\n", "R", station.getR("J/(kg K)"), "J/(kg K)");
    printf("%20s %10.5f %10s\n", "a", station.getA("m/s"), "m/s");
    printf("%20s %10.5f %10s\n", "rho", station.getRho("kg/m^3"), "kg/m^3");
}

function printFlowParameters(station) {
    printf("%20s %10.5f %10s\n", "Velocity:", station.getW("m/s"), "m/s");
    printf("%20s %10.5f %10s\n", "Mach number:", station.getMach(), "");
    printf("%20s %10.5f %10s\n", "Pressure ratio:", station.getPi(), "");
    printf("%20s %10.5f %10s\n", "Area ratio:", station.getFr(), "");
}


function printPerformance(exit) {
    printFlowParameters(exit);

    printf("\nIdeal performance:\n");
    printf("%20s %10.5f %10s\n", "Specific impulse (vac):", exit.getIs_v("m/s", false), "m/s");
    printf("%20s %10.5f %10s\n", "Specific impulse (opt):", exit.getIs("m/s", false), "m/s");
    printf("%20s %10.5f %10s\n", "Specific impulse (SL):", exit.getIs_H(1, "atm", "m/s", false), "m/s");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (vac):", exit.getCf_v(false), "");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (opt):", exit.getCf(false), "");
    printf("%20s %10.5f %10s\n", "Thrust coeff. (SL):", exit.getCf_H(1, "atm", false), "");

    printf("\nDelivered performance:\n");
    try {
        printf("%20s %10.5f %10s\n", "Specific impulse (vac):", exit.getIs_v("m/s", true), "m/s");
        printf("%20s %10.5f %10s\n", "Specific impulse (opt):", exit.getIs("m/s", true), "m/s");
        printf("%20s %10.5f %10s\n", "Specific impulse (SL):", exit.getIs_H(1, "atm", "m/s", true), "m/s");
        printf("%20s %10.5f %10s\n", "Thrust coeff. (vac):", exit.getCf_v(true), "");
        printf("%20s %10.5f %10s\n", "Thrust coeff. (opt):", exit.getCf(true), "");
        printf("%20s %10.5f %10s\n", "Thrust coeff. (SL):", exit.getCf_H(1, "atm", true), "");
    } catch (e) {
        printf("Not available: %s\n", e);
    }

    printf("\nAmbient conditions for optimum expansion (opt):\n");
    printf("%20s %10.5f %10s\n", "Pressure:", exit.getP("atm"), "atm");
}

function printOverexpandedPerformance(tp) {
    var pc_crit = tp.getPaCrit("atm");
    printf("\nAmbient conditions for overexpansion:\n");
    printf("%20s %10.5f %10s\n", "Pressure >", pc_crit, "atm");
    if (pc_crit>0 && pc_crit<1) {
        printf("\nIdeal overexpanded performance:\n");
        printf("%20s %10.5f %10s\n", "Specific impulse (SL):", tp.getIs_H(1, "atm", "m/s", false), "m/s");
        printf("%20s %10.5f %10s\n", "Thrust coeff. (SL):", tp.getCf_H(1, "atm", false), "");

        printf("\nDelivered overexpanded performance:\n");
        try {
            printf("%20s %10.5f %10s\n", "Specific impulse (SL):", tp.getIs_H(1, "atm", "m/s", true), "m/s");
            printf("%20s %10.5f %10s\n", "Thrust coeff. (SL):", tp.getCf_H(1, "atm", true), "");
        } catch (e) {
            printf("Not available: %s\n", e);
        }
    }
}

function printProducts(products) {

    printf("\nCombustion composition:\n");
    printf("%20s %10s %10s %4s\n", "Name", "Mass Frac", "Mole Frac", "Cond");

    var sum1 = 0;
    var sum2 = 0;

    for (var i=0; i<products.size(); ++i) {
        // Reaction product
        var s = products.getSpecies(i);

        var massFraction = products.getFraction(i, "mass");
        var moleFraction = products.getFraction(i, "mole");;

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

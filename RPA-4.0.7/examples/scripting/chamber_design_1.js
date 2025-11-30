pc = new Object();
pc.propellant = new Propellant(5);
pc.propellant.addOxidizer("O2(L)", 1.0);
pc.propellant.addFuel("H2(L)", 1.0);
pc.optimize = false;

cc = {
    pc: 1,
    pc_unit: "MPa",
    ions: true,
    multiphase: false,
    condensed: false,
    solveNozzleFlow: true,
    nozzleInlet_Ar: 2.5,
    freezing_Ar: 2,
    nozzleExit_Ar: 20,
    solveOverexpation: true
};

tp = new TheoreticalPerformance(pc, cc);
tp.solve();
tp.printResults();

// Chamber for thrust 100 kN
chamberDesignConfig = {
    performance: tp,
    n: 1,
    sizing_type: "THRUST",
    thrust: 100,
    thrust_unit: "kN",
    pa: 0,
    pa_unit: "atm",
    length_type: "L_STAR",
    Lstar: 1000,
    Lstar_unit: "mm",
    b: 35,
    b_unit: "degrees",
    R1_to_Rt: 1.0,
    R2_to_R2max: 0.4
};

cd = new ChamberDesign(chamberDesignConfig);
size = cd.getSize(50);
if (!!size && !!size["contour"] && size.contour.length===0) {
    // If object "contour" is not null but csize.contour.length==0,
    // increase number of points
    size = cd.getSize(100);
}
printChamberSize(size);

// Chamber for thrust 10 kN
// Re-use all other configuration parameters
chamberDesignConfig.thrust = 10;
chamberDesignConfig.thrust_unit = "kN";

cd = new ChamberDesign(chamberDesignConfig);
size = cd.getSize(50);
if (!!size && !!size["contour"] && size.contour.length===0) {
    // If object "contour" is not null but csize.contour.length==0,
    // increase number of points
    size = cd.getSize(100);
}
printChamberSize(size);

function printChamberSize(csize) {
    printf("\n");
    printf("**************************************************************************\n");
    printf("Chamber size\n");

    if (!!csize) {
        printf("Combustion chamber:\n");
        printf("    Dc = %7.2f mm         b  = %7.2f deg\n", csize.Dc*1000., 57.296*csize.b);
        printf("    R2 = %7.2f mm         R1 = %7.2f mm\n", csize.R2*1000., csize.R1*1000.);
        printf("    L* = %7.2f mm\n", csize.Lstar*1000.);
        printf("    Lc = %7.2f mm       Lcyl = %7.2f deg\n", csize.Lc*1000., csize.Lcyl*1000.);
        printf("    Dt = %7.2f mm\n", csize.Dt*1000.);

        // Print out the chaber geometry
        printf("Chamber geometry:\n");
        printf("%3s %8s %8s\n", "#", "x [mm]", "r [mm]");

        if (!!csize["contour"] && csize.contour.length>0) {
            for (var ci=0; ci<csize.contour.length; ci++) {
                var cpoint = csize.contour[ci];
                printf("%3d %8.3f %8.3f\n", ci, cpoint.x, cpoint.r);
            }
        }

    } else {
        printf("Combustion chamber size is not available\n");
    }
}


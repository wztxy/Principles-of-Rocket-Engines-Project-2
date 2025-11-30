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

//******************************************************
// Design thrust chamber for for thrust 100 kN in vacuum (already set in chamberDesignConfig above)

cd = new ChamberDesign(chamberDesignConfig);
csize = cd.getSize(50);
if (!!csize && !!csize["contour"] && csize.contour.length===0) {
    // If object "contour" is not null but csize.contour.length==0,
    // increase number of points
    csize = cd.getSize(100);
}

nozzleDesignConfig = {
    chamberDesign: cd,
    type: "PARABOLIC",
    theta_1: 27,
    theta_1_unit: "degrees",
    theta_e: 9.8,
    theta_e_unit: "degrees",
    R1_to_Rt: 0.385
};

nd = new NozzleDesign(nozzleDesignConfig);
nsize = nd.getSize(true, 50);
if (!!nsize["contour"] && nsize.contour.length===0) {
    // If object "contour" is not null but nsize.contour.length==0,
    // increase number of points
    nsize = nd.getSize(true, 100);
}

printThrustChamberSize(csize, nsize);

//******************************************************
// Design thrust chamber for for thrust 10 kN in vacuum
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

nozzleDesignConfig.chamberDesign = cd;

nd = new NozzleDesign(nozzleDesignConfig);
nsize = nd.getSize(true, 50);
if (!!nsize["contour"] && nsize.contour.length===0) {
    // If object "contour" is not null but nsize.contour.length==0,
    // increase number of points
    nsize = nd.getSize(true, 100);
}

printThrustChamberSize(size, nsize);

//******************************************************
// Design thrust chamber for for thrust 10 kN in vacuum and conical nozzle
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

nozzleDesignConfig.chamberDesign = cd;
nozzleDesignConfig.type = "CONICAL";
nozzleDesignConfig.theta_e = 15;
nozzleDesignConfig.theta_e_unit = "degrees";

nd = new NozzleDesign(nozzleDesignConfig);
nsize = nd.getSize(true, 50);
if (!!nsize["contour"] && nsize.contour.length===0) {
    // If object "contour" is not null but nsize.contour.length==0,
    // increase number of points
    nsize = nd.getSize(true, 100);
}

printThrustChamberSize(size, nsize);

//******************************************************
// Design thrust chamber for for thrust 10 kN in vacuum and MOC nozzle
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

nozzleDesignConfig.chamberDesign = cd;
nozzleDesignConfig.type = "MOC";

nd = new NozzleDesign(nozzleDesignConfig);
nsize = nd.getSize(true, 50);
if (!!nsize["contour"] && nsize.contour.length===0) {
    // If object "contour" is not null but nsize.contour.length==0,
    // increase number of points
    nsize = nd.getSize(true, 100);
}

printThrustChamberSize(size, nsize);

//******************************************************

function printThrustChamberSize(csize, nsize) {
    printf("\n");
    printf("**************************************************************************\n");
    printf("Thrust chamber size\n");

    if (!!csize && !!nsize) {
        printf("Combustion chamber:\n");
        printf("    Dc = %7.2f mm         b  = %7.2f deg\n", csize.Dc*1000., 57.296*csize.b);
        printf("    R2 = %7.2f mm         R1 = %7.2f mm\n", csize.R2*1000., csize.R1*1000.);
        printf("    L* = %7.2f mm\n", csize.Lstar*1000.);
        printf("    Lc = %7.2f mm       Lcyl = %7.2f deg\n", csize.Lc*1000., csize.Lcyl*1000.);
        printf("    Dt = %7.2f mm\n", csize.Dt*1000.);
        printf("Nozzle type %s:\n", nsize.type);
        printf("    Rn = %7.2f mm        Tn  = %7.2f deg\n", nsize.R1*1000., 57.296*nsize.theta_1);
        printf("    Le = %7.2f mm        Te  = %7.2f deg\n", nsize.L*1000., 57.296*nsize.theta_e);
        printf("    De = %7.2f mm\n", nsize.De*1000.);
        printf(" Ae/At = %7.2f\n", nsize.Ae/nsize.At);
        printf(" Le/Dt = %7.2f\n", nsize.L/csize.Dt);

        // Print out the chaber geometry
        printf("Chamber geometry:\n");
        printf("%3s %8s %8s\n", "#", "x [mm]", "r [mm]");

        if (!!csize["contour"] && csize.contour.length>0) {
            for (var ci=0; ci<csize.contour.length; ci++) {
                var cpoint = csize.contour[ci];
                printf("%3d %8.3f %8.3f\n", ci, cpoint.x, cpoint.r);
            }
        }

        if (!!nsize["contour"] && nsize.contour.length>0) {
            var cl = csize.contour.length - 1; // Fist point is skipped (see below), starting with "1"
            for (var ni=1; ni<nsize.contour.length; ni++) { // Start with "1" because first point is throat which is already printed out
                var npoint = nsize.contour[ni];
                printf("%3d %8.3f %8.3f\n", cl+ni, npoint.x, npoint.r);
            }
        }

    } else {
        printf("Thrust chamber size is not available\n");
    }
}




// Load functions to print out thermal analysis results
load("thermal_analysis_functions.js");

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

chamberDesignConfig = {
    performance: tp,
    n: 1,
    sizing_type: "THRUST",
    thrust: 10,
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


csize = cd.getSize(50);
if (!!csize && !!csize["contour"] && csize.contour.length===0) {
    // If object "contour" is not null but csize.contour.length==0,
    // increase number of points
    csize = cd.getSize(100);
}
nsize = nd.getSize(true, 50);
if (!!nsize["contour"] && nsize.contour.length===0) {
    // If object "contour" is not null but nsize.contour.length==0,
    // increase number of points
    nsize = nd.getSize(true, 100);
}
printThrustChamberSizeWithoutContour(csize, nsize);

//**************************************************************

// Heat transfer configuration
// Calculate heat transfer for fixed wall temperature Twg=1400 K
// without radiation heat transfer, IEVLEV approach, without cooling

heatTransferConfiguration = {
    chamberDesign: cd,
    nozzleDesign: nd,
    throttle_value: 1,
    points: 50,
    applyCooling: false,
    Twg: 1400,
    Twg_unit: "K",
    approach: "IEVLEV",
    wallLayer: 0.025,
    withRadiationHeatTransfer: false
};

ht = new HeatTransferAnalysis(heatTransferConfiguration);

ht_results = ht.getResults();
printHeatTransferResults(ht_results);

//**************************************************************

// Heat transfer configuration
// Calculate heat transfer for fixed wall temperature Twg=1400 K
// with radiation heat transfer, IEVLEV approach, without cooling

heatTransferConfiguration = {
    chamberDesign: cd,
    nozzleDesign: nd,
    throttle_value: 1,
    points: 50,
    applyCooling: false,
    Twg: 1400,
    Twg_unit: "K",
    approach: "IEVLEV",
    wallLayer: 0.025,
    withRadiationHeatTransfer: true,
    eps_w: 0.8
};

ht = new HeatTransferAnalysis(heatTransferConfiguration);

ht_results = ht.getResults();
printHeatTransferResults(ht_results);

//**************************************************************

// Heat transfer configuration
// Calculate heat transfer for fixed wall temperature Twg=1400 K
// with radiation heat transfer, BARTZ approach, without cooling

heatTransferConfiguration = {
    chamberDesign: cd,
    nozzleDesign: nd,
    throttle_value: 1,
    points: 50,
    applyCooling: false,
    Twg: 1400,
    Twg_unit: "K",
    approach: "BARTZ",
    wallLayer: 0.025,
    withRadiationHeatTransfer: true,
    eps_w: 0.8
};

ht = new HeatTransferAnalysis(heatTransferConfiguration);

ht_results = ht.getResults();
printHeatTransferResults(ht_results);




// Load functions to print out thermal analysis results
load("thermal_analysis_functions.js");

// This Mixture  object will be used as a coolant for all cases below,
// both for regen and film cooling
coolantMix = new Mixture();
coolantMix.addSpecies("H2(L)", 1.0);

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

heatTransferConfiguration = {
    chamberDesign: cd,
    nozzleDesign: nd,
    throttle_value: 1,
    points: 50,
    approach: "IEVLEV",
    wallLayer: 0.025,
    withRadiationHeatTransfer: true,
    eps_w: 0.8,
    applyCooling: true,
    wallCoolingSegments: [
        {
            id: "r1",
            location: 0,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 80,
            lambda_unit: "W/(m K)",
            type: "COAXIAL",
            hc: 1,
            hc_unit: "mm",
            coolant: coolantMix,
            mdot_bar: 0.2,
            oppositeFlow: true,
            coolant_p: 3,
            coolant_p_unit: "MPa",
            coolant_T: 22,
            coolant_T_unit: "K"
        },
        {
            id: "r2",
            location: 500,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 80,
            lambda_unit: "W/(m K)",
            type: "COAXIAL",
            hc: 1,
            hc_unit: "mm",
            inputFrom: "r1"
        }
    ]
};

ht = new HeatTransferAnalysis(heatTransferConfiguration);

ht_results = ht.getResults();
printHeatTransferResults(ht_results);

//**************************************************************

heatTransferConfiguration = {
    chamberDesign: cd,
    nozzleDesign: nd,
    throttle_value: 1,
    points: 50,
    approach: "IEVLEV",
    wallLayer: 0.025,
    withRadiationHeatTransfer: true,
    eps_w: 0.8,
    applyCooling: true,
    wallCoolingSegments: [
        {
            id: "r1",
            location: 0,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 80,
            lambda_unit: "W/(m K)",
            type: "COAXIAL",
            hc: 1,
            hc_unit: "mm",
            coolant: coolantMix,
            mdot_bar: 0.2,
            oppositeFlow: true,
            coolant_p: 3,
            coolant_p_unit: "MPa",
            coolant_T: 22,
            coolant_T_unit: "K"
        },
        {
            id: "r2",
            location: 500,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 80,
            lambda_unit: "W/(m K)",
            type: "COAXIAL",
            hc: 1,
            hc_unit: "mm",
            inputFrom: "r1"
        }
    ],
    applyFilmCooling : true,
    filmCoolingSlots : [
        {
            location: 400,
            location_unit: "mm",
            mdot_bar: 0.02,
            coolant: coolantMix
        }
    ]
};

ht = new HeatTransferAnalysis(heatTransferConfiguration);

ht_results = ht.getResults();
printHeatTransferResults(ht_results);

//**************************************************************

heatTransferConfiguration = {
    chamberDesign: cd,
    nozzleDesign: nd,
    throttle_value: 1,
    points: 50,
    approach: "IEVLEV",
    wallLayer: 0.025,
    withRadiationHeatTransfer: true,
    eps_w: 0.8,
    applyCooling: true,
    wallCoolingSegments: [
        {
            id: "r1",
            location: 0,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 270,
            lambda_unit: "W/(m K)",
            type: "TUBULAR",
            n: 25,
            d1: 14,
            d1_unit: "mm",
            dmin: 12,
            dmin_unit: "mm",
            d2: 16,
            d2_unit: "mm",
            coolant: coolantMix,
            mdot_bar: 0.2,
            oppositeFlow: true,
            coolant_p: 3,
            coolant_p_unit: "MPa",
            coolant_T: 22,
            coolant_T_unit: "K"
        },
        {
            id: "r2",
            location: 500,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 270,
            lambda_unit: "W/(m K)",
            type: "TUBULAR",
            n: 50,
            d1: 8,
            d1_unit: "mm",
            d2: 18,
            d2_unit: "mm",
            inputFrom: "r1"
        }
    ]
};

ht = new HeatTransferAnalysis(heatTransferConfiguration);

ht_results = ht.getResults();
printHeatTransferResults(ht_results);

//**************************************************************

heatTransferConfiguration = {
    chamberDesign: cd,
    nozzleDesign: nd,
    throttle_value: 1,
    points: 50,
    approach: "IEVLEV",
    wallLayer: 0.025,
    withRadiationHeatTransfer: true,
    eps_w: 0.8,
    applyCooling: true,
    wallCoolingSegments: [
        {
            id: "r1",
            location: 0,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 270,
            lambda_unit: "W/(m K)",
            type: "CHANNEL",
            n: 110,
            channel_type: "CONST_CHANNEL_WIDTH",

            hc1: 2.5,
            hc1_unit: "mm",
            a1: 1.5,
            a1_unit: "mm",

            hcmin: 2.5,
            hcmin_unit: "mm",
            amin: 1.5,
            amin_unit: "mm",

            hc2: 2.5,
            hc2_unit: "mm",
            a2: 1.5,
            a2_unit: "mm",

            coolant: coolantMix,
            mdot_bar: 0.2,
            oppositeFlow: true,
            coolant_p: 3,
            coolant_p_unit: "MPa",
            coolant_T: 22,
            coolant_T_unit: "K"
        },
        {
            id: "r2",
            location: 500,
            location_unit: "mm",
            h: 1,
            h_unit: "mm",
            lambda: 270,
            lambda_unit: "W/(m K)",
            type: "CHANNEL",
            n: 200,
            channel_type: "CONST_CHANNEL_WIDTH",

            hc1: 2.5,
            hc1_unit: "mm",
            a1: 1.5,
            a1_unit: "mm",

            hc2: 2.5,
            hc2_unit: "mm",
            a2: 1.5,
            a2_unit: "mm",

            inputFrom: "r1"
        }
    ]
};

ht = new HeatTransferAnalysis(heatTransferConfiguration);

ht_results = ht.getResults();
printHeatTransferResults(ht_results);

//**************************************************************

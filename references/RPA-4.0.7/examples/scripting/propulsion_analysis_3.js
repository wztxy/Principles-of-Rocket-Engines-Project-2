// Load functions to print out nozzle station parameters
load("chamber_station_functions.js");
// Load functions to print out thermal analysis results
load("thermal_analysis_functions.js");

cfg = new ConfigFile("examples/A-1.cfg");

raw = JSON.parse(cfg.toString());

// Mofify loaded configuration

// Increase Ae/At ratio
raw.nozzleConditions["nozzleExitConditions"] = {
   "areaRatio":40
};

raw.nozzleConditions["nozzleEficiency"] = {
   "estimateBellNozzleEfficicncy":true,
   "estimateOverexpansionPerformance":true  // Add flag to estimate overexpanded performance
};

// Add throttle settings to solve throttled performance
raw.nozzleConditions["throttleSettings"] = {
   "range":{
      "min":0.8,
      "max":1.3,
      "numberOfPoints":5
   }
};

// Add ambient conditions to solve altitude performance
raw.nozzleConditions["ambientConditions"] = {
    "range":{
       "from":{
          "value":1,
          "units":"atm"
       },
       "to":{
          "value":0.001,
          "units":"atm"
       },
       "numberOfPoints":30
    }
};

// Add nozzle stations
raw.nozzleConditions["nozzleStations"] = {
    "station":[
       {
          "areaRatio":{
             "location":"NOZZLE_INLET",
             "ratio":1.3
          }
       },
       {
          "areaRatio":{
             "location":"NOZZLE_EXIT",
             "ratio":10
          }
       },
       {
          "areaRatio":{
             "location":"NOZZLE_EXIT",
             "ratio":20
          }
       }
    ]
};

// Variants for different types of nozzle:
// Change NOZZLE_TYPE to one of the following values:
//  "PARABOLIC"
//  "CONICAL"
//  "MOC"

var NOZZLE_TYPE="PARABOLIC";

if ("CONICAL"==NOZZLE_TYPE) {
    // Conical nozzle
    printf("Using conical nozzle configuration...\n");
    raw.thrustChamberSize["nozzleGeometry"] = {
        "conicalNozzleWithHalfAngle":15,
        "RntoRt":0.382
    };
} else
if ("CONICAL"==NOZZLE_TYPE) {
    // Parabolic nozzle
    printf("Using parabolic nozzle configuration...\n");
    raw.thrustChamberSize["nozzleGeometry"] = {
        "parabolicContour":{
            "Tn":27.4,
            "Te":9.8
        },
        "RntoRt":0.382
    };
} else
if ("MOC"==NOZZLE_TYPE) {
    printf("Using MOC nozzle configuration...\n");
    raw.thrustChamberSize["nozzleGeometry"] = {
        "ticContour":{
            // Use default settings
        },
        "RntoRt":0.382
    };
}

// Replace original configuration with updated one
cfg.fromString(JSON.stringify(raw));

// Run configured propulsion analysis
p = new PropulsionAnalysis(cfg, false); // false - don't print out the results

// Print out results for theoretical performance
p.getPerformanceAnalysis().printResults();;

// Print out results for altitude performance
printResultsOfAltitudePerformanceAnalysis(p);

// Print out results for throttled performance
printResultsOfThrottlingPerformanceAnalysis(p);

// Print out results for nozzle stations
printNozzleStations(p);

// Print out chamber design/size
printChamberSize(p);

// Print out chamber thermal analysis
printChamberHeatTransferAnalysis(p);


//*******************************************
// Print out results for altitude performance
function printResultsOfAltitudePerformanceAnalysis(p) {
    if (p.hasAltitudePerformanceAnalysis()) {
        printf("\n");
        printf("**************************************************************************\n");
        printf("Altitude Performance\n");

        ap = p.getAltitudePerformanceAnalysis();
        if (ap.length>0) {

            if (!!ap[0]["delivered"]) {
                printf("%2s %12s %12s %12s %12s %12s %12s %12s %12s %12s %12s\n",
                       "#", "p_a", "H",
                       "Is", "Is_overexp", "Cf", "Cf_overexp",
                       "Is_d", "Is_overexp_d", "Cf_d", "Cf_overexp_d");
                printf("%2s %12s %12s %12s %12s %12s %12s %12s %12s %12s %12s\n",
                       "", "[atm]", "[km]",
                       "[m/s]", "[m/s]", "", "",
                       "[m/s]", "[m/s]", "", "");
                printf("************************************************************************************************************************************\n");
            } else {
                printf("%2s %12s %12s %12s %12s %12s %12s\n",
                       "#", "p_a", "H",
                       "Is", "Is_overexp", "Cf", "Cf_overexp");
                printf("%2s %12s %12s %12s %12s %12s %12s\n",
                       "", "[atm]", "[km]",
                       "[m/s]", "[m/s]", "", "");
                printf("*********************************************************************************\n");
            }

            for (var i=0; i<ap.length; i++) {
                a = ap[i];
                if (!!a["delivered"]) {
                    printf("%2d %12.5f %12.5f %12.5f %12s %12.5f %12s %12.5f %12s %12.5f %12s\n",
                           i, Units.convertPressure(a.pa, "Pa", "atm"), a.h/1000,
                           a.Is, overexpValue("%10.5f", a.Is_overexp), a.Cf, overexpValue("%10.5f", a.Cf_overexp),
                           a.Is_d, overexpValue("%10.5f", a.Is_overexp_d), a.Cf_d, overexpValue("%10.5f", a.Cf_overexp_d));
                } else {
                    printf("%2d %12.5f %12.5f %12.5f %12s %12.5f %12s\n",
                           i, Units.convertPressure(a.pa, "Pa", "atm"), a.h/1000,
                           a.Is, overexpValue("%10.5f", a.Is_overexp), a.Cf, overexpValue("%10.5f", a.Cf_overexp));
                }
            }

            if (!!ap[0]["delivered"]) {
                printf("************************************************************************************************************************************\n");
            } else {
                printf("*********************************************************************************\n");
            }

        }

    }
}

function overexpValue(f, v) {
    if (v!==0) {
        return sprintf(f, v);
    }
    return "-"
}

//*******************************************
// Print out results for throttled performance
function printResultsOfThrottlingPerformanceAnalysis(p) {
    if (p.hasThrottlingPerformanceAnalysis()) {
        printf("\n");
        printf("**************************************************************************\n");
        printf("Throttled Performance\n");

        ttp = p.getThrottlingPerformanceAnalysis();
        if (ttp.length>0) {

            printf("%2s %4s %12s %12s %12s %12s\n", "#", "R", "Is_SL", "Cf_SL", "Is_SL_d", "Cf_SL_d");
            printf("%2s %4s %12s %12s %12s %12s\n", "", "", "[m/s]", "", "[m/s]", "");

            for (var i=0; i<ttp.length; i++) {
                a = ttp[i];
                printf("%2d %4.2f %12.5f %12.5f %12.5f %12.5f\n", i, a.getR(),
                       a.getIs_H(1, "atm", "m/s", false), a.getCf_H(1, "atm", false),
                       a.getIs_H(1, "atm", "m/s", true), a.getCf_H(1, "atm", true));
            }

        }
    }
}

//*******************************************
// Print out results for nozzle stations
function printNozzleStations(p) {
    if (p.getStationsListSize()>0) {
        printf("\n");
        printf("**************************************************************************\n");
        printf("Nozzle stations %d\n", p.getStationsListSize());

        for (var i=0; i<p.getStationsListSize(); i++) {
            s = p.getStation(i);
            printNozzleStation(s, s.getAr()===40); // Use functions defined in external script "chamber_station_functions.js"
        }
    }
}

//*******************************************
// Print out chamber design
function printChamberSize(p) {
    printf("\n");
    printf("**************************************************************************\n");
    printf("Chamber size\n");

    csize = p.getChamberSize(50);
    nsize = p.getNozzleSize(true, 50);

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

        // Print out the chaber geometry
        printf("Chamber geometry:\n");
        printf("%3s %8s %8s\n", "#", "x [mm]", "r [mm]");

        if (!!csize["contour"] && csize.contour.length===0) {
            // If object "contour" is not null but csize.contour.length==0,
            // increase number of points
            csize = p.getChamberSize(100);
        }
        if (!!csize["contour"] && csize.contour.length>0) {
            for (var ci=0; ci<csize.contour.length; ci++) {
                var cpoint = csize.contour[ci];
                printf("%3d %8.3f %8.3f\n", ci, cpoint.x, cpoint.r);
            }
        }

        if (!!nsize["contour"] && nsize.contour.length===0) {
            // If object "contour" is not null but nsize.contour.length==0,
            // increase number of points
            nsize = p.getNozzleSize(true, 100);
        }
        if (!!nsize["contour"] && nsize.contour.length>0) {
            var cl = csize.contour.length - 1; // Fist point is skipped (see below), starting with "1"
            for (var ni=1; ni<nsize.contour.length; ni++) { // Start with "1" because first point is throat which is already printed out
                var npoint = nsize.contour[ni];
                printf("%3d %8.3f %8.3f\n", cl+ni, npoint.x, npoint.r);
            }
        }

    } else {
        printf("Combustion chamber size is not available\n");
    }
}

//*******************************************
// Print out chamber heat transfer analysis
function printChamberHeatTransferAnalysis(p) {
    printf("\n");
    printf("**************************************************************************\n");
    printf("Chamber heat transfer analysis\n");

    t = p.getNozzleHeatTransferAnalysis();
    if (!!t) {
        t_results = t.getResults();
        printHeatTransferResults(t_results);
    } else {
        printf("Chamber heat transfer analysis is not available\n");
    }
}

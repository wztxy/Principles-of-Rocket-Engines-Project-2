// This script contains set of functions used in other scripts.

function printThrustChamberSizeWithoutContour(csize, nsize) {
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
    } else {
        printf("Thrust chamber size is not available\n");
    }
}

function printHeatTransferResults(ht_results) {
    printf("\n");
    printf("**************************************************************************\n");
    printf("Heat transfer analysis\n");

    if (!!ht_results) {
        printf("Chamber throttle level R=%4.2f\n", ht_results.throttle_level);
        printf("Convective heat transfer approach: %s\n", ht_results.approach);

        if (!!ht_results["stations"] && ht_results.stations.length>0) {

            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("%3s %8s %8s %10s %10s %10s %10s %8s %6s %8s %6s %8s %8s %10s %10s %10s %6s %10s",
                   "#", "x", "r", "Q total", "Q rad", "Q conv", "alpha", "Twg", "h_bc", "Twi", "h", "Twc", "Tc", "pc", "wc", "rhoc", "id", "type");
            if (ht_results.maxPass>1) {
                printf("%6s %8s %6s %8s %8s %10s %10s %10s %6s %10s\n",
                       "h_bc", "Twi", "h", "Twc", "Tc", "pc", "wc", "rhoc", "id", "type");
            }
            printf("\n");

            printf("%3s %8s %8s %10s %10s %10s %10s %8s %6s %8s %6s %8s %8s %10s %10s %10s %6s %10s",
                   "", "mm", "mm", "kW/m^2", "kW/m^2", "kW/m^2", "kW/m^2-K", "K", "mm", "K", "mm", "K", "K", "MPa", "m/s", "kg/m^3", "", "");
            if (ht_results.maxPass>1) {
                printf("%6s %8s %6s %8s %8s %10s %10s %10s %6s %10s\n",
                       "mm", "K", "mm", "K", "K", "MPa", "m/s", "kg/s", "", "");
            }
            printf("\n");

            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            for (var i=0; i<ht_results.stations.length; i++) {
                var s = ht_results.stations[i];
                printf("%3d %8.2f %8.2f %10.3f %10.3f %10.3f %10.3f %8.3f ", i, s.x*1000, s.r*1000, s.q_total/1000, s.q_rad/1000, s.q_conv/1000, s.alpha/1000, s.Twg);

                if (!!s["cooling"] && s.cooling.length>0) {
                    // Cooling is an array because regen cooling segments can be configured with two passes.
                    for (var j=0; j<s.cooling.length; j++) {
                        var cl = s.cooling[j];

                        var type = cl.type;
                        if (type.startsWith("radiation")) {
                            printf("%6.2f %8.3f %6.2f %8.3f %8s %10s %10s %10s ", cl.bcH*1000, cl.Twi, cl.h*1000, cl.Twc, "-", "-", "-", "-");
                        } else
                        if (type.startsWith("regen channel")) {
                            printf("%6.2f %8.3f %6.2f %8.3f %8.3f %10.3f %10.3f %10.3f ", cl.bcH*1000, cl.Twi, cl.h*1000, cl.Twc, cl.Tc, cl.pc/1e6, cl.wc, cl.rhoc);
                        } else
                        if (type.startsWith("regen tubular")) {
                            printf("%6.2f %8.3f %6.2f %8.3f %8.3f %10.3f %10.3f %10.3f ", cl.bcH*1000, cl.Twi, cl.h*1000, cl.Twc, cl.Tc, cl.pc/1e6, cl.wc, cl.rhoc);
                        } else
                        if (type.startsWith("regen coaxial")) {
                            printf("%6.2f %8.3f %6.2f %8.3f %8.3f %10.3f %10.3f %10.3f ", cl.bcH*1000, cl.Twi, cl.h*1000, cl.Twc, cl.Tc, cl.pc/1e6, cl.wc, cl.rhoc);
                        } else {
                            // Unknown type
                            printf("%6s %8s %6s %8s %8s %10s %10s %10s ", "-", "-", "-", "-", "-", "-", "-", "-");
                        }

                        printf("%6s %10s", cl.id, type);
                    }
                }

                printf("\n");
            }

            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }

        printf("Tw_bar = %8.3f K\n", ht_results.Tw_bar);

    } else {
        printf("Heat transfer results are not available\n");
    }
}

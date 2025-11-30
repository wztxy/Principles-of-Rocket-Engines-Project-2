// Create new configuration from JavaScript object and run propulsion analysis

printf("\n**********************\n");
printf("Original configuration\n");
printf("\n**********************\n");


cfg = new ConfigFile("examples/A-1.cfg");

a1 = new PropulsionAnalysis(cfg, true);

printf("\n**********************\n");
printf("Modified configuration\n");
printf("\n**********************\n");

raw = JSON.parse(cfg.toString());
raw.engineDefinition.chamberPressure.value = 10;
raw.engineDefinition.chamberPressure.units = "MPa";
cfg.fromString(JSON.stringify(raw));

a2 = new PropulsionAnalysis(cfg, true);

printf("\n**********************\n");
printf("Modified configuration loop and with custom output\n");
printf("\n**********************\n");

// Array of pressure values in MPa
p = [10, 15, 20];

raw = JSON.parse(cfg.toString());
raw.engineDefinition.chamberPressure.units = "MPa";

for (var i=0; i<p.length; i++) {
    printf("Pressure p = %5.2f MPa\n", p[i]);

    raw.engineDefinition.chamberPressure.value = p[i];
    cfg.fromString(JSON.stringify(raw));

    pa = new PropulsionAnalysis(cfg, false);
    csize = pa.getChamberSize(50);
    nsize = pa.getNozzleSize(false, 50);

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
    printf("\n**********************\n");
}



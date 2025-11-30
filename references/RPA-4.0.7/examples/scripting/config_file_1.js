// Create new configuration from JavaScript object and run propulsion analysis

raw = {
    version:4,
    application:"RPA",
    engineDefinition:{
       name:"Aestus",
       description:"Test case",
       chamberPressure:{
          value:10.3,
          units:"bar"
       },
       bipropellantMixture:{
          oxidizer:[
             {
                name:"N2O4(L)",
                massFraction:1
             }
          ],
          fuel:[
             {
                name:"CH6N2(L)",
                massFraction:1
             }
          ],
          mixtureRatio:{
             value:2.05
          }
       }
    },
    nozzleConditions:{
       nozzleInletConditions:{
          areaRatio:2.3843
       },
       nozzleExitConditions:{
          areaRatio:84
       },
       equilibriumFlow:{
          shifting:false
       },
       reactionEficiency:{
          estimateReactionEfficiency:true
       },
       nozzleEficiency:{
          assignedNozzleEfficiency:0.98246,
          estimateOverexpansionPerformance:true
       },
       nozzleFlowEffcets:{
          multiphase:true,
          ionization:true
       }
    }
};

printf("Creating configuration file from JavaScript object...\n");

cfg = new ConfigFile();
cfg.fromString(JSON.stringify(raw));

printf("Running configured propulsion analysis...\n");

a = new PropulsionAnalysis(cfg, true);

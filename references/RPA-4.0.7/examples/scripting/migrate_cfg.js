// Script for migrating configuration files from format for RPA 2.x to format for RPA 4.x

if (!!!args || args.length<2) {
    printf("\nUsage: RPA-Script.exe --input ./examples/scripting/migrate_cfg.js inputFile outputFile\n");
    exit();
}

var fname_input = args[0];
var fname_output = args[1];

printf("\nMigrating file '%s' to '%s'...\n", fname_input, fname_output);

// Load old-format configuration file
var input = "";
var f = new File(fname_input, "r");
while (true) {
    var s = f.readLine();
    if (!!!s) {
        break;
    }
    input += String(s);
}
f.close();

// Adjust the configuration to become valid JSON
input = String('{'+input+'}')
    .replace(/= \(/g, ': [')
    .replace(/\);/g, '],')
    .replace(/;/g, ',')
    .replace(/=/g, ':')
    .replace(/(\w+)\s+:/g, '"$1" :')
    .replace(/\r?\n/g, '')
    .replace(/\s*,\s*\}/g, '}')
    .replace(/\s*,\s*\]/g, ']');

// Parse text to JSON object
var obj = JSON.parse(input);

if (!!!obj["version"] || obj["version"]>=4 ) {
    printf("\nInvalid format or version of configuration file %s\n", fname_input);
    exit();
}

var info = sprintf("Converted from configuration file for RPA 2.x:\n%s", fname_input);

// Template to create new configuration file
raw = {
    version: 4,
    application: "RPA",
    engineDefinition: {
       name: "Unknown",
       description: info,
       chamberPressure: {
       }
    },
    nozzleConditions: {
       nozzleInletConditions: {
       },
       nozzleExitConditions: {
       },
       equilibriumFlow: {
          shifting: false
       },
       reactionEficiency: {
          estimateReactionEfficiency: true
       },
       nozzleEficiency: {
          estimateBellNozzleEfficicncy: true, // typo in "Efficicncy" here - it should be, otherwise config won't be correct
          estimateOverexpansionPerformance: false
       },
       nozzleFlowEffcets: {
          multiphase: true,
          ionization: true
       }
    }
};

if (!!obj["name"]) {
    raw.engineDefinition.name = obj["name"];
} else {
    raw.engineDefinition.name = "";
}

if (!!obj["info"]) {
    raw.engineDefinition.description = obj["info"]+"\n"+info;
}

if (!!obj["combustionChamberConditions"] && !!obj["combustionChamberConditions"]["pressure"]) {
    raw.engineDefinition.chamberPressure = {
        value: obj.combustionChamberConditions.pressure["value"],
        units: obj.combustionChamberConditions.pressure["unit"]
    };
}

if (!!obj["generalOptions"]) {
    raw.nozzleConditions.nozzleFlowEffcets = {
        multiphase: !!obj.generalOptions["multiphase"],
        ionization: !!obj.generalOptions["ions"]
    };
}

if (!!obj["propellant"]) {

    if (!!obj.propellant["components"]) {
        raw.engineDefinition.propulsionSystem = "CHEMICAL_BIPROPELLANT";
        raw.engineDefinition.bipropellantMixture = {
            oxidizer:[],
            fuel:[],
            mixtureRatio:[]
        };

        var components = obj.propellant["components"];

        if (!!components["ratio"]) {
            var ratioType = "OF";
            if (!!components.ratio["unit"]) {
                switch (components.ratio["unit"]) {
                case "O/F":
                    ratioType = "OF";
                    break;
                case "alpha":
                    ratioType = "ALPHA";
                    break;
                case "optimal":
                    ratioType = "OPTIMUM";
                    break;
                }
            }
            raw.engineDefinition.bipropellantMixture.mixtureRatio = {
                value: components.ratio["value"],
                type: ratioType
            };
        }

        if (!!components["oxidizer"]) {
            var ox = components["oxidizer"];
            for (var i=0; i<ox.length; i++) {
                var s = ox[i];
                raw.engineDefinition.bipropellantMixture.oxidizer.push(
                    {
                        name: s["name"],
                        massFraction: s["massFraction"]
                    }
                );
                if (!!s["T"]) {
                    raw.engineDefinition.bipropellantMixture.oxidizer[i].temperature = {
                        value: s.T["value"],
                        units: s.T["unit"]
                    };
                }
            }
        }

        if (!!components["fuel"]) {
            var fuel = components["fuel"];
            for (var i=0; i<fuel.length; i++) {
                var s = fuel[i];
                raw.engineDefinition.bipropellantMixture.fuel.push(
                    {
                        name: s["name"],
                        massFraction: s["massFraction"]
                    }
                );
                if (!!s["T"]) {
                    raw.engineDefinition.bipropellantMixture.fuel[i].temperature = {
                        value: s.T["value"],
                        units: s.T["unit"]
                    };
                }
            }
        }
    }

    if (!!obj.propellant["mixture"]) {
        raw.engineDefinition.propulsionSystem = "CHEMICAL_MONOPROPELLANT";
        raw.engineDefinition.propellantMixture = {
            propellant:[]
        };

        var mixture = obj.propellant["mixture"];

        if (!!mixture["species"]) {
            var species = mixture["species"];
            for (var i=0; i<species.length; i++) {
                var s = species[i];
                raw.engineDefinition.propellantMixture.propellant.push(
                    {
                        name: s["name"],
                        massFraction: s["massFraction"]
                    }
                );
                if (!!s["T"]) {
                    raw.engineDefinition.propellantMixture.propellant[i].temperature = {
                        value: s.T["value"],
                        units: s.T["unit"]
                    };
                }
            }
        }
    }

}

if (!!obj["nozzleFlow"] && !!obj.nozzleFlow["calculateNozzleFlow"]) {

    if (!!obj.nozzleFlow["nozzleInletConditions"]) {
        if (!!obj.nozzleFlow.nozzleInletConditions["contractionAreaRatio"]) {
            raw.nozzleConditions.nozzleInletConditions = {
                areaRatio: obj.nozzleFlow.nozzleInletConditions.contractionAreaRatio
            };
        } else
        if (!!obj.nozzleFlow.nozzleInletConditions["massFlux"]) {
            var unit = "kg_over_m2_s";
            switch(obj.nozzleFlow.nozzleInletConditions.massFlux["unit"]) {
            case "lbm/(ft2-s)":
                unit = "lbm_over_ft2_s";
                break;
            case "kg/(m2-s)":
            default:
                unit = "kg_over_m2_s";
                break;
            }
            raw.nozzleConditions.nozzleInletConditions.massFlux = {
                value: obj.nozzleFlow.nozzleInletConditions.massFlux["value"],
                units: unit
            };
        } else {
            raw.nozzleConditions.nozzleInletConditions = {
                infinityArea: true
            };
        }
    }

    if (!!obj.nozzleFlow["nozzleExitConditions"]) {
        if (!!obj.nozzleFlow.nozzleExitConditions["pressure"]) {
            raw.nozzleConditions.nozzleExitConditions.pressure = {
                value: obj.nozzleFlow.nozzleExitConditions.pressure["value"],
                units: obj.nozzleFlow.nozzleExitConditions.pressure["unit"]
            };
        } else
        if (!!obj.nozzleFlow.nozzleExitConditions["areaRatio"]) {
            raw.nozzleConditions.nozzleExitConditions = {
                areaRatio: obj.nozzleFlow.nozzleExitConditions.areaRatio
            };
        } else
        if (!!obj.nozzleFlow.nozzleExitConditions["pressureRatio"]) {
            raw.nozzleConditions.nozzleExitConditions = {
                pressureRatio: obj.nozzleFlow.nozzleExitConditions.pressureRatio
            };
        }
    }

    if (!!obj.nozzleFlow["freezingConditions"] && !!obj.nozzleFlow.freezingConditions["calculate"]) {
        if (!!obj.nozzleFlow.freezingConditions["pressureRatio"]) {
            raw.nozzleConditions.equilibriumFlow = {
               frozenAtPressureRatio: obj.nozzleFlow.freezingConditions["pressureRatio"]
            };
        } else
        if (!!obj.nozzleFlow.freezingConditions["expansionRatio"]) {
            raw.nozzleConditions.equilibriumFlow = {
               frozenAtAreaRatio: obj.nozzleFlow.freezingConditions["expansionRatio"]
            };
        }
    } else {
        raw.nozzleConditions.equilibriumFlow = {
           shifting: true
        };
    }

    if (!!obj.nozzleFlow["efficiencies"]) {
        if (!!obj.nozzleFlow.efficiencies["nozzle"]) {
            raw.nozzleConditions.nozzleEficiency = {
               assignedNozzleEfficiency: obj.nozzleFlow.efficiencies["nozzle"]
            };
        } else
        if (!!obj.nozzleFlow.efficiencies["nozzleLength"]) {
            raw.nozzleConditions.nozzleEficiency = {
               bellNozzleWithLength: obj.nozzleFlow.efficiencies["nozzleLength"]
            };
        } else
        if (!!obj.nozzleFlow.efficiencies["nozzleHalfAngle"]) {
            raw.nozzleConditions.nozzleEficiency = {
               conicalNozzleWithHalfAngle: obj.nozzleFlow.efficiencies["nozzleHalfAngle"]
            };
        } else {
            raw.nozzleConditions.nozzleEficiency = {
               estimateBellNozzleEfficicncy: true // typo in "Efficicncy" here - it should be, otherwise config won't be correct
            };
        }
    }

    if (!!obj["generalOptions"]) {
        raw.nozzleConditions.nozzleEficiency.estimateOverexpansionPerformance = !!obj.generalOptions["flowSeparation"];
    }

    if (!!obj.nozzleFlow["ambientConditions"]) {
        if (!!obj.nozzleFlow.ambientConditions["pressure"]) {
            raw.nozzleConditions.ambientConditions = {
                fixed: {
                    value: obj.nozzleFlow.ambientConditions.pressure["value"],
                    units: obj.nozzleFlow.ambientConditions.pressure["unit"]
                }
            };
        } else
        if (!!obj.nozzleFlow.ambientConditions["minPressure"] && !!obj.nozzleFlow.ambientConditions["maxPressure"]) {
            raw.nozzleConditions.ambientConditions = {
                range: {
                    from: {
                        value: obj.nozzleFlow.ambientConditions.minPressure["value"],
                        units: obj.nozzleFlow.ambientConditions.minPressure["unit"]
                    },
                    to: {
                        value: obj.nozzleFlow.ambientConditions.maxPressure["value"],
                        units: obj.nozzleFlow.ambientConditions.maxPressure["unit"]
                    },
                    numberOfPoints: 30
                }
            };
        }
    }

    if (!!obj.nozzleFlow["throttlingConditions"]) {
        if (!!obj.nozzleFlow.throttlingConditions["flowrate"]) {
            raw.nozzleConditions.throttleSettings = {
                fixed: obj.nozzleFlow.throttlingConditions["flowrate"]
            };
        } else
        if (!!obj.nozzleFlow.throttlingConditions["minFlowrate"] && !!obj.nozzleFlow.throttlingConditions["maxFlowrate"]) {
            raw.nozzleConditions.throttleSettings = {
                range: {
                    min: obj.nozzleFlow.throttlingConditions["minFlowrate"],
                    max: obj.nozzleFlow.throttlingConditions["maxFlowrate"],
                    numberOfPoints: 10
                }
            };
        }
    }

} else {
    delete raw.nozzleConditions;
}

if (!!obj["engineSize"]) {
    raw.thrustChamberSize = {
        sizing: {
            numberOfChambers: 1
        },
        combustionChamberGeometry: {
        },
        nozzleGeometry: {
            RntoRt: 0.382
        }
    };

    if (!!obj.engineSize["thrust"]) {
        raw.thrustChamberSize.sizing.thrust = {
            value:{
                value: obj.engineSize.thrust["value"],
                units: obj.engineSize.thrust["unit"]
            }
        };
        if (!!obj.engineSize["ambientConditions"]) {
            raw.thrustChamberSize.sizing.thrust.ambientPressure = {
                value: obj.engineSize.ambientConditions["value"],
                units: obj.engineSize.ambientConditions["unit"]
            };
        }
    } else
    if (!!obj.engineSize["throatDiameter"]) {
        raw.thrustChamberSize.sizing.throatDiameter = {
            value: obj.engineSize.throatDiameter["value"],
            units: obj.engineSize.throatDiameter["unit"]
        };
    } else
    if (!!obj.engineSize["flowrate"]) {
        var unit = "kg_over_s";
        switch(obj.engineSize.flowrate["unit"]) {
        case "lbm/s":
            unit = "lbm_over_s";
            break;
        case "kg/s":
        default:
            unit = "kg_over_s";
            break;
        }
        raw.thrustChamberSize.sizing.mdot = {
            value: obj.engineSize.flowrate["value"],
            units: unit
        };
    }

    if (!!obj.engineSize["chambersNo"]) {
        raw.thrustChamberSize.sizing.numberOfChambers = obj.engineSize["chambersNo"];
    }

    if (!!obj.engineSize["chamberGeometry"]) {
        if (!!obj.engineSize.chamberGeometry["length"]) {
            raw.thrustChamberSize.combustionChamberGeometry.chamberLength = {
                value: {
                    value: obj.engineSize.chamberGeometry.length["value"],
                    units: obj.engineSize.chamberGeometry.length["unit"]
                },
                type: !!!obj.engineSize.chamberGeometry["characteristicLength"]?"Lc":"Lstar"
            };
        }
        if (!!obj.engineSize.chamberGeometry["contractionAngle"]) {
            raw.thrustChamberSize.combustionChamberGeometry.contractionAngle = obj.engineSize.chamberGeometry["contractionAngle"];
        }
        if (!!obj.engineSize.chamberGeometry["R1_to_Rt_ratio"]) {
            raw.thrustChamberSize.combustionChamberGeometry.R1toRt = obj.engineSize.chamberGeometry["R1_to_Rt_ratio"];
        }
        if (!!obj.engineSize.chamberGeometry["R2_to_R2max_ratio"]) {
            raw.thrustChamberSize.combustionChamberGeometry.R1toRt = obj.engineSize.chamberGeometry["R2_to_R2max_ratio"];
        }

        if (!!obj.engineSize.chamberGeometry["TOC"] || !!obj.engineSize.chamberGeometry["TOC_L"]) {
            // MOC TIC
            raw.thrustChamberSize.nozzleGeometry = {
                ticContour:{
                }
            };
            if (!!obj.engineSize.chamberGeometry["Tw_to_T0"]) {
                raw.thrustChamberSize.nozzleGeometry.ticContour.temperature = obj.engineSize.chamberGeometry["Tw_to_T0"];
            }
            if (!!obj.engineSize.chamberGeometry["TOC_L"]) {
                raw.thrustChamberSize.nozzleGeometry.ticContour.type = "MAX_THRUST_FIXED_AREA_RATIO_AND_LENGTH";
                if (!!obj.engineSize.chamberGeometry["Le"]) {
                    raw.thrustChamberSize.nozzleGeometry.ticContour.length = obj.engineSize.chamberGeometry["Le"];
                }
            }
        } else {
            // Parabolic
            raw.thrustChamberSize.nozzleGeometry = {
                parabolicContour: {
                }
            };
            if (!!obj.engineSize.chamberGeometry["initialAngle"]) {
                raw.thrustChamberSize.nozzleGeometry.parabolicContour.Tn = obj.engineSize.chamberGeometry["initialAngle"];
            }
            if (!!obj.engineSize.chamberGeometry["exitAngle"]) {
                raw.thrustChamberSize.nozzleGeometry.parabolicContour.Te = obj.engineSize.chamberGeometry["exitAngle"];
            }
            if (!!obj.engineSize.chamberGeometry["Le_to_Lc15"]) {
                raw.thrustChamberSize.nozzleGeometry.parabolicContour.LeC15 = obj.engineSize.chamberGeometry["Le_to_Lc15"];
            }
        }

        if (!!obj.engineSize.chamberGeometry["Rn_to_Rt_ratio"]) {
            raw.thrustChamberSize.nozzleGeometry.RntoRt = obj.engineSize.chamberGeometry["Rn_to_Rt_ratio"];
        }

    }
}


if (!!obj["chamberCooling"]) {
    raw.thermalAnalysis = {
        generalOptions: {
            throttleLevel: 1,
            numberOfStations: 50
        },
        convectiveHeatTransfer: {
            type: "IEVLEV",
            wallLayerThickness: 0.025
        },
        radiationHeatTransfer: {
            emissivity: 0.8
        }
    };

    if (!!obj.chamberCooling["heatTransfer"]) {

        if (!!obj.chamberCooling.heatTransfer["relationsType"]) {
            switch(obj.chamberCooling.heatTransfer["relationsType"]) {
            case "Bartz":
                raw.thermalAnalysis.convectiveHeatTransfer.type = "BARTZ";
                break;
            case "Combined":
                raw.thermalAnalysis.convectiveHeatTransfer.type = "COMBINED";
                break;
            case "Ievlev":
            default:
                raw.thermalAnalysis.convectiveHeatTransfer.type = "IEVLEV";
                break;
            }
        }

        if (!!obj.chamberCooling.heatTransfer["wallLayerHeight"]) {
            raw.thermalAnalysis.convectiveHeatTransfer.wallLayerThickness = obj.chamberCooling.heatTransfer["wallLayerHeight"];
        }

        if (!!obj.chamberCooling.heatTransfer["numberOfStations"]) {
            raw.thermalAnalysis.generalOptions.numberOfStations = obj.chamberCooling.heatTransfer["numberOfStations"];
        }

        if (!!obj.chamberCooling.heatTransfer["radiationHeatTransfer"] && !!obj.chamberCooling.heatTransfer.radiationHeatTransfer["hotSideWallSurfaceEmissivity"]) {
            raw.thermalAnalysis.radiationHeatTransfer.emissivity = obj.chamberCooling.heatTransfer.radiationHeatTransfer["hotSideWallSurfaceEmissivity"];
        } else {
            delete raw.thermalAnalysis.radiationHeatTransfer;
        }

    } else {
        delete raw.thermalAnalysis.convectiveHeatTransfer;
        delete raw.thermalAnalysis.radiationHeatTransfer;
    }

    if (!!obj.chamberCooling["chamberCoolingSections"]) {
        raw.thermalAnalysis.generalOptions.applyCooling = {
            designCoolingJacketGeometry : false
        };
        raw.thermalAnalysis.coolingSegments = [];

        var sections = obj.chamberCooling["chamberCoolingSections"];
        for (var i=0; i<sections.length; i++) {
            var s = sections[i];
            var c = null;

            var segment = null;

            if (!!s["channelJacketDesign"]) {
                var c = s.channelJacketDesign;

                raw.thermalAnalysis.coolingSegments.push(
                    {
                        regenCooling: {
                            type: "CHANNEL_DESIGN",
                            location: {
                                value: 0,
                                units: "mm"
                            },
                            wall: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            },
                            thermalBarrier: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            },
                            channelGeometry: {
                            },
                            ribGeometry: {
                            }
                        }
                    }
                );
                var index = raw.thermalAnalysis.coolingSegments.length-1;
                segment = raw.thermalAnalysis.coolingSegments[index]["regenCooling"];

                if (!!c["height1"]) {
                    segment.param1 = {
                        value: c.height1["value"],
                        units: c.height1["unit"]
                    };
                }
                if (!!c["height_min"]) {
                    segment.param2 = {
                        value: c.height_min["value"],
                        units: c.height_min["unit"]
                    };
                }
                if (!!c["height2"]) {
                    segment.param3 = {
                        value: c.height2["value"],
                        units: c.height2["unit"]
                    };
                }

                if (!!c["a1"]) {
                    segment.channelGeometry.a1 = {
                        value: c.a1["value"],
                        units: c.a1["unit"]
                    };
                    delete segment.ribGeometry;
                }

                if (!!c["amin"]) {
                    segment.channelGeometry.amin = {
                        value: c.amin["value"],
                        units: c.amin["unit"]
                    };
                    delete segment.ribGeometry;
                }
                if (!!c["a2"]) {
                    segment.channelGeometry.a2 = {
                        value: c.a2["value"],
                        units: c.a2["unit"]
                    };
                    delete segment.ribGeometry;
                }

                if (!!c["b1"]) {
                    segment.ribGeometry.a1 = {
                        value: c.b1["value"],
                        units: c.b1["unit"]
                    };
                    delete segment.channelGeometry;
                }

                if (!!c["bmin"]) {
                    segment.ribGeometry.amin = {
                        value: c.bmin["value"],
                        units: c.bmin["unit"]
                    };
                    delete segment.channelGeometry;
                }
                if (!!c["b2"]) {
                    segment.ribGeometry.a2 = {
                        value: c.b2["value"],
                        units: c.b2["unit"]
                    };
                    delete segment.channelGeometry;
                }

                if (!!c["gamma"]) {
                    segment.helixAngel = c["helix"];
                }

                if (!!c["n"]) {
                    segment.number = c["n"];
                }

            }

            if (!!s["tubularJacketDesign"]) {
                var c = s.tubularJacketDesign;

                raw.thermalAnalysis.coolingSegments.push(
                    {
                        regenCooling: {
                            type: "TUBULAR_DESIGN",
                            location: {
                                value: 0,
                                units: "mm"
                            },
                            wall: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            },
                            thermalBarrier: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            }
                        }
                    }
                );
                var index = raw.thermalAnalysis.coolingSegments.length-1;
                segment = raw.thermalAnalysis.coolingSegments[index]["regenCooling"];

                if (!!c["n"]) {
                    segment.number = c["n"];
                }

                if (!!c["helix"]) {
                    segment.helix = c["helix"];
                }

                if (!!c["d"]) {
                    segment.param1 = {
                        value: c.d["value"],
                        units: c.d["unit"]
                    };
                    segment.param3 = {
                        value: c.d["value"],
                        units: c.d["unit"]
                    };
                }

            }

            if (!!s["slotJacketDesign"]) {
                var c = s.slotJacketDesign;

                raw.thermalAnalysis.coolingSegments.push(
                    {
                        regenCooling: {
                            type: "COAXIAL_SHELLS",
                            location: {
                                value: 0,
                                units: "mm"
                            },
                            wall: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            },
                            thermalBarrier: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            }
                        }
                    }
                );
                var index = raw.thermalAnalysis.coolingSegments.length-1;
                segment = raw.thermalAnalysis.coolingSegments[index]["regenCooling"];

                if (!!c["height"]) {
                    segment.param1 = {
                        value: c.height["value"],
                        units: c.height["unit"]
                    };
                }

            }

            if (!!s["radiationCooling"]) {
                var c = s.radiationCooling;

                raw.thermalAnalysis.coolingSegments.push(
                    {
                        radiationCooling: {
                            location: {
                                value: 0,
                                units: "mm"
                            },
                            wall: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            },
                            thermalBarrier: {
                                thickness: {
                                },
                                conductivity: {
                                }
                            }
                        }
                    }
                );
                var index = raw.thermalAnalysis.coolingSegments.length-1;
                segment = raw.thermalAnalysis.coolingSegments[index]["radiationCooling"];

                if (!!c["coldSideWallSurfaceEmissivity"]) {
                    segment.emissivity = c["coldSideWallSurfaceEmissivity"];
                }

            }

            if (!!segment && !!c) {
                if (!!c["location"]) {
                    segment.location = {
                        value: c.location["value"],
                        units: c.location["unit"]
                    };
                }

                if (!!c["wallThickness"]) {
                    segment.wall.thickness = {
                        value: c.wallThickness["value"],
                        units: c.wallThickness["unit"]
                    };
                } else {
                    delete segment.wall.thickness;
                }

                if (!!c["wallConductivity"]) {
                    var unit = "W_over_m_K";
                    switch(c.wallConductivity["unit"]) {
                    case "Btu/(hr ft R)":
                        unit = "Btu_over_hr_ft_R"
                        break;
                    case "Btu/(s in R)":
                        unit = "Btu_over_s_in_R"
                        break;
                    case "W/(m K)":
                    default:
                        unit = "W_over_m_K"
                        break;
                    }
                    segment.wall.conductivity = {
                        value: c.wallConductivity["value"],
                        units: unit
                    };
                } else {
                    delete segment.wall.conductivity;
                }

                if (!!c["thermalInsulationThickness"]) {
                    segment.thermalBarrier.thickness = {
                        value: c.thermalInsulationThickness["value"],
                        units: c.thermalInsulationThickness["unit"]
                    };

                    if (!!c["thermalInsulationConductivity"]) {
                        var unit = "W_over_m_K";
                        switch(c.thermalInsulationConductivity["unit"]) {
                        case "Btu/(hr ft R)":
                            unit = "Btu_over_hr_ft_R"
                            break;
                        case "Btu/(s in R)":
                            unit = "Btu_over_s_in_R"
                            break;
                        case "W/(m K)":
                        default:
                            unit = "W_over_m_K"
                            break;
                        }
                        segment.thermalBarrier.conductivity = {
                            value: c.thermalInsulationConductivity["value"],
                            units: unit
                        };
                    } else {
                        delete segment.thermalBarrier.conductivity;
                    }

                } else {
                    delete segment.thermalBarrier;
                }

                if (!!c["coolant"]) {
                    segment.coolant = {
                        coolant: []
                    };
                    var p_ = null;
                    var T_ = null;

                    for (var j=0; j<c.coolant.length; j++) {
                        if (c.coolant[j]["name"]) {
                            segment.coolant.coolant.push(
                                {
                                    name: c.coolant[j]["name"],
                                    massFraction: c.coolant[j]["massFraction"]
                                }
                            );
                        }
                        if (c.coolant[j]["p"]) {
                            p_ = {
                                value: c.coolant[j].p["value"],
                                units: c.coolant[j].p["unit"]
                            };
                        }
                        if (c.coolant[j]["T"]) {
                            T_ = {
                                value: c.coolant[j].T["value"],
                                units: c.coolant[j].T["unit"]
                            };
                        }
                    }

                    if (!!p_) {
                        segment.coolant.pressure = p_;
                    }
                    if (!!T_) {
                        segment.coolant.temperature = T_;
                    }

                    if (!!c["flowrate"]) {
                        segment.coolant.mdot = c["flowrate"]*100.0;
                    }

                } else
                if (!!c["coolantFrom"]) {
                    segment.fromExitId = c["coolantFrom"];
                }

                if (!!c["oppositeFlow"]) {
                    segment.oppositeFlow = c["oppositeFlow"];
                }

                if (!!c["twoPass"]) {
                    segment.twoPass = c["twoPass"];
                }

                if (!!c["id"]) {
                    segment.exitId = c["id"];
                }

            }

        }

    }

    if (!!obj.chamberCooling["filmSlots"]) {
        if (!!!raw.thermalAnalysis["coolingSegments"]) {
            raw.thermalAnalysis.coolingSegments = [];
        }

        var slots = obj.chamberCooling["filmSlots"];
        for (var i=0; i<slots.length; i++) {
            var s = slots[i];

            raw.thermalAnalysis.coolingSegments.push(
                {
                    filmCooling: {
                        location: {
                        },
                        coolant: []
                    }
                }
            );
            var index = raw.thermalAnalysis.coolingSegments.length-1;
            segment = raw.thermalAnalysis.coolingSegments[index]["filmCooling"];

            if (!!s["location"]) {
                segment.location = {
                    value: s.location["value"],
                    units: s.location["unit"]
                };
            } else {
                delete segment.location;
            }

            if (!!s["flowrate"]) {
                segment.mdot = s["flowrate"]*100.0;
            }

            if (!!s["coolant"]) {
                for (var j=0; j<s.coolant.length; j++) {
                    if (s.coolant[j]["name"]) {
                        segment.coolant.push(
                            {
                                name: s.coolant[j]["name"],
                                massFraction: s.coolant[j]["massFraction"]
                            }
                        );
                    }
                }
            } else {
                delete segment.coolant;
            }
        }

    }

}

// Create new configuration file
printf("Provided configuration file succesfully processed.\n");
cfg = new ConfigFile();
cfg.fromString(JSON.stringify(raw));
cfg.write(fname_output);
printf("Created file '%s'\n", fname_output);
printf("Done!\n\n");

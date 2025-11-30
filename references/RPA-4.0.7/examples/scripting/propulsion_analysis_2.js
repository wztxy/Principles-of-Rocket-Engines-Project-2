cfg = new ConfigFile("examples/A-1.cfg");

p = new PropulsionAnalysis(cfg, false); // false - don't print out the results

tp = p.getPerformanceAnalysis();
tp.printResults();

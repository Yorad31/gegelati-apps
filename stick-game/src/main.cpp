#include <iostream>
#include <unordered_set>
#include <numeric>
#include <string>
#include <cfloat>
#include <inttypes.h>

#include <gegelati.h>

#include "stickGameAdversarial.h"
#include "resultTester.h"



int main(int argc, char* argv[]) {
	if (argc > 2 && (strcmp("-evaluate", argv[1]) == 0)) {
		agentTest(argv[2]);
	}

	// Create the instruction set for programs
	Instructions::Set set;
	auto minus = [](int a, int b)->double {return (double)a - (double)b; };
	auto cast = [](int a)->double {return (double)a; };
	auto add = [](double a, double b)->double {return a + b; };
	auto max = [](double a, double b)->double {return std::max(a, b); };
	auto nulltest = [](double a)->double {return (a == 0.0) ? 10.0 : 0.0; };
	auto modulo = [](double a, double b)->double {
		if (b != 0.0) { return fmod(a, b); }
		else { return  DBL_MIN; }	};

	set.add(*(new Instructions::LambdaInstruction<double, double>(modulo)));
	set.add(*(new Instructions::LambdaInstruction<int, int>(minus)));
	set.add(*(new Instructions::LambdaInstruction<double, double>(add)));
	set.add(*(new Instructions::LambdaInstruction<int>(cast)));
	set.add(*(new Instructions::LambdaInstruction<double, double>(max)));
	set.add(*(new Instructions::LambdaInstruction<double>(nulltest)));

	// Set the parameters for the learning process.
	// (Controls mutations probability, program lengths, and graph size
	// among other things)
	// Loads them from the file params.json
	Learn::LearningParameters params;
	File::ParametersParser::loadParametersFromJson(ROOT_DIR "/params.json", params);
#ifdef NB_GENERATIONS
	params.nbGenerations = NB_GENERATIONS;
#endif // !NB_GENERATIONS


	// Instantiate the LearningEnvironment
	StickGameAdversarial le;

	// Instantiate and init the learning agent
	Learn::ParallelLearningAgent la(le, set, params);
	la.init();

	// Use the basic logging
	Log::LABasicLogger logger(la);

	// Create an exporter for all graphs
	File::TPGGraphDotExporter dotExporter("out_000.dot", la.getTPGGraph());

	// File for printing best policy stat.
	std::ofstream stats;
	stats.open("bestPolicyStats.md");
	Log::LAPolicyStatsLogger logStats(la, stats);

	// Export parameters before starting training.
	// These may differ from imported parameters because of LE or machine specific
	// settings such as thread count of number of actions.
	File::ParametersParser::writeParametersToJson("exported_params.json", params);

	// Train for NB_GENERATIONS generations
	for (int i = 0; i < params.nbGenerations; i++) {
		char buff[16];
		sprintf(buff, "out_%03d.dot", i);
		dotExporter.setNewFilePath(buff);
		dotExporter.print();

		la.trainOneGeneration(i);
	}

	// Keep best policy
	la.keepBestPolicy();

	// Clear introns instructions
	la.getTPGGraph().clearProgramIntrons();

	// Export the graph
	dotExporter.setNewFilePath("out_best.dot");
	dotExporter.print();

	TPG::PolicyStats ps;
	ps.setEnvironment(la.getTPGGraph().getEnvironment());
	ps.analyzePolicy(la.getBestRoot().first);
	std::ofstream bestStats;
	bestStats.open("out_best_stats.md");
	bestStats << ps;
	bestStats.close();

	// close log file also
	stats.close();

	// cleanup
	for (unsigned int i = 0; i < set.getNbInstructions(); i++) {
		delete (&set.getInstruction(i));
	}

	return 0;
}

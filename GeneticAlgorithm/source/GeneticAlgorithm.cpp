#include "GeneticAlgorithm.h"
#include "MidIO.h"
#include "CompositionGenerator.h"
#include "Fitness.h"
#include "Selection.h"
#include "Mutation.h"
#include "Crossover.h"
#include "Parameters.h"
#include "RunInfo.h"

#include <time.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void GA_RunAlgorithm(Parameters params)
{
    g_AlgorithmParameters = params;
	//string midiDirectory = AlgorithmParameters.mainDirectory + "midi/";
	//string csvDirectory = AlgorithmParameters.mainDirectory + "csv/";
	string overallRunsInfo;
	//string midiSysCall = "\"mkdir \"" + AlgorithmParameters.mainDirectory + "midi/\"\"";
	//string csvSysCall = "\"mkdir \"" + AlgorithmParameters.mainDirectory + "csv/\"\"";
	//system(midiSysCall.c_str()); system(csvSysCall.c_str()); //make directories

	for (auto runIndex = 0; runIndex < g_AlgorithmParameters.m_iNumRuns; runIndex++)
    {
		cout << "Starting Run " << runIndex + 1 << endl;

		/*RUNNING OF ALGORITHM*/
		Population initialPop = INIT_GeneratePopulation();
		RunInfo runInfo;
		Population population = initialPop;
		clock_t totalTime = clock(), totalFitnessTime(0), totalSelectionTime(0), totalMutationTime(0), totalCrossoverTime(0);
		double topFitness = 0.0, oldTopFitness = 0.0;
		int stagnateCtr = 0;
		for (auto gen = 0; gen <= g_AlgorithmParameters.m_iNumGenerations && topFitness <= g_AlgorithmParameters.m_dbThreshold; gen++)
        {
			//evaluate each individual
			clock_t fitnessTime = clock();
			OP_EvaluateAll(&population);
			totalFitnessTime += clock() - fitnessTime;

			//set population fitness for this generation
			runInfo.m_vecPopFitnesses.push_back(population.AvgFitness());
			//get best individual
			int bestIndividualIndex = population.GetBestFitIndex();
			runInfo.m_vecBestIndividuals.push_back(population[bestIndividualIndex]);
			double currentBestFitness = population[bestIndividualIndex].GetFitness();
			if (oldTopFitness + 0.000001 >= currentBestFitness)
				stagnateCtr++;
			else stagnateCtr = 0;
			oldTopFitness = currentBestFitness;
			if (stagnateCtr >= 100) break;

			if (gen >= g_AlgorithmParameters.m_iNumGenerations) break; //done

			//perform selection of elites
			clock_t selectionTime = clock();
			vector<Chromosome> elites = OP_SelectElites(population);
			totalSelectionTime += clock() - selectionTime;

			//generate mutations
			clock_t mutationTime = clock();
			vector<Chromosome> mutations = OP_MutateElites(elites);
			totalMutationTime += clock() - mutationTime;

			//generate crossovers
			clock_t crossoverTime = clock();
			vector<Chromosome> crossovers = OP_CrossElites(elites);
			totalCrossoverTime += clock() - crossoverTime;

			//combine elites, mutations, and crossovers to get a new population
			population.ClearChromosomes();
			population.AddChromosomes(elites);
			population.AddChromosomes(mutations);
			population.AddChromosomes(crossovers);

			topFitness = runInfo.m_vecBestIndividuals[gen].GetFitness();
			cout << "Completed generation " << gen + 1 << " with best fit individual having " <<
				topFitness << " fitness, with average population fitness " <<
				runInfo.m_vecPopFitnesses[gen] << " and stagnate count of " << stagnateCtr << endl;
		}
		runInfo.m_clkTotalRuntime = clock() - totalTime;
		runInfo.m_clkFitnessRuntime = totalFitnessTime;
		runInfo.m_clkSelectionRuntime = totalSelectionTime;
		runInfo.m_clkMutationRuntime = totalMutationTime;
		runInfo.m_clkCrossoverRuntime = totalCrossoverTime;
		runInfo.m_Best = runInfo.m_vecBestIndividuals[runInfo.m_vecBestIndividuals.size() - 1];
		runInfo.m_dbFinalPopFitness = runInfo.m_vecPopFitnesses[runInfo.m_vecPopFitnesses.size() - 1];
		runInfo.m_FinalPop = population;
		/*END ALGORITHM RUN*/
		cout << "Finished Run " << runIndex + 1 << endl;

		//directory name for this run
		string dirname, makeDirname;

		//make midi directory
		dirname = g_AlgorithmParameters.m_StrMainOutputDir + "run" + to_string(runIndex + 1) + "finalPopMidi/";
		makeDirname = "\"mkdir \"" + dirname + "\"\"";
		system(makeDirname.c_str());

		cout << "Writing MIDI files to " + dirname << endl;
		//write MIDI files of run
		//mididirectory/run#/#fitness.mid
        const int iFinalPopSize = runInfo.m_FinalPop.Size();
		for (auto i = 0; i < iFinalPopSize; i++)
        {
            MidIO_CompToMIDI(dirname + "individual" + to_string(i) + "fitness" +
				to_string(runInfo.m_FinalPop[i].GetFitness()) + ".mid", runInfo.m_FinalPop[i].GetComposition());
		}
		//write the best composition of this run
		//IO::writeCompositionToMIDI(dirname + "best.mid", runInfo.best.composition());
        MidIO_CompToMIDI(g_AlgorithmParameters.m_StrMainOutputDir + "run" + to_string(runIndex + 1) + "best.mid", runInfo.m_Best.GetComposition());

		//make csv directory
		dirname = g_AlgorithmParameters.m_StrMainOutputDir + "run" + to_string(runIndex + 1) + "csv/";
		makeDirname = "\"mkdir \"" + dirname + "\"\"";
		system(makeDirname.c_str());

		cout << "Writing CSV files to " + dirname << endl; 
		//write csv files of run
		ofstream runInfoFile;
		runInfoFile.open(dirname + "run_info.csv");
		runInfoFile << "Generation,Top Fitnesses,Total Fitnesses,Age of Best" << endl;
		for (auto i = 0; i < runInfo.m_vecBestIndividuals.size(); i++)
        {
			runInfoFile << i + 1u << "," << runInfo.m_vecBestIndividuals[i].GetFitness() << "," << runInfo.m_vecPopFitnesses[i] 
				<< "," << runInfo.m_vecBestIndividuals[i].GetAge() << endl;
		}
		runInfoFile.close();

		overallRunsInfo += to_string(runInfo.m_Best.GetFitness()) + "," + to_string(runInfo.m_dbFinalPopFitness) + "," +
			to_string(runInfo.m_clkTotalRuntime / 1000.0) + "," + to_string(runInfo.m_clkFitnessRuntime / 1000.0) + "," +
			to_string(runInfo.m_clkSelectionRuntime / 1000.0) + "," + to_string(runInfo.m_clkMutationRuntime / 1000.0) + "," +
			to_string(runInfo.m_clkCrossoverRuntime / 1000.0) + "\n";
	}

	cout << "Writing info about all runs to " + g_AlgorithmParameters.m_StrMainOutputDir + "allruns.csv" << endl;
	ofstream runsInfoFile;
	runsInfoFile.open(g_AlgorithmParameters.m_StrMainOutputDir + "allruns.csv");
	runsInfoFile << "Final Highest Fitness (0-1),Final Population Fitness (0-1),Time to Run (s),Fitness Time (s),Selection Time (s), Mutation Time (s), Crossover Time (s)" << endl;
	runsInfoFile << overallRunsInfo;
	runsInfoFile.close();
}

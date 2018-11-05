#include "GeneticAlgorithm.h"
#include "CompositionGenerator.h"
#include "Fitness.h"
#include "Selection.h"
#include "Mutation.h"
#include "Crossover.h"
#include <time.h>
#include <vector>
#include <iostream>

geneticalgorithm::RunInfo geneticalgorithm::runGA(Population initialPop) {
	RunInfo runInfo;
	Population population = initialPop;
	clock_t totalTime = clock(), totalFitnessTime(0), totalSelectionTime(0), totalMutationTime(0), totalCrossoverTime(0);
	for (unsigned int gen = 0; gen <= AlgorithmParameters.numGenerations; gen++) {
		//evaluate each individual
		clock_t fitnessTime = clock();
		fitness::evaluateAll(&population);
		totalFitnessTime += clock() - fitnessTime;

		//set population fitness for this generation
		runInfo.popFitnesses.push_back(population.avgFitness());
		//get best individual
		unsigned int bestIndividualIndex = population.getBestFitIndex();
		runInfo.bestIndividuals.push_back(population[bestIndividualIndex]);

		if (gen >= AlgorithmParameters.numGenerations) break; //done

		//perform selection of elites
		clock_t selectionTime = clock();
		std::vector<Chromosome> elites = operators::selection::selectElites(population);
		totalSelectionTime += clock() - selectionTime;
		//generate mutations
		clock_t mutationTime = clock();
		std::vector<Chromosome> mutations = operators::mutation::mutateElites(elites);
		totalMutationTime += clock() - mutationTime;

		//generate crossovers
		clock_t crossoverTime = clock();
		std::vector<Chromosome> crossovers = operators::crossover::crossElites(elites);
		totalCrossoverTime = clock() - crossoverTime;

		//combine elites, mutations, and crossovers to get a new population
		population.clearChromosomes(); 
		population.addChromosomes(elites);
		population.addChromosomes(mutations);
		population.addChromosomes(crossovers);

		std::cout << "Completed generation " << gen + 1 << " with best fit individual having " <<
			runInfo.bestIndividuals[gen].fitness() << " fitness, with average population fitness " << 
			runInfo.popFitnesses[gen] << std::endl;
	}

	runInfo.totalRuntime = clock() - totalTime;
	runInfo.fitnessRuntime = totalFitnessTime;
	runInfo.selectionRuntime = totalSelectionTime;
	runInfo.mutationRuntime = totalMutationTime;
	runInfo.crossoverRuntime = totalCrossoverTime;
	runInfo.best = runInfo.bestIndividuals[runInfo.bestIndividuals.size() - 1];
	runInfo.finalPopFitness = runInfo.popFitnesses[runInfo.popFitnesses.size() - 1];
	runInfo.finalPop = population;
	return runInfo;
}

geneticalgorithm::RunInfo geneticalgorithm::runGA() {
	return runGA(initialization::generatePopulation());
}

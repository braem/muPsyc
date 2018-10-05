#pragma once
#include <vector>
#include "Chromosome.h"

namespace geneticalgorithm {
	class Population {
	private:
		std::vector<Chromosome> c;
		double f;

	public:
		Population() {}
		Population(std::vector<Chromosome> chromosomes) : c(chromosomes) {}

		double fitness() const { return f; }
		std::vector<Chromosome> chromosomes() const { return c; }
		unsigned int size() const { return c.size(); }

		void setFitness(double fitness) { f = fitness; }
		void addToFitness(double fitness) { f += fitness; }
		void setChromosomes(std::vector<Chromosome> chromosomes) { c = chromosomes; }

		bool replaceChromosomeAt(unsigned int index, Chromosome chromosome) {
			if (index >= size()) return false;
			else { c[index] = chromosome; return true; }
		}
		void addChromosome(Chromosome chromosome) { c.push_back(chromosome); }
		void addChromosomes(std::vector<Chromosome> chromosomes) { c.insert(c.begin(), chromosomes.begin(), chromosomes.end()); }
		bool addChromosomeAt(unsigned int index, Chromosome chromosome) {
			if (index >= size()) return false;
			else { c.insert(c.begin() + index, chromosome); return true; }
		}
		bool removeChromosomeAt(unsigned int index) {
			if (index >= size()) return false;
			else { c.erase(c.begin() + index); return true; }
		}
		void clearChromosomes() { c.clear(); }

		Chromosome& operator[](unsigned int i) { return c[i]; }
		const Chromosome& operator[](unsigned int i)const { return c[i]; }
	};
}
#pragma once
#include "GAPopulation.h"
#include "PlayerBase.h"
#include <fstream>
#include <windows.h>

class GeneticAlgorithm
{
	float pm, pc;
	unsigned int populationsize, generations;
	GAPopulation population;
	Genome genome_template;
	Genome genome_reference;
	bool elitism;
	bool sell;
	bool ff_cost;
	DWORD ms_begin;	//futásidö méréséhez szükséges változók
	DWORD ms_end;

public:

	GeneticAlgorithm(void);
	~GeneticAlgorithm(void);
	void pMutation(float);
	void pCrossover(float);
	void popSize(unsigned int);
	void MaxGenerations(unsigned int);
	void CreateInitialPop(void);
	void SetTemplateGenome(Genome&);
	void SetReferenceGenome(Genome&);
	void SetElitism(bool);
	void SetSell(bool);
	void Setff_cost(bool);

	void evolve(bool);
	void step();
	//Listázó függvények
	void ShowGene(Genome&, bool);
	void ShowGene_Managed(Genome&, bool);
	void ShowGene_Ref(Genome&);
	void ShowGene_File(Genome&, ofstream&);
	//Fitness
	float FitnessFunction(Genome&);
	void setPlayersKeep(vector<bool>);
};

#pragma once

#include "Genome.h"

class GAPopulation
{
	friend class GeneticAlgorithm;
	unsigned int currentsize;
	bool IsFixed;		//true -> fixed popsize  
	vector<Genome> genomes;
	float fitness_max;
	float fitness_avg;
	int bestunit;
	vector<bool> players_keep;
public:
	
	GAPopulation(void);
	~GAPopulation(void);
	int Mutation_1C(Genome&, float pm);
	int Mutation_1CS(Genome&, float pm);					//Speciális müvelet, a cserélt játékosokat eladjuk
	int Roulette(void);
	void UXCrossover(Genome& g1, Genome& g2, float pc);		//Egyenletes keresztezés
	int UXCrossover_FC(Genome& g1, Genome& g2, float pc);	//Egyenletes keresztezés + Feasibility Check
	void SetSize(int);
	int GetSize(void);
	float GETfitness_max(void);								//Ki is számolja
	float GETfitness_avg(void);
	int GETBestUnit(void);

	int Initialize_Single(Genome&);	//Kezdetben minden gént ugyanolyanra állít be: a template genomra
};


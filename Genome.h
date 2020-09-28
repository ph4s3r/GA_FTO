#pragma once

#include "GARandom.h"
#include "PlayerBase.h"

#include <sstream>
#include <iostream>

using namespace std;

class Genome
{
	vector<int> gene;
	int length;
	vector<int> gene_domain_min;
	vector<int> gene_domain_max;
	float fitness;
	

public:
	string name;
	int budget;
	Genome(void);

	//A konstruktor nem vizsgálja, hogy egy gén már szerepelt-e máshol, csak a saját sorozatban
	Genome(int , vector<int> , vector<int>, int );

	Genome(const Genome&);	//Copy konstruktor
	~Genome(void);

	void SetFitness(float f);
	float GetFitness(void);
	int SetGenotype(vector<int> g);
	vector<int> GetGenotype(void);
	int GetLength(void)const;
	vector<int> GetDomMin(void)const;
	vector<int> GetDomMax(void)const;

	Genome& operator=(const Genome& other);

};

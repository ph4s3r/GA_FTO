#include "GAPopulation.h"

GAPopulation::GAPopulation(void)
{
	currentsize = 0;
	genomes.reserve(50);
	players_keep.reserve(11);
	for(int i = 0; i < 11; i++) {
		players_keep.push_back(0);
	}
}

GAPopulation::~GAPopulation(void)
{
	/*for(unsigned int i = 0; i < genomes.size(); i++) {
		genomes[i].~Genome();
	}*/
	//genomes.clear();
	//genomes.~vector();
	//players_keep.clear();
	//players_keep.~vector();
}

void GAPopulation::SetSize(int s) {
	currentsize = s;
}

int GAPopulation::GetSize(void) {
	return (currentsize);
}

int GAPopulation::Initialize_Single(Genome& g) {
	if(currentsize) {
		if(IsFixed) {
			for(unsigned int i = 0; i < currentsize; i++) {
				genomes.push_back(g);
				stringstream ss;
				ss << " Variation ";
				ss << i;
				genomes[i].name += ss.str();
			}
		}
		return 0;
	}
	else return 1;
}

int GAPopulation::Mutation_1C(Genome& g, float pm){
	int pmd = 0;	//hany gen mutalodott, ezt visszaadjuk
	int clength = g.GetLength();
	int newGene = 0;
	bool Isin = false;	//új gén kereséséhez kell
	int tries = 0;

	vector<int> domain_min = g.GetDomMin();
	vector<int> domain_max = g.GetDomMax();
	vector<int> currentGene = g.GetGenotype();

	for(int i = 0; i < clength; i++) {
		if(players_keep[i] == false) {
			if(float_rand(0.0, 1.0) < pm) {		//pm valoszinuseggel fut le
				pmd++;
				newGene = int_rand(domain_min[i], domain_max[i]);
				while(tries++ < 200) {
					Isin=false;
						if(i==0){
							if(pdb.GoalKeepers[newGene].GetCost() > g.budget) Isin = true;
						}
						else if(i>0 && i<5){
							if(pdb.Defenders[newGene].GetCost() > g.budget) Isin = true;
						}
						else if(i>4 && i<9){
							if(pdb.MidFielders[newGene].GetCost() > g.budget) Isin = true;
						}
						else{
							if(pdb.Attackers[newGene].GetCost() > g.budget) Isin = true;
						}
					//Ha tényleg cserélni szeretnénk (nincs a gén eddig a kromoszómában, plusz van rá pénzünk)
					if(!Isin){
						for(int j = 0; j < clength; j++) {
						if(newGene == currentGene[j]) {
							Isin = true;
							newGene = int_rand(domain_min[i], domain_max[i]);
							}
						}
					}
					if(!Isin){ 
						if(i==0){
							g.budget -= pdb.GoalKeepers[newGene].GetCost();
						}
						else if(i>0 && i<5){
							g.budget -= pdb.Defenders[newGene].GetCost();
						}
						else if(i>4 && i<9){
							g.budget -= pdb.MidFielders[newGene].GetCost();
						}
						else{
							g.budget -= pdb.Attackers[newGene].GetCost();
						}
						currentGene.insert(currentGene.begin()+i, newGene);
						break;
					}
						
				}
			}
		}
	}
	g.SetGenotype(currentGene);
	/*domain_max.clear();
	domain_max.~vector();
	domain_min.clear();
	domain_min.~vector();
	currentGene.clear();
	currentGene.~vector();*/
		return pmd;
}

int GAPopulation::Mutation_1CS(Genome& g, float pm){
	int pmd = 0;	//hany gen mutalodott, ezt visszaadjuk
	int clength = g.GetLength();
	int newGene = 0;
	bool Isin = false;	//új gén kereséséhez kell
	int tries = 0;

	vector<int> domain_min = g.GetDomMin();
	vector<int> domain_max = g.GetDomMax();
	vector<int> currentGene = g.GetGenotype();

	for(int i = 0; i < clength; i++) {
		if(players_keep[i] == false) {
		if(float_rand(0.0, 1.0) < pm) {		//pm valoszinuseggel fut le
			pmd++;
			newGene = int_rand(domain_min[i], domain_max[i]);
			while(tries++ < 200) {
				Isin=false;
					if(i==0){
						if(pdb.GoalKeepers[newGene].GetCost() > g.budget) Isin = true;
					}
					else if(i>0 && i<5){
						if(pdb.Defenders[newGene].GetCost() > g.budget) Isin = true;
					}
					else if(i>4 && i<9){
						if(pdb.MidFielders[newGene].GetCost() > g.budget) Isin = true;
					}
					else{
						if(pdb.Attackers[newGene].GetCost() > g.budget) Isin = true;
					}
				//Ha tényleg cserélni szeretnénk (nincs a gén eddig a kromoszómában, plusz van rá pénzünk)
				if(!Isin){
					for(int j = 0; j < clength; j++) {
					if(newGene == currentGene[j]) {
						Isin = true;
						newGene = int_rand(domain_min[i], domain_max[i]);
						}
					}
				}
				if(!Isin){ 
					if(i==0){
						g.budget -= pdb.GoalKeepers[newGene].GetCost();
						g.budget += pdb.GoalKeepers[currentGene[i]].GetCost();
					}
					else if(i>0 && i<5){
						g.budget -= pdb.Defenders[newGene].GetCost();
						g.budget += pdb.Defenders[currentGene[i]].GetCost();
					}
					else if(i>4 && i<9){
						g.budget -= pdb.MidFielders[newGene].GetCost();
						g.budget += pdb.MidFielders[currentGene[i]].GetCost();
					}
					else{
						g.budget -= pdb.Attackers[newGene].GetCost();
						g.budget += pdb.Attackers[currentGene[i]].GetCost();
					}
					currentGene.erase(currentGene.begin()+i);
					currentGene.insert(currentGene.begin()+i, newGene);
					break;
				}
					
			}
		}
	}
	}
	g.SetGenotype(currentGene);
	/*domain_max.clear();
	domain_max.~vector();
	domain_min.clear();
	domain_min.~vector();
	currentGene.clear();
	currentGene.~vector();*/
	return pmd;
}


float GAPopulation::GETfitness_max(void){
	float max = 0.0f;
	for(unsigned int i = 0; i < currentsize; i++) {
		if(genomes[i].GetFitness() > max) max = genomes[i].GetFitness();
	}
	fitness_max = max;
	return max;
}

int GAPopulation::GETBestUnit(void){
	int best = 0;
	float max = 0.0f;
	for(unsigned int i = 0; i < currentsize; i++) {
		if(genomes[i].GetFitness() > max){
			max = genomes[i].GetFitness();
			best = i;
		}
	}
	bestunit = best;
	return best; 
}

float GAPopulation::GETfitness_avg(void){
	float avg = 0.0f;
	//Átlagfitness meghatározása
	float sum = 0.0f;
	for(unsigned int i = 0; i < currentsize; i++) {
		sum += genomes[i].GetFitness();
	}
	if(sum == 0.0)avg = 0.0f;
	else avg = sum / currentsize;
	fitness_avg = avg;
	return avg;	
}

int GAPopulation::Roulette() {
	float sum = 0;
	float wheel = 0;
	float* fitnesses;				//fitness ertekek egy tombbe
	fitnesses = new float[currentsize];
	float chosen = float_rand(0.0, 1.0);

	for(unsigned int i = 0; i < currentsize; i++) {
		sum += genomes[i].GetFitness();
		fitnesses[i] = genomes[i].GetFitness();
	}

	for(unsigned int i = 0; i < currentsize; i++) {
		fitnesses[i] /= sum;
	}
	//Normalt fitness ertekek vannak
	float r = float_rand(0.0, 1.0);

	for(unsigned int i = 0; i < currentsize; i++) {
		wheel+=fitnesses[i];
		if(wheel >= r) return i;
	}
	fitnesses = 0;
	delete fitnesses;
	return 0;
}

int GAPopulation::UXCrossover_FC(Genome& g1, Genome& g2, float pc) {
	
		bool g1_feasible = true;
		bool g2_feasible = true;

	if(float_rand(0.0, 1.0) < pc) {
		vector<int> temp_genotype_a;
		vector<int> temp_genotype_b;
		vector<int> temp_genotype_a2;
		vector<int> temp_genotype_b2;
		temp_genotype_a = g1.GetGenotype();
		temp_genotype_b = g2.GetGenotype();
		temp_genotype_a2 = g1.GetGenotype();
		temp_genotype_b2 = g2.GetGenotype(); 
		//Cserélési helyek kiválasztása
		int len = g1.GetLength();
		bool* cpoints = new bool[len];
		for(int i = 0; i < len; i++) {
			if(float_rand(0.0, 1.0) < 0.5)cpoints[i] = 1;
			else
				cpoints[i] = 0;
		}

		for(int i = 0; i < len; i++) {
			if(cpoints[i]) {		//ha van cserélési pont
				temp_genotype_a2.erase(temp_genotype_a2.begin()+i);
				temp_genotype_b2.erase(temp_genotype_b2.begin()+i);
				temp_genotype_a2.insert(temp_genotype_a2.begin()+i, temp_genotype_b[i]);
				temp_genotype_b2.insert(temp_genotype_b2.begin()+i, temp_genotype_a[i]);
			}
		}
		//Végrehajtás, de elötte ellenörzés, hogy életképes egyedek születtek-e
		//2 Segédtömb, hogy a genotípust önmagával össze tudjuk hasonlítani
				temp_genotype_b = temp_genotype_b2;
				temp_genotype_a = temp_genotype_a2;
		//Feasibility Check
		for(int i = 0; i < len; i++) {
			for(int j = 0; j < len; j++) {
				if(j!=i) {
					if(temp_genotype_a2[i] == temp_genotype_a[j]) {
						g1_feasible = false;
					}
					if(temp_genotype_b2[i] == temp_genotype_b[j]) {
						g2_feasible = false;
					}
				}
			}
		}

		//Undo infeasibles
		if(g1_feasible)g1.SetGenotype(temp_genotype_a2);
		if(g2_feasible)g2.SetGenotype(temp_genotype_b2);
		cpoints = 0;
		delete[] cpoints;
	/*temp_genotype_a2.clear();
	temp_genotype_a2.~vector();
	temp_genotype_a.clear();
	temp_genotype_a.~vector();
	temp_genotype_b2.clear();
	temp_genotype_b2.~vector();
	temp_genotype_b.clear();
	temp_genotype_b.~vector();*/
	}
	
	//Visszaadjuk, mennyi egyed volt vállalható
	return (int)g1_feasible+(int)g2_feasible;
}

void GAPopulation::UXCrossover(Genome& g1, Genome& g2, float pc) {		//return 1 = megtörtént, 0 ha nem
	if(float_rand(0.0, 1.0) < pc) {
		vector<int> temp_genotype_a;
		vector<int> temp_genotype_b;
		vector<int> temp_genotype_a2;
		vector<int> temp_genotype_b2;
		temp_genotype_a = g1.GetGenotype();
		temp_genotype_b = g2.GetGenotype();
		temp_genotype_a2 = g1.GetGenotype();
		temp_genotype_b2 = g2.GetGenotype(); 
		//Cserélési helyek kiválasztása
		int len = g1.GetLength();
		bool* cpoints = new bool[len];
		for(int i = 0; i < len; i++) {
			if(float_rand(0.0, 1.0) < 0.5)cpoints[i] = 1;
			else
				cpoints[i] = 0;
		}

		for(int i = 0; i < len; i++) {
			if(cpoints[i]) {		//ha van cserélési pont
				temp_genotype_a2.erase(temp_genotype_a2.begin()+i);
				temp_genotype_b2.erase(temp_genotype_b2.begin()+i);
				temp_genotype_a2.insert(temp_genotype_a2.begin()+i, temp_genotype_b[i]);
				temp_genotype_b2.insert(temp_genotype_b2.begin()+i, temp_genotype_a[i]);
			}
		}
		//Végrehajtás
		g1.SetGenotype(temp_genotype_a2);
		g2.SetGenotype(temp_genotype_b2);
		cpoints = 0;
		delete[] cpoints;
	/*temp_genotype_a2.clear();
	temp_genotype_a2.~vector();
	temp_genotype_a.clear();
	temp_genotype_a.~vector();
	temp_genotype_b2.clear();
	temp_genotype_b2.~vector();
	temp_genotype_b.clear();
	temp_genotype_b.~vector();*/
	}

}
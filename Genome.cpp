#include "Genome.h"

Genome::Genome(void) {
	budget = 20000000;
	length = 0;
	gene.reserve(11);
	gene_domain_max.reserve(11);
	gene_domain_min.reserve(11);
	name = "Unnamed";
}

Genome::Genome(int len, vector<int> d_min, vector<int> d_max, int budget): length(len), budget(budget)
{
	this->budget*=1000;			//(K GBP)-ben adtuk meg
	int newGene = 0;
	bool Isin = false;
	gene.reserve(11);
	gene_domain_max.reserve(11);
	gene_domain_min.reserve(11);

	gene_domain_min = d_min;
	gene_domain_max = d_max;

	//Első lépésben átmásoljuk a domain tömböket
	for(unsigned int i = 0; i < gene.size(); i++) {

		//Majd random gének a domainen belül, de ne ugyanazok
		newGene = int_rand(gene_domain_min[i], gene_domain_max[i]);
		while(true) {
			Isin=false;
			//Ha tényleg cserélni szeretnénk (nincs a gén eddig a kromoszómában)
			for(unsigned int j = 0; j <= i; j++) {
				if(newGene == gene[j])Isin = true;
			}
			if(!Isin){ 
				gene.insert(gene.begin()+i, newGene);
				break;
			}
			else
				newGene = int_rand(gene_domain_min[i], gene_domain_max[i]);


		}
	}
}

Genome::Genome(const Genome& g) {
	gene.reserve(11);
	gene_domain_max.reserve(11);
	gene_domain_min.reserve(11);
	length = g.length;
	gene = g.gene;
	gene_domain_min = g.gene_domain_min;
	gene_domain_max = g.gene_domain_max;
	name = g.name;
	budget = g.budget;
	fitness = g.fitness;
}

Genome::~Genome(void)
{
	/*gene.clear();
	gene_domain_max.clear();
	gene_domain_min.clear();
	gene.~vector();
	gene_domain_max.~vector();
	gene_domain_min.~vector();*/
}
float Genome::GetFitness(void) {
	return fitness;
}

void Genome::SetFitness(float f) {
	fitness = f;
}

int Genome::SetGenotype(vector<int> g){
	gene = g;
	return 0;
}

vector<int> Genome::GetGenotype(){
	return gene;
}

int Genome::GetLength(void)const{
	return length;
}
vector<int> Genome::GetDomMin(void)const{
	return gene_domain_min;
}
vector<int> Genome::GetDomMax(void)const{
	return gene_domain_max;
}

Genome& Genome::operator=(const Genome& g) {
	length = g.length;
	gene = g.gene;
	gene_domain_min = g.gene_domain_min;
	gene_domain_max = g.gene_domain_max;
	name = g.name;
	budget = g.budget;
	fitness = g.fitness;

	return *this;
}


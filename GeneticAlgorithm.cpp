#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(void)
{
}

GeneticAlgorithm::~GeneticAlgorithm(void)
{
	genome_reference.~Genome();
	genome_template.~Genome();
	population.~GAPopulation();
}

void GeneticAlgorithm::pMutation(float pmut) {
	pm = pmut;
}

void GeneticAlgorithm::pCrossover(float crs) {
	pc = crs;
}

void GeneticAlgorithm::popSize(unsigned int pop) {
	populationsize = pop;
}

void GeneticAlgorithm::MaxGenerations(unsigned int gens) {
	generations = gens;
}

void GeneticAlgorithm::CreateInitialPop(void) {
	population.SetSize(populationsize);
	population.Initialize_Single(genome_template);
}

void GeneticAlgorithm::SetTemplateGenome(Genome& g) {
	genome_template = g;
}

void GeneticAlgorithm::SetReferenceGenome(Genome& g) {
	genome_reference = g;
}

void GeneticAlgorithm::SetElitism(bool e){
	elitism = e;
}

void GeneticAlgorithm::evolve(bool Printout) {

	cout << endl << "Referencia csapat: " << genome_reference.name << endl << endl;
	genome_reference.SetFitness(1.000f);
	ShowGene_Ref(genome_reference);

	int Nmuts_all = 0;//mennyi gén mutálódik az evolúció alatt?
	int Ncross_all = 0;
	ofstream outfile("evo.txt");
	ms_begin = GetTickCount();	
	for(unsigned int gens = 1; gens <= generations; gens++) {

		//Futási idö mérése

		if(Printout)outfile << endl << endl;
		if(Printout)outfile << "********** Generáció " << gens << " **********" << endl;
		if(Printout)outfile << "*********************************" << endl;
		int best = 0; //Legjobb egyed indexe
		int Nmuts_g = 0;//mennyi gén mutálódik az adott generációban?
		int Ncross_g = 0;//mennyi gén mutálódik az adott generációban?

		//Fitness kiértékelés
		for(unsigned int i = 0; i < populationsize; i++) {
			population.genomes[i].SetFitness(FitnessFunction(population.genomes[i]));
		}

		//Melyik a legfittebb egyed?
		best = population.GETBestUnit();
		if(Printout)outfile << "A Legfittebb egyed: " << population.genomes[best].name << "  (" << population.GETfitness_max() << ") " << endl;
		//Mennyi a populáció átlagfitnesse?
		if(Printout)outfile << "A populáció átlagfitnesse: " << population.GETfitness_avg() << endl;

		//Elitizmus -> legfittebb egyed kimentése
		Genome elite(genome_template);
		elite = population.genomes[best];

		//Rekombinációs müveletek, rulettkerék szelekció, feasibility vizsgálat
		Ncross_g += population.UXCrossover_FC(population.genomes[population.Roulette()], population.genomes[population.Roulette()], pc);

		//Mutációk pm valószínüséggel
		if(!sell) {
			for(unsigned int i = 0; i < populationsize; i++) {
				Nmuts_g += population.Mutation_1C(population.genomes[i], pm);
			}
		}
		else {
			for(unsigned int i = 0; i < populationsize; i++) {
				Nmuts_g += population.Mutation_1CS(population.genomes[i], pm);
			}
		}
		Nmuts_all += Nmuts_g;
		Ncross_all += Ncross_g;

		//Elitizmus -> legfittebb egyed visszahelyezése
		if(elitism){
			population.genomes[best] = elite;
		}

		if(gens == generations){
			cout <<  endl << endl << endl;
			cout <<	"A legjobb csapat osszeallitasa a kovetkezo:" << endl;
			ShowGene(population.genomes[best], 1);
			if(Printout)outfile << endl << "Az evolúció alatt " << Nmuts_all << " gén mutálódott" << endl;
			if(Printout)outfile << "Az evolúció alatt " << Ncross_all << " egyed rekombinálódott" << endl;
			ms_end = GetTickCount();
			int msec = ms_end - ms_begin;
			cout << "Az evolucio " << msec << " msec alatt futott le." << endl;
		}

	}

}

void GeneticAlgorithm::step() {
	;
}

void GeneticAlgorithm::SetSell(bool s) {
	sell = s;
}

void GeneticAlgorithm::ShowGene(Genome& gen, bool fshow){

	vector<int> g = gen.GetGenotype();
	cout << endl;
	if(fshow) {
		cout << "  ->  " << gen.name << "  <- " << endl;
		if(gen.GetFitness() < 0.0f && gen.GetFitness() > 5.0f)cout << " fitness erteke nincs kiszamitva. " << endl;
		else cout << "		fitness:	" << gen.GetFitness() << endl;
		cout << "A az osszes jatekos erteke: ";
		int cost_all = 0;
		for(int i = 0; i < 11; i++) {
			if(i==0){
				cost_all+=pdb.GoalKeepers[g[i]].GetCost();
			}
			else if(i>0 && i<5){
				cost_all+=pdb.Defenders[g[i]].GetCost();
			}
			else if(i>4 && i<9){
				cost_all+=pdb.MidFielders[g[i]].GetCost();
			}
			else{
				cost_all+=pdb.Attackers[g[i]].GetCost();
			}
		}
		cout << cost_all/1000 << "K  GBP" << endl;
		cout  << "A kezdo budget-bol maradt: ";
		cout << gen.budget/1000 <<  "K GBP" << endl;

		cout << "Jatekosok: " << endl;
	}	
	if(g[0] < pdb.Get_GoalKeepers_Registered() && g[0] >= 0){cout << "	" << pdb.GoalKeepers[g[0]].name;
	if(genome_template.name != pdb.GoalKeepers[g[0]].Home_Club)cout	<<  "	(" << pdb.GoalKeepers[g[0]].Home_Club << ") " << endl;
	else cout << endl;
	}

	if(g[1] < pdb.Get_Defenders_Registered() && g[1] >= 0){cout << "	" << pdb.Defenders[g[1]].name;
	if(genome_template.name != pdb.Defenders[g[1]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[1]].Home_Club << ") " << endl; 
	else cout << endl;
	}
	if(g[2] < pdb.Get_Defenders_Registered() && g[2] >= 0) {cout << "	" << pdb.Defenders[g[2]].name;
	if(genome_template.name != pdb.Defenders[g[2]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[2]].Home_Club << ") " << endl;
	else cout << endl;
	}
	if(g[3] < pdb.Get_Defenders_Registered() && g[3] >= 0){cout << "	" << pdb.Defenders[g[3]].name;
	if(genome_template.name != pdb.Defenders[g[3]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[3]].Home_Club << ") " << endl;
	else cout << endl;
	}
	if(g[4] < pdb.Get_Defenders_Registered() && g[4] >= 0){cout << "	" << pdb.Defenders[g[4]].name;
	if(genome_template.name != pdb.Defenders[g[4]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[4]].Home_Club << ") " << endl;
	else cout << endl;
	}

	if(g[5] < pdb.Get_MidFielders_Registered() && g[5] >= 0){cout << "	" << pdb.MidFielders[g[5]].name;
	if(genome_template.name != pdb.MidFielders[g[5]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[5]].Home_Club << ") " << endl; 
	else cout << endl;
	}
	if(g[6] < pdb.Get_MidFielders_Registered() && g[6] >= 0){cout << "	" << pdb.MidFielders[g[6]].name;
	if(genome_template.name != pdb.MidFielders[g[6]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[6]].Home_Club << ") " << endl;
	else cout << endl;
	}
	if(g[7] < pdb.Get_MidFielders_Registered() && g[7] >= 0){cout << "	" << pdb.MidFielders[g[7]].name;
	if(genome_template.name != pdb.MidFielders[g[7]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[7]].Home_Club << ") " << endl;
	else cout << endl;
	}
	if(g[8] < pdb.Get_MidFielders_Registered() && g[8] >= 0){cout << "	" << pdb.MidFielders[g[8]].name;
	if(genome_template.name != pdb.MidFielders[g[8]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[8]].Home_Club << ") " << endl;
	else cout << endl;
	}

	if(g[9] < pdb.Get_Attackers_Registered() && g[9] >= 0){cout << "	" << pdb.Attackers[g[9]].name;
	if(genome_template.name != pdb.Attackers[g[9]].Home_Club)cout	<<  "	(" << pdb.Attackers[g[9]].Home_Club << ") " << endl;
	else cout << endl;
	}
	if(g[10] < pdb.Get_Attackers_Registered() && g[10] >= 0){cout << "	" << pdb.Attackers[g[10]].name;
	if(genome_template.name != pdb.Attackers[g[10]].Home_Club)cout	<<  "	(" << pdb.Attackers[g[10]].Home_Club << ") " << endl; 
	else cout << endl;
	}
	cout << endl;
}

void GeneticAlgorithm::ShowGene_Managed(Genome& gen, bool fshow){

	vector<int> g = gen.GetGenotype();

	cout << endl << "	[ 1  ]";
	if(g[0] < pdb.Get_GoalKeepers_Registered() && g[0] >= 0){cout << "	" << pdb.GoalKeepers[g[0]].name;
	if(genome_template.name != pdb.GoalKeepers[g[0]].Home_Club)cout	<<  "	(" << pdb.GoalKeepers[g[0]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 2  ]";
	if(g[1] < pdb.Get_Defenders_Registered() && g[1] >= 0){cout << "	" << pdb.Defenders[g[1]].name;
	if(genome_template.name != pdb.Defenders[g[1]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[1]].Home_Club << ") " << endl; 
	else cout << endl;
	}
	cout << "	[ 3  ]";
	if(g[2] < pdb.Get_Defenders_Registered() && g[2] >= 0) {cout << "	" << pdb.Defenders[g[2]].name;
	if(genome_template.name != pdb.Defenders[g[2]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[2]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 4  ]";
	if(g[3] < pdb.Get_Defenders_Registered() && g[3] >= 0){cout << "	" << pdb.Defenders[g[3]].name;
	if(genome_template.name != pdb.Defenders[g[3]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[3]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 5  ]";
	if(g[4] < pdb.Get_Defenders_Registered() && g[4] >= 0){cout << "	" << pdb.Defenders[g[4]].name;
	if(genome_template.name != pdb.Defenders[g[4]].Home_Club)cout	<<  "	(" << pdb.Defenders[g[4]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 6  ]";
	if(g[5] < pdb.Get_MidFielders_Registered() && g[5] >= 0){cout << "	" << pdb.MidFielders[g[5]].name;
	if(genome_template.name != pdb.MidFielders[g[5]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[5]].Home_Club << ") " << endl; 
	else cout << endl;
	}
	cout << "	[ 7  ]";
	if(g[6] < pdb.Get_MidFielders_Registered() && g[6] >= 0){cout << "	" << pdb.MidFielders[g[6]].name;
	if(genome_template.name != pdb.MidFielders[g[6]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[6]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 8  ]";
	if(g[7] < pdb.Get_MidFielders_Registered() && g[7] >= 0){cout << "	" << pdb.MidFielders[g[7]].name;
	if(genome_template.name != pdb.MidFielders[g[7]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[7]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 9  ]";
	if(g[8] < pdb.Get_MidFielders_Registered() && g[8] >= 0){cout << "	" << pdb.MidFielders[g[8]].name;
	if(genome_template.name != pdb.MidFielders[g[8]].Home_Club)cout	<<  "	(" << pdb.MidFielders[g[8]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 10 ]";
	if(g[9] < pdb.Get_Attackers_Registered() && g[9] >= 0){cout << "	" << pdb.Attackers[g[9]].name;
	if(genome_template.name != pdb.Attackers[g[9]].Home_Club)cout	<<  "	(" << pdb.Attackers[g[9]].Home_Club << ") " << endl;
	else cout << endl;
	}
	cout << "	[ 11 ]";
	if(g[10] < pdb.Get_Attackers_Registered() && g[10] >= 0){cout << "	" << pdb.Attackers[g[10]].name;
	if(genome_template.name != pdb.Attackers[g[10]].Home_Club)cout	<<  "	(" << pdb.Attackers[g[10]].Home_Club << ") " << endl; 
	else cout << endl;
	}
	cout << endl;
}

void GeneticAlgorithm::ShowGene_Ref(Genome& gen){

	vector<int> g = gen.GetGenotype();

	cout << "  ->  " << gen.name << "  <- " << endl;

	if(g[0] < pdb.Get_GoalKeepers_Registered() && g[0] >= 0){cout << "	" << pdb.GoalKeepers[g[0]].name;	}
	cout << endl;
		if(g[1] < pdb.Get_Defenders_Registered() && g[1] >= 0){cout << "	" << pdb.Defenders[g[1]].name;	}
		cout << endl;
			if(g[2] < pdb.Get_Defenders_Registered() && g[2] >= 0) {cout << "	" << pdb.Defenders[g[2]].name;	}
			cout << endl;
				if(g[3] < pdb.Get_Defenders_Registered() && g[3] >= 0){cout << "	" << pdb.Defenders[g[3]].name;	}
				cout << endl;
					if(g[4] < pdb.Get_Defenders_Registered() && g[4] >= 0){cout << "	" << pdb.Defenders[g[4]].name;	}
					cout << endl;
						if(g[5] < pdb.Get_MidFielders_Registered() && g[5] >= 0){cout << "	" << pdb.MidFielders[g[5]].name;	}
						cout << endl;
							if(g[6] < pdb.Get_MidFielders_Registered() && g[6] >= 0){cout << "	" << pdb.MidFielders[g[6]].name;	}
							cout << endl;
								if(g[7] < pdb.Get_MidFielders_Registered() && g[7] >= 0){cout << "	" << pdb.MidFielders[g[7]].name;	}
								cout << endl;
									if(g[8] < pdb.Get_MidFielders_Registered() && g[8] >= 0){cout << "	" << pdb.MidFielders[g[8]].name;	}
									cout << endl;
										if(g[9] < pdb.Get_Attackers_Registered() && g[9] >= 0){cout << "	" << pdb.Attackers[g[9]].name;	}
										cout << endl;
											if(g[10] < pdb.Get_Attackers_Registered() && g[10] >= 0){cout << "	" << pdb.Attackers[g[10]].name;	}
}

void GeneticAlgorithm::ShowGene_File(Genome& gen, ofstream& outfile){
	vector<int> g = gen.GetGenotype();
	outfile << endl << endl;
	outfile << "  ->  " << gen.name << "  <- " << endl;
	if(gen.GetFitness() < 0.0f && gen.GetFitness() > 5.0f)outfile << " fitness erteke nincs kiszamitva. " << endl;
	else outfile << "	fitness:	" << gen.GetFitness() << endl;
	outfile << "A kezdö budget-böl maradt: ";
	outfile << gen.budget/1000 <<  "K GBP" << endl;
	outfile << "Jatekosok: " << endl;

	//Kiíratás a kezdö klubbal együtt

	if(g[0] < pdb.Get_GoalKeepers_Registered() && g[0] >= 0){outfile << "	" << pdb.GoalKeepers[g[0]].name;
	if(genome_template.name != pdb.GoalKeepers[g[0]].Home_Club)outfile	<<  "	(" << pdb.GoalKeepers[g[0]].Home_Club << ") " << endl;
	else outfile << endl;
	}

	if(g[1] < pdb.Get_Defenders_Registered() && g[1] >= 0){outfile << "	" << pdb.Defenders[g[1]].name;
	if(genome_template.name != pdb.Defenders[g[1]].Home_Club)outfile	<<  "	(" << pdb.Defenders[g[1]].Home_Club << ") " << endl; 
	else outfile << endl;
	}
	if(g[2] < pdb.Get_Defenders_Registered() && g[2] >= 0) {outfile << "	" << pdb.Defenders[g[2]].name;
	if(genome_template.name != pdb.Defenders[g[2]].Home_Club)outfile	<<  "	(" << pdb.Defenders[g[2]].Home_Club << ") " << endl;
	else outfile << endl;
	}
	if(g[3] < pdb.Get_Defenders_Registered() && g[3] >= 0){outfile << "	" << pdb.Defenders[g[3]].name;
	if(genome_template.name != pdb.Defenders[g[3]].Home_Club)outfile	<<  "	(" << pdb.Defenders[g[3]].Home_Club << ") " << endl;
	else outfile << endl;
	}
	if(g[4] < pdb.Get_Defenders_Registered() && g[4] >= 0){outfile << "	" << pdb.Defenders[g[4]].name;
	if(genome_template.name != pdb.Defenders[g[4]].Home_Club)outfile	<<  "	(" << pdb.Defenders[g[4]].Home_Club << ") " << endl;
	else outfile << endl;
	}

	if(g[5] < pdb.Get_MidFielders_Registered() && g[5] >= 0){outfile << "	" << pdb.MidFielders[g[5]].name;
	if(genome_template.name != pdb.MidFielders[g[5]].Home_Club)outfile	<<  "	(" << pdb.MidFielders[g[5]].Home_Club << ") " << endl; 
	else outfile << endl;
	}
	if(g[6] < pdb.Get_MidFielders_Registered() && g[6] >= 0){outfile << "	" << pdb.MidFielders[g[6]].name;
	if(genome_template.name != pdb.MidFielders[g[6]].Home_Club)outfile	<<  "	(" << pdb.MidFielders[g[6]].Home_Club << ") " << endl;
	else outfile << endl;
	}
	if(g[7] < pdb.Get_MidFielders_Registered() && g[7] >= 0){outfile << "	" << pdb.MidFielders[g[7]].name;
	if(genome_template.name != pdb.MidFielders[g[7]].Home_Club)outfile	<<  "	(" << pdb.MidFielders[g[7]].Home_Club << ") " << endl;
	else outfile << endl;
	}
	if(g[8] < pdb.Get_MidFielders_Registered() && g[8] >= 0){outfile << "	" << pdb.MidFielders[g[8]].name;
	if(genome_template.name != pdb.MidFielders[g[8]].Home_Club)outfile	<<  "	(" << pdb.MidFielders[g[8]].Home_Club << ") " << endl;
	else outfile << endl;
	}

	if(g[9] < pdb.Get_Attackers_Registered() && g[9] >= 0){outfile << "	" << pdb.Attackers[g[9]].name;
	if(genome_template.name != pdb.Attackers[g[9]].Home_Club)outfile	<<  "	(" << pdb.Attackers[g[9]].Home_Club << ") " << endl;
	else outfile << endl;
	}
	if(g[10] < pdb.Get_Attackers_Registered() && g[10] >= 0){outfile << "	" << pdb.Attackers[g[10]].name;
	if(genome_template.name != pdb.Attackers[g[10]].Home_Club)outfile	<<  "	(" << pdb.Attackers[g[10]].Home_Club << ") " << endl; 
	else outfile << endl;
	}
}

float GeneticAlgorithm::FitnessFunction(Genome& g) {
	//A paraméterként kapott csapat lesz a 'saját' csapat, míg a referencia csapat az 'ellenfél'
	int len = g.GetLength();
	vector<int> gene = g.GetGenotype();
	vector<int> rgene = genome_reference.GetGenotype();
	float score = 0.0f;			//fitness végsö értéke lesz
	int my_sum_playerscost = 0;
	int en_sum_playerscost = 0;
	//Attribútumok a számításhoz
	int en_attrib_sum = 0;			//ellen mezönyjátákosai
	int my_attrib_sum = 0;			//saját mezönyjátákosaink
	float diff_attrib_sum = 0.0f;	//fenti pontok aránya: 1nél nagyobb ha mi vagyunk a jobbak
	int my_GK = 0;					//vizsgált csapat kapusa
	int en_GK = 0;					//ellen csapat kapusa
	float diff_GK = 0.0f;	
	int my_GK_def = 0;				//saját kapus reflex
	int en_AT_shot = 0;				//ellen támadók lövési pontossága
	int en_GK_def = 0;				//ellen kapus reflex
	int my_AT_shot = 0;				//saját támadók lövési pontossága
	float diff_defshot = 0.0f;		//számítás fenitekböl
	int my_tw = 0;					//teamwork
	int en_tw = 0;
	float diff_tw = 0.0f;
	int my_pass = 0;
	int en_pass = 0;				//passzolási pontosság
	float diff_pass = 0.0f;
	int my_agi = 0;					//mezönyjátékosokagilitás
	int en_agi = 0;
	float diff_agi = 0.0f;
	int my_cond = 0;				//mezönyjátékosok kondíciója
	int en_cond = 0;
	float diff_cond = 0.0f;
	float diff_playercosts = 0.0f;

	//érdekes attribútumok kinyerése
	my_GK_def = pdb.GoalKeepers[gene[0]].attribs[0];
	en_GK_def = pdb.GoalKeepers[rgene[0]].attribs[0]; 

	for(int i = 0; i < len; i++)
		for(int j = 0; j < 12; j++) {
			if(i==0){	//Kapusok
				my_GK += pdb.GoalKeepers[gene[i]].attribs[j];
				en_GK += pdb.GoalKeepers[rgene[i]].attribs[j];
				if(j==7) {
					my_tw += pdb.GoalKeepers[gene[i]].attribs[7];
					en_tw += pdb.GoalKeepers[rgene[i]].attribs[7];
				}
			}
			if(i>0&&i<5){	//Védelem
				my_attrib_sum += pdb.Defenders[gene[i]].attribs[j];
				en_attrib_sum += pdb.Defenders[rgene[i]].attribs[j];
				if(j==3) {
					my_pass += pdb.Defenders[gene[i]].attribs[3];
					en_pass += pdb.Defenders[rgene[i]].attribs[3];
				}
				if(j==7) {
					my_tw += pdb.Defenders[gene[i]].attribs[7];
					en_tw += pdb.Defenders[rgene[i]].attribs[7];
				}
				if(j>9 && j <12) {
					my_agi += pdb.Defenders[gene[i]].attribs[10];
					en_agi += pdb.Defenders[rgene[i]].attribs[10];
					my_cond += pdb.Defenders[gene[i]].attribs[11];
					en_cond += pdb.Defenders[rgene[i]].attribs[11];
				}
			}
			else if(i>4&&i<9){	//Középmezöny
				my_attrib_sum += pdb.MidFielders[gene[i]].attribs[j];
				en_attrib_sum += pdb.MidFielders[rgene[i]].attribs[j];
				if(j==3) {
					my_pass += pdb.MidFielders[gene[i]].attribs[3];
					en_pass += pdb.MidFielders[rgene[i]].attribs[3];
				}
				if(j==7) {
					my_tw += pdb.MidFielders[gene[i]].attribs[7];
					en_tw += pdb.MidFielders[rgene[i]].attribs[7];
				}
				if(j>9 && j <12) {
					my_agi += pdb.MidFielders[gene[i]].attribs[10];
					en_agi += pdb.MidFielders[rgene[i]].attribs[10];
					my_cond += pdb.MidFielders[gene[i]].attribs[11];
					en_cond += pdb.MidFielders[rgene[i]].attribs[11];
				}
			}
			else if(i>8){ //Támadók
				if(j==0) {
					my_AT_shot += pdb.Attackers[gene[i]].attribs[0];
					en_AT_shot += pdb.Attackers[rgene[i]].attribs[0];
				}
				if(j==3) {
					my_pass += pdb.Attackers[gene[i]].attribs[3];
					en_pass += pdb.Attackers[rgene[i]].attribs[3];
				}
				if(j==7) {
					my_tw += pdb.Attackers[gene[i]].attribs[7];
					en_tw += pdb.Attackers[rgene[i]].attribs[7];
				}

				if(j>9 && j <12) {
					my_agi += pdb.Attackers[gene[i]].attribs[10];
					en_agi += pdb.Attackers[rgene[i]].attribs[10];
					my_cond += pdb.Attackers[gene[i]].attribs[11];
					en_cond += pdb.Attackers[rgene[i]].attribs[11];
				}
				my_attrib_sum += pdb.Attackers[gene[i]].attribs[j];
				en_attrib_sum += pdb.Attackers[rgene[i]].attribs[j];
			}	
		}

		for(int i = 0; i < len; i++){
			if(i==0){
				my_sum_playerscost += pdb.GoalKeepers[gene[i]].GetCost();
				en_sum_playerscost += pdb.GoalKeepers[rgene[i]].GetCost();
			}
			else if(i>0 && i<5){
				my_sum_playerscost += pdb.Defenders[gene[i]].GetCost();
				en_sum_playerscost += pdb.Defenders[rgene[i]].GetCost();
			}
			else if(i>4 && i<9){
				my_sum_playerscost += pdb.MidFielders[gene[i]].GetCost();
				en_sum_playerscost += pdb.MidFielders[rgene[i]].GetCost();
			}
			else{
				my_sum_playerscost += pdb.Attackers[gene[i]].GetCost();
				en_sum_playerscost += pdb.Attackers[rgene[i]].GetCost();
			}
		}


		//számítás az attribútumokkal
		my_AT_shot /=2;
		diff_attrib_sum = ((float)my_attrib_sum/(float)en_attrib_sum);
		diff_agi = ((float)my_agi/(float)en_agi);
		diff_cond = ((float)my_cond/(float)en_cond);
		diff_defshot = ((float)my_AT_shot/(float)en_GK_def + (float)en_AT_shot/(float)my_GK_def);
		diff_GK = ((float)my_GK/(float)en_GK);
		diff_pass = ((float)my_pass/(float)en_pass);
		diff_tw  = ((float)my_tw/(float)en_tw);

		//diff_playercosts = ((float)my_sum_playerscost/(float)en_sum_playerscost);

		score = 0.44f*diff_attrib_sum+
			0.1f*diff_agi+
			0.1f*diff_cond+
			0.08f*diff_defshot+
			0.08f*diff_GK+
			0.08f*diff_pass+
			0.12f*diff_tw;
		return  score;

}
void GeneticAlgorithm::Setff_cost(bool c) {
	ff_cost = c;
}

void GeneticAlgorithm::setPlayersKeep(vector<bool> b) {
	population.players_keep = b;
}

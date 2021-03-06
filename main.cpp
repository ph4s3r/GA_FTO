#include "PlayerBase.h"
#include "GeneticAlgorithm.h"

#define MDEBUG 0

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//Referencia csapat beállítása
void SetRefTeam(GeneticAlgorithm& ga, vector<int> pdmin, vector<int> pdmax, string team);
void SetManagedTeam(GeneticAlgorithm& ga, vector<int> pdmin, vector<int> pdmax, string team, int sbudget);

int main(void) {

#if MDEBUG
	//Memory Leak Detecting
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtDumpMemoryLeaks();
#endif

int re = 1;		//Uj evolucio futtatás jelzése
while(re) {

	//Genetikus Algoritmus osztaly létrehozása
	GeneticAlgorithm ga;

/********************** GA paraméterei ************************/
/**************************************************************/
	cout << "	*******************************************************" << endl;
	cout << "	*** Futballcsapatok osszeallitasanak optimalizalasa ***" << endl << "	***	        Genetikus algoritmussal		    ***" << endl;
	cout << "	*******************************************************" << endl << endl << endl;
	
	cout << "Mely csapatot szeretnenk menedzselni? " << endl << endl;
	//pdb.EraseDB();
	pdb.ReadTeamDatabases();
	int team_chosen;
	cout << ">> ";
	cin >> team_chosen;

	cout << endl << "Mely csapatot szeretnenk referencia csapatnak? " << endl << endl;	
	int team_ref;
	cout << ">> ";
	cin >> team_ref;

	//A következökben a teams.txt fájlban meghatározott csapat-fájlok neveit beolvassuk
	//Elsönek a ref. csapatét majd másodiknak a menedzselni kívánt csapatot, majd a többit
	vector<string> teams = pdb.Get_TeamDBFiles();
	pdb.Load(teams[team_ref]);		//Referencia csapat
	pdb.Load(teams[team_chosen]);
	for(int i = 1; i < pdb.teamfiles_loaded; i++) {
		if(i != team_chosen && i != team_ref) pdb.Load(teams[i]);
	}

	cout << endl << "Kezdo budget: (K GBP) " << endl;
	cout << ">> ";
	int start_budget = 20000;
	cin >> start_budget;
	cout << endl;

	cout << endl << "A menedzselni kivant csapat kezdo osszeallitasa a kovetkezo:"  << endl;

	//Mennyi játékos lett?
	pdb.FinalizeDB();		//Ezzel a metódussal levágjuk a referenciacsapatnak elhasznált adatbázis részt a domainböl
	int GoalKeepers = pdb.Get_GoalKeepers_Registered();
	int Defenders = pdb.Get_Defenders_Registered();
	int Midfielders = pdb.Get_MidFielders_Registered();
	int Attackers = pdb.Get_Attackers_Registered();

//Kromoszóma típusa: egy tömb, elemei a játékosok ID-je

	//ID-k: mind a 4 típusú játékosból különböző mennyiség van, ezek az értékek a domain maximuma
	vector<int> playerDomain_Min;
	vector<int> playerDomain_Max;
	playerDomain_Max.reserve(11);
	playerDomain_Min.reserve(11);

	//A domain-t annak megfelelöen állitjuk be, hogy a referencia csapat játékosai közül ne lehessen választani

	playerDomain_Min.push_back(1);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(4);
	playerDomain_Min.push_back(2);
	playerDomain_Min.push_back(2);

	//Használható játékosok: az Adatbázisban levö regisztrált játékosok
	playerDomain_Max.push_back(GoalKeepers);

	playerDomain_Max.push_back(Defenders);
	playerDomain_Max.push_back(Defenders);
	playerDomain_Max.push_back(Defenders);
	playerDomain_Max.push_back(Defenders);

	playerDomain_Max.push_back(Midfielders);
	playerDomain_Max.push_back(Midfielders);
	playerDomain_Max.push_back(Midfielders);
	playerDomain_Max.push_back(Midfielders);

	playerDomain_Max.push_back(Attackers);
	playerDomain_Max.push_back(Attackers);
	//Látható, hogy a csapatok felállását 4-4-2-re választottuk

	//Sablon gén, megmondja a genetikus algoritmusnak, milyen génekkel inicializálja a kezdeti populációt
	Genome gen_template(11, playerDomain_Min, playerDomain_Max, start_budget);

	//Fitness számításhoz a referencia csapat beállítása
	SetRefTeam(ga, playerDomain_Min, playerDomain_Max, teams[team_ref]);
	SetManagedTeam(ga, playerDomain_Min, playerDomain_Max, teams[team_chosen], start_budget);

	//playerDomain_Min.clear();
	//playerDomain_Min.~vector();
	//playerDomain_Max.clear();
	//playerDomain_Max.~vector();

	cout << "Ha van jatekos akit nem adunk el, vigyuk be a szamat, ha nincs: 0 " << endl << endl;

	vector<bool> players_keep;
	players_keep.reserve(11);
	for(int i = 0; i < 11; i++) {
		players_keep.push_back(0);
	}

	while(true) {
		int temp;

		cout << ">> ";
		cin >> temp;
		if(temp != 0)players_keep[temp-1] = true;
		else break;
	}

	cout << endl << "		*** Genetikus algoritmus parameterei *** " << endl << endl;
	cout << "Maximum generacioszam?" << endl;
	cout << ">> ";
	int _g;
	cin >> _g;
	cout << "Populacio merete (fix): " << endl;
	cout << ">> ";
	int _p;
	cin >> _p;
	cout << "Crossover valoszinuseg (float) : " << endl;
	cout << ">> ";
	float _pc;
	cin >> _pc;
	cout << "Mutacio valoszinuseg (float) : " << endl;
	cout << ">> ";
	float _pm;
	cin >> _pm;
	cout << "Legyen elitizmus? (0/1) : " << endl;
	cout << ">> ";
	bool _se;
	cin >> _se;

	cout << "Kerunk kimutatast az evolucio lefutasarol? (0/1) : " << endl;
	cout << ">> ";
	bool printOut;
	cin >> printOut;

					ga.MaxGenerations(_g);
					ga.popSize(_p);
					ga.pCrossover(_pc);
					ga.pMutation(_pm);
					ga.SetElitism(_se);
					ga.SetSell(true);
					ga.setPlayersKeep(players_keep);
					//players_keep.clear();
					//players_keep.~vector();

/**************************************************************/
/**************************************************************/

	//Kezdeti populáció létrehozása
	ga.CreateInitialPop();

//GA futtatása

	ga.evolve(printOut);
	
	if(printOut)system("evo.txt");

	re = 0;
	cout << "Uj evolucio? 0/1 :  " << endl;
	cin  >> re;
	}
	cout << "  Viszlat!  " << endl;
	//pdb.EraseDB();
}

void SetRefTeam(GeneticAlgorithm& ga, vector<int> pdmin, vector<int> pdmax, string rteam) {
//Létrehozunk egy referencia gént: ID-k tömbje: 11 játékos a következő sorrendben:
	Genome RefTeam(11, pdmin, pdmax, 0);
	vector<int> RefGene;
	RefGene.reserve(11);
	int itG = 0;
	int itD = 0;
	int itM = 0;
	int itA = 0;
	for(int i = 0; i < 11; i++) {
		if(i==0){
				while(itG < pdb.Get_GoalKeepers_Registered()) {
					if(pdb.GoalKeepers[itG].Home_Club == rteam) {
						RefGene.push_back(itG);
						itG++;
						break;
					}
					itG++;
				}
			}
			else if(i>0 && i<5){
				while(itD < pdb.Get_Defenders_Registered()) {
					if(pdb.Defenders[itD].Home_Club == rteam) {
						RefGene.push_back(itD);
						itD++;
						break;
					}
					itD++;
				}
			}
			else if(i>4 && i<9){
				while(itM < pdb.Get_MidFielders_Registered()) {
					if(pdb.MidFielders[itM].Home_Club == rteam) {
						RefGene.push_back(itM);
						itM++;
						break;
					}
					itM++;
				}
			}
			else{
				while(itA < pdb.Get_Attackers_Registered()) {
					if(pdb.Attackers[itA].Home_Club == rteam) {
						RefGene.push_back(itA);
						itA++;
						break;
					}
					itA++;
				}
			}
	}

	RefTeam.SetGenotype(RefGene);

	//RefGene.clear();
	//RefGene.~vector();

	RefTeam.name = string(rteam);

	ga.SetReferenceGenome(RefTeam);

}

void SetManagedTeam(GeneticAlgorithm& ga, vector<int> pdmin, vector<int> pdmax, string myteam, int start_budget) {
//Létrehozunk egy referencia gént: ID-k tömbje: 11 játékos a következő sorrendben:
	Genome Managed_Team(11, pdmin, pdmax, start_budget);
	vector<int> MT;
	MT.reserve(11);

	//Az érték az indexnek megfelelő játékos-típus containerében a játékos indexe,
	//a beolvasás sorrendjében a második txt-fájl tartalma

	int itG = 0;
	int itD = 0;
	int itM = 0;
	int itA = 0;
	for(int i = 0; i < 11; i++) {
		if(i==0){
				while(itG < pdb.Get_GoalKeepers_Registered()) {
					if(pdb.GoalKeepers[itG].Home_Club == myteam) {
						MT.push_back(itG);
						itG++;
						break;
					}
					itG++;
				}
			}
			else if(i>0 && i<5){
				while(itD < pdb.Get_Defenders_Registered()) {
					if(pdb.Defenders[itD].Home_Club == myteam) {
						MT.push_back(itD);
						itD++;
						break;
					}
					itD++;
				}
			}
			else if(i>4 && i<9){
				while(itM < pdb.Get_MidFielders_Registered()) {
					if(pdb.MidFielders[itM].Home_Club == myteam) {
						MT.push_back(itM);
						itM++;
						break;
					}
					itM++;
				}
			}
			else{
				while(itA < pdb.Get_Attackers_Registered()) {
					if(pdb.Attackers[itA].Home_Club == myteam) {
						MT.push_back(itA);
						itA++;
						break;
					}
					itA++;
				}
			}
	}

	Managed_Team.SetGenotype(MT);

	//MT.clear();
	//MT.~vector();

	Managed_Team.name = string(myteam);

	ga.SetTemplateGenome(Managed_Team);

	ga.ShowGene_Managed(Managed_Team, 0);
}
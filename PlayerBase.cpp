#include "PlayerBase.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

PlayerBase::PlayerBase(void): GoalKeepers_Registered(0), Defenders_Registered(0), MidFielders_Registered(0), Attackers_Registered(0), teamfiles_loaded(0)
{
	GoalKeepers.reserve(55);
	Defenders.reserve(55);
	MidFielders.reserve(55);
	Attackers.reserve(55);
	//Egyelöre max 20 csapattal dolgozhatunk
	teamDB_files.reserve(20);
}

PlayerBase::~PlayerBase(void)
{
}

int PlayerBase::addGK(	string name,	int attribs[], unsigned int cost, string club) {

		Player player_temp;
		for(int i = 0; i < 12; i++) {
			player_temp.attribs.push_back(attribs[i]);
		}
		player_temp.name = name;
		player_temp.cost = cost;
		player_temp.Position = 0;
		player_temp.Home_Club = club;

		GoalKeepers.push_back(player_temp);

	return 1;
}
int PlayerBase::addDF(	string name,	int attribs[], unsigned int cost, string club) {

		Player player_temp;
		for(int i = 0; i < 12; i++) {
			player_temp.attribs.push_back(attribs[i]);
		}
		player_temp.name = name;
		player_temp.cost = cost;
		player_temp.Position = 1;
		player_temp.Home_Club = club;

		Defenders.push_back(player_temp);
	
	return 1;
}
int PlayerBase::addMF(	string name,	int attribs[], unsigned int cost, string club) {

		Player player_temp;
		for(int i = 0; i < 12; i++) {
			player_temp.attribs.push_back(attribs[i]);
		}
		player_temp.name = name;
		player_temp.cost = cost;
		player_temp.Position = 2;
		player_temp.Home_Club = club;


		MidFielders.push_back(player_temp);

	return 1;
}
int PlayerBase::addAT(	string name,	int attribs[], unsigned int cost, string club) {

		Player player_temp;
		for(int i = 0; i < 12; i++) {
			player_temp.attribs.push_back(attribs[i]);
		}
		player_temp.name = name;
		player_temp.cost = cost;
		player_temp.Position = 3;
		player_temp.Home_Club = club;

		Attackers.push_back(player_temp);

	return 1;
}

int PlayerBase::Load(string filename) {
	ifstream myfile(filename.c_str());

	const int MAX_LENGTH = 500;
	const int MAX_WORD = 50;
	const int LINES_SIZE = 80;
	char temp[LINES_SIZE][MAX_WORD];
	int i = 0;
	int players_loaded = 0;

	if (myfile.is_open()){
		while(!myfile.eof() && myfile.getline(temp[i++], MAX_LENGTH, '\n'));

		myfile.close();
	}
	else {
		cout << "Hiba! nem sikerült betölteni a " << filename << " állományból" << endl;
		return 0;
	}


	for(int j = 0; j <= i-1; j+=5) {	//soronként betesszük egy játékos tulajdonságaihoz

		int attributes_temp[12];
		char* attr_chararray = strtok(temp[j+3],"	  \n");

		for(int k = 0; attr_chararray  && k < 12; k++)
		{
			attributes_temp[k] = atoi(attr_chararray);
			attr_chararray = strtok (NULL, "	  \n");
		}
		attr_chararray = 0;
		delete attr_chararray;
		int cost_temp = atoi(temp[j+1]);

		switch (atoi(temp[j+2])) {
		case 0 :	//GKEPPER
			if( (addGK(temp[j], attributes_temp, cost_temp, filename)) == 1)players_loaded++;
			break;
		case 1 :	//DEFENDER
			if( (addDF(temp[j], attributes_temp, cost_temp, filename)) == 1)players_loaded++;
			break;
		case 2 :	//MIDFIELDER
			if( (addMF(temp[j], attributes_temp, cost_temp, filename)) == 1)players_loaded++;
			break;
		case 3 :	//ATTACKER
			if( (addAT(temp[j], attributes_temp, cost_temp, filename)) == 1)players_loaded++;
			break;
		}
	}
	
	//visszaadja, hogy hány playert sikerült betölteni...
	return players_loaded-4;
}

int PlayerBase::Get_GoalKeepers_Registered(void){
	return GoalKeepers_Registered;
}
int PlayerBase::Get_Defenders_Registered(void){
	return Defenders_Registered;
}
int PlayerBase::Get_MidFielders_Registered(void){
	return MidFielders_Registered;
}
int PlayerBase::Get_Attackers_Registered(void){
	return Attackers_Registered;
}

int PlayerBase::ReadTeamDatabases(void) {
	string filename("teams.txt");
	ifstream myfile(filename.c_str());

	const int MAX_LENGTH = 500;
	const int MAX_WORD = 20;
	const int LINES_SIZE = 20;
	char temp[LINES_SIZE][MAX_WORD];
	int i = 0;
	int readTeams = 0;

	if (myfile.is_open()){
		while(!myfile.eof() && myfile.getline(temp[i++], MAX_LENGTH, '\n'));
		myfile.close();
	}
	else {
		cout << "Hiba! nem sikerült betölteni a " << filename << " állományból" << endl;
		return 0;
	}
	readTeams = i;
	for(int i = 0; i < readTeams; i++) {
		teamDB_files.push_back(temp[i]);
	}

	for(unsigned int i = 0; i < teamDB_files.size(); i++) {
		cout << "	[ " << i << " ]  " << teamDB_files[i] << endl;
	}

	cout << endl;
	teamfiles_loaded = readTeams;
	return readTeams;
}

vector<string> PlayerBase::Get_TeamDBFiles(void){
	return teamDB_files;
}

void PlayerBase::FinalizeDB(void) {
	Attackers_Registered = Attackers.size();
	GoalKeepers_Registered = GoalKeepers.size();
	Defenders_Registered = Defenders.size();
	MidFielders_Registered = MidFielders.size();
	Defenders_Registered-=5;
	Attackers_Registered-=3;
	GoalKeepers_Registered-=2;
	MidFielders_Registered-=5;
}
void PlayerBase::EraseDB(void){
	for(unsigned int i = 0; i < GoalKeepers_Registered; i++) {
		GoalKeepers[i].~Player();
	}
	for(unsigned int i = 0; i < Defenders_Registered; i++) {
		Defenders[i].~Player();
	}
	for(unsigned int i = 0; i < MidFielders_Registered; i++) {
		MidFielders[i].~Player();
	}
	for(unsigned int i = 0; i < Attackers_Registered; i++) {
		Attackers[i].~Player();
	}

	GoalKeepers.clear();
	GoalKeepers.~vector();
	Defenders.clear();
	Defenders.~vector();
	MidFielders.clear();
	MidFielders.~vector();
	Attackers.clear();
	Attackers.~vector();
	teamfiles_loaded = 0;
	teamDB_files.clear();
	teamDB_files.~vector();
}

PlayerBase pdb;
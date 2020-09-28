#pragma once

#include "Player.h"

class PlayerBase
{
	//Mennyi j�t�kos haszn�lhat� sz�munkra az adatb�zisb�l?
	unsigned int GoalKeepers_Registered;
	unsigned int Defenders_Registered;
	unsigned int MidFielders_Registered;
	unsigned int Attackers_Registered;
	
public:
	//STL T�rol�k
	vector<Player> Defenders;
	vector<Player> GoalKeepers;
	vector<Player> Attackers;
	vector<Player> MidFielders;
	vector<string> teamDB_files;

	int teamfiles_loaded;
	
	PlayerBase(void);
	int addGK(	string name,
	int attribs[],
	unsigned int cost, string club
	);
	int addDF(	string name,
	int attribs[],
	unsigned int cost, string club
	);
	int addMF(	string name,
	int attribs[],
	unsigned int cost, string club
	);
	int addAT(	string name,
	int attribs[],
	unsigned int cost, string club
	);

	int Get_GoalKeepers_Registered(void);
	int Get_Defenders_Registered(void);
	int Get_MidFielders_Registered(void);
	int Get_Attackers_Registered(void);

	~PlayerBase(void);
	int Load(string filename);
	int ReadTeamDatabases(void);
	void FinalizeDB(void);
	void EraseDB(void);		//�sszes t�rol� t�rl�se

	vector<string> Get_TeamDBFiles(void);
};

extern PlayerBase pdb;
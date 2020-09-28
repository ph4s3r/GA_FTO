#pragma once

#include <string>
#include <vector>

using namespace std;

class Player
{
	friend class GeneticAlgorithm;
	friend class PlayerBase;
	string name;
	
	vector<int> attribs;
	unsigned int cost;
	string cost_currency;
	string cost_currency_short;
	unsigned short int Position;	//0: GoalKeeper
									//1: Defender
									//2: Midfielder
									//3: Attacker
	
public:
	string Home_Club;
	Player(void);
	Player(const Player& p);
	~Player(void);
	Player& operator=(const Player& p);
	int GetCost(void);
};

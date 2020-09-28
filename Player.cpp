#include "PlayerBase.h"

Player::Player(void): cost(0), Position(0), cost_currency("United Kingdom Pounds"), cost_currency_short("GBP"), name(""), Home_Club("")
{
	attribs.reserve(12);
}

Player::~Player(void)
{
	/*attribs.clear();
	attribs.~vector();*/
}

Player::Player(const Player& p) {
	cost = p.cost;
	cost_currency = p.cost_currency;
	cost_currency_short = p.cost_currency_short;
	Position = p.Position;
	name = p.name;
	attribs = p.attribs;
	Home_Club = p.Home_Club;
}

Player& Player::operator=(const Player& p){
	if(this==&p)return *this;
	cost = p.cost;
	cost_currency = p.cost_currency;
	cost_currency_short = p.cost_currency_short;
	Position = p.Position;
	name = p.name;
	Home_Club = p.Home_Club;
	attribs = p.attribs;

	return *this;
}

int Player::GetCost(void) {
	return cost;
}
 
#include "DamageHandler.h"

//Damage management for both sides, player and monsters.

DamageHandler* DamageHandler::sm_instance = 0;

DamageHandler::DamageHandler()
	:sm_monsterDmg(0),
	sm_playerDmg(0)
{
}

/*
DamageHandler* DamageHandler::GetInstance()
{
	if (!sm_instance)
	{
		DamageHandler::sm_instance = new DamageHandler();
		return sm_instance;
	}
	else
	{
		return sm_instance;
	}
}
*/
void DamageHandler::ResetDamage()
{
	sm_playerDmg = 0;
	sm_monsterDmg = 0;
}

void DamageHandler::DealDamageToPlayer(int damage)
{
	sm_playerDmg += damage;
}

void DamageHandler::DealDamageToMonster(int damage)
{
	sm_monsterDmg += damage;
}

int DamageHandler::GetPlayerDamage()
{
	return (int)sm_playerDmg;
}

int DamageHandler::GetMonsterDamage()
{
	return (int)sm_monsterDmg;
}

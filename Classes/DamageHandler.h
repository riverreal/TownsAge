#pragma once

class DamageHandler
{
public:
	static DamageHandler* GetInstance() 
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
	};

	void ResetDamage();
	void DealDamageToPlayer(int damage);
	void DealDamageToMonster(int damage);
	int GetPlayerDamage();
	int GetMonsterDamage();
private:
	DamageHandler();

	static DamageHandler* sm_instance;

	//Stores the dmg per frame
	int sm_playerDmg;
	int sm_monsterDmg;

	
};
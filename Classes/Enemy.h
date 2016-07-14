#pragma once

#include "cocos2d.h"
#include <vector>
#include "Effect.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	///Sets a new Static Type Enemy.
	///Moves around in a 1D plane, unable to jump.
	void SetStaticEnemy(std::string name, cocos2d::Vec2 position, float area);
	///Sets a new Dynamic Type Enemy.
	///Moves around in a 2D plane, able to jump.
	void SetDynamicEnemy(std::string name, cocos2d::Vec2 position);
	///Sets a new Flying Type Enemy.
	///Moves around in a 2D plane, ignoring gravity.
	void SetFlyingEnemy(std::string name, cocos2d::Vec2 position);
	///Sets a new Mine Type Enemy.
	///Doesn't move at first. Its behaviour resembles a Mine.
	void SetMineEnemy(std::string name, cocos2d::Vec2 position);

	///Initializes the enemies and adds them as a child of gameNode
	void InitEnemies(cocos2d::Node* gameNode);
	///Updates the enemies to calculate AI.
	///Returns true if the player is collisding with a monster
	bool updateEnemies(cocos2d::Rect playerRect);
	bool collideWith(cocos2d::Rect rect);
	void receiveDamage(bool attackInstanced, Effect* effect);
	
	cocos2d::Vec2 GetStaticEnemyPos(unsigned int index);

private:
	void staticWalk(bool directionRight, cocos2d::Sprite * subject, float origin, float area, float vel);
	cocos2d::Rect GetFrameSpriteRect(int frameCount, int frameStep, int size, int row, int animationCount);

	enum ENEMY_STATE_ID
	{
		ENEMY_STATE_IDLE,
		ENEMY_STATE_WALKING,
		ENEMY_STATE_BONUS,
		ENEMY_STATE_BONUS2,
		ENEMY_STATE_ATTACKING,
		ENEMY_STATE_CHASING,
		ENEMY_STATE_DEAD
	};

	struct EnemyState
	{
		EnemyState()
			:stateID(ENEMY_STATE_IDLE),
			isHostile(false),
			stateDuration(1),
			directionRight(false),
			timeCounter(0),
			firstTimeState(true),
			isEnraged(false),
			HP(10),
			originX(0),
			area(0),
			damage(0),
			damageDealt(false)
		{
		};

		EnemyState(int hp)
			:stateID(ENEMY_STATE_IDLE),
			isHostile(false),
			stateDuration(1),
			directionRight(false),
			timeCounter(0),
			firstTimeState(true),
			isEnraged(false),
			HP(hp),
			originX(0),
			area(0),
			damage(0),
			damageDealt(false)
		{
		};

		EnemyState(int hp, int state)
			:stateID(state),
			isHostile(false),
			stateDuration(1),
			directionRight(false),
			timeCounter(0),
			firstTimeState(true),
			isEnraged(false),
			HP(hp),
			originX(0),
			area(0),
			damage(0),
			damageDealt(false)
		{
		};

		EnemyState(int hp, float areaOfAct)
			:stateID(ENEMY_STATE_IDLE),
			isHostile(false),
			stateDuration(1),
			directionRight(false),
			timeCounter(0),
			firstTimeState(true),
			isEnraged(false),
			HP(hp),
			originX(0),
			area(areaOfAct),
			damage(0),
			damageDealt(false)
		{
		};

		int stateID;
		bool isHostile;
		float stateDuration;
		bool directionRight;
		int timeCounter;
		bool firstTimeState;
		bool isEnraged;
		bool damageDealt;
		int damage;
		int HP;
		float originX;
		float area; //Area of action (only for static)
	};

private:
	int m_collidedMonsterIndex;
	std::vector<cocos2d::Sprite*> m_staticEnemyVector;
	std::vector<EnemyState> m_staticEnemyState;

	std::vector<cocos2d::Sprite*> m_dynemicEnemyVector;
	std::vector<cocos2d::Sprite*> m_flyingEnemyVector;
	std::vector<cocos2d::Sprite*> m_mineEnemyVector;

	 std::string m_name[9]; /*= 
	{
		"bear",
		"lizard",
		"snake",
		"spider",
		"bat",
		"scorpion",
		"frog",
		"hippo",
		"soldier"
	};*/

	 std::string m_path[9];/* =
	{
		"img/enemies/forest_bear.png",
		"img/enemies/forest_lizard.png",
		"img/enemies/desert_snake.png",
		"img/enemies/cave_spider.png",
		"img/enemies/cave_bat.png",
		"img/enemies/desert_scorpion.png",
		"img/enemies/swamp_frog.png",
		"img/enemies/swamp_hippo.png",
		"img/enemies/soldier.png"
	};*/

	
};
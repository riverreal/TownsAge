#include "Enemy.h"
#include "DamageHandler.h"

using namespace cocos2d;

Enemy::Enemy()
	:m_collidedMonsterIndex(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::SetStaticEnemy(std::string name, cocos2d::Vec2 position, float area)
{
	std::string filepath;
	int type;
	for (int i = 0; i < 9; ++i)
	{
		if (m_name[i] == name)
		{
			filepath = m_path[i];
			type = i;
		}
	}

	auto sprite = Sprite::create(filepath);
	sprite->setTag(type);
	sprite->setScale(0.8);
	sprite->setPosition(Vec2(position.x + 60, position.y + 60));
	m_staticEnemyVector.push_back(sprite);

	EnemyState state(50, area);
	state.originX = position.x + 60;
	state.damage = 40;
	m_staticEnemyState.push_back(state);
}

void Enemy::SetDynamicEnemy(std::string name, cocos2d::Vec2 position)
{
}

void Enemy::SetFlyingEnemy(std::string name, cocos2d::Vec2 position)
{
}

void Enemy::SetMineEnemy(std::string name, cocos2d::Vec2 position)
{
}

void Enemy::InitEnemies(cocos2d::Node * gameNode)
{
	for (auto& enemy : m_staticEnemyVector)
	{
		gameNode->addChild(enemy);
	}
}

bool Enemy::updateEnemies(Rect playerRect)
{
	//returns if enemy is colliding with the player
	bool result = false;
	for (int i = 0; i < m_staticEnemyVector.size(); ++i)
	{
		//enlarged player rect for bear to detect.
		auto playerArea = Rect(Vec2(playerRect.getMidX() - m_staticEnemyState[i].area/2, playerRect.origin.y),Size(m_staticEnemyState[i].area, playerRect.size.height));

		if (m_staticEnemyState[i].HP <= 0)
		{
			if (m_staticEnemyState[i].stateID != ENEMY_STATE_DEAD)
			{
				m_staticEnemyState[i].stateID = ENEMY_STATE_DEAD;
				m_staticEnemyState[i].stateDuration = 2.0f;
				m_staticEnemyVector[i]->stopAllActions();
				m_staticEnemyVector[i]->setColor(Color3B::WHITE);
				m_staticEnemyState[i].firstTimeState = true;
			}
		}

		if (m_staticEnemyState[i].stateID != ENEMY_STATE_DEAD)
		{
			//Chase the player
			if (m_staticEnemyVector[i]->getBoundingBox().intersectsRect(playerArea))
			{
				if (m_staticEnemyState[i].stateID != ENEMY_STATE_CHASING && m_staticEnemyState[i].stateID != ENEMY_STATE_ATTACKING)
				{
					if (playerRect.getMidX() <= m_staticEnemyVector[i]->getBoundingBox().origin.x)
					{
						m_staticEnemyState[i].directionRight = false;
					}
					else if (playerRect.getMidX() > m_staticEnemyVector[i]->getBoundingBox().origin.x)
					{
						m_staticEnemyState[i].directionRight = true;
					}

					m_staticEnemyState[i].stateID = ENEMY_STATE_CHASING;
					m_staticEnemyState[i].firstTimeState = true;
					m_staticEnemyState[i].stateDuration = 0.65;
					m_staticEnemyVector[i]->stopAllActions();
					m_staticEnemyVector[i]->setColor(Color3B::WHITE);
				}
			}

			//Attack the player
			if (m_staticEnemyVector[i]->getBoundingBox().intersectsRect(playerRect))
			{
				result = true;
				m_collidedMonsterIndex = i;
				if (m_staticEnemyState[i].stateID != ENEMY_STATE_ATTACKING)
				{
					m_staticEnemyState[i].stateID = ENEMY_STATE_ATTACKING;
					m_staticEnemyState[i].firstTimeState = false;
					m_staticEnemyState[i].stateDuration = 0.66;
					m_staticEnemyState[i].timeCounter = 0;
					m_staticEnemyVector[i]->stopAllActions();
					m_staticEnemyVector[i]->setColor(Color3B::WHITE);
				}
			}
		}

		//default rect
		auto frameInSpritesheet = Rect(0, 0, 128, 128);
		//frame steps
		int framesForNextAnim = 20;
		int nextAnimForAttack = 10;

		switch (m_staticEnemyState[i].stateID)
		{
		case ENEMY_STATE_IDLE:
			if (m_staticEnemyState[i].firstTimeState)
			{
				m_staticEnemyState[i].stateDuration = cocos2d::RandomHelper::random_int(1, 3);
				m_staticEnemyState[i].directionRight = cocos2d::RandomHelper::random_int(0, 1);
				m_staticEnemyState[i].firstTimeState = false;
				m_staticEnemyState[i].timeCounter = 0;
				m_staticEnemyVector[i]->stopAllActions();
				m_staticEnemyVector[i]->setColor(Color3B::WHITE);
			}
			
			frameInSpritesheet = GetFrameSpriteRect(m_staticEnemyState[i].timeCounter, framesForNextAnim, 128, 0, 1);

			if (m_staticEnemyState[i].timeCounter / 60 >= m_staticEnemyState[i].stateDuration)
			{
				m_staticEnemyState[i].firstTimeState = true;
				auto random = cocos2d::RandomHelper::random_int(0, 9);
				if (random < 7)
				{
					//40% probability
					random = ENEMY_STATE_IDLE;

					if (random < 4)
					{
						//40% probability
						random = ENEMY_STATE_WALKING;
					}
				}
				else
				{
					//20% probability
					random = ENEMY_STATE_BONUS;
				}

				m_staticEnemyState[i].stateID = random;
			}
			break;
		case ENEMY_STATE_WALKING:
			if (m_staticEnemyState[i].firstTimeState)
			{
				m_staticEnemyState[i].stateDuration = cocos2d::RandomHelper::random_int(1, 5);
				m_staticEnemyState[i].directionRight = cocos2d::RandomHelper::random_int(0, 1);
				m_staticEnemyState[i].firstTimeState = false;
				m_staticEnemyState[i].timeCounter = 0;
				staticWalk(m_staticEnemyState[i].directionRight, m_staticEnemyVector[i], m_staticEnemyState[i].originX, m_staticEnemyState[i].area, 20);
			}

			frameInSpritesheet = GetFrameSpriteRect(m_staticEnemyState[i].timeCounter, framesForNextAnim, 128, 2, 4);
			
			if (m_staticEnemyState[i].timeCounter / 60 >= m_staticEnemyState[i].stateDuration)
			{
				m_staticEnemyState[i].firstTimeState = true;
				m_staticEnemyState[i].stateID = cocos2d::RandomHelper::random_int(0, 1);
			}
			break;
		case ENEMY_STATE_ATTACKING:

			frameInSpritesheet = GetFrameSpriteRect(m_staticEnemyState[i].timeCounter, nextAnimForAttack, 128, 3, 4);
			
			if ((m_staticEnemyState[i].timeCounter / 60.0f) >= m_staticEnemyState[i].stateDuration)
			{
				
				//if still collided with the player at the end
				if (m_staticEnemyVector[i]->getBoundingBox().intersectsRect(playerRect))
				{
					//auto damage = DamageHandler::GetInstance();
					//damage->DealDamageToPlayer(m_staticEnemyState[i].damage + cocos2d::RandomHelper::random_int(-5, 5));
					result = true;
				}

				m_staticEnemyState[i].stateDuration = 0;
				m_staticEnemyState[i].timeCounter = 0;
				m_staticEnemyState[i].firstTimeState = true;
				m_staticEnemyState[i].stateID = ENEMY_STATE_IDLE;
				m_staticEnemyState[i].damageDealt = false;
			}
			else if ((m_staticEnemyState[i].timeCounter / 60.0f) >= (m_staticEnemyState[i].stateDuration / 2))
			{
				if (m_staticEnemyState[i].damageDealt == false)
				{
					auto damage = DamageHandler::GetInstance();
					damage->DealDamageToPlayer(m_staticEnemyState[i].damage + cocos2d::RandomHelper::random_int(-5, 5));
					m_staticEnemyState[i].damageDealt = true;
				}
			}
			break;
		case ENEMY_STATE_CHASING:
			if (m_staticEnemyState[i].firstTimeState)
			{
				//m_staticEnemyState[i].directionRight = cocos2d::RandomHelper::random_int(0, 1);
				m_staticEnemyState[i].firstTimeState = false;
				m_staticEnemyState[i].timeCounter = 0;
				staticWalk(m_staticEnemyState[i].directionRight, m_staticEnemyVector[i], m_staticEnemyState[i].originX, m_staticEnemyState[i].area, 40);
			}

			frameInSpritesheet = GetFrameSpriteRect(m_staticEnemyState[i].timeCounter, nextAnimForAttack, 128, 2, 4);

			if (m_staticEnemyState[i].timeCounter / 60 >= m_staticEnemyState[i].stateDuration)
			{
				m_staticEnemyState[i].firstTimeState = true;
				m_staticEnemyState[i].stateID = ENEMY_STATE_IDLE;
			}
			break;
		case ENEMY_STATE_BONUS:
			if (m_staticEnemyState[i].firstTimeState)
			{
				m_staticEnemyVector[i]->stopAllActions();
				m_staticEnemyVector[i]->setColor(Color3B::WHITE);
				m_staticEnemyState[i].stateDuration = 1.0;
				m_staticEnemyState[i].directionRight = cocos2d::RandomHelper::random_int(0, 1);
				m_staticEnemyState[i].firstTimeState = false;
				m_staticEnemyState[i].timeCounter = 0;
			}
			frameInSpritesheet = GetFrameSpriteRect(m_staticEnemyState[i].timeCounter, framesForNextAnim, 128, 0, 4);
			if (m_staticEnemyState[i].timeCounter / 60 >= m_staticEnemyState[i].stateDuration)
			{
				m_staticEnemyState[i].firstTimeState = true;
				m_staticEnemyState[i].stateID = ENEMY_STATE_BONUS2;
			}
			break;
		case ENEMY_STATE_BONUS2:
			if (m_staticEnemyState[i].firstTimeState)
			{
				m_staticEnemyVector[i]->stopAllActions();
				m_staticEnemyVector[i]->setColor(Color3B::WHITE);
				m_staticEnemyState[i].stateDuration = cocos2d::RandomHelper::random_int(4, 9);
				m_staticEnemyState[i].firstTimeState = false;
				m_staticEnemyState[i].timeCounter = 0;
			}
			frameInSpritesheet = GetFrameSpriteRect(m_staticEnemyState[i].timeCounter, framesForNextAnim, 128, 1, 2);
			if (m_staticEnemyState[i].timeCounter / 60 >= m_staticEnemyState[i].stateDuration)
			{
				m_staticEnemyState[i].firstTimeState = true;
				m_staticEnemyState[i].stateID = ENEMY_STATE_IDLE;
			}
			break;
		case ENEMY_STATE_DEAD:
			if (m_staticEnemyState[i].firstTimeState)
			{
				m_staticEnemyState[i].firstTimeState = false;
				m_staticEnemyState[i].timeCounter = 0;
				m_staticEnemyVector[i]->runAction(TintTo::create(m_staticEnemyState[i].stateDuration, Color3B(200, 200, 200)));
			}

			frameInSpritesheet = GetFrameSpriteRect(m_staticEnemyState[i].timeCounter, framesForNextAnim + 10, 128, 4, 4);

			if ((m_staticEnemyState[i].timeCounter / 60) >= m_staticEnemyState[i].stateDuration)
			{
				m_staticEnemyState.erase(m_staticEnemyState.begin() + i);
				m_staticEnemyVector.erase(m_staticEnemyVector.begin() + i);
				return false;
			}

			break;
		default:
			break;
		}
		m_staticEnemyVector[i]->setTextureRect(frameInSpritesheet);
		m_staticEnemyState[i].timeCounter++;
	}

	return result;
}

bool Enemy::collideWith(cocos2d::Rect rect)
{
	for (auto &monster : m_staticEnemyVector)
	{
		if(monster->getBoundingBox().intersectsRect(rect))
			return true;
	}
	return false;
}

void Enemy::receiveDamage(bool attackInstanced)
{
	if (m_collidedMonsterIndex != -1)
	{
		auto damage = DamageHandler::GetInstance();
		//CCLOG("Monster damage: %i", damage->GetMonsterDamage());
		if (damage->GetMonsterDamage() != 0)
		{
			if (!attackInstanced)
			{
				m_staticEnemyState[m_collidedMonsterIndex].HP -= damage->GetMonsterDamage();
				CCLOG("Monster knockback!");
				auto tintRed = TintTo::create(0.05, Color3B::RED);
				auto delay = DelayTime::create(0.1);
				auto tintBack = TintTo::create(0.05, Color3B::WHITE);
				m_staticEnemyVector[m_collidedMonsterIndex]->runAction(Sequence::create(tintRed, delay, tintBack, NULL));

				//push back
				int direction = 1;
				if (m_staticEnemyState[m_collidedMonsterIndex].directionRight)
				{
					direction = -1;
				}

				auto pushBack = MoveBy::create(0.08f, Vec2(10 * direction, 0));
				//m_staticEnemyVector[m_collidedMonsterIndex]->runAction(pushBack);
			}
		}
	}
	
}

void Enemy::staticWalk(bool directionRight, Sprite * subject, float origin, float area, float vel)
{	
	if (subject->getPositionX() > origin + area && directionRight)
	{
		vel = 10;
		directionRight = false;
	}
	else if (subject->getPositionX() < origin - area && !directionRight)
	{
		vel = 10;
		directionRight = true;
	}

	auto moveRight = MoveBy::create(0.5, Vec2(vel, 0));
	auto moveLeft = MoveBy::create(0.5, Vec2(-vel, 0));

	subject->stopAllActions();

	if (directionRight) //right
	{
		subject->setFlippedX(true);
		subject->runAction(RepeatForever::create(moveRight));
	}
	else //left
	{
		subject->setFlippedX(false);
		subject->runAction(RepeatForever::create(moveLeft));
	}
}

Rect Enemy::GetFrameSpriteRect(int frameCount, int frameStep, int size, int row, int animationCount)
{
	auto res = frameCount % (frameStep * animationCount);
	auto rect = Rect(size * floor(res/frameStep), row * size, size, size);
	return rect;
}

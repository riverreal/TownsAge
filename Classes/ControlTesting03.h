#ifndef __Control3_H__
#define __Control3_H__

#include "cocos2d.h"
#include <vector>
#include "GameData.h"

class Control3 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float dt);

	void walk(bool directionRight, cocos2d::Sprite* subject);
	void npcWalk(bool directionRight, cocos2d::Sprite* subject);
	void setViewpoint(cocos2d::Vec2 position);
	cocos2d::Vec2 convertToTilePosition(cocos2d::Vec2 position);
	void simplePhysics(); //will go inside update

	void npcAI();
	void spawnNPC(int npcNumber);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	//controls
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);

    // implement the "static create()" method manually
    CREATE_FUNC(Control3);

private:
	

	struct NPCStates
	{
	public:
		//Default state contructor
		NPCStates() 
			:stateID(NPC_STATE_IDLE),
			isBuilding(false),
			stateDuration(1.0f),
			npcType(NPC_TYPE_BEARD),
			directionRight(false),
			timeCounter(0),
			firstTimeState(true)
		{};

		//Parameter asignment contructor
		NPCStates(int stateID, bool isBuilding, float stateDuration, int npcType, bool directionRight)
			:stateID(stateID),
			isBuilding(isBuilding),
			stateDuration(stateDuration),
			npcType(npcType),
			directionRight(directionRight),
			timeCounter(0),
			firstTimeState(true)
		{};

		int stateID;
		bool isBuilding;
		//duration of the state in seconds
		float stateDuration;
		int npcType;
		bool directionRight;
		int timeCounter;
		bool firstTimeState;
	};

	cocos2d::Sprite* m_character;
	std::vector<cocos2d::Sprite*> m_npcVector;
	std::vector<NPCStates> m_npcStateVector;
	cocos2d::TMXTiledMap* m_tilemap;
	cocos2d::TMXLayer* m_collisionLayer;
	bool m_directionRight;

	//Control related
	cocos2d::Rect m_rightRect;
	cocos2d::Rect m_leftRect;
	bool m_aButtonPressed;
	cocos2d::Rect m_aButtonRect;
	int m_moveTouchID;
	int m_actionTouchID;

	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_uiNode;

	//---------------------
	//----physics variables/constants
	//---------------------

	//gravity acceleration for character
	const float GRAVITY = -0.2;
	
	//max speed for character
	const float MAX_SPEED = 3;

	//max jump capacity
	const float MAX_JUMP = 5;

	//player states
	bool m_standing;
	bool m_onGround;
	bool m_jumpInstanced;

	//player variables
	float m_speedX;
	float m_speedY;
	float m_accelerationX;
	float m_frictionX;
	float m_jumpForce;

};

#endif // __Control3_H__

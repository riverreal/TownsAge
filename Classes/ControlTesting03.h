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
	void popUp(cocos2d::Sprite* subject);
	void cleanPopUp();
	void removePopUp();
	void npcWalk(bool directionRight, cocos2d::Sprite* subject);
	void resourcePopup();
	void setViewpoint(cocos2d::Vec2 position);
	cocos2d::Vec2 convertToTilePosition(cocos2d::Vec2 position);
	void simplePhysics(); //will go inside update

	void changeArea(bool right);
	void resetScene();
	void finishActionInstance();
	void spreadResource();

	void showItems();

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
	
	struct PlayerInventory
	{
		PlayerInventory()
			:itemType(0),
			itemAmount(0)
		{
		}

		PlayerInventory(int type, int amount)
			:itemType(type),
			itemAmount(amount)
		{
		}

		int itemType;
		int itemAmount;
	};

	struct NPCStates
	{
	public:
		//Default state contructor
		NPCStates() 
			:stateID(NPC_STATE_IDLE),
			isBuilding(false),
			isTalking(false),
			stateDuration(1.0f),
			npcType(NPC_TYPE_GIRL1),
			directionRight(false),
			timeCounter(0),
			firstTimeState(true),
			buildingNum1(1),
			buildingNum2(0),
			buildingNum3(0)
		{};

		//Parameter asignment contructor
		NPCStates(int stateID, bool isBuilding, float stateDuration, int npcType, bool directionRight)
			:stateID(stateID),
			isBuilding(isBuilding),
			isTalking(false),
			stateDuration(stateDuration),
			npcType(npcType),
			directionRight(directionRight),
			timeCounter(0),
			firstTimeState(true),
			buildingNum1(1),
			buildingNum2(0),
			buildingNum3(0)
		{};

		int stateID;
		bool isBuilding;
		bool isTalking;
		//duration of the state in seconds
		float stateDuration;
		int npcType;
		bool directionRight;
		int timeCounter;
		bool firstTimeState;
		int buildingNum1;
		int buildingNum2;
		int buildingNum3;
	};

	cocos2d::Sprite* m_character;
	std::vector<cocos2d::Sprite*> m_npcVector;
	std::vector<cocos2d::Sprite*> m_resourceVector;
	std::vector<cocos2d::Sprite*> m_resourceIconVector;
	std::vector<cocos2d::Sprite*> m_itemsVector;
	std::vector<NPCStates> m_npcStateVector;
	cocos2d::TMXTiledMap* m_tilemap;
	cocos2d::TMXLayer* m_collisionLayer;
	bool m_directionRight;
	bool m_isTalking;
	bool m_isCreatedPopUp;
	bool m_isCreatedActionPopUp;
	int m_talkChecker;

	//Control related
	cocos2d::Rect m_rightRect;
	cocos2d::Rect m_leftRect;
	bool m_aButtonPressed;
	bool m_jButtonPressed;
	cocos2d::Rect m_aButtonRect;
	cocos2d::Rect m_jButtonRect;
	int m_moveTouchID;
	int m_actionTouchID;
	int m_jumpTouchID;

	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_uiNode;

	int m_talkingNPCIndex;

	//Sprite names
	const std::string POPUP_SPRITE = "PopUp";
	const std::string RESOURCE_1 = "Res1";
	const std::string RESOURCE_COUNT_1 = "ResCount1";
	const std::string RESOURCE_2 = "Res2";
	const std::string RESOURCE_COUNT_2 = "ResCount2";
	const std::string RESOURCE_3 = "Res3";
	const std::string RESOURCE_COUNT_3 = "ResCount3";
	const std::string HOUSE_ICON_SPRITE = "HouseIcon";

	//---------------------
	//----physics variables/constants
	//---------------------

	//gravity acceleration for character
	const float GRAVITY = -0.2;
	
	//max speed for character
	const float MAX_SPEED = 4;

	//max jump capacity
	const float MAX_JUMP = 6;

	//player states
	bool m_standing;
	bool m_onGround;
	bool m_jumpInstanced;
	bool m_onNext;
	bool m_isActing;
	bool m_actingInstanced;
	bool m_actingWithEnemy;
	bool m_createdResourcePopup;
	int m_actingResourceIndex;
	int m_actedResourceCount;

	float m_inventoryY;

	//player variables
	float m_speedX;
	float m_speedY;
	float m_accelerationX;
	float m_frictionX;
	float m_jumpForce;

	PlayerInventory m_inventorySlot[10];
};

#endif // __Control3_H__

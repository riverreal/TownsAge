#ifndef __Control3_H__
#define __Control3_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include "GameData.h"
#include "Enemy.h"
#include "ParallaxBackground.h"
#include "Effect.h"
#include "SimpleAudioEngine.h"
#include "Ending.h"

#pragma execution_character_set("utf-8");

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
	cocos2d::Vec2 setViewpoint(cocos2d::Vec2 position);
	cocos2d::Vec2 convertToTilePosition(cocos2d::Vec2 position);
	void simplePhysics(); //will go inside update
	void playerAnimationCache();

	void changeArea(int area);
	void resetScene();
	void finishActionInstance();
	void spreadResource(std::string name, cocos2d::Vec2 originPos);
	void swingPlayer(int swingType);
	void showItems(float invX, float invY);
	void updateHPBar();

	void enterBuildMode();
	void exitBuildMode();
	void moveBuildMode(bool right);
	void confirmBuildMode();

	void renderBuildings();

	void equipmentCache();
	void addItemsToInventory(std::vector<int> itemList, std::vector<int> itemCountList);
	void addItemToInventory(int item, int count);

	void resetContractCollect();

	//assign the parameter points depending on the building type
	void assignEndingParameters(int buildingType);

	void confirmCrafting();

	void npcAI();
	void spawnNPC(int npcNumber);

	void receivePlayerDamage();

	void resetPlayerActions();
	void createBuilding(float posX, float npcfootY);

    // a selector callback
    void menuCloseCallback();

	void popUpMsg(int msgType);
	void closeMsg();
	
	void nextAge();
	void nextStep();

	void saveGame();
    
	//controls
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);

    // implement the "static create()" method manually
    CREATE_FUNC(Control3);

private:
	
	enum SWING_TYPE
	{
		SWING_AXE,
		SWING_SWORD,
		SWING_PICK
	};

	enum MSG_TYPE
	{
		MSG_CONFIRM,
		MSG_TUTORIAL,
		MSG_CHANGE_MAP,
		MSG_BUILD_MODE,
		MSG_BUILDING_EFFECT,
		MSG_CRAFTING,
		MSG_CONTRACT,
		MSG_TMP
	};

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

		//Item Type integer stored as RESOURCE_ID
		int itemType;
		//Amount of item of the same type
		int itemAmount;
	};

	struct BuildingSlot
	{
	public:
		BuildingSlot(bool constructable, int buildingType)
			:constructable(constructable), buildingType(buildingType)
		{
		}

		BuildingSlot()
			:constructable(true), buildingType(0), sprite(nullptr)
		{
		}

		bool constructable;
		int buildingType;
		cocos2d::Sprite* sprite;
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
			npcType(NPC_TYPE_CAVE_GIRL1),
			directionRight(false),
			timeCounter(0),
			firstTimeState(true),
			buildingNum1(1),
			buildingNum2(0),
			buildingNum3(0),
			buildingDone(false)
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
			buildingNum3(0),
			buildingDone(false)
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
		bool buildingDone;
	};

private:
	//Constants
	//Sprite name constants
	const std::string POPUP_SPRITE = "PopUp";
	const std::string RESOURCE_1 = "Res1";
	const std::string RESOURCE_COUNT_1 = "ResCount1";
	const std::string RESOURCE_2 = "Res2";
	const std::string RESOURCE_COUNT_2 = "ResCount2";
	const std::string RESOURCE_3 = "Res3";
	const std::string RESOURCE_COUNT_3 = "ResCount3";
	const std::string HOUSE_ICON_SPRITE = "HouseIcon";
	//Sprite tag constants
	const int JOYSTICK_SPRITE_NUM = 88;
	//In-game value constants
	//Max time of the day. (This affects the progressing velocity of the day timer)
	const int MAX_TIME_DAY = 72000; //72000 = 20 min, 90000 = 25 min, 3600 = 1 min

	//classes
	//Enemy class
	Enemy m_enemy;
	//Parallax Background class
	ParallaxBackground m_background;
	bool m_bgEnabled;
	//Effect Class
	Effect* m_effects;

	//Age number
	unsigned int m_age;

	//cocos / game specific pointers
	cocos2d::Sprite* m_character;
	cocos2d::Sprite* m_hpBar;
	std::vector<cocos2d::Sprite*> m_npcVector;
	std::vector<cocos2d::Sprite*> m_resourceVector;
	std::vector<cocos2d::Sprite*> m_resourceIconVector;
	std::vector<cocos2d::Sprite*> m_itemsVector;
	std::vector <cocos2d::Label*> m_itemCountVector;
	std::vector<NPCStates> m_npcStateVector;
	cocos2d::AnimationCache* m_playerAnimCache;
	cocos2d::TMXTiledMap* m_tilemap;
	cocos2d::TMXLayer* m_collisionLayer;
	bool m_directionRight;
	bool m_isTalking;
	bool m_isCreatedPopUp;
	bool m_isCreatedActionPopUp;
	int m_talkChecker;
	int m_interactingBuildingType;
	bool m_isCrafting;
	bool m_isCraftingTouch;

	//Stores the player position before the build mode stars to reset the viewpoint back to the player position correctly.
	cocos2d::Vec2 m_playerPosPreBuildMode;

	std::vector<BuildingSlot> m_buildingsVector;

	//Areas that area shown in the area selection menu (stored as int, the type of area)
	std::vector<int> m_accesibleAreas;

	bool m_disableGame;
	bool m_isPopUpMsg;

	//game timer
	float m_timeOfDay;
	int m_frameCounter;

	//Ending Parameters (5)
	int m_trueEnding;
	int m_happyEnding;
	int m_goodEnding;
	int m_badEnding;
	int m_worstEnding;

	//Control related
	cocos2d::Rect m_rightRect;
	cocos2d::Rect m_leftRect;
	cocos2d::Rect m_JoyStick;
	cocos2d::Rect m_StopButton;
	bool m_aButtonPressed;
	bool m_jButtonPressed;
	cocos2d::Rect m_aButtonRect;
	cocos2d::Rect m_jButtonRect;
	int m_moveTouchID;
	int m_actionTouchID;
	int m_jumpTouchID;

	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_uiNode;
	cocos2d::Node* m_temporaryNode;
	cocos2d::Node* m_backgroundNode;
	cocos2d::Node* m_skyNode;
	cocos2d::Node* m_foregroundNode;

	int m_talkingNPCIndex;

	//---------------------
	//----physics variables/constants
	//---------------------

	//gravity acceleration for character
	const float GRAVITY = -0.2;
	
	//max speed for character
	const float MAX_SPEED = 4;

	//max jump capacity
	const float MAX_JUMP = 4.8f;

	//player states
	bool m_standing;
	bool m_jumpAnim;
	bool m_onGround;
	bool m_jumpInstanced;
	bool m_onNext;
	bool m_isActing;
	bool m_actingInstanced;
	bool m_actingWithEnemy;
	bool m_createdResourcePopup;
	int m_actingResourceIndex;
	int m_actedResourceCount;
	bool m_swinging;
	bool m_attackOnce;
	bool m_animationInstanced;
	int m_buildingSlots;
	float m_groundLevel;
	float m_buildingStart;
	float m_buildingEnd;
	int m_slotSize;
	float m_inventoryY;
	float m_inventoryX;

	//contract variables
	//3 states: contract available / in-contract / contract finished
	int m_contractState;
	//this determines the duration and price of the contract
	int m_contractLevel;
	//counter that checks if contract duration is passed
	int m_contractTime;
	//
	int m_contractArea;
	int m_contractBuildingCount;
	//max number of items per collect is 5
	std::string m_contractCollectSave[5] = {"collectSlot1", "collectSlot2" , "collectSlot3" , "collectSlot4" , "collectSlot5" };
	std::string m_contractCollectCountSave[5] = { "collectSlotCount1", "collectSlotCount2" , "collectSlotCount3" , "collectSlotCount4" , "collectSlotCount5" };
	std::vector<PlayerInventory> m_contractCollect;

	bool m_fromContract;

	//player variables
	float m_speedX;
	float m_speedY;
	float m_accelerationX;
	float m_frictionX;
	float m_jumpForce;
	int m_damage;
	int m_hp;

	int m_craftingResource1;
	int m_craftingResource2;

	bool m_isTutorialHome;
	bool m_isTutorial;
	bool m_stepDone;
	int m_step;
	bool m_inTimeMachine;
	int m_buildingModeIndex;
	//ui flags
	bool m_isUIEnabled;
	bool m_isMoveUIEnabled;
	bool m_isJumpUIEnabled;
	bool m_isActionUIEnabled;
	bool m_isItemUIEnabled;

	//ボタン配置、サイズ関係置き場
	bool _ButtonSwap;

	PlayerInventory m_inventorySlot[10];
	int m_gold;
};

#endif // __Control3_H__

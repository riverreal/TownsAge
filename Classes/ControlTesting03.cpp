#include "ControlTesting03.h"
#include "TitleScene.h"
#include "DamageHandler.h"
#include "Option.h"
#include <sstream>

USING_NS_CC;

Scene* Control3::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Control3::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Control3::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_isTutorialHome = false;
	m_inTimeMachine = false;
	m_isTutorial = true;
	m_stepDone = true;

	//physics variable initialization
	m_standing = true;
	m_onGround = false;
	m_jumpInstanced = false;
	m_speedX = 0;
	m_speedY = 0;
	m_accelerationX = 0.1;
	m_frictionX = 1;
	m_jumpForce = 1.5;
	m_damage = 10;
	m_step = 0;
	
	//control init
	m_aButtonPressed = false;
	m_jButtonPressed = false;

	//node initialization
	m_gameNode = Node::create();
	m_uiNode = Node::create();
	m_temporaryNode = Node::create();
	m_skyNode = Node::create();

	//facing right by default
	m_directionRight = true;
	m_isTalking = false;
	m_isCreatedPopUp = false;
	m_isCreatedActionPopUp = false;
	m_onNext = false;
	m_isActing = false;
	m_actingWithEnemy = false;
	m_actingInstanced = false;
	m_swinging = false;
	m_disableGame = false;

	m_actedResourceCount = 0;

	m_talkChecker = 0;

	auto def = UserDefault::getInstance();
	//Area system
	// <--- Left Map      HOME     Right Map --->
	// -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5
	int area = def->getIntegerForKey("MapArea");
	bool fromRight = def->getBoolForKey("FromRight");
	int playerX;
	int playerY;
	//right map
	if (area > 0)
	{
		if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::FOREST01)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/forest01.tmx");
		}
		else if(def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::MAP_TUTORIAL)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/tutorial.tmx");
		}
		else if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::MAP_TUTORIAL2)
		{
			m_isTutorialHome = true;
			m_tilemap = TMXTiledMap::create("img/tilemap/tutorial2.tmx");
		}
		else if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::HOME_MAP)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/home.tmx");
		}
		else
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/tutorial2.tmx");
		}
	}
	//left map
	else if (area < 0)
	{
		if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::FOREST01)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/forest01.tmx");
		}
		else if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::MAP_TUTORIAL)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/tutorial.tmx");
		}
		else if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::MAP_TUTORIAL2)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/tutorial2.tmx");
		}
		else if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::HOME_MAP)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/home.tmx");
		}
		else
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/tutorial.tmx");
		}
	}
	//home map
	else
	{
		m_tilemap = TMXTiledMap::create("img/tilemap/home.tmx");
	}

	m_gameNode->addChild(m_tilemap, -1);
	m_tilemap->setCascadeOpacityEnabled(true);
	auto objectGroup = m_tilemap->getObjectGroup("player");
	ValueMap comeFrom;
	comeFrom = objectGroup->getObject("NextLeft");
	float leftArrowX = comeFrom["x"].asInt();
	float leftArrowY = comeFrom["y"].asInt();
	comeFrom = objectGroup->getObject("NextRight");
	float rightArrowX = comeFrom["x"].asInt();
	float rightArrowY = comeFrom["y"].asInt();

	ValueMap spawnBox = objectGroup->getObject("Spawn");
	if (spawnBox["x"].asInt() != 0)
	{
		playerX = spawnBox["x"].asInt();
		playerY = spawnBox["y"].asInt();
	}
	else
	{
		if (fromRight)
		{
			playerX = leftArrowX + 52;
			playerY = leftArrowY;
		}
		else
		{
			playerX = rightArrowX - 52;
			playerY = rightArrowY;
		}
	}

	auto collectableGroup = m_tilemap->getObjectGroup("collectable");
	if (collectableGroup)
	{
		auto collectVector = collectableGroup->getObjects();
		for (auto& item : collectVector)
		{
			if (item.asValueMap()["name"].asString() == "tree")
			{
				auto sprite = Sprite::create("img/resources/tree02.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName(item.asValueMap()["name"].asString());
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "appletree")
			{
				auto sprite = Sprite::create("img/resources/tree03.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName(item.asValueMap()["name"].asString());
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "engine")
			{
				auto sprite = Sprite::create("img/timeMachine/TimeMachineEngine.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName(item.asValueMap()["name"].asString());
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
		}
	}

	

	auto leftArrow = Sprite::create("img/ui/red-arrow.png");
	leftArrow->setRotation(-90);
	leftArrow->setScale(0.1);
	leftArrow->setPosition(leftArrowX + m_tilemap->getTileSize().width / 2, leftArrowY + m_tilemap->getTileSize().height / 2);
	leftArrow->setName("LeftArrow");
	m_gameNode->addChild(leftArrow);

	auto rightArrow = Sprite::create("img/ui/red-arrow.png");
	rightArrow->setRotation(90);
	rightArrow->setScale(0.1);
	rightArrow->setPosition(rightArrowX + m_tilemap->getTileSize().width / 2, rightArrowY + m_tilemap->getTileSize().height / 2);
	rightArrow->setName("RightArrow");
	m_gameNode->addChild(rightArrow);

	m_inventoryY = visibleSize.height / 1.1;
	auto inventoryFrame = Sprite::create("img/ui/inventory.png");
	inventoryFrame->setOpacity(100);
	inventoryFrame->setPosition(visibleSize.width / 2, visibleSize.height / 1.1);
	m_uiNode->addChild(inventoryFrame);

	m_inventorySlot[0].itemType = def->getIntegerForKey("inventorySlotType01");
	m_inventorySlot[1].itemType = def->getIntegerForKey("inventorySlotType02");
	m_inventorySlot[2].itemType = def->getIntegerForKey("inventorySlotType03");
	m_inventorySlot[3].itemType = def->getIntegerForKey("inventorySlotType04");
	m_inventorySlot[4].itemType = def->getIntegerForKey("inventorySlotType05");
	m_inventorySlot[5].itemType = def->getIntegerForKey("inventorySlotType06");
	m_inventorySlot[6].itemType = def->getIntegerForKey("inventorySlotType07");
	m_inventorySlot[7].itemType = def->getIntegerForKey("inventorySlotType08");
	m_inventorySlot[8].itemType = def->getIntegerForKey("inventorySlotType09");
	m_inventorySlot[9].itemType = def->getIntegerForKey("inventorySlotType10");
	m_inventorySlot[0].itemAmount = def->getIntegerForKey("inventorySlotAmount01");
	m_inventorySlot[1].itemAmount = def->getIntegerForKey("inventorySlotAmount02");
	m_inventorySlot[2].itemAmount = def->getIntegerForKey("inventorySlotAmount03");
	m_inventorySlot[3].itemAmount = def->getIntegerForKey("inventorySlotAmount04");
	m_inventorySlot[4].itemAmount = def->getIntegerForKey("inventorySlotAmount05");
	m_inventorySlot[5].itemAmount = def->getIntegerForKey("inventorySlotAmount06");
	m_inventorySlot[6].itemAmount = def->getIntegerForKey("inventorySlotAmount07");
	m_inventorySlot[7].itemAmount = def->getIntegerForKey("inventorySlotAmount08");
	m_inventorySlot[8].itemAmount = def->getIntegerForKey("inventorySlotAmount09");
	m_inventorySlot[9].itemAmount = def->getIntegerForKey("inventorySlotAmount10");

	m_hp = def->getIntegerForKey("hp");

	m_collisionLayer = m_tilemap->getLayer("col");
	m_collisionLayer->setVisible(false);

	m_character = Sprite::create("img/sprites/player/idle1.png");
	m_character->setScale(1.024);
	m_character->setZOrder(20);
	if (def->getBoolForKey("FromRight"))
	{
		m_character->setFlippedX(true);
	}

	m_character->setPosition(playerX + m_tilemap->getTileSize().width, playerY + m_tilemap->getTileSize().height);
	m_gameNode->addChild(m_character);

	playerAnimationCache();
	equipmentCache();

	m_character->runAction(RepeatForever::create(Animate::create(m_playerAnimCache->getAnimation("idle"))));

	if (area == mapData::HOME_MAP)
	{
		//spawn 4 npcs
		spawnNPC(4);
	}

	
	///////////////////////////////////////////////////////////////////////////////////////////////
	_ButtonSwap = def->getBoolForKey("ButtonSwap");
	//ここスティックに
	auto leftButton = Sprite::create("img/dpad.png");
	leftButton->setPosition(visibleSize.width / 10 * 0.75, visibleSize.height / 6);
	leftButton->setScaleX(6.5);
	
	leftButton->setRotation(-90);
	leftButton->setOpacity(0);
	m_uiNode->addChild(leftButton);

	//処理は真ん中にストップボタンをつけて画像はスティックにすればそれっぽくなる。
	auto StickBack = Sprite::create("img/ui/pad2.png");
	StickBack->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
	StickBack->setScale(1.5);
	StickBack->setOpacity(def->getFloatForKey("button"));
	m_uiNode->addChild(StickBack);

	auto JoyStick = Sprite::create("img/ui/stick.png");
	JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
	JoyStick->setScale(1.5);
	JoyStick->setOpacity(def->getFloatForKey("button"));
	JoyStick->setTag(JOYSTICK_SPRITE_NUM);
	m_uiNode->addChild(JoyStick);
	//ストップボタン
	auto stopButton = Sprite::create("img/dpad.png");
	stopButton->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
	stopButton->setScaleY(6.5);
	stopButton->setOpacity(0);
	m_uiNode->addChild(stopButton);
	/////////////////////////////


	auto rightButton = Sprite::create("img/dpad.png");
	rightButton->setPosition(visibleSize.width / 10 * 2.25, visibleSize.height / 6);
	rightButton->setScaleX(6.5);
	rightButton->setRotation(90);
	rightButton->setOpacity(0);
	m_uiNode->addChild(rightButton);
	
		auto jButton = Sprite::create("img/ui/Jump.png");
		if (_ButtonSwap == false)
		{
			jButton->setPosition(visibleSize.width / 10 * 7.3, visibleSize.height / 6);
			jButton->setOpacity(def->getFloatForKey("button"));
			jButton->setScale(0.35);
			m_uiNode->addChild(jButton);

		}
		else if (_ButtonSwap)
		{
			jButton->setPosition(visibleSize.width / 10 * 9, visibleSize.height / 4);
			jButton->setOpacity(def->getFloatForKey("button"));
			jButton->setScale(0.35);
			m_uiNode->addChild(jButton);

		}
		
		auto aButton = Sprite::create("img/ui/Attack.png");
		if (_ButtonSwap == false)
		{
			aButton->setPosition(visibleSize.width / 10 * 9, visibleSize.height / 4);
			aButton->setOpacity(def->getFloatForKey("button"));
			aButton->setScale(0.35);
			m_uiNode->addChild(aButton);

		}
		else if (_ButtonSwap)
		{
			aButton->setPosition(visibleSize.width / 10 * 7.3, visibleSize.height / 6);
			aButton->setOpacity(def->getFloatForKey("button"));
			aButton->setScale(0.35);
			m_uiNode->addChild(aButton);

		}
	
	
	
	

	// add a "close" icon to exit the progress. it's an autorelease object
	/*
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Control3::menuCloseCallback, this));
	//closeItem->setScale(3);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getBoundingBox().getMaxX(),
		origin.y + closeItem->getBoundingBox().getMaxY()));
	closeItem->setVisible(false);

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	m_uiNode->addChild(menu);
	*/

	m_rightRect = Rect(rightButton->getBoundingBox());
	m_leftRect = Rect(leftButton->getBoundingBox());
	m_JoyStick = Rect(JoyStick->getBoundingBox());
	m_StopButton = Rect(stopButton->getBoundingBox());
	m_jButtonRect = Rect(jButton->getBoundingBox());
	m_aButtonRect = Rect(aButton->getBoundingBox());

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(Control3::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Control3::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Control3::onTouchesEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	m_gameNode->setScale(1.5);

	m_gameNode->setPosition(Vec2::ZERO);
	m_uiNode->setPosition(Vec2::ZERO);
	m_temporaryNode->setPosition(Vec2::ZERO);
	m_skyNode->setPosition(Vec2::ZERO);
	this->addChild(m_gameNode);
	this->addChild(m_uiNode);
	this->addChild(m_temporaryNode);
	this->addChild(m_skyNode, -10);
	//mid day
	//bg = LayerGradient::create(Color4B(34, 98, 206, 255), Color4B(193, 205, 219, 255), Vec2(0, -1));
	//Dawn
	//bg = LayerGradient::create(Color4B(11, 40, 110, 255), Color4B(169, 110, 78, 255), Vec2(0, -1));
	
	auto bg = LayerGradient::create(Color4B(34, 98, 206, 255), Color4B(173, 240, 245, 255), Vec2(0, -1));
	m_skyNode->addChild(bg, -50);

	auto enemyGroup = m_tilemap->getObjectGroup("enemy");
	if (enemyGroup)
	{
		auto enemyVector = enemyGroup->getObjects();
		for (auto& enemy : enemyVector)
		{
			auto position = Vec2(enemy.asValueMap()["x"].asFloat(), enemy.asValueMap()["y"].asFloat());
			m_enemy.SetStaticEnemy(enemy.asValueMap()["name"].asString(), position, enemy.asValueMap()["range"].asFloat());
		}
	}

	m_enemy.InitEnemies(m_gameNode);

	setViewpoint(m_character->getPosition());
	m_hpBar = Sprite::create("img/ui/hpBar.png");
	m_hpBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_hpBar->setPosition(inventoryFrame->getBoundingBox().getMinX(), m_inventoryY - 60);
	updateHPBar();
	m_uiNode->addChild(m_hpBar);

	auto engineSilluette = Sprite::create("img/timeMachine/TimeMachineEngine.png");
	engineSilluette->setScale(1.7);
	engineSilluette->setPosition(visibleSize.width / 1.05, m_inventoryY);
	if (!def->getBoolForKey("HasTimeMachinePart"))
	{
		engineSilluette->setColor(Color3B::BLACK);
		engineSilluette->setOpacity(150);
	}
	engineSilluette->setName("engineSilluette");
	m_uiNode->addChild(engineSilluette);

	showItems();
	int index = m_resourceVector.size();
	this->scheduleUpdate();

	auto fadeIn = FadeIn::create(0.6f);
	auto ui = (Sprite*)m_uiNode;
	ui->setCascadeOpacityEnabled(true);
	ui->setOpacity(0);
	ui->runAction(fadeIn);

	if (m_isTutorialHome)
	{
		//Spawn 1 npc

		//Create a default state first
		NPCStates randomState;
		int maxType = NPC_TYPE_PREHISTORIC_MAX - 1;
		int maxBuilding = buildingData::MAX_PREHISTORIC - 1;
		randomState.npcType = cocos2d::RandomHelper::random_int(0, maxType);
		randomState.buildingNum1 = buildingData::HOUSE;
		//WARNING: npcStateVector and npcVector has to share the same index!
		m_npcStateVector.push_back(randomState);
		m_npcVector.push_back(Sprite::create(npcTypePath[m_npcStateVector[0].npcType]));
		m_npcVector[0]->setPositionY(m_character->getPositionY() - 15);
		m_npcVector[0]->setPositionX(cocos2d::RandomHelper::random_int(0, 500) + 800);
		m_npcVector[0]->setScale(0.1272);
		m_gameNode->addChild(m_npcVector[0]);

		auto timeMachine = Sprite::create("img/timeMachine/TimeMachine1.png");
		auto timeMachineObject = objectGroup->getObject("TimeMachine");
		auto timeMachineX = timeMachineObject["x"].asFloat();
		auto timeMachineY = timeMachineObject["y"].asFloat();
		timeMachine->setPosition(timeMachineX, timeMachineY);
		timeMachine->setName("timeMachine");
		timeMachine->setScale(2.5);
		m_gameNode->addChild(timeMachine);
	}

    return true;
}

void Control3::walk(bool directionRight, Sprite* subject)
{
	resetPlayerActions();
	
	if (directionRight) //right
	{
		subject->setFlippedX(true);
	}
	else //left
	{
		subject->setFlippedX(false);
	}
	
	
	auto frame1 = RotateTo::create(0.1, -20);
	auto frame2 = RotateTo::create(0.1, 20);
	auto sequence = Sequence::create(frame1, frame2, NULL);
}

void Control3::popUp(cocos2d::Sprite * subject)
{
	if (!m_isCreatedPopUp)
	{
		if (!m_gameNode->getChildByName(POPUP_SPRITE))
		{
			auto popup = Sprite::create("img/ui/popup_yellow.png");
			popup->setPosition(subject->getPosition());
			popup->setName(POPUP_SPRITE);
			popup->setScaleY(1.5);
			popup->setScaleX(0.8);
			popup->setOpacity(0);

			auto house = Sprite::create(buildingData::buildingPath[m_npcStateVector[m_talkingNPCIndex].buildingNum1]);
			house->setPosition(subject->getPositionX(), subject->getPositionY() + 3);
			house->setName(HOUSE_ICON_SPRITE);
			house->setScale(0.7);
			house->setOpacity(0);

			m_gameNode->addChild(popup);
			m_gameNode->addChild(house);

			auto moveUp = MoveBy::create(0.5f, Vec2(0.0f, 80));
			auto fadeIn = FadeIn::create(0.5f);
			auto action = Spawn::create(moveUp, fadeIn, NULL);
			auto actionCpy = action->clone();
			popup->runAction(action);
			house->runAction(actionCpy);
			m_isCreatedPopUp = true;
		}
	}
}

void Control3::cleanPopUp()
{
	m_gameNode->removeChildByName(POPUP_SPRITE);
	m_gameNode->removeChildByName(HOUSE_ICON_SPRITE);
	m_gameNode->removeChildByName(RESOURCE_1);
	m_gameNode->removeChildByName(RESOURCE_2);
	m_gameNode->removeChildByName(RESOURCE_3);
	m_gameNode->removeChildByName(RESOURCE_COUNT_1);
	m_gameNode->removeChildByName(RESOURCE_COUNT_2);
	m_gameNode->removeChildByName(RESOURCE_COUNT_3);
}

void Control3::removePopUp()
{
	if (m_isCreatedPopUp)
	{
		auto popup = m_gameNode->getChildByName(POPUP_SPRITE);
		auto house = m_gameNode->getChildByName(HOUSE_ICON_SPRITE);
		m_isCreatedPopUp = false;
		if (popup)
		{
			auto moveDown = MoveBy::create(0.3f, Vec2(0, -80));
			auto fadeOut = FadeOut::create(0.3f);
			auto cleanPopUp = CallFunc::create([this]() {this->cleanPopUp(); });

			auto action = Sequence::create(Spawn::create(moveDown, fadeOut, NULL), cleanPopUp, NULL);
			auto actionCpy = action->clone();
			auto actionCpy2 = action->clone();
			auto actionCpy3 = action->clone();
			auto actionCpy4 = action->clone();
			auto actionCpy5 = action->clone();
			auto actionCpy6 = action->clone();
			auto actionCpy7 = action->clone();

			popup->runAction(action);
			house->runAction(actionCpy);

			auto res1 = m_gameNode->getChildByName(RESOURCE_1);
			auto resCount1 = m_gameNode->getChildByName(RESOURCE_COUNT_1);
			if (res1)
			{
				res1->runAction(actionCpy2);
				resCount1->runAction(actionCpy5);

				auto res2 = m_gameNode->getChildByName(RESOURCE_2);
				auto resCount2 = m_gameNode->getChildByName(RESOURCE_COUNT_2);
				if (res2)
				{
					res2->runAction(actionCpy3);
					resCount2->runAction(actionCpy6);
					auto res3 = m_gameNode->getChildByName(RESOURCE_3);
					auto resCount3 = m_gameNode->getChildByName(RESOURCE_COUNT_3);
					if (res3)
					{
						res3->runAction(actionCpy4);
						resCount3->runAction(actionCpy7);
					}
				}
			}
		}
	}
}

void Control3::npcWalk(bool directionRight, cocos2d::Sprite * subject)
{
	auto moveRight = MoveBy::create(0.5, Vec2(20, 0));
	auto moveLeft = MoveBy::create(0.5, Vec2(-20, 0));

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

	auto frame1 = RotateTo::create(0.1, -20);
	auto frame2 = RotateTo::create(0.1, 20);
	auto sequence = Sequence::create(frame1, frame2, NULL);
	subject->runAction(RepeatForever::create(sequence));
}

void Control3::resourcePopup()
{
	bool created = false;

	CCLOG("resource vector size: %i", static_cast<int>(m_resourceVector.size()));

	for (int i = 0; i < m_resourceVector.size(); i++)
	{
		auto resource = m_resourceVector[i];
		if (resource != nullptr)
		{
			if (m_character->getBoundingBox().intersectsRect(resource->getBoundingBox()))
			{
				if (resource->getName() == "tree" || resource->getName() == "appletree")
				{
					created = true;
					if (!m_isCreatedActionPopUp)
					{
						m_actingResourceIndex = i;
						resource->setColor(Color3B(200, 200, 200));
						m_isCreatedActionPopUp = true;
						m_isActing = true;
						m_actingWithEnemy = false;
					}
				}
				else if (resource->getName() == "engine")
				{
					created = true;
					if (!m_isCreatedActionPopUp)
					{
						m_isCreatedActionPopUp = true;
						resource->setColor(Color3B::BLACK);
						auto resourcePos = m_gameNode->convertToWorldSpace(resource->getPosition());
						auto engineClone = Sprite::create("img/timeMachine/TimeMachineEngine.png");
						//engineClone->setTexture(resource->getTexture());
						engineClone->setScale(m_uiNode->getChildByName("engineSilluette")->getScale());
						engineClone->setPosition(resourcePos);
						m_gameNode->removeChild(resource);
						m_uiNode->addChild(engineClone);
						auto moveTo = MoveTo::create(1.0f, m_uiNode->getChildByName("engineSilluette")->getPosition());
						engineClone->runAction(moveTo);
						m_resourceVector.erase(m_resourceVector.begin() + i);
						auto def = UserDefault::getInstance();
						def->setBoolForKey("HasTimeMachinePart", true);
					}
				}
			}
			else
			{
				if (resource->getName() == "tree" || resource->getName() == "appletree")
				{
					if (!created)
					{
						resource->setColor(Color3B(255, 255, 255));
						m_actingResourceIndex = -1;
						if (m_isCreatedActionPopUp)
						{
							//m_gameNode->removeChildByName("actionPopup");
							m_isCreatedActionPopUp = false;
							m_actingWithEnemy = false;
							m_isActing = false;
						}
					}
				}
			}
		}
	}

	if (!m_isCreatedActionPopUp)
	{
		m_actedResourceCount = 0;
	}
	
}

void Control3::setViewpoint(cocos2d::Vec2 position)
{
	auto visibleSize = Director::getInstance()->getWinSize();

	int x = (visibleSize.width / 2) - position.x;
	int y = (visibleSize.height / 2.3) - position.y;

	//player-centered position non-scaled
	auto defPosition = Vec2(x, y);
	
	auto newViewPoint = Vec2(x * m_gameNode->getScaleX(), y * m_gameNode->getScaleY());
	auto viewPointDelta = newViewPoint - defPosition;

	//scaled position
	defPosition += viewPointDelta;
	defPosition -= (visibleSize / 2) * (m_gameNode->getScale() - 1);
	
	m_gameNode->setPosition(defPosition);
}

Vec2 Control3::convertToTilePosition(Vec2 position)
{
	int x = position.x / m_tilemap->getTileSize().width;
	int y = ((m_tilemap->getMapSize().height * m_tilemap->getTileSize().height) - position.y) / m_tilemap->getTileSize().height;

	return Vec2(x , y);
}

void Control3::simplePhysics()
{
	auto swingTime = 0.34f;

	//----Gracity for character----
	m_speedY += GRAVITY;
	//-----------------------------

	//-------------------------------------------------------
	//------ACTIONS------------------------------------------
	//-------------------------------------------------------

	//DMGHandler Player recieves dmg if dmg was dealt
	m_actingWithEnemy = m_enemy.updateEnemies(m_character->getBoundingBox());

	if (m_aButtonPressed)
	{
		if (m_isActing)
		{
			//it's a resource
			if (!m_actingWithEnemy)
			{
				if (!m_actingInstanced)
				{
					if (m_resourceVector[m_actingResourceIndex])
					{
						auto endInstance = CallFunc::create([this]() {this->finishActionInstance(); });
						CallFunc* swingPlayer = CallFunc::create([this]() {this->swingPlayer(SWING_AXE); });;
						if (m_resourceVector[m_actingResourceIndex]->getName() == "tree" || m_resourceVector[m_actingResourceIndex]->getName() == "appletree")
						{
							swingPlayer = CallFunc::create([this]() {this->swingPlayer(SWING_AXE); });
						}

						auto swingDelay = DelayTime::create(swingTime);
						if (m_actedResourceCount <= 5)
						{
							m_actingInstanced = true;
							auto shake1 = RotateBy::create(0.05, 5);
							auto shake2 = RotateBy::create(0.05, -10);
							m_resourceVector[m_actingResourceIndex]->runAction(Sequence::create(shake1, shake2, shake1, swingPlayer, swingDelay, endInstance, NULL));
							m_actedResourceCount++;
						}
						else
						{
							m_actingInstanced = true;
							auto shake1 = RotateBy::create(0.05, 10);
							auto shake2 = RotateBy::create(0.05, -20);
							auto fadeOut = FadeOut::create(0.1);
							auto delay = DelayTime::create(0.1);
							m_resourceVector[m_actingResourceIndex]->runAction(Sequence::create(shake1, shake2, shake1, fadeOut, delay, swingPlayer, swingDelay, endInstance, NULL));
						}
					}
				}
			}
		}

		if (!m_actingInstanced)
		{
			m_actingInstanced = true;
			auto endInstance = CallFunc::create([this]() {this->finishActionInstance(); });
			auto swingPlayer = CallFunc::create([this]() {this->swingPlayer(SWING_SWORD); });
			auto swingDelay = DelayTime::create(swingTime);
			auto preSwingDelay = DelayTime::create(swingTime/2);
			m_gameNode->runAction(Sequence::create(preSwingDelay, swingPlayer, swingDelay, endInstance, NULL));
		}
	}

	if (!m_animationInstanced)
	{
		if (m_standing)
		{
			m_animationInstanced = true;
			resetPlayerActions();
			m_character->runAction(RepeatForever::create(Animate::create(m_playerAnimCache->getAnimation("idle"))));
		}
		else
		{
			m_animationInstanced = true;
			auto walkingAnim = Animate::create(m_playerAnimCache->getAnimation("walking"));
			m_character->runAction(RepeatForever::create(walkingAnim));
		}
	}
	
	if (m_isTutorialHome)
	{
		if (!m_inTimeMachine)
		{
			auto timeMachine = m_gameNode->getChildByName("timeMachine");
			if (m_character->getBoundingBox().intersectsRect(timeMachine->getBoundingBox()))
			{
				m_inTimeMachine = true;
				m_character->setPosition(m_gameNode->convertToWorldSpace(m_character->getPosition()));
				m_character->retain();
				m_character->removeFromParent();
				m_temporaryNode->addChild(m_character);
				m_character->release();
				m_character->setScale(m_character->getScale() * m_gameNode->getScale());

				timeMachine->setPosition(m_gameNode->convertToWorldSpace(timeMachine->getPosition()));
				timeMachine->retain();
				timeMachine->removeFromParent();
				m_temporaryNode->addChild(timeMachine);
				timeMachine->release();
				timeMachine->setScale(timeMachine->getScale() * m_gameNode->getScale());

				auto timeMachinePart = m_uiNode->getChildByName("engineSilluette");
				timeMachinePart->retain();
				timeMachinePart->removeFromParent();
				m_temporaryNode->addChild(timeMachinePart);
				timeMachinePart->release();

				m_gameNode->setCascadeOpacityEnabled(true);
				m_uiNode->setCascadeOpacityEnabled(true);
				m_skyNode->setCascadeOpacityEnabled(true);
				m_temporaryNode->setZOrder(10);

				auto screenFiller = Sprite::create("img/bg.png");
				auto visibleSize = Director::getInstance()->getVisibleSize();
				screenFiller->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				screenFiller->setColor(Color3B::BLACK);
				screenFiller->setScale(3);
				screenFiller->setOpacity(0);
				m_temporaryNode->addChild(screenFiller, -1);

				screenFiller->runAction(FadeIn::create(2));

				m_disableGame = true;
				m_character->runAction(Animate::create(m_playerAnimCache->getAnimation("idle")));

				timeMachinePart->setZOrder(-1);

				//--------------------------------------------------------------
				//Definition of actions needed for time travel.
				//--------------------------------------------------------------
				auto delay = DelayTime::create(1.5);
				auto moveTo = MoveTo::create(1, timeMachine->getPosition());

				auto tempNode = m_temporaryNode;

				auto transform = CallFunc::create([timeMachine, tempNode]() {
					auto scaleUp = ScaleBy::create(0.1, 1.2);
					auto scaleDown = ScaleBy::create(0.1, 0.8);

					auto newTimeMachine = Sprite::create("img/timeMachine/TimeMachine2.png");
					newTimeMachine->setPosition(timeMachine->getPosition());
					newTimeMachine->setName("newTime");
					newTimeMachine->setScale(timeMachine->getScale());
					tempNode->addChild(newTimeMachine, 2);
					newTimeMachine->setVisible(false);


					auto hideOld = Hide::create();

					auto showNewFunc = CallFunc::create([newTimeMachine]() {
						auto showNew = Show::create();
						newTimeMachine->runAction(showNew);
					});

					auto hideAndShow = Spawn::create(hideOld, showNewFunc, NULL);

					timeMachine->runAction(Sequence::create(scaleUp, scaleDown, scaleUp, scaleDown, scaleUp, scaleDown, scaleUp, scaleDown, scaleUp, scaleDown, scaleUp, hideAndShow, NULL));
				});

				auto reachDelay = DelayTime::create(0.5);

				auto transformDelay = DelayTime::create(1.6);

				auto player = m_character;

				auto jumpPlayer = CallFunc::create([player, tempNode]() {
					auto pos = tempNode->getChildByName("newTime")->getPosition();
					auto diffX = -pos.x + player->getPositionX();
					auto diffY = -pos.y + player->getPositionY();
					auto moveX = MoveBy::create(0.4, Vec2(-diffX, 0));
					auto moveY = MoveBy::create(0.7, Vec2(0, -diffY + 8));
					auto fadeOutPlayer = FadeOut::create(0.7);
					auto sequence = Sequence::create(Spawn::create(moveX, moveY, NULL), fadeOutPlayer, NULL);

					player->runAction(sequence);
				});

				auto jumpDelay = DelayTime::create(1.8);

				auto twirling = CallFunc::create([tempNode, player, timeMachinePart]() {
					timeMachinePart->setVisible(false);
					player->setVisible(false);
					auto newTime = tempNode->getChildByName("newTime");
					auto skew = SkewBy::create(0.5, RandomHelper::random_int(-20, 20), RandomHelper::random_int(-20, 20));
					auto skew2 = SkewBy::create(0.5, RandomHelper::random_int(-20, 20), RandomHelper::random_int(-20, 20));
					auto skew3 = SkewBy::create(0.5, RandomHelper::random_int(-20, 20), RandomHelper::random_int(-20, 20));
					auto skew4 = SkewBy::create(0.5, RandomHelper::random_int(-20, 20), RandomHelper::random_int(-20, 20));
					auto skew5 = SkewBy::create(0.5, RandomHelper::random_int(-100, 100), RandomHelper::random_int(-100, 100));
					auto blinking = Blink::create(2.5f, 10);
					newTime->runAction(Sequence::create(skew, skew2, skew3, skew4, skew5, NULL));
					newTime->runAction(blinking);
					newTime->runAction(FadeOut::create(3.5f));
				});

				auto twirlingDelay = DelayTime::create(4.5f);


				auto backToTitleScreen = CallFunc::create([this]() {this->menuCloseCallback(); });

				//Complete sequence of actions for the time travel animation.
				//transform, jumpPlayer and twirling are lambda functions
				timeMachinePart->runAction(Sequence::create(delay, moveTo, reachDelay, transform, transformDelay, jumpPlayer, jumpDelay, twirling, twirlingDelay, backToTitleScreen, NULL));
			}
		}
	}

	if (m_isTutorial)
	{
		if (!m_isTutorialHome)
		{
			//step manager
			if (m_step == 0)
			{
				if (m_character->getPositionX() >= 1350 && m_character->getPositionX() <= 1420)
				{
					nextStep();
				}
			}
			else if (m_step == 1)
			{
				if (m_character->getPositionX() >= 2025 && m_character->getPositionX() <= 2100)
				{
					nextStep();
				}
			}
			else if (m_step == 2)
			{
				if (m_character->getPositionX() >= 2750 && m_character->getPositionX() <= 2825)
				{
					nextStep();
				}
			}
			else if (m_step == 3)
			{
				if (m_character->getPositionX() >= 4415 && m_character->getPositionX() <= 4500)
				{
					nextStep();
				}
			}
			else if (m_step == 4)
			{
				if (m_character->getPositionX() >= 4820 && m_character->getPositionX() <= 4900)
				{
					nextStep();
				}
			}
			else if (m_step == 5)
			{
				if (m_character->getPositionX() >= 5480 && m_character->getPositionX() <= 5580)
				{
					nextStep();
				}
			}


			if (m_stepDone)
			{
				m_stepDone = false;

				auto visibleSize = Director::getInstance()->getVisibleSize();

				switch (m_step)
				{
					//first step
				case 0:
				{
					auto arrow = Sprite::create("img/ui/tutorialArrow.png");
					arrow->setRotation(90);
					arrow->setOpacity(230);
					arrow->setScale(0.35);
					arrow->setPosition(visibleSize.width / 2, visibleSize.height / 1.4);
					arrow->setName("HorizontalArrow");
					m_uiNode->addChild(arrow);

					auto delay = DelayTime::create(0.7);
					auto blink = Blink::create(3, 3);
					auto hide = Hide::create();
					arrow->runAction(Sequence::create(hide, delay, blink, hide, NULL));
					
					//チュートリアルで光らせるやつ
					auto square1 = Sprite::create("img/dpadCover.png");
					square1->setScale(1.6);
					square1->setOpacity(150);
					square1->setPosition(m_rightRect.getMidX(), m_rightRect.getMidY());
					square1->setName("RightCover");
					m_uiNode->addChild(square1);

					auto square2 = Sprite::create("img/dpadCover.png");
					square2->setScale(1.6);
					square2->setOpacity(150);
					square2->setPosition(m_leftRect.getMidX(), m_leftRect.getMidY());
					square2->setName("LeftCover");
					m_uiNode->addChild(square2);
					//////あとでジョイスティック用に変える
					
					auto delay2 = DelayTime::create(0.7);
					auto blink2 = Blink::create(3, 6);
					auto hide2 = Hide::create();
					square1->runAction(Sequence::create(hide2, delay2, blink2, hide2, NULL));

					auto delay3 = DelayTime::create(0.7);
					auto blink3 = Blink::create(3, 6);
					auto hide3 = Hide::create();
					square2->runAction(Sequence::create(hide3, delay3, blink3, hide3, NULL));
				}
				break;
				case 1:
				{
					auto circle = Sprite::create("img/ui/AttackCover.png");
					circle->setOpacity(255);
					circle->setPosition(m_jButtonRect.getMidX(), m_jButtonRect.getMidY());
					circle->setName("ButtonCover");
					circle->setScale(0.4);
					circle->setVisible(false);
					m_uiNode->addChild(circle);

					auto delay = DelayTime::create(0.7);
					auto blink = Blink::create(3, 3);
					auto hide = Hide::create();
					circle->runAction(Sequence::create(hide, blink, hide, NULL));
				}
				break;
				case 2:
				{
					auto arrow = static_cast<Sprite*>(m_uiNode->getChildByName("HorizontalArrow"));

					auto blink = Blink::create(3, 3);
					auto hide = Hide::create();
					arrow->runAction(Sequence::create(hide, blink, hide, NULL));

				}
				break;
				case 3:
				{
					auto arrowV = Sprite::create("img/ui/tutorialArrow.png");
					arrowV->setRotation(180);
					arrowV->setOpacity(230);
					arrowV->setScale(0.2);
					arrowV->setVisible(false);
					arrowV->setPosition(m_resourceVector[0]->getPositionX(), m_resourceVector[0]->getPositionY() + 100);
					arrowV->setName("VerticalArrow");
					m_gameNode->addChild(arrowV);

					auto blink = Blink::create(3, 3);
					auto hide = Hide::create();
					arrowV->runAction(Sequence::create(blink, NULL));

					auto circle = m_uiNode->getChildByName("ButtonCover");
					circle->setPosition(m_aButtonRect.getMidX(), m_aButtonRect.getMidY());

					auto blink2 = Blink::create(3, 3);
					circle->runAction(Sequence::create(blink2, NULL));

				}
				break;
				case 4:
				{
					auto arrowV = m_gameNode->getChildByName("VerticalArrow");
					Vec2 enginePos = m_gameNode->getChildByName("engine")->getPosition();
					arrowV->setPosition(enginePos.x, enginePos.y + 70);
					auto blink = Blink::create(3, 3);
					arrowV->runAction(Sequence::create(blink, NULL));

				}
				break;
				case 5:
				{
					auto circle = m_uiNode->getChildByName("ButtonCover");

					auto blink = Blink::create(3, 3);
					circle->runAction(blink);
				}
				break;
				case 6:
				{
					auto arrow = static_cast<Sprite*>(m_uiNode->getChildByName("HorizontalArrow"));

					auto blink = Blink::create(3, 3);
					arrow->runAction(blink);
				}
				break;
				default:
					break;
				}
			}
		}

		else
		{
			if (m_step == 0)
			{
				auto diff = m_npcVector[0]->getPositionX() - m_character->getPositionX();
				if (abs(diff) <= 300)
				{
					nextStep();
				}
			}
			else if (m_step == 1)
			{
				if (m_character->getPositionX() >= 1860 && m_character->getPositionX() <= 1950)
				{
					nextStep();
				}
			}
			else if (m_step == 2)
			{
				if (m_character->getPositionX() >= 2220 && m_character->getPositionX() <= 2330)
				{
					nextStep();
				}
			}

			if (m_stepDone)
			{
				m_stepDone = false;
				auto visibleSize = Director::getInstance()->getVisibleSize();

				switch (m_step)
				{
				case 0:
					break;
				case 1:
				{
					auto arrowV = Sprite::create("img/ui/tutorialArrow.png");
					arrowV->setRotation(180);
					arrowV->setOpacity(230);
					arrowV->setScale(0.2);
					arrowV->setVisible(false);
					arrowV->setPosition(m_npcVector[0]->getPositionX(), m_npcVector[0]->getPositionY() + 100);
					arrowV->setName("VerticalArrow");
					m_gameNode->addChild(arrowV);

					auto blink = Blink::create(2, 2);
					arrowV->runAction(Sequence::create(blink, NULL));
				}
				break;

				case 2:
				{
					auto arrow = Sprite::create("img/ui/tutorialArrow.png");
					arrow->setRotation(90);
					arrow->setOpacity(230);
					arrow->setScale(0.35);
					arrow->setPosition(visibleSize.width / 2, visibleSize.height / 1.4);
					arrow->setName("HorizontalArrow");
					m_uiNode->addChild(arrow);

					auto delay = DelayTime::create(0.7);
					auto blink = Blink::create(3, 3);
					auto hide = Hide::create();
					arrow->runAction(Sequence::create(blink, hide, NULL));
				}
				break;
				case 3:
				{
					auto arrowV = m_gameNode->getChildByName("VerticalArrow");
					Vec2 enginePos = m_gameNode->getChildByName("timeMachine")->getPosition();
					arrowV->setPosition(enginePos.x, enginePos.y + 100);
					auto blink = Blink::create(3, 3);
					arrowV->runAction(Sequence::create(blink, NULL));
				}
				break;
				default:

					break;
				}
			}
		}
	}

	//Collision with resource icons
	for (int i = 0; i < m_resourceIconVector.size(); ++i)
	{
		auto resourceIcon = m_resourceIconVector[i];
		if (m_character->getBoundingBox().intersectsRect(resourceIcon->getBoundingBox()))
		{
			resourceIcon->setVisible(false);
			bool noSlot = true;
			bool noSpace = false;
			int firstVoidSlot = -1;
			for (int j = 0; j < sizeof(m_inventorySlot) / sizeof(m_inventorySlot[0]); ++j)
			{
				//Put item in the same slot
				if (resourceIcon->getTag() == m_inventorySlot[j].itemType)
				{
					m_inventorySlot[j].itemAmount++;
					noSlot = false;
				}
				//Put item in new slot
				else
				{
					//if void slot
					if (m_inventorySlot[j].itemType == RESOURCE_NONE)
					{
						//we havent get the first void slot yet
						if (firstVoidSlot == -1)
						{
							//get the void slot
							firstVoidSlot = j;
						}
					}
				}
			}

			if (noSlot)
			{
				if (firstVoidSlot != -1)
				{
					m_inventorySlot[firstVoidSlot].itemType = resourceIcon->getTag();
					m_inventorySlot[firstVoidSlot].itemAmount++;
				}
				else
				{
					noSpace = true;
				}
			}

			if (!noSpace)
			{
				m_gameNode->removeChild(resourceIcon);
				m_resourceIconVector.erase(m_resourceIconVector.begin() + i);
			}

			CCLOG("amount in slot1: %d", m_inventorySlot[0].itemAmount);
			CCLOG("amount in slot2: %d", m_inventorySlot[1].itemAmount);
			CCLOG("amount in slot3: %d", m_inventorySlot[2].itemAmount);
			showItems();
		}
	}

	//Character Collision Check Points (x,y)
	//
	//	(MIN, MAX)		(MAX, MAX)
	//		0				1
	//      -----------------
	//		|				|
	//		|				|
	//		|				|
	//		|	CHARACTER	|
	//		|				|
	//		|				|
	//		|				|
	//      -----------------
	//		3				2
	//	(MIN, MIN)		(MAX,MIN)

	auto characterBB = m_character->getBoundingBox();
	auto borderToToeDiff = (characterBB.getMaxX() - characterBB.getMidX()) / 1.5;
	auto borderToMidDiff = (characterBB.getMaxY() - characterBB.getMidY()) / 2;
	Vec2 characterColCheckPoints[] = 
	{
		Vec2(characterBB.getMinX() + borderToToeDiff/2, characterBB.getMaxY()), //0 TOP_LEFT     <--
		Vec2(characterBB.getMaxX() - borderToToeDiff/2, characterBB.getMaxY()), //1 TOP_RIGHT    -->
		Vec2(characterBB.getMidX(), characterBB.getMaxY()), //2 TOP_MID      ---
		Vec2(characterBB.getMinX() + borderToToeDiff/2, characterBB.getMidY() + borderToMidDiff), //3 MID_LEFT1     <--
		Vec2(characterBB.getMaxX() - borderToToeDiff/2, characterBB.getMidY() + borderToMidDiff), //4 MID_RIGHT1    -->
		Vec2(characterBB.getMaxX() - borderToToeDiff, characterBB.getMinY()), //5 BOTTOM_RIGHT -->
		Vec2(characterBB.getMinX() + borderToToeDiff, characterBB.getMinY()), //6 BOTTOM_LEFT  <--
		Vec2(characterBB.getMidX(), characterBB.getMinY()),  //7 BOTTOM_MID   ---
		Vec2(characterBB.getMinX() + borderToToeDiff/2, characterBB.getMidY() - borderToMidDiff/2), //8 MID_LEFT2
		Vec2(characterBB.getMaxX() - borderToToeDiff/2, characterBB.getMidY() - borderToMidDiff/2), //9 MID_RIGHT2
	};

	Vec2 rightCheckPoints[] =
	{
		characterColCheckPoints[1],
		characterColCheckPoints[4],
		characterColCheckPoints[9],
		Vec2(characterColCheckPoints[5].x, characterColCheckPoints[5].y + 10)
	};

	Vec2 leftCheckPoints[] = 
	{
		characterColCheckPoints[0],
		characterColCheckPoints[3],
		characterColCheckPoints[8],
		Vec2(characterColCheckPoints[6].x, characterColCheckPoints[6].y + 10),
	};

	//if not stopped
	if (!m_standing)
	{
		//RIGHT Collision detection
		if (m_directionRight)
		{
			bool nulled = false;
			for (int i = 0; i < 4; ++i)
			{
				auto nextTile = convertToTilePosition(rightCheckPoints[i]);
				auto rightTile = m_collisionLayer->getTileAt(nextTile);
				if (rightTile != NULL)
				{
					if (m_character->getBoundingBox().intersectsRect(rightTile->getBoundingBox()))
					{
						m_speedX = 0;
						nulled = true;
						
						auto correctionDelta = rightCheckPoints[0].x - rightTile->getBoundingBox().getMinX();
						if (correctionDelta > 0 && correctionDelta < 20)
						{
							m_character->setPositionX(m_character->getPositionX() - correctionDelta);
						}
						
					}
				}
				else
				{
					if (m_speedX < (-m_accelerationX * 10))
					{
						m_speedX = -m_accelerationX * 10;
					}

					m_speedX += m_accelerationX;
					m_speedX = MIN(m_speedX, MAX_SPEED);
				}
			}

			if (nulled)
			{
				//correction
				m_speedX = 0;
			}
		}
		//LEFT Collision detection
		else
		{
			bool nulled = false;
			for (int i = 0; i < 4; ++i)
			{
				auto nextTile = convertToTilePosition(leftCheckPoints[i]);
				auto leftTile = m_collisionLayer->getTileAt(nextTile);
				if (leftTile != NULL)
				{
					if (m_character->getBoundingBox().intersectsRect(leftTile->getBoundingBox()))
					{
						m_speedX = 0;
						
						auto correctionDelta = leftTile->getBoundingBox().getMaxX() - leftCheckPoints[0].x;
						if ( correctionDelta > 0 && correctionDelta < 20)
						{
							m_character->setPositionX(m_character->getPositionX() + correctionDelta);
						}
						
						nulled = true;
					}
				}
				else
				{
					if (m_speedX >(m_accelerationX * 10))
					{
						m_speedX = m_accelerationX * 10;
					}
					m_speedX -= m_accelerationX;
					m_speedX = MAX(m_speedX, -MAX_SPEED);
				}
			}

			if (nulled)
			{
				//correction
				m_speedX = 0;

			}
		}
	}
	//if the player is standing
	else
	{
		//Add Frinction to stop player
		if (m_speedX > 0)
		{
			//friction only when not moving
			if((m_speedX - m_frictionX) > 0) 
			{
				m_speedX -= m_frictionX;
			}
			else 
			{
				m_speedX = 0;
			}
		}
		else if (m_speedX < 0)
		{
			//friction only when not moving
			if((m_speedX + m_frictionX) < 0)
			{
				m_speedX += m_frictionX;
			}
			else
			{
				m_speedX = 0;
			}
		}
	}

	//Bottom Collision detection
	for (int i = 0; i < 3; ++i)
	{
		auto nextTileBottom = convertToTilePosition(characterColCheckPoints[i + 5]); //i+5 = BOTTOM
		auto bottomTile = m_collisionLayer->getTileAt(nextTileBottom);
		if (bottomTile != NULL)
		{
			if (m_character->getBoundingBox().intersectsRect(bottomTile->getBoundingBox()))
			{
				auto correctionDelta = bottomTile->getBoundingBox().getMaxY() - m_character->getBoundingBox().getMinY();
				if (correctionDelta <= 10)
				{
					m_character->setPositionY(m_character->getPositionY() + correctionDelta);
					if (m_speedY < 0)
					{
						m_speedY = 0;
						if (!m_jButtonPressed)
						{
							m_onGround = true;
						}
					}
				}
			}
		}
	}

	//TOP Collision detection
	for (int i = 0; i < 3; ++i)
	{
		auto nextTileTop = convertToTilePosition(characterColCheckPoints[i]);
		auto upperTile = m_collisionLayer->getTileAt(nextTileTop);
		if (upperTile != NULL)
		{
			if (m_character->getBoundingBox().intersectsRect(upperTile->getBoundingBox()))
			{
				if (m_speedY > 0)
				{
					m_speedY = 0;
					m_jumpInstanced = false;

					//position correction
					auto correctionDelta = upperTile->getBoundingBox().getMinY() - m_character->getBoundingBox().getMaxY();
					if (correctionDelta < -6)
					{
						//m_speedY += correctionDelta / 5;
					}
				}
			}
		}
	}
	
	//-------------------------------
	//---------Jumping---------------
	//-------------------------------
	if (m_jButtonPressed)
	{
		if (m_onGround && !m_jumpInstanced)
		{
			m_onGround = false;
			m_speedY += m_jumpForce;
			m_jumpInstanced = true;
		}
		else if (m_jumpInstanced) //gradual jump
		{
			m_speedY += m_jumpForce*0.5;
			if (m_speedY > MAX_JUMP)
			{
				m_speedY = MAX_JUMP;
				m_jumpInstanced = false;
			}
		}
	}

	for (int i = 0; i < m_npcVector.size(); ++i)
	{
		if (m_character->getBoundingBox().intersectsRect(m_npcVector[i]->getBoundingBox()))
		{
			if (!m_isTalking)
			{
				m_npcStateVector[i].isTalking = true;
				m_talkingNPCIndex = i;
				m_talkChecker++;
			}
			
			m_isTalking = true;
		}
		else
		{
			if(m_npcStateVector[i].isTalking)
			{
				m_npcStateVector[i].isTalking = false;
				m_talkChecker--;
			}

			if (m_talkChecker == 0)
			{
				m_isTalking = false;
				removePopUp();
			}
		}
	}
	if (!m_onNext)
	{
		auto leftArrow = m_gameNode->getChildByName("LeftArrow");
		if (leftArrow)
		{
			if (m_character->getBoundingBox().intersectsRect(leftArrow->getBoundingBox()))
			{
				//to the left
				changeArea(false);
			}
		}
		auto rightArrow = m_gameNode->getChildByName("RightArrow");
		if (rightArrow)
		{
			if (m_character->getBoundingBox().intersectsRect(rightArrow->getBoundingBox()))
			{
				changeArea(true);
			}
		}
	}

	m_character->setPositionY(m_character->getPositionY() + m_speedY);
	m_character->setPositionX(m_character->getPositionX() + m_speedX);
}

void Control3::playerAnimationCache()
{
	auto walking = Animation::create();
	walking->addSpriteFrameWithFileName("img/sprites/player/walk1.png");
	walking->addSpriteFrameWithFileName("img/sprites/player/walk2.png");
	walking->addSpriteFrameWithFileName("img/sprites/player/walk3.png");
	walking->addSpriteFrameWithFileName("img/sprites/player/walk4.png");
	walking->setLoops(1);
	walking->setDelayPerUnit(0.1);

	auto idle = Animation::create();
	idle->addSpriteFrameWithFileName("img/sprites/player/idle1.png");
	idle->addSpriteFrameWithFileName("img/sprites/player/idle2.png");
	idle->addSpriteFrameWithFileName("img/sprites/player/idle3.png");
	idle->setLoops(1);
	idle->setDelayPerUnit(0.2);

	auto swing = Animation::create();
	swing->addSpriteFrameWithFileName("img/sprites/player/swing2.png");
	swing->addSpriteFrameWithFileName("img/sprites/player/swing1.png");
	swing->addSpriteFrameWithFileName("img/sprites/player/swing3.png");
	swing->addSpriteFrameWithFileName("img/sprites/player/swing4.png");
	swing->setLoops(1);
	swing->setDelayPerUnit(0.085);

	m_playerAnimCache = AnimationCache::getInstance();
	m_playerAnimCache->addAnimation(walking, "walking");
	m_playerAnimCache->addAnimation(idle, "idle");
	m_playerAnimCache->addAnimation(swing, "swing");
}

void Control3::changeArea(bool right)
{
	m_onNext = true;
	auto def = UserDefault::getInstance();
	auto area = def->getIntegerForKey("MapArea");
	
	if (right)
	{
		area++;
		/*
		if (area <= 13)
		{
			area++;
		}
		else
		{
			//area = 0;
		}
		*/
		def->setBoolForKey("FromRight", true);
	}
	else
	{
		area--;
		/*
		if (area >= 13)
		{
			area++;
		}
		else
		{
			//area = 0;
		}
		*/
		def->setBoolForKey("FromRight", false);
	}
	def->setIntegerForKey("MapArea", area);

	def->setIntegerForKey("inventorySlotType01", m_inventorySlot[0].itemType);
	def->setIntegerForKey("inventorySlotType02", m_inventorySlot[1].itemType);
	def->setIntegerForKey("inventorySlotType03", m_inventorySlot[2].itemType);
	def->setIntegerForKey("inventorySlotType04", m_inventorySlot[3].itemType);
	def->setIntegerForKey("inventorySlotType05", m_inventorySlot[4].itemType);
	def->setIntegerForKey("inventorySlotType06", m_inventorySlot[5].itemType);
	def->setIntegerForKey("inventorySlotType07", m_inventorySlot[6].itemType);
	def->setIntegerForKey("inventorySlotType08", m_inventorySlot[7].itemType);
	def->setIntegerForKey("inventorySlotType09", m_inventorySlot[8].itemType);
	def->setIntegerForKey("inventorySlotType10", m_inventorySlot[9].itemType);

	def->setIntegerForKey("inventorySlotAmount01", m_inventorySlot[0].itemAmount);
	def->setIntegerForKey("inventorySlotAmount02", m_inventorySlot[1].itemAmount);
	def->setIntegerForKey("inventorySlotAmount03", m_inventorySlot[2].itemAmount);
	def->setIntegerForKey("inventorySlotAmount04", m_inventorySlot[3].itemAmount);
	def->setIntegerForKey("inventorySlotAmount05", m_inventorySlot[4].itemAmount);
	def->setIntegerForKey("inventorySlotAmount06", m_inventorySlot[5].itemAmount);
	def->setIntegerForKey("inventorySlotAmount07", m_inventorySlot[6].itemAmount);
	def->setIntegerForKey("inventorySlotAmount08", m_inventorySlot[7].itemAmount);
	def->setIntegerForKey("inventorySlotAmount09", m_inventorySlot[8].itemAmount);
	def->setIntegerForKey("inventorySlotAmount10", m_inventorySlot[9].itemAmount);

	def->setIntegerForKey("hp", m_hp);

	def->flush();
	auto resetScene = CallFunc::create([this]() {this->resetScene(); });
	this->runAction(resetScene);
}

void Control3::resetScene()
{
	auto scene = Control3::createScene();
	Director::getInstance()->replaceScene(scene);
}

void Control3::finishActionInstance()
{
	resetPlayerActions();
	//m_character->runAction(RepeatForever::create(Animate::create(m_playerAnimCache->getAnimation("idle"))));
	m_actingInstanced = false;
	if (!m_actingWithEnemy)
	{
		if (m_actedResourceCount > 5)
		{
			if (m_actingResourceIndex > -1)
			{
				spreadResource();
				m_gameNode->removeChild(m_resourceVector[m_actingResourceIndex]);
				m_resourceVector.erase(m_resourceVector.begin() + m_actingResourceIndex);
				m_isActing = false;
			}
			m_actedResourceCount = 0;
		}
	}
	else
	{	
		
	}
	m_swinging = false;
	m_animationInstanced = false;
	m_attackOnce = false;
}

void Control3::spreadResource()
{
	float movingTime = 0.5f;

	int resourceTypes[5];

	auto name = m_resourceVector[m_actingResourceIndex]->getName();

	for (auto& item : resourceData::resourceSpread)
	{
		if (item.name == name)
		{
			resourceTypes[0] = item.icon1;
			resourceTypes[1] = item.icon2;
			resourceTypes[2] = item.icon3;
			resourceTypes[3] = item.icon4;
			resourceTypes[4] = item.icon5;
		}
	}

	Sprite* resIcon[5];
	for (int i = 0; i < 5; ++i)
	{
		if (resourceTypes[i] != 0)
		{
			resIcon[i] = Sprite::create(resourceData::resourceIconPath[resourceTypes[i]]);

			resIcon[i]->setPosition(m_resourceVector[m_actingResourceIndex]->getPosition());

			resIcon[i]->setTag(resourceTypes[i]);
			m_gameNode->addChild(resIcon[i]);
			m_resourceIconVector.push_back(resIcon[i]);

			auto xDistance = RandomHelper::random_int(-40, 40);
			auto moveX = MoveBy::create(movingTime, Vec2(xDistance, 0));

			auto yDistance = RandomHelper::random_int(0, 40);
			auto moveY1 = MoveBy::create(movingTime / 2, Vec2(0, yDistance));
			auto floor = m_resourceVector[m_actingResourceIndex]->getBoundingBox().getMinY();
			auto topPointToFloor = resIcon[i]->getPositionY() + yDistance - floor - 20;
			auto moveY2 = MoveBy::create(movingTime / 2, Vec2(0, abs(topPointToFloor) * -1));

			auto sequence = Sequence::create(moveY1, moveY2, NULL);
			auto spawn = Spawn::create(sequence, moveX, NULL);
			resIcon[i]->runAction(spawn);
		}
	}
}

void Control3::swingPlayer(int swingType)
{
	if (!m_swinging)
	{
		auto xMagnitude = 23;
		auto duration = 0.34f;
		resetPlayerActions();
		m_character->runAction(RepeatForever::create(Animate::create(m_playerAnimCache->getAnimation("swing"))));
		m_swinging = true;
		//Object to swing Initialization
		Sprite* toSwing = (Sprite*)m_gameNode->getChildByName("axe");
		switch (swingType)
		{
		case SWING_AXE:
			toSwing = (Sprite*)m_gameNode->getChildByName("axe");
			break;
		case SWING_SWORD:
			toSwing = (Sprite*)m_gameNode->getChildByName("sword");
			break;
		case SWING_PICK:
			toSwing = (Sprite*)m_gameNode->getChildByName("axe");
			break;
		default:
			toSwing = (Sprite*)m_gameNode->getChildByName("axe");
			break;
		};
		toSwing->setVisible(true);
		auto side = !m_character->isFlippedX();
		toSwing->setFlippedX(!side);
		auto intSide = side ? -1 : 1;
		toSwing->setRotation(20 * -intSide);
		toSwing->setPosition(m_character->getPositionX() + xMagnitude * intSide, m_character->getPositionY() + 39);

		//Object to swing animation
		auto moveX = Sequence::create(MoveBy::create(duration * 0.5f, Vec2(10 * intSide, 0)), MoveBy::create(duration*0.5f, Vec2(10 * -intSide, 0)), NULL);
		auto moveY = MoveBy::create(duration, Vec2(0, -71));
		auto rotate = RotateBy::create(duration, intSide * 100);
		auto func = CallFunc::create([toSwing]() {toSwing->setVisible(false); });
		auto fadeOut = Sequence::create(DelayTime::create(0.32f), func, NULL);
		auto spawn = Spawn::create(moveY, rotate, fadeOut, NULL);
		toSwing->runAction(spawn);
	}
}

void Control3::showItems()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int inventoryX = visibleSize.width/2 - 500 + 50;

	for (int i = 0; i < 10; ++i)
	{
		if (m_inventorySlot[i].itemType != 0)
		{
			bool firstTime = true;
			//for (int j = 0; j < m_itemsVector.size(); ++j)
			{
				if (i < m_itemsVector.size())
				{
					auto itemInList = m_itemsVector[i];
					if(itemInList->getTag() == m_inventorySlot[i].itemType)
					{
						firstTime = false;
						if (m_inventorySlot[i].itemAmount != 0)
						{
							std::ostringstream ostr;
							ostr << m_inventorySlot[i].itemAmount;
							auto newCounter = Label::createWithTTF(ostr.str(), "fonts/arial.ttf", 17);
							newCounter->setPositionX(inventoryX + 100 * i + 17);
							newCounter->setPositionY(m_inventoryY - 20);
							m_uiNode->removeChild(m_itemCountVector[i]);
							m_uiNode->addChild(newCounter);
							m_itemCountVector[i] = newCounter;
						}
						else
						{
							m_uiNode->removeChild(m_itemCountVector[i]);
							m_uiNode->removeChild(itemInList);
							m_itemsVector.erase(m_itemsVector.begin() + i);
							m_itemCountVector.erase(m_itemCountVector.begin() + i);
							m_inventorySlot[i].itemType = RESOURCE_NONE;
							bool wasChanged = false;
							for (int j = i + 1; j < 10 - i; ++j)
							{
								if (m_inventorySlot[j].itemType != RESOURCE_NONE)
								{
									m_inventorySlot[j-1] = m_inventorySlot[j];
									m_inventorySlot[j].itemAmount = 0;
									m_inventorySlot[j].itemType = RESOURCE_NONE;

									wasChanged = true;
								}
							}

							if (wasChanged)
							{
								for (int j = 0; j < m_itemsVector.size(); ++j)
								{
									m_uiNode->removeChild(m_itemsVector[j]);
									m_uiNode->removeChild(m_itemCountVector[j]);
								}
								//resets vectors
								m_itemsVector.clear();
								m_itemCountVector.clear();
								//recursively calls it self to update if there was a change
								showItems();
							}
						}
						
					}
				}
			}

			if (firstTime)
			{
				auto sprite = Sprite::create(resourceData::resourceIconPath[m_inventorySlot[i].itemType]);
				sprite->setPositionX(inventoryX + 100 * i);
				sprite->setPositionY(m_inventoryY);
				sprite->setTag(m_inventorySlot[i].itemType);
				m_uiNode->addChild(sprite);
				m_itemsVector.push_back(sprite);

				std::ostringstream ostr;
				ostr << m_inventorySlot[i].itemAmount;
				auto counter = Label::createWithTTF(ostr.str(), "fonts/arial.ttf", 17);
				counter->setPositionX(sprite->getPositionX() + 17);
				counter->setPositionY(sprite->getPositionY() - 20);
				m_uiNode->addChild(counter);
				m_itemCountVector.push_back(counter);
			}
		}	
	}
}

void Control3::updateHPBar()
{
	auto maxHP = 1000.0f;
	float hp = m_hp;
	float hpForScale = (hp / maxHP);
	m_hpBar->setScaleX(hpForScale);
}

void Control3::equipmentCache()
{
	auto axe = Sprite::create("img/ui/axe.png");
	axe->setFlippedX(true);
	axe->setVisible(false);
	axe->setName("axe");
	m_gameNode->addChild(axe);

	auto sword = Sprite::create("img/ui/sword.png");
	sword->setFlippedX(true);
	sword->setVisible(false);
	sword->setName("sword");
	m_gameNode->addChild(sword);
}

void Control3::npcAI()
{
	for (int i = 0; i < m_npcStateVector.size(); ++i)
	{
		if (m_npcStateVector[i].isBuilding)
		{
			if (m_npcStateVector[i].stateID != NPC_STATE_BUILDING)
			{
				m_npcStateVector[i].stateID = NPC_STATE_BUILDING;

				m_npcVector[i]->stopAllActions();
				m_npcVector[i]->setRotation(0);
				m_npcStateVector[i].firstTimeState = true;
			}
		}

		if (m_npcStateVector[i].isTalking && !m_npcStateVector[i].isBuilding)
		{
			m_npcStateVector[i].stateID = NPC_STATE_IDLE;

			m_npcVector[i]->stopAllActions();
			m_npcVector[i]->setRotation(0);

			popUp(m_npcVector[i]);
		}

		if (m_npcStateVector[i].stateID == NPC_STATE_WALKING)
		{
			if (m_npcStateVector[i].firstTimeState)
			{
				m_npcVector[i]->stopAllActions();
				m_npcVector[i]->setRotation(0);
				m_npcStateVector[i].stateDuration = cocos2d::RandomHelper::random_int(0, 2);
				m_npcStateVector[i].directionRight = cocos2d::RandomHelper::random_int(0, 1);
				m_npcStateVector[i].firstTimeState = false;
				npcWalk(m_npcStateVector[i].directionRight, m_npcVector[i]);
			}
			
			m_npcStateVector[i].timeCounter++;

			if ((m_npcStateVector[i].timeCounter / 60) >= m_npcStateVector[i].stateDuration)
			{
				m_npcStateVector[i].firstTimeState = true;
				m_npcStateVector[i].timeCounter = 0;
				m_npcStateVector[i].stateDuration = 0;
				m_npcStateVector[i].stateID = NPC_STATE_IDLE;
			}
		}
		else if (m_npcStateVector[i].stateID == NPC_STATE_IDLE)
		{
			if (m_npcStateVector[i].firstTimeState)
			{
				m_npcVector[i]->setRotation(0);
				m_npcVector[i]->stopAllActions();
				m_npcStateVector[i].stateDuration = cocos2d::RandomHelper::random_int(0, 5) + 1;
				m_npcStateVector[i].firstTimeState = false;
			}
			m_npcStateVector[i].timeCounter++;

			if ((m_npcStateVector[i].timeCounter / 60) >= m_npcStateVector[i].stateDuration)
			{
				m_npcStateVector[i].firstTimeState = true;
				m_npcStateVector[i].timeCounter = 0;
				m_npcStateVector[i].stateDuration = 0;
				m_npcStateVector[i].stateID = NPC_STATE_WALKING;
			}
		}
		else if (m_npcStateVector[i].stateID == NPC_STATE_BUILDING)
		{
			if (m_npcStateVector[i].firstTimeState)
			{
				m_npcStateVector[i].firstTimeState = false;
				auto buildingObject = m_tilemap->getObjectGroup("player")->getObject("Building");
				auto posX = buildingObject["x"].asFloat();
				auto diff = m_npcVector[i]->getPositionX() - posX;
				CCLOG("Diff: %f", diff);
				m_npcStateVector[i].directionRight = true;
				//Determine the npc direction.
				if (diff > 0)
				{
					m_npcStateVector[i].directionRight = false;
				}

				//(diff / 40) gives the time needed to travel to the destination, in seconds. Since NPCs move at 40 units per second.
				m_npcStateVector[i].stateDuration = abs(diff / 40.0f);
				npcWalk(m_npcStateVector[i].directionRight, m_npcVector[i]);
			}

			if (!m_npcStateVector[i].buildingDone)
			{
				m_npcStateVector[i].timeCounter++;

				if ((m_npcStateVector[i].timeCounter / 60) >= m_npcStateVector[i].stateDuration)
				{
					m_npcStateVector[i].buildingDone = true;
					m_npcVector[i]->stopAllActions();
					m_npcVector[i]->setRotation(0);
					m_npcStateVector[i].timeCounter = 0;
					m_npcStateVector[i].stateDuration = 0;

					auto fadeOut = FadeOut::create(1.5);

					m_npcVector[i]->runAction(fadeOut);
					createBuilding(m_npcVector[i]->getPositionX(), m_npcVector[i]->getBoundingBox().getMinY());
				}
			}
			
		}
	}
}

void Control3::spawnNPC(int npcNumber)
{
	for (int i = 0; i < npcNumber; ++i)
	{
		//Create a default state first
		NPCStates randomState;
		int maxType = NPC_TYPE_PREHISTORIC_MAX-1;
		int maxBuilding = buildingData::MAX_PREHISTORIC-1;
		randomState.npcType = cocos2d::RandomHelper::random_int(0, maxType);
		randomState.buildingNum1 = cocos2d::RandomHelper::random_int(1, maxBuilding);
		//WARNING: npcStateVector and npcVector has to share the same index!
		m_npcStateVector.push_back(randomState);
		m_npcVector.push_back(Sprite::create(npcTypePath[m_npcStateVector[i].npcType]));
		m_npcVector[i]->setPositionY(m_character->getPositionY() - 5);
		m_npcVector[i]->setPositionX(cocos2d::RandomHelper::random_int(0, 2800) + 1500);
		m_npcVector[i]->setScale(0.1272);
		m_gameNode->addChild(m_npcVector[i]);
	}
}

void Control3::receivePlayerDamage()
{
	auto damage = DamageHandler::GetInstance();
	m_hp -= damage->GetPlayerDamage();
	if (damage->GetPlayerDamage() != 0)
	{
		auto direction = 1;
		if (m_character->isFlippedX())
		{
			direction = -1;
		}

		//knock back
		m_speedX += 10 * direction;
		auto tintRed = TintTo::create(0.05, Color3B::RED);
		auto delay = DelayTime::create(0.1);
		auto tintBack = TintTo::create(0.05, Color3B::WHITE);
		m_character->runAction(Sequence::create(tintRed, delay, tintBack, NULL));
		CCLOG("knock back!");
	}
}

void Control3::resetPlayerActions()
{
	m_character->stopAllActions();
	m_character->setColor(Color3B::WHITE);
	m_character->setRotation(0);
}

void Control3::createBuilding(float posX, float npcfootY)
{
	auto building = Sprite::create(buildingData::buildingPath[m_npcStateVector[m_talkingNPCIndex].buildingNum1]);
	building->setScale(3.0f);
	building->setOpacity(0);
	m_gameNode->addChild(building);
	//+ (building->getContentSize().height * m_gameNode->getScale() * building->getScale())/2
	building->setPosition(Vec2(posX, npcfootY + ((building->getContentSize().height * m_gameNode->getScaleY() * building->getScaleY())/3)));

	auto anim = Animation::create();
	anim->addSpriteFrameWithFile("img/buildings/p_a01.png");
	anim->addSpriteFrameWithFile("img/buildings/p_a02.png");
	anim->addSpriteFrameWithFile("img/buildings/p_a03.png");
	anim->setDelayPerUnit(5.3f);
	anim->setLoops(1);

	auto fadeIn = FadeIn::create(1.0f);

	auto animAction = Animate::create(anim);
	building->runAction(animAction);
	building->runAction(fadeIn);
}

void Control3::update(float dt)
{
	if (!m_disableGame)
	{
		auto damage = DamageHandler::GetInstance();
		damage->ResetDamage();
		setViewpoint(m_character->getPosition());
		resourcePopup();
		simplePhysics();
		npcAI();

		auto sword = (Sprite*)m_gameNode->getChildByName("sword");
		sword->setPosition(m_character->getPosition());
		if (m_actingInstanced)
		{
			if (m_enemy.collideWith(sword->getBoundingBox()) && sword->isVisible() && !m_attackOnce)
			{
				auto damage = DamageHandler::GetInstance();
				damage->DealDamageToMonster(m_damage);
				m_attackOnce = true;
			}
		}


		//dmg calc
		//CCLOG("Monster damage: %i", damage->GetMonsterDamage());
		m_enemy.receiveDamage(false);
		receivePlayerDamage();

		updateHPBar();
	}
	
}


///タッチ開始
void Control3::onTouchesBegan(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (!m_disableGame)
	{
		for (auto &t : touch)
		{
			Rect touchPoint = Rect(t->getLocation().x, t->getLocation().y, 1, 1);
			if (touchPoint.intersectsRect(m_rightRect))
			{
				//if (!m_swinging)
				{
					walk(true, m_character);
					m_directionRight = true;
					m_standing = false;
					m_animationInstanced = false;
					m_moveTouchID = t->getID();
				}
			}
			else if (touchPoint.intersectsRect(m_leftRect))
			{
				//if (!m_swinging)
				{
					walk(false, m_character);
					m_directionRight = false;
					m_standing = false;
					m_animationInstanced = false;
					m_moveTouchID = t->getID();
				}
			}
			else if (touchPoint.intersectsRect(m_StopButton))
			{
				//if (!m_swinging)
				{
					m_standing = true;
					//m_animationInstanced = false;
					m_moveTouchID = t->getID();
				}
			}

			if (touchPoint.intersectsRect(m_jButtonRect))
			{
				m_jButtonPressed = true;
				m_jumpTouchID = t->getID();
			}

			if (touchPoint.intersectsRect(m_aButtonRect))
			{
				m_aButtonPressed = true;
				m_actionTouchID = t->getID();
			}

			auto icon = m_gameNode->getChildByName(HOUSE_ICON_SPRITE);
			if (icon != NULL)
			{
				if (icon->isVisible())
				{
					auto iconPos = m_gameNode->convertToWorldSpace(Vec2(icon->getBoundingBox().getMinX(), icon->getBoundingBox().getMinY()));
					auto iconRect = Rect(iconPos.x, iconPos.y, icon->getContentSize().width * icon->getScale() * m_gameNode->getScale(), icon->getContentSize().height * icon->getScale() * m_gameNode->getScale());

					if (touchPoint.intersectsRect(iconRect))
					{
						int fontSize = 14;
						int resSize = 1.2;
						int outline = 0;

						auto popup = m_gameNode->getChildByName(POPUP_SPRITE);
						icon->setPositionY(icon->getPositionY() + 11);

						int resNum = 1;
						icon->setVisible(false);
						int buildingNum = m_npcStateVector[m_talkingNPCIndex].buildingNum1;
						auto res1 = Sprite::create(buildingData::resourcePath[buildingData::buildingRecepe[buildingNum].resource1]);
						res1->setPosition(icon->getPosition());
						res1->setName(RESOURCE_1);
						res1->setScale(resSize);
						m_gameNode->addChild(res1);

						res1->setOpacity(150);
						auto fadeIn1 = FadeIn::create(0.3);
						res1->runAction(fadeIn1);

						std::ostringstream ostr;
						ostr << buildingData::buildingRecepe[buildingNum].res1Count;
						std::string count1 = "x" + ostr.str();
						ostr.str("");
						auto resCounter1 = Label::createWithTTF(count1, "fonts/arial.ttf", fontSize * m_gameNode->getScale());
						resCounter1->setColor(Color3B::BLACK);
						resCounter1->enableOutline(Color4B::BLACK, outline);
						resCounter1->setPosition(res1->getPositionX(), res1->getPositionY() - 30);
						resCounter1->setName(RESOURCE_COUNT_1);
						m_gameNode->addChild(resCounter1);

						Sprite* res2 = nullptr;
						Label* resCounter2 = nullptr;

						if (buildingData::buildingRecepe[buildingNum].resource2 != buildingData::BUILDING_NONE)
						{
							res2 = Sprite::create(buildingData::resourcePath[buildingData::buildingRecepe[buildingNum].resource2]);
							res2->setPosition(icon->getPositionX() - 60, icon->getPositionY());
							res2->setName(RESOURCE_2);
							res2->setScale(resSize);
							m_gameNode->addChild(res2);
							ostr << buildingData::buildingRecepe[buildingNum].res2Count;
							std::string count2 = "x" + ostr.str();
							ostr.str("");
							resCounter2 = Label::createWithTTF(count2, "fonts/arial.ttf", fontSize * m_gameNode->getScale());
							resCounter2->setColor(Color3B::BLACK);
							resCounter2->enableOutline(Color4B::BLACK, outline);
							resCounter2->setPosition(res2->getPositionX(), res2->getPositionY() - 30);
							resCounter2->setName(RESOURCE_COUNT_2);
							m_gameNode->addChild(resCounter2);

							res2->setOpacity(150);
							auto fadeIn2 = FadeIn::create(0.3);
							res2->runAction(fadeIn2);

							resNum++;

							if (buildingData::buildingRecepe[buildingNum].resource3 != buildingData::BUILDING_NONE)
							{
								auto res3 = Sprite::create(buildingData::resourcePath[buildingData::buildingRecepe[buildingNum].resource3]);
								res3->setPosition(icon->getPositionX() + 60, icon->getPositionY());
								res3->setName(RESOURCE_3);
								res3->setScale(resSize);
								m_gameNode->addChild(res3);
								ostr << buildingData::buildingRecepe[buildingNum].res3Count;
								std::string count3 = "x" + ostr.str();
								auto resCounter3 = Label::createWithTTF(count3, "fonts/arial.ttf", fontSize * m_gameNode->getScale());
								resCounter3->setColor(Color3B::BLACK);
								resCounter3->enableOutline(Color4B::BLACK, outline);
								resCounter3->setPosition(res3->getPositionX(), res3->getPositionY() - 30);
								resCounter3->setName(RESOURCE_COUNT_3);
								m_gameNode->addChild(resCounter3);

								res3->setOpacity(150);
								auto fadeIn3 = FadeIn::create(0.3);
								res3->runAction(fadeIn3);

								resNum++;
							}
						}

						switch (resNum)
						{
						case 1:
							break;
						case 2:
							res1->setPositionX(res1->getPositionX() + 40);
							resCounter1->setPositionX(res1->getPositionX());
							res2->setPositionX(res2->getPositionX() + 20);
							resCounter2->setPositionX(res2->getPositionX());
							break;
						case 3:
							break;
						default:
							break;
						}

						popup->setScaleX(popup->getScaleX() * resNum);
						popup->setScaleY(2);
					}
				}
			}

			auto buildingRes1 = m_gameNode->getChildByName(RESOURCE_1);
			if (buildingRes1)
			{
				if (buildingRes1->getOpacity() == 255 && m_npcStateVector[m_talkingNPCIndex].isTalking)
				{
					auto resPos = m_gameNode->convertToWorldSpace(Vec2(buildingRes1->getBoundingBox().getMinX(), buildingRes1->getBoundingBox().getMinY()));
					auto iconRect = Rect(resPos.x, resPos.y, buildingRes1->getContentSize().width * buildingRes1->getScale() * m_gameNode->getScale(), buildingRes1->getContentSize().height * buildingRes1->getScale() * m_gameNode->getScale());
					if (iconRect.intersectsRect(touchPoint))
					{
						int buildingNum = m_npcStateVector[m_talkingNPCIndex].buildingNum1;
						int resType = buildingData::buildingRecepe[buildingNum].resource1;

						for (int i = 0; i < 10; ++i)
						{
							if (m_inventorySlot[i].itemType == resType)
							{
								if (m_inventorySlot[i].itemAmount >= buildingData::buildingRecepe[buildingNum].res1Count)
								{
									m_inventorySlot[i].itemAmount -= buildingData::buildingRecepe[buildingNum].res1Count;
									showItems();
									m_npcStateVector[m_talkingNPCIndex].isBuilding = true;
									m_npcStateVector[m_talkingNPCIndex].isTalking = false;
									removePopUp();
								}
							}
						}


					}
				}
			}
		}
	}
}

void Control3::onTouchesMoved(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	

	if (!m_disableGame)
	{
		for (auto &t : touch)
		{
			auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
			
			Rect touchPoint = Rect(t->getLocation().x, t->getLocation().y, 2, 2);
			if (touchPoint.intersectsRect(m_rightRect))
			{
				//if (!m_directionRight)
				{
					//if (!m_swinging)
					{
						walk(true, m_character);
						m_directionRight = true;
						m_standing = false;
						m_animationInstanced = false;
						m_moveTouchID = t->getID();
						//ジョイスティックpos移動
						JoyStick->setPosition(visibleSize.width / 10 * 1.8, visibleSize.height / 6);
					}
				}
			}
			else if (touchPoint.intersectsRect(m_leftRect))
			{
				//if (m_directionRight)
				{
					//if (!m_swinging)
					{
						walk(false, m_character);
						m_directionRight = false;
						m_standing = false;
						m_animationInstanced = false;
						m_moveTouchID = t->getID();
						//ジョイスティックpos移動
						JoyStick->setPosition(visibleSize.width / 10 * 1.2, visibleSize.height / 6);

					}
				}
			}
			///ここにストップボタン追加
			else if (touchPoint.intersectsRect(m_StopButton))
			{
				m_standing = true;
				m_animationInstanced = false;
				m_moveTouchID = -1;
				//ジョイスティックpos移動
				JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
			}

		}
	}
}

void Control3::onTouchesEnded(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (!m_disableGame)
	{
		for (auto &t : touch)
		{
			auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
			if (t->getID() == m_moveTouchID)
			{
				m_standing = true;
				m_animationInstanced = false;
				m_moveTouchID = -1;
				JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
			}
			if (t->getID() == m_jumpTouchID)
			{
				m_jButtonPressed = false;
				m_jumpInstanced = false;
				m_jumpTouchID = -1;
			}
			if (t->getID() == m_actionTouchID)
			{
				m_aButtonPressed = false;
				m_actionTouchID = -1;
			}
		}
	}
}

void Control3::menuCloseCallback()
{
	auto scene = TitlleScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void Control3::nextStep()
{
	m_stepDone = true;
	m_step++;
}

#include "ControlTesting03.h"
#include "TitleScene.h"
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

	//physics variable initialization
	m_standing = true;
	m_onGround = false;
	m_jumpInstanced = false;
	m_speedX = 0;
	m_speedY = 0;
	m_accelerationX = 0.1;
	m_frictionX = 1;
	m_jumpForce = 1.5;

	//control init
	m_aButtonPressed = false;
	m_jButtonPressed = false;

	//node initialization
	m_gameNode = Node::create();
	m_uiNode = Node::create();

	//facing right by default
	m_directionRight = true;
	m_isTalking = false;
	m_isCreatedPopUp = false;
	m_isCreatedActionPopUp = false;
	m_onNext = false;
	m_isActing = false;
	m_actingWithEnemy = false;
	m_actingInstanced = false;

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
			m_tilemap = TMXTiledMap::create("img/tilemap/forest02.tmx");
		}
	}
	//left map
	else if (area < 0)
	{
		if (def->getIntegerForKey(mapData::leftMap[abs(area)].c_str()) == mapData::FOREST01)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/forest02.tmx");
		}
	}
	//home map
	else
	{
		m_tilemap = TMXTiledMap::create("img/tilemap/home.tmx");
	}

	m_gameNode->addChild(m_tilemap, -1);
	auto objectGroup = m_tilemap->getObjectGroup("player");
	ValueMap comeFrom;
	comeFrom = objectGroup->getObject("NextLeft");
	float leftArrowX = comeFrom["x"].asInt();
	float leftArrowY = comeFrom["y"].asInt();
	comeFrom = objectGroup->getObject("NextRight");
	float rightArrowX = comeFrom["x"].asInt();
	float rightArrowY = comeFrom["y"].asInt();

	/*
	if (area == mapData::HOME_MAP)
	{
		auto playerSpawnPoint = objectGroup->getObject("playerPoint");
		playerX = playerSpawnPoint["x"].asInt();
		playerY = playerSpawnPoint["y"].asInt();
	}
	*/
	
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

	m_collisionLayer = m_tilemap->getLayer("col");
	m_collisionLayer->setVisible(false);

	m_character = Sprite::create("img/sprites/characterBoxed.png");
	m_character->setScale(0.128);
	m_character->setZOrder(20);
	if (def->getBoolForKey("FromRight"))
	{
		m_character->setFlippedX(true);
	}

	m_character->setPosition(playerX + m_tilemap->getTileSize().width, playerY + m_tilemap->getTileSize().height);
	m_gameNode->addChild(m_character);
	if (area == mapData::HOME_MAP)
	{
		//spawn 4 npcs
		spawnNPC(4);
	}

	auto leftButton = Sprite::create("img/dpad.png");
	leftButton->setPosition(visibleSize.width / 10 * 1, visibleSize.height / 6);
	leftButton->setScale(1.5);
	leftButton->setRotation(-90);
	leftButton->setOpacity(100);
	m_uiNode->addChild(leftButton);

	auto rightButton = Sprite::create("img/dpad.png");
	rightButton->setPosition(visibleSize.width / 10 * 3, visibleSize.height / 6);
	rightButton->setScale(1.5);
	rightButton->setRotation(90);
	rightButton->setOpacity(100);
	m_uiNode->addChild(rightButton);

	auto jButton = Sprite::create("img/ui/j-button.png");
	jButton->setPosition(visibleSize.width / 10 * 7.3, visibleSize.height / 6);
	jButton->setOpacity(100);
	jButton->setScale(0.7);
	m_uiNode->addChild(jButton);

	auto aButton = Sprite::create("img/ui/a-button.png");
	aButton->setPosition(visibleSize.width / 10 * 9, visibleSize.height / 4);
	aButton->setOpacity(100);
	aButton->setScale(0.7);
	m_uiNode->addChild(aButton);

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Control3::menuCloseCallback, this));
	//closeItem->setScale(3);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getBoundingBox().getMaxX(),
		origin.y + closeItem->getBoundingBox().getMaxY()));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	m_uiNode->addChild(menu);

	m_rightRect = Rect(rightButton->getBoundingBox());
	m_leftRect = Rect(leftButton->getBoundingBox());
	m_jButtonRect = Rect(jButton->getBoundingBox());
	m_aButtonRect = Rect(aButton->getBoundingBox());

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(Control3::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Control3::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Control3::onTouchesEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	m_gameNode->setScale(1.5);

	//setViewpoint(m_character->getPosition());
	m_gameNode->setPosition(Vec2::ZERO);
	m_uiNode->setPosition(Vec2::ZERO);
	this->addChild(m_gameNode);
	this->addChild(m_uiNode);
	//mid day
	//bg = LayerGradient::create(Color4B(34, 98, 206, 255), Color4B(193, 205, 219, 255), Vec2(0, -1));
	//Dawn
	//bg = LayerGradient::create(Color4B(11, 40, 110, 255), Color4B(169, 110, 78, 255), Vec2(0, -1));
	
	auto bg = LayerGradient::create(Color4B(34, 98, 206, 255), Color4B(173, 240, 245, 255), Vec2(0, -1));
	this->addChild(bg, -50);

	setViewpoint(m_character->getPosition());
	showItems();
	int index = m_resourceVector.size();
	this->scheduleUpdate();

    return true;
}

void Control3::walk(bool directionRight, Sprite* subject)
{
	subject->stopAllActions();
	
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
	//subject->runAction(RepeatForever::create(sequence));
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
			house->setScale(1.2);
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

	for (int i = 0; i < m_resourceVector.size(); ++i)
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
						/*
						auto popup = Sprite::create("img/ui/axe.png");
						popup->setPosition(m_character->getPositionX(), m_character->getBoundingBox().getMaxY() + popup->getContentSize().height / 2);
						popup->setName("actionPopup");
						popup->setZOrder(2);
						m_gameNode->addChild(popup);
						*/
						m_isCreatedActionPopUp = true;
						m_isActing = true;
						m_actingWithEnemy = false;
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
	//----Gracity for character----
	m_speedY += GRAVITY;
	//-----------------------------

	//-------------------------------------------------------
	//------ACTIONS------------------------------------------
	//-------------------------------------------------------

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
						if (m_actedResourceCount <= 5)
						{
							m_actingInstanced = true;
							auto shake1 = RotateBy::create(0.05, 5);
							auto shake2 = RotateBy::create(0.05, -10);
							m_resourceVector[m_actingResourceIndex]->runAction(Sequence::create(shake1, shake2, shake1, endInstance, NULL));
							m_actedResourceCount++;
						}
						else
						{
							m_actingInstanced = true;
							auto shake1 = RotateBy::create(0.05, 10);
							auto shake2 = RotateBy::create(0.05, -20);
							auto fadeOut = FadeOut::create(0.1);
							auto delay = DelayTime::create(0.1);
							m_resourceVector[m_actingResourceIndex]->runAction(Sequence::create(shake1, shake2, shake1, fadeOut, delay, endInstance, NULL));
							
						}
					}
				}
			}
		}
	}

	//Collision with resources
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
						//firstVoidSlot = (firstVoidSlot == -1) ? i : firstVoidSlot;

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
						/*
						auto correctionDelta = rightCheckPoints[0].x - rightTile->getBoundingBox().getMinX();
						if (correctionDelta > 0 && correctionDelta < 20)
						{
							m_character->setPositionX(m_character->getPositionX() - correctionDelta);
						}
						*/
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
						/*
						auto correctionDelta = leftTile->getBoundingBox().getMaxX() - leftCheckPoints[0].x;
						if ( correctionDelta > 0 && correctionDelta < 20)
						{
							m_character->setPositionX(m_character->getPositionX() + correctionDelta);
						}
						*/
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
	m_actingInstanced = false;
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

void Control3::showItems()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int inventoryX = visibleSize.width/2 - 500 + 50;

	for (int i = 0; i < 10; ++i)
	{
		if (m_inventorySlot[i].itemType != 0)
		{
			bool firstTime = true;
			for (auto& itemInList : m_itemsVector)
			{
				if (itemInList->getTag() == m_inventorySlot[i].itemType)
				{
					firstTime = false;
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
			}
		}
		
	}

	/*
	int multiplyer = -1;
	for (int i = 0; i < 10; ++i)
	{
		if (m_inventorySlot[i].itemType != 0)
		{
			bool isCreated = false;
			for (auto& itemIn : m_itemsVector)
			{
				if (itemIn->getTag() == m_inventorySlot[i].itemType)
				{
					isCreated = true;
				}
			}
			
			if (!isCreated)
			{
				multiplyer++;
				auto item = Sprite::create(resourceData::resourceIconPath[m_inventorySlot[i].itemType]);
				item->setPositionX(inventoryX + 100 * multiplyer);
				item->setPositionY(m_inventoryY);
				item->setTag(m_inventorySlot[i].itemType);
				m_itemsVector.push_back(item);
				m_uiNode->addChild(item);
				
			}
			
		}
	}
	*/
}

void Control3::npcAI()
{
	for (int i = 0; i < m_npcStateVector.size(); ++i)
	{
		if (m_npcStateVector[i].isTalking)
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

void Control3::update(float dt)
{
	setViewpoint(m_character->getPosition());
	simplePhysics();
	npcAI();
	resourcePopup();
}

void Control3::onTouchesBegan(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (auto &t : touch)
	{
		Rect touchPoint = Rect(t->getLocation().x, t->getLocation().y, 1, 1);
		if (touchPoint.intersectsRect(m_rightRect))
		{
			walk(true, m_character);
			m_directionRight = true;
			m_standing = false;
			m_moveTouchID = t->getID();
		}
		else if (touchPoint.intersectsRect(m_leftRect))
		{
			walk(false, m_character);
			m_directionRight = false;
			m_standing = false;
			m_moveTouchID = t->getID();
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
	}
}

void Control3::onTouchesMoved(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (auto &t : touch)
	{
		Rect touchPoint = Rect(t->getLocation().x, t->getLocation().y, 2, 2);
		if (touchPoint.intersectsRect(m_rightRect))
		{
			if(!m_directionRight)
			{
				walk(true, m_character);
				m_directionRight = true;
				m_standing = false;
				m_moveTouchID = t->getID();
			}
		}
		else if (touchPoint.intersectsRect(m_leftRect))
		{
			if (m_directionRight)
			{
				walk(false, m_character);
				m_directionRight = false;
				m_standing = false;
				m_moveTouchID = t->getID();
			}
		}
	}
}

void Control3::onTouchesEnded(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	for (auto &t : touch)
	{
		if (t->getID() == m_moveTouchID)
		{
			m_character->stopAllActions();
			m_character->setRotation(0);
			m_standing = true;
			m_moveTouchID = -1;
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

void Control3::menuCloseCallback(Ref* pSender)
{
	auto scene = TitlleScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
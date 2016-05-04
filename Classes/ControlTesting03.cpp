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

	//node initialization
	m_gameNode = Node::create();
	m_uiNode = Node::create();

	//facing right by default
	m_directionRight = true;
	m_isTalking = false;
	m_isCreatedPopUp = false;
	m_onNext = false;

	auto def = UserDefault::getInstance();
	//Area system
	// <--- Left Map      HOME     Right Map --->
	// -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5
	int area = def->getIntegerForKey("MapArea");
	bool fromRight = def->getBoolForKey("FromRight");
	int playerX;
	int playerY;
	CCLOG("AREA: %d", area);
	//right map
	if (area > 0)
	{
		if (def->getIntegerForKey(mapData::rightMap[area].c_str()) == mapData::FOREST01)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/forest01.tmx");
		}
	}
	//left map
	else if (area < 0)
	{
		if (def->getIntegerForKey(mapData::leftMap[abs(area)].c_str()) == mapData::FOREST01)
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/forest01.tmx");
		}
	}
	//home map
	else
	{
		m_tilemap = TMXTiledMap::create("img/tilemap/test.tmx");
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
	//else
	//{
		if (fromRight)
		{
			playerX = leftArrowX + 42;
			playerY = leftArrowY;
		}
		else
		{
			playerX = rightArrowX - 42;
			playerY = rightArrowY;
		}
	//}

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

	m_collisionLayer = m_tilemap->getLayer("col");
	m_collisionLayer->setVisible(false);

	m_character = Sprite::create("img/sprites/character.png");
	m_character->setScale(0.064);
	m_character->setZOrder(20);
	if (def->getBoolForKey("FromRight"))
	{
		m_character->setFlippedX(true);
	}

	m_character->setPosition(playerX + m_tilemap->getTileSize().width / 2, playerY + m_tilemap->getTileSize().height / 2);
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

	auto aButton = Sprite::create("img/ui/a-button.png");
	aButton->setPosition(visibleSize.width / 10 * 8, visibleSize.height / 6);
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
	m_aButtonRect = Rect(aButton->getBoundingBox());

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(Control3::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Control3::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Control3::onTouchesEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	m_gameNode->setScale(3);

	//setViewpoint(m_character->getPosition());
	m_gameNode->setPosition(Vec2::ZERO);
	m_uiNode->setPosition(Vec2::ZERO);
	this->addChild(m_gameNode);
	this->addChild(m_uiNode);

	this->scheduleUpdate();

    return true;
}

void Control3::walk(bool directionRight, Sprite* subject)
{
	subject->stopAllActions();
	
	if (directionRight) //right
	{
		subject->setFlipX(true);
	}
	else //left
	{
		subject->setFlipX(false);
	}
	
	auto frame1 = RotateTo::create(0.1, -20);
	auto frame2 = RotateTo::create(0.1, 20);
	auto sequence = Sequence::create(frame1, frame2, NULL);
	subject->runAction(RepeatForever::create(sequence));
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
			popup->setScaleY(0.7);
			popup->setScaleX(0.4);
			popup->setOpacity(0);

			auto house = Sprite::create(buildingData::buildingPath[m_npcStateVector[m_talkingNPCIndex].buildingNum1]);
			house->setPosition(subject->getPositionX(), subject->getPositionY() + 3);
			house->setName(HOUSE_ICON_SPRITE);
			house->setScale(0.5);
			house->setOpacity(0);

			m_gameNode->addChild(popup);
			m_gameNode->addChild(house);

			auto moveUp = MoveBy::create(0.7f, Vec2(0.0f, 35));
			auto fadeIn = FadeIn::create(0.7f);
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
			auto moveDown = MoveBy::create(0.4f, Vec2(0, -30));
			auto fadeOut = FadeOut::create(0.4f);
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
		subject->setFlipX(true);
		subject->runAction(RepeatForever::create(moveRight));
	}
	else //left
	{
		subject->setFlipX(false);
		subject->runAction(RepeatForever::create(moveLeft));
	}

	auto frame1 = RotateTo::create(0.1, -20);
	auto frame2 = RotateTo::create(0.1, 20);
	auto sequence = Sequence::create(frame1, frame2, NULL);
	subject->runAction(RepeatForever::create(sequence));
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

	Vec2 characterColCheckPoints[4] = 
	{
		Vec2(m_character->getBoundingBox().getMinX(), m_character->getBoundingBox().getMaxY()), //0 TOP_LEFT
		Vec2(m_character->getBoundingBox().getMaxX(), m_character->getBoundingBox().getMaxY()), //1 TOP_RIGHT
		Vec2(m_character->getBoundingBox().getMaxX(), m_character->getBoundingBox().getMinY()), //2 BOTTOM_RIGHT
		Vec2(m_character->getBoundingBox().getMinX(), m_character->getBoundingBox().getMinY())  //3 BOTTOM_LEFT
	};

	enum COL_CHECKPOINTS{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT
	};

	if (!m_standing)
	{
		if (m_directionRight)
		{
			auto nextTile = convertToTilePosition(Vec2(m_character->getBoundingBox().getMaxX(), m_character->getPositionY()));
			if (m_collisionLayer->getTileAt(nextTile) == NULL)
			{
				if (m_speedX < (-m_accelerationX * 10))
				{
					m_speedX = -m_accelerationX * 10;
				}

				m_speedX += m_accelerationX;
				m_speedX = MIN(m_speedX, MAX_SPEED);
			}
			else
			{
				m_speedX = 0;
			}
		}
		else
		{
			auto nextTile = convertToTilePosition(Vec2(m_character->getBoundingBox().getMinX(), m_character->getPositionY()));
			if (m_collisionLayer->getTileAt(nextTile) == NULL)
			{
				if (m_speedX > (m_accelerationX * 10))
				{
					m_speedX = m_accelerationX * 10;
				}
				m_speedX -= m_accelerationX;
				m_speedX = MAX(m_speedX, -MAX_SPEED);
			}
			else
			{
				m_speedX = 0;
			}
		}
	}
	else
	{
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

	//up-down collision detection
	auto nextTileBottom = convertToTilePosition(Vec2(m_character->getBoundingBox().getMidX(), m_character->getBoundingBox().getMinY()));
	auto bottomTile = m_collisionLayer->getTileAt(nextTileBottom);
	if (bottomTile != NULL)
	{
		if(m_speedY < 0)
		{
			m_speedY = 0;
			if (!m_aButtonPressed)
			{
				m_onGround = true;
			}

			//position correction
			
			auto correctionDelta = bottomTile->getBoundingBox().getMaxY() - m_character->getBoundingBox().getMinY();
				//m_character->setPositionY(m_character->getPositionY() + correctionDelta);
			if (correctionDelta > 6)
			{
				m_speedY += correctionDelta / 5;
			}
		}
	}

	/*for (int i = 0; i < 2; ++i)
	{
		auto nextTileBottom = convertToTilePosition(characterColCheckPoints[i+2]); //i+2 = BOTTOM
		auto bottomTile = m_collisionLayer->getTileAt(nextTileBottom);
		if (bottomTile != NULL)
		{
			if (m_speedY < 0)
			{
				m_speedY = 0;
				if (!m_aButtonPressed)
				{
					m_onGround = true;
				}

				//position correction

				auto correctionDelta = bottomTile->getBoundingBox().getMaxY() - m_character->getBoundingBox().getMinY();
				//m_character->setPositionY(m_character->getPositionY() + correctionDelta);
				if (correctionDelta > 4)
				{
					m_speedY += correctionDelta / 5;
				}
			}
		}*/

	auto nextTileTop = convertToTilePosition(Vec2(m_character->getBoundingBox().getMidX(), m_character->getBoundingBox().getMaxY()));
	auto upperTile = m_collisionLayer->getTileAt(nextTileTop);
	if (m_collisionLayer->getTileAt(nextTileTop) != NULL)
	{
		if (m_speedY > 0)
		{
			m_speedY = 0;
			m_jumpInstanced = false;

			//position correction

			auto correctionDelta = upperTile->getBoundingBox().getMinY() - m_character->getBoundingBox().getMaxY();
			if (correctionDelta < -6)
			{
				m_speedY += correctionDelta / 5;
			}
		}
	}

	//-------------------------------
	//---------Jumping---------------
	//-------------------------------
	if (m_aButtonPressed)
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
		static int talkChecker = 0;

		if (m_character->getBoundingBox().intersectsRect(m_npcVector[i]->getBoundingBox()))
		{
			if (!m_isTalking)
			{
				m_npcStateVector[i].isTalking = true;
				m_talkingNPCIndex = i;
				talkChecker++;
			}
			
			m_isTalking = true;
		}
		else
		{
			if(m_npcStateVector[i].isTalking)
			{
				m_npcStateVector[i].isTalking = false;
				talkChecker--;
			}

			if (talkChecker == 0)
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
	CCLOG("New Area: %d", area);
	def->flush();
	auto resetScene = CallFunc::create([this]() {this->resetScene(); });
	this->runAction(resetScene);
}

void Control3::resetScene()
{
	auto scene = Control3::createScene();
	Director::getInstance()->replaceScene(scene);
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
		else
		{
			
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
		m_npcVector[i]->setPositionY(m_character->getPositionY());
		m_npcVector[i]->setPositionX(cocos2d::RandomHelper::random_int(0, 1200) + 200);
		m_npcVector[i]->setScale(0.106);
		m_gameNode->addChild(m_npcVector[i]);
	}
}

void Control3::update(float dt)
{
	setViewpoint(m_character->getPosition());
	simplePhysics();
	npcAI();
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
					auto popup = m_gameNode->getChildByName(POPUP_SPRITE);
					icon->setPositionY(icon->getPositionY() + 5);

					int resNum = 1;
					icon->setVisible(false);
					int buildingNum = m_npcStateVector[m_talkingNPCIndex].buildingNum1;
					auto res1 = Sprite::create(buildingData::resourcePath[buildingData::buildingRecepe[buildingNum].resource1]);
					res1->setPosition(icon->getPosition());
					res1->setName(RESOURCE_1);
					res1->setScale(0.5);
					m_gameNode->addChild(res1);
					std::ostringstream ostr;
					ostr << buildingData::buildingRecepe[buildingNum].res1Count;
					std::string count1 = "x" + ostr.str();
					ostr.str("");
					auto resCounter1 = LabelTTF::create(count1, "Arial", 3 * m_gameNode->getScale());
					resCounter1->setColor(Color3B::BLACK);
					resCounter1->setPosition(res1->getPositionX(), res1->getPositionY() - 15);
					resCounter1->setName(RESOURCE_COUNT_1);
					m_gameNode->addChild(resCounter1);

					Sprite* res2 = nullptr;
					LabelTTF* resCounter2 = nullptr;

					if (buildingData::buildingRecepe[buildingNum].resource2 != buildingData::BUILDING_NONE)
					{
						res2 = Sprite::create(buildingData::resourcePath[buildingData::buildingRecepe[buildingNum].resource2]);
						res2->setPosition(icon->getPositionX() - 33, icon->getPositionY());
						res2->setName(RESOURCE_2);
						res2->setScale(0.5);
						m_gameNode->addChild(res2);
						ostr << buildingData::buildingRecepe[buildingNum].res2Count;
						std::string count2 = "X" + ostr.str();
						ostr.str("");
						resCounter2 = LabelTTF::create(count2, "Arial", 3 * m_gameNode->getScale());
						resCounter2->setColor(Color3B::BLACK);
						resCounter2->setPosition(res2->getPositionX(), res2->getPositionY() - 15);
						resCounter2->setName(RESOURCE_COUNT_2);
						m_gameNode->addChild(resCounter2);

						resNum++;

						if (buildingData::buildingRecepe[buildingNum].resource3 != buildingData::BUILDING_NONE)
						{
							auto res3 = Sprite::create(buildingData::resourcePath[buildingData::buildingRecepe[buildingNum].resource3]);
							res3->setPosition(icon->getPositionX() + 33, icon->getPositionY());
							res3->setName(RESOURCE_3);
							res3->setScale(0.5);
							m_gameNode->addChild(res3);
							ostr << buildingData::buildingRecepe[buildingNum].res3Count;
							std::string count3 = "X" + ostr.str();
							auto resCounter3 = LabelTTF::create(count3, "Arial", 3 * m_gameNode->getScale());
							resCounter3->setColor(Color3B::BLACK);
							resCounter3->setPosition(res3->getPositionX(), res3->getPositionY() - 15);
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
						res1->setPositionX(res1->getPositionX() + 18);
						resCounter1->setPositionX(res1->getPositionX());
						res2->setPositionX(res2->getPositionX() + 15);
						resCounter2->setPositionX(res2->getPositionX());
						break;
					case 3:
						break;
					default:
						break;
					}

					popup->setScaleX(popup->getScaleX() * resNum);
					popup->setScaleY(1);
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
		else if (t->getID() == m_actionTouchID)
		{
			m_aButtonPressed = false;
			m_jumpInstanced = false;
			m_actionTouchID = -1;
		}
	}
}

void Control3::menuCloseCallback(Ref* pSender)
{
	auto scene = TitlleScene::createScene();
	Director::getInstance()->replaceScene(scene);
}


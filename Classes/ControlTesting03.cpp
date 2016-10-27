#include "ControlTesting03.h"
#include "TitleScene.h"
#include "DamageHandler.h"
#include "Option.h"
#include "Ending.h"
#include <sstream>

USING_NS_CC;
using namespace CocosDenshion;
using namespace ui;

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto def = UserDefault::getInstance();

	auto audioEngine = SimpleAudioEngine::getInstance();

	m_isTutorial = def->getBoolForKey("tutorial");

	m_stepDone = true;
	m_inTimeMachine = false;

	//initial time of the day
	m_timeOfDay = def->getIntegerForKey("timeOfDay");
	m_frameCounter = 0;

	m_isPopUpMsg = false;

	//physics variable initialization
	m_standing = true;
	m_onGround = false;
	m_jumpInstanced = false;
	m_isCrafting = false;
	m_jumpAnim = false;
	m_speedX = 0;
	m_speedY = 0;
	m_accelerationX = 0.1;
	m_frictionX = 1;
	m_jumpForce = 1.5f;
	m_damage = 10;
	m_step = 0;
	m_isCraftingTouch = false;
	
	//control init
	m_aButtonPressed = false;
	m_jButtonPressed = false;

	//node initialization
	m_gameNode = Node::create();
	m_uiNode = Node::create();
	m_temporaryNode = Node::create();
	m_backgroundNode = Node::create();
	m_skyNode = Node::create();
	m_foregroundNode = Node::create();

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

	m_fromContract = false;

	m_craftingResource1 = 0;
	m_craftingResource2 = 0;

	m_actedResourceCount = 0;

	m_talkChecker = 0;

	int area = def->getIntegerForKey("MapArea");
	m_age = def->getIntegerForKey("AgeNumber");
	bool fromRight = def->getBoolForKey("FromRight");
	m_contractState = def->getIntegerForKey("contractState");
	m_contractTime = def->getIntegerForKey("contractCount");
	m_contractLevel = def->getIntegerForKey("contractLevel");
	m_contractArea = def->getIntegerForKey("contractArea");
	m_contractCollect.push_back(PlayerInventory(def->getIntegerForKey("collectSlot1"), def->getIntegerForKey("collectSlotCount1")));
	m_contractCollect.push_back(PlayerInventory(def->getIntegerForKey("collectSlot2"), def->getIntegerForKey("collectSlotCount2")));
	m_contractCollect.push_back(PlayerInventory(def->getIntegerForKey("collectSlot3"), def->getIntegerForKey("collectSlotCount3")));
	m_contractCollect.push_back(PlayerInventory(def->getIntegerForKey("collectSlot4"), def->getIntegerForKey("collectSlotCount4")));
	m_contractCollect.push_back(PlayerInventory(def->getIntegerForKey("collectSlot5"), def->getIntegerForKey("collectSlotCount5")));

	m_trueEnding = def->getIntegerForKey("trueEnding");
	m_happyEnding = def->getIntegerForKey("happyEnding");
	m_goodEnding = def->getIntegerForKey("goodEnding");
	m_badEnding = def->getIntegerForKey("badEnding");
	m_worstEnding = def->getIntegerForKey("worstEnding");

	int playerX;
	int playerY;

	std::string bgmFileName;
	std::string footstepSFXPath;

	audioEngine->preloadEffect("sound/sfx/main/resource/eating_sound.mp3");
	audioEngine->preloadEffect("sound/sfx/main/resource/pon.mp3");

	switch (area)
	{
	case mapData::HOME_MAP:
	{
		bgmFileName = "sound/bgm/town.mp3";
		footstepSFXPath = "sound/sfx/main/walking/forest_steps.mp3";
		audioEngine->preloadEffect("sound/sfx/main/building/completed.mp3");
		audioEngine->preloadEffect("sound/sfx/main/building/nokogiri.mp3");
		audioEngine->preloadEffect("sound/sfx/main/building/tonkachi.mp3");

		if (m_isTutorial)
		{
			m_isTutorialHome = true;
			m_tilemap = TMXTiledMap::create("img/tilemap/tutorial2.tmx");
		}
		else
		{
			m_tilemap = TMXTiledMap::create("img/tilemap/home.tmx");
		}
	}
		break;

	case mapData::CAVE01:
	case mapData::CAVE02:
	case mapData::DESERT02:
	{
		bgmFileName = "sound/bgm/cave.mp3";
		footstepSFXPath = "sound/sfx/main/walking/cave_steps.mp3";
		audioEngine->preloadBackgroundMusic(bgmFileName.c_str());
		audioEngine->setBackgroundMusicVolume(def->getFloatForKey("volume", 1));
		m_tilemap = TMXTiledMap::create(mapData::mapPath[area]);
	}
	break;

	case mapData::FOREST01:
	{
		bgmFileName = "sound/bgm/forest.mp3";
		footstepSFXPath = "sound/sfx/main/walking/forest_steps.mp3";
		audioEngine->preloadBackgroundMusic(bgmFileName.c_str());
		audioEngine->setBackgroundMusicVolume(def->getFloatForKey("volume", 1));
		m_tilemap = TMXTiledMap::create(mapData::mapPath[area]);
	}
	break;

	case mapData::MONTAIN01:
	case mapData::MONTAIN02:
	case mapData::MONTAIN03:
	case mapData::FOREST02:
	{
		bgmFileName = "sound/bgm/forest.mp3";
		footstepSFXPath = "sound/sfx/main/walking/forest_steps.mp3";
		audioEngine->preloadBackgroundMusic(bgmFileName.c_str());
		audioEngine->setBackgroundMusicVolume(def->getFloatForKey("volume", 1));
		m_tilemap = TMXTiledMap::create(mapData::mapPath[area]);
	}
	break;

	case mapData::DESERT01:
	case mapData::OASIS01:
	{
		bgmFileName = "sound/bgm/desert.mp3";
		footstepSFXPath = "sound/sfx/main/walking/sandy_steps.mp3";
		audioEngine->preloadBackgroundMusic(bgmFileName.c_str());
		audioEngine->setBackgroundMusicVolume(def->getFloatForKey("volume", 1));
		m_tilemap = TMXTiledMap::create(mapData::mapPath[area]);
	}
	break;

	case mapData::SWAMP02:
	case mapData::SWAMP01:
	{
		bgmFileName = "sound/bgm/swamp.mp3";
		footstepSFXPath = "sound/sfx/main/walking/forest_steps.mp3";
		audioEngine->preloadBackgroundMusic(bgmFileName.c_str());
		audioEngine->setBackgroundMusicVolume(def->getFloatForKey("volume", 1));
		m_tilemap = TMXTiledMap::create(mapData::mapPath[area]);
	}
	break;

	default:
	{
		
		m_tilemap = TMXTiledMap::create(mapData::mapPath[area]);
	}
		break;
	}

	audioEngine->playBackgroundMusic(bgmFileName.c_str(), true);
	audioEngine->stopAllEffects();

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

	//-------------------------------------------
	//building slot init
	auto buildingStartPoint = objectGroup->getObject("BuildingRangeStart")["x"].asFloat();
	auto buildingEndPoint = objectGroup->getObject("BuildingRangeEnd")["x"].asFloat();

	m_buildingStart = buildingStartPoint;
	m_buildingEnd = buildingEndPoint;

	auto buildingAreaLenght = abs(buildingStartPoint - buildingEndPoint);
	m_slotSize = 70;

	m_buildingSlots = floor(buildingAreaLenght / m_slotSize);

	for (int i = 0; i < m_buildingSlots; ++i)
	{
		m_buildingsVector.push_back(BuildingSlot());
	}

	m_groundLevel = objectGroup->getObject("GroundLevel")["y"].asFloat();

	if (def->getBoolForKey("FromMenu"))
	{
		ValueMap spawnBox = objectGroup->getObject("Spawn");
		if (spawnBox["x"].asInt() != 0)
		{
			playerX = spawnBox["x"].asInt();
			playerY = spawnBox["y"].asInt();
		}
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
			if (item.asValueMap()["name"].asString() == "tree1")
			{
				auto sprite = Sprite::create("img/resources/tree01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			
			else if (item.asValueMap()["name"].asString() == "tree2")
			{
				auto sprite = Sprite::create("img/resources/tree02.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}

			else if (item.asValueMap()["name"].asString() == "tree3")
			{
				auto sprite = Sprite::create("img/resources/tree04.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "tree4")
			{
				auto sprite = Sprite::create("img/resources/tree05.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}

			else if (item.asValueMap()["name"].asString() == "tree5")
			{
				auto sprite = Sprite::create("img/resources/tree06.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}

			else if (item.asValueMap()["name"].asString() == "tree6")
			{
				auto sprite = Sprite::create("img/resources/tree07.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}

			else if (item.asValueMap()["name"].asString() == "tree7")
			{
				auto sprite = Sprite::create("img/resources/tree08.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}

			else if (item.asValueMap()["name"].asString() == "tree8")
			{
				auto sprite = Sprite::create("img/resources/tree09.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}

			else if (item.asValueMap()["name"].asString() == "tree9")
			{
				auto sprite = Sprite::create("img/resources/tree10.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}

			else if (item.asValueMap()["name"].asString() == "tree10")
			{
				auto sprite = Sprite::create("img/resources/tree11.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("tree");
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
				auto sprite = Sprite::create("img/timeMachine/TimeMachinePart1.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName(item.asValueMap()["name"].asString());
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "antena")
			{
				if (!def->getBoolForKey("HasTimeMachinePart2"))
				{
					auto sprite = Sprite::create("img/timeMachine/TimeMachinePart2.png");
					sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
					sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
					sprite->setName(item.asValueMap()["name"].asString());
					m_gameNode->addChild(sprite);
					m_resourceVector.push_back(sprite);
				}
			}
			else if (item.asValueMap()["name"].asString() == "monitor")
			{
				if (!def->getBoolForKey("HasTimeMachinePart3"))
				{
					auto sprite = Sprite::create("img/timeMachine/TimeMachinePart3.png");
					sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
					sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
					sprite->setName(item.asValueMap()["name"].asString());
					m_gameNode->addChild(sprite);
					m_resourceVector.push_back(sprite);
				}
				
			}
			else if (item.asValueMap()["name"].asString() == "cactus01")
			{
				auto sprite = Sprite::create("img/resources/cactus01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("cactus");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "cactus02")
			{
				auto sprite = Sprite::create("img/resources/cactus02.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("cactus");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "cactus03")
			{
				auto sprite = Sprite::create("img/resources/cactus03.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("cactus");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "cactus04")
			{
				auto sprite = Sprite::create("img/resources/cactus04.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("cactus");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "stone1")
			{
				auto sprite = Sprite::create("img/resources/stone01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("stone1");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "stone2")
			{
				auto sprite = Sprite::create("img/resources/stone02.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("stone2");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "steel1")
			{
				auto sprite = Sprite::create("img/resources/steel01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("steel");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "carbon1")
			{
				auto sprite = Sprite::create("img/resources/carbon01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("carbon");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "straw1")
			{
				auto sprite = Sprite::create("img/resources/straw01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("straw");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "mush1")
			{
				auto sprite = Sprite::create("img/resources/mush01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("mush1");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "mush2")
			{
				auto sprite = Sprite::create("img/resources/mush02.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("mush2");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "mush3")
			{
				auto sprite = Sprite::create("img/resources/mush01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("mush1");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "emerald")
			{
				auto sprite = Sprite::create("img/resources/emerald.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("emerald");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "nendo")
			{
				auto sprite = Sprite::create("img/resources/nendo.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("nendo");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "sand")
			{
				auto sprite = Sprite::create("img/resources/sand01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("sand");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "sharp")
			{
				auto sprite = Sprite::create("img/resources/sharp.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("sharp");
				m_gameNode->addChild(sprite);
				m_resourceVector.push_back(sprite);
			}
			else if (item.asValueMap()["name"].asString() == "stick")
			{
				auto sprite = Sprite::create("img/resources/stick01.png");
				sprite->setPositionX(item.asValueMap()["x"].asInt() + sprite->getContentSize().width / 2);
				sprite->setPositionY(item.asValueMap()["y"].asInt() + sprite->getContentSize().height / 2);
				sprite->setName("stick");
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

	// init inventory----------------------------------------------------------
	m_inventoryY = visibleSize.height / 1.1;
	m_inventoryX = visibleSize.width / 2 - 500 + 50;
	auto inventoryFrame = Sprite::create("img/ui/inventory.png");
	//inventoryFrame->setOpacity(100);
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

	//init gold
	m_gold = def->getIntegerForKey("gold");

	m_hp = def->getIntegerForKey("hp");

	m_collisionLayer = m_tilemap->getLayer("col");
	m_collisionLayer->setVisible(false);


	m_character = Sprite::create("img/sprites/player/newCharacter/idle1.png");
	m_character->setScale(1.024f * 0.4216f);
	m_character->setZOrder(20);
	if (def->getBoolForKey("FromRight"))
	{
		m_character->setFlippedX(true);
	}

	m_character->setPosition(playerX + m_tilemap->getTileSize().width, playerY + m_tilemap->getTileSize().height);
	m_gameNode->addChild(m_character);

	playerAnimationCache();
	equipmentCache();

	m_character->runAction(RepeatForever::create(Animate::create(m_playerAnimCache->getAnimation("idle"))))->setTag(1);

	if (area == mapData::HOME_MAP && !m_isTutorial)
	{
		//spawn 4 npcs
		spawnNPC(4);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	_ButtonSwap = def->getBoolForKey("ButtonSwap");
	//‚±‚±ƒXƒeƒBƒbƒN‚É
	auto leftButton = Sprite::create("img/dpad.png");
	leftButton->setPosition(visibleSize.width / 10 * 0.75, visibleSize.height / 6);
	leftButton->setScaleX(6.5);
	
	leftButton->setRotation(-90);
	leftButton->setOpacity(0);
	m_uiNode->addChild(leftButton);

	//ˆ—‚Í^‚ñ’†‚ÉƒXƒgƒbƒvƒ{ƒ^ƒ“‚ð‚Â‚¯‚Ä‰æ‘œ‚ÍƒXƒeƒBƒbƒN‚É‚·‚ê‚Î‚»‚ê‚Á‚Û‚­‚È‚éB
	auto StickBack = Sprite::create("img/ui/pad2.png");
	StickBack->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
	StickBack->setScale(1.5);
	StickBack->setOpacity(def->getFloatForKey("button"));
	StickBack->setName("stickBack");
	m_uiNode->addChild(StickBack);

	auto JoyStick = Sprite::create("img/ui/stick.png");
	JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
	JoyStick->setScale(1.5);
	JoyStick->setOpacity(def->getFloatForKey("button"));
	JoyStick->setTag(JOYSTICK_SPRITE_NUM);
	JoyStick->setName("joystick");
	m_uiNode->addChild(JoyStick);
	//ƒXƒgƒbƒvƒ{ƒ^ƒ“
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
	jButton->setName("jButton");
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
	aButton->setName("aButton");
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
	m_gameNode->setScale(1.5f);

	m_gameNode->setPosition(Vec2::ZERO);
	m_uiNode->setPosition(Vec2::ZERO);
	m_temporaryNode->setPosition(Vec2::ZERO);
	m_backgroundNode->setPosition(Vec2::ZERO);
	m_skyNode->setPosition(Vec2::ZERO);
	this->addChild(m_gameNode);
	this->addChild(m_uiNode, 10);
	this->addChild(m_backgroundNode, -10);
	this->addChild(m_skyNode, -30);
	this->addChild(m_foregroundNode, 20);
	this->addChild(m_temporaryNode);

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

	if (area == mapData::CAVE01 || area == mapData::CAVE02)
	{
		m_bgEnabled = false;
	}
	else
	{
		m_bgEnabled = true;
	}

	if (m_bgEnabled)
	{
		auto backgroundGroup = m_tilemap->getObjectGroup("bg");

		auto cloudLimitX = 0;
		cloudLimitX = backgroundGroup->getObject("screenLimit")["x"].asFloat();

		//Background
		auto cloudLevel = RandomHelper::random_int(mapData::minLevel[area], mapData::maxLevel[area]);
		auto weatherType = mapData::weatherType[area];
		m_background.Init(m_character->getPosition(), m_timeOfDay, visibleSize, m_gameNode, MAX_TIME_DAY, cloudLevel, cloudLimitX + 200, weatherType);
		m_background.SetCloudLevelRange(mapData::minLevel[area], mapData::maxLevel[area]);

		if (backgroundGroup)
		{
			auto backgroundVector = backgroundGroup->getObjects();
			for (auto &bg : backgroundVector)
			{
				if (bg.asValueMap()["name"].asString() == "forest01")
				{
					auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
					m_background.AddParallaxLayer("img/bg/forest_bg01.png", 0.82f, bgPos, m_gameNode->getScale(), 2);
				}
				else if (bg.asValueMap()["name"].asString() == "forest02")
				{
					auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
					m_background.AddParallaxLayer("img/bg/forest_bg02.png", 0.91f, bgPos, m_gameNode->getScale(), 1);
				}
				else if (bg.asValueMap()["name"].asString() == "montain01")
				{
					auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
					m_background.AddParallaxLayer("img/bg/staticBG_forest01.png", 0.2f, bgPos, m_gameNode->getScale() * 1.4f, m_background.DEPTH_LEVEL_BACKGROUND);
				}
				else if (bg.asValueMap()["name"].asString() == "montain02")
				{
					auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
					m_background.AddParallaxLayer("img/bg/staticBG_forest02.png", 0.11f, bgPos, m_gameNode->getScale() * 1.2f, m_background.DEPTH_LEVEL_BACKGROUND + 1);
				}
				else if (bg.asValueMap()["name"].asString() == "montain03")
				{
					auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
					m_background.AddParallaxLayer("img/bg/staticBG_forest03.png", 0.015f, bgPos, m_gameNode->getScale() * 1.0f, m_background.DEPTH_LEVEL_BACKGROUND + 2);
				}
				else if (bg.asValueMap()["name"].asString() == "desert01")
				{
					auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
					m_background.AddParallaxLayer("img/bg/staticBG_desert01.png", 0.115f, bgPos, m_gameNode->getScale() * 1.0f, m_background.DEPTH_LEVEL_BACKGROUND);
				}
				else if (bg.asValueMap()["name"].asString() == "desert02")
				{
					auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
					m_background.AddParallaxLayer("img/bg/staticBG_desert02.png", 0.015f, bgPos, m_gameNode->getScale() * 1.2f, m_background.DEPTH_LEVEL_BACKGROUND + 1);
				}
			}
		}

		m_backgroundNode->addChild(m_background.GetBackgroundNode());
		m_skyNode->addChild(m_background.GetSkyNode());
		m_foregroundNode->addChild(m_background.GetFrontNode());
		m_foregroundNode->addChild(m_background.GetShadeSprite());
	}
	

	m_effects = new Effect();
	//For healing and dmg effects
	m_foregroundNode->addChild(m_effects->GetColorScreen(), -100);

	setViewpoint(m_character->getPosition());
	m_hpBar = Sprite::create("img/ui/hpBar.png");
	m_hpBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_hpBar->setPosition(inventoryFrame->getBoundingBox().getMinX(), m_inventoryY - 60);
	updateHPBar();
	m_uiNode->addChild(m_hpBar);
	/*
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
	*/
	showItems(m_inventoryX, m_inventoryY);
	int index = m_resourceVector.size();
	this->scheduleUpdate();

	if (!def->getBoolForKey("firstTimeTutorial"))
	{
		auto fadeIn = FadeIn::create(1.6f);
		auto ui = (Sprite*)m_uiNode;
		ui->setCascadeOpacityEnabled(true);
		ui->setOpacity(0);
		ui->runAction(fadeIn);
	}

	m_isUIEnabled = true;
	m_isMoveUIEnabled = true;
	m_isJumpUIEnabled = true;
	m_isItemUIEnabled = true;
	m_isActionUIEnabled = true;

	if (m_isTutorialHome)
	{
		//Spawn 1 npc
		if (m_age == 0)
		{
			m_accesibleAreas.clear();
			auto randomArea = RandomHelper::random_int(2, 10);
			m_accesibleAreas.push_back(randomArea);

			int nextRandom = RandomHelper::random_int(2, 10);
			while (nextRandom == randomArea)
			{
				nextRandom = RandomHelper::random_int(2, 10);
			}

			auto partAreaProbability = RandomHelper::random_int(0, 100);
			if (partAreaProbability > 90)
			{
				m_accesibleAreas.push_back(11);
			}

			m_accesibleAreas.push_back(nextRandom);
			
			// NPC Spawn----------------------------------------------
			// Spawn a girl first
			if (!def->getBoolForKey("HasBuiltBase"))
			{
				//Create a default state first
				NPCStates randomState;
				int maxType = NPC_TYPE_PREHISTORIC_MAX - 1;
				int maxBuilding = buildingData::MAX_PREHISTORIC - 1;
				randomState.npcType = NPC_TYPE_CAVE_GIRL3; //cocos2d::RandomHelper::random_int(0, maxType);
				randomState.buildingNum1 = buildingData::BASE; //cocos2d::RandomHelper::random_int(1, 5);
															   //WARNING: npcStateVector and npcVector has to share the same index!
				m_npcStateVector.push_back(randomState);
				m_npcVector.push_back(Sprite::create(npcTypePath[m_npcStateVector[0].npcType]));
				ValueMap spawnBox = objectGroup->getObject("Spawn");
				float npcY;
				if (spawnBox["y"].asInt() != 0)
				{
					npcY = spawnBox["y"].asFloat();
				}
				m_npcVector[0]->setScale(0.636f);
				m_npcVector[0]->setZOrder(1);
				m_npcVector[0]->setPositionY(m_groundLevel + (abs(m_npcVector[0]->getBoundingBox().getMinY() - m_npcVector[0]->getBoundingBox().getMaxY()) / 2));
				m_npcVector[0]->setPositionX(cocos2d::RandomHelper::random_int(0, 300) + 1850);
				m_gameNode->addChild(m_npcVector[0]);
			}
			else
			{
				spawnNPC(3);
			}

			// Time Machine-----------------------------------
			/*
			m_buildingsVector[m_buildingSlots - 2].buildingType = buildingData::TIME_MACHINE01;
			m_buildingsVector[m_buildingSlots - 2].constructable = false;
			m_buildingsVector[m_buildingSlots - 3].constructable = false;
			*/
			/*
			auto timeMachine = Sprite::create("img/timeMachine/TimeMachine1.png");
			auto timeMachineObject = objectGroup->getObject("TimeMachine");
			auto timeMachineX = timeMachineObject["x"].asFloat();
			auto timeMachineY = timeMachineObject["y"].asFloat();
			timeMachine->setPosition(timeMachineX, timeMachineY);
			timeMachine->setName("timeMachine");
			timeMachine->setScale(2.5);
			m_gameNode->addChild(timeMachine);
			*/

			// Building----------------------------------------
			/*
			m_buildingsVector[m_buildingSlots / 2].buildingType = buildingData::HOUSE;
			m_buildingsVector[m_buildingSlots / 2].constructable = false;
			m_buildingsVector[(m_buildingSlots / 2) - 1].constructable = false;
			m_buildingsVector[(m_buildingSlots / 2) - 2].constructable = false;
			*/
			/*
			auto building = Sprite::create("img/buildings/p_building01stage3.png");
			auto buildingObject = m_tilemap->getObjectGroup("player")->getObject("Building");
			auto posX = buildingObject["x"].asFloat();
			building->setScale(0.52f);
			m_gameNode->addChild(building);

			building->setPosition(posX, npcY * 1.28);
			*/
			m_step = -1;
			if (def->getBoolForKey("firstTimeTutorial"))
			{
				m_isUIEnabled = false;
				m_isMoveUIEnabled = false;
				m_isJumpUIEnabled = false;
				m_isItemUIEnabled = false;
				m_isActionUIEnabled = false;
				m_gameNode->setScale(2.4f);
				m_step = -1;
				auto ui = (Sprite*)m_uiNode;
				ui->setCascadeOpacityEnabled(true);
				ui->setOpacity(0);
				jButton->setOpacity(0);
				aButton->setOpacity(0);
				JoyStick->setOpacity(0);
				StickBack->setOpacity(0);

				auto walkToLeft = FlipX::create(true);
				auto walkToRight = FlipX::create(false);
				auto initialDelay = DelayTime::create(1.3f);
				auto inBetweenDelay = DelayTime::create(0.6f);
				auto nextStep = CallFunc::create([this]() {this->nextStep(); });
				auto zoomOut = CallFunc::create([this]() {this->m_gameNode->runAction(ScaleTo::create(0.7f, 1.5f)); });
				auto showMSG = CallFunc::create([this]() {this->popUpMsg(MSG_TYPE::MSG_TUTORIAL); });
				auto lookAroundSequence = Sequence::create(initialDelay, walkToLeft, inBetweenDelay, walkToRight, inBetweenDelay, 
					walkToLeft, inBetweenDelay, walkToRight, inBetweenDelay, walkToLeft, inBetweenDelay, nextStep, zoomOut, initialDelay, initialDelay, showMSG,NULL);
				
				m_character->runAction(lookAroundSequence)->setTag(2);

				m_accesibleAreas.clear();
				m_accesibleAreas.push_back(mapData::FOREST01);

				def->setBoolForKey("firstTimeTutorial", false);
			}

			if (def->getBoolForKey("HasTimeMachinePart"))
			{
				
			}
		}
		else if (m_age == 1)
		{
			m_step = -1;
			spawnNPC(4);
			
			m_accesibleAreas.clear();
			auto randomArea = RandomHelper::random_int(2, 10);
			m_accesibleAreas.push_back(randomArea);

			int nextRandom = RandomHelper::random_int(2, 10);
			while (nextRandom == randomArea)
			{
				nextRandom = RandomHelper::random_int(2, 10);
			}
			m_accesibleAreas.push_back(nextRandom);

			int thirdRandom = RandomHelper::random_int(2, 10);
			while (thirdRandom == randomArea || thirdRandom == nextRandom)
			{
				thirdRandom = RandomHelper::random_int(2, 10);
			}
			m_accesibleAreas.push_back(thirdRandom);

			int forthRandom = RandomHelper::random_int(2, 10);
			while (forthRandom == randomArea || forthRandom == nextRandom || forthRandom == thirdRandom)
			{
				forthRandom = RandomHelper::random_int(2, 10);
			}
			m_accesibleAreas.push_back(forthRandom);
			

			auto partAreaProbability = RandomHelper::random_int(0, 100);
			if (partAreaProbability > 90)
			{
				m_accesibleAreas.push_back(12);
			}

			

			/*
			auto building = Sprite::create("img/buildings/weaponShop.png");
			auto buildingObject = m_tilemap->getObjectGroup("player")->getObject("Building");
			auto posX = buildingObject["x"].asFloat();
			building->setScale(1.4f);
			m_gameNode->addChild(building);

			//Create a default state first
			NPCStates randomState;
			int maxType = NPC_TYPE_PREHISTORIC_MAX - 1;
			int maxBuilding = buildingData::MAX_PREHISTORIC - 1;
			randomState.npcType = NPC_TYPE_BLACKSMITH;//cocos2d::RandomHelper::random_int(0, maxType);
			randomState.buildingNum1 = buildingData::BUILDING_NONE;
			//WARNING: npcStateVector and npcVector has to share the same index!
			m_npcStateVector.push_back(randomState);
			m_npcVector.push_back(Sprite::create(npcTypePath[m_npcStateVector[0].npcType]));
			ValueMap spawnBox = objectGroup->getObject("Spawn");
			float npcY;
			if (spawnBox["y"].asInt() != 0)
			{
				npcY = spawnBox["y"].asFloat() + 12;
			}
			m_npcVector[0]->setPositionY(npcY);
			m_npcVector[0]->setPositionX(cocos2d::RandomHelper::random_int(0, 500) + 1650);
			m_npcVector[0]->setScale(0.48);
			m_gameNode->addChild(m_npcVector[0]);

			building->setPosition(posX, npcY * 1.27);

			auto timeMachine = Sprite::create("img/timeMachine/TimeMachine2.png");
			auto timeMachineObject = objectGroup->getObject("TimeMachine");
			auto timeMachineX = timeMachineObject["x"].asFloat();
			auto timeMachineY = timeMachineObject["y"].asFloat();
			timeMachine->setPosition(timeMachineX, timeMachineY);
			timeMachine->setName("timeMachine");
			timeMachine->setScale(2.5);
			m_gameNode->addChild(timeMachine);
			*/
		}
	}

	if (m_age == 1)
	{
		m_step = -1;
		if (m_isTutorial && !m_isTutorialHome)
		{
			auto cameraEye = Sprite::create();
			cameraEye->setPosition(m_character->getPosition());
			cameraEye->setName("tutorialCameraEye");
			m_gameNode->addChild(cameraEye);

			//m_uiNode->setVisible(false);
		}
	}

	renderBuildings();

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
			house->setScale(42 / house->getContentSize().height);
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

	for (int i = 0; i < m_resourceVector.size(); i++)
	{
		auto resource = m_resourceVector[i];
		if (resource != nullptr)
		{
			if (m_character->getBoundingBox().intersectsRect(resource->getBoundingBox()))
			{
				
				if (resource->getName() == "engine")
				{
					created = true;
					//if (!m_isCreatedActionPopUp)
					{
						m_isCreatedActionPopUp = true;
						resource->setColor(Color3B::BLACK);
						auto resourcePos = m_gameNode->convertToWorldSpace(resource->getPosition());
						auto engineClone = Sprite::create("img/timeMachine/TimeMachineEngine.png");
						//engineClone->setTexture(resource->getTexture());
						engineClone->setScale(1.7f);
						engineClone->setPosition(resourcePos);
						m_gameNode->removeChild(resource);
						m_uiNode->addChild(engineClone);
						auto moveTo = FadeOut::create(1.0f);

						engineClone->runAction(moveTo);
						m_resourceVector.erase(m_resourceVector.begin() + i);
						auto def = UserDefault::getInstance();
						def->setBoolForKey("HasTimeMachinePart1", true);
					}
				}
				else if (resource->getName() == "antena")
				{
					created = true;
					//if (!m_isCreatedActionPopUp)
					{
						m_isCreatedActionPopUp = true;
						auto moveTo = FadeOut::create(1.0f);
						resource->runAction(moveTo);
						m_resourceVector.erase(m_resourceVector.begin() + i);
						auto def = UserDefault::getInstance();
						def->setBoolForKey("HasTimeMachinePart2", true);
					}
				}
				else if (resource->getName() == "monitor")
				{
					created = true;
					//if (!m_isCreatedActionPopUp)
					{
						m_isCreatedActionPopUp = true;
						auto moveTo = FadeOut::create(1.0f);
						resource->runAction(moveTo);
						m_resourceVector.erase(m_resourceVector.begin() + i);
						auto def = UserDefault::getInstance();
						def->setBoolForKey("HasTimeMachinePart3", true);
					}
				}
				else
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
			}
			else
			{
				if (resource->getName() == "engine")
				{

				}

				else
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

Vec2 Control3::setViewpoint(cocos2d::Vec2 position)
{
	auto visibleSize = Director::getInstance()->getWinSize();

	int x = (visibleSize.width / 2.0f) - position.x;
	int y = (visibleSize.height / m_background.Interpolate(1.5, m_gameNode->getScale(), 2.4, 2.8, 2.0)) - position.y;

	//player-centered position non-scaled
	auto defPosition = Vec2(x, y);
	
	auto newViewPoint = Vec2(x * m_gameNode->getScaleX(), y * m_gameNode->getScaleY());
	auto viewPointDelta = newViewPoint - defPosition;

	//scaled position
	defPosition += viewPointDelta;
	defPosition -= (visibleSize / 2) * (m_gameNode->getScale() - 1);
	
	m_gameNode->setPosition(defPosition);
	return defPosition;
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
						if (m_resourceVector[m_actingResourceIndex]->getName() == "tree" || m_resourceVector[m_actingResourceIndex]->getName() == "appletree" || m_resourceVector[m_actingResourceIndex]->getName() == "cactus")
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
		if (m_standing && m_onGround)
		{
			//once = false;
			m_animationInstanced = true;
			resetPlayerActions();
			m_character->runAction(RepeatForever::create(Animate::create(m_playerAnimCache->getAnimation("idle"))))->setTag(1);
		}
		else if(!m_standing && m_onGround)
		{
			//once = false;
			m_animationInstanced = true;
			resetPlayerActions();
			auto walkingAnim = Animate::create(m_playerAnimCache->getAnimation("walking"));
			m_character->runAction(RepeatForever::create(walkingAnim))->setTag(1);
		}
	}
	
	if (m_onGround)
	{
		if (m_jumpAnim && m_animationInstanced)
		{
			m_animationInstanced = false;
			m_jumpAnim = false;
		}
	}
	else 
	{
		//if (!m_animationInstanced)
		{
			m_animationInstanced = true;
			m_jumpAnim = true;
			resetPlayerActions();
			auto jumpAnim = Animate::create(m_playerAnimCache->getAnimation("jump"));
			m_character->runAction(jumpAnim)->setTag(1);
		}
	}
	
	//Time Travel
	if (m_isTutorialHome)
	{
		if (!m_inTimeMachine)
		{
			//When player collides with time machine -> run the time travelling cutscene
			auto timeMachine = m_gameNode->getChildByName("timeMachine");

			auto def = UserDefault::getInstance();
			if (def->getBoolForKey("HasTimeMachinePart"))
			{
				if (m_character->getBoundingBox().intersectsRect(timeMachine->getBoundingBox()) &&  m_step == 3)
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

					m_character->setPositionX(m_character->getPositionX() * 1.39f);
					m_character->setPositionY(m_character->getPositionY() * 1.05f);

					timeMachine->setPositionX(timeMachine->getPositionX() * 1.39f);
					timeMachine->setPositionY(timeMachine->getPositionY() * 1.05f);

					auto timeMachinePart = m_uiNode->getChildByName("engineSilluette");
					timeMachinePart->retain();
					timeMachinePart->removeFromParent();
					m_temporaryNode->addChild(timeMachinePart);
					timeMachinePart->release();

					m_gameNode->setCascadeOpacityEnabled(true);
					m_uiNode->setCascadeOpacityEnabled(true);
					m_backgroundNode->setCascadeOpacityEnabled(true);
					m_temporaryNode->setZOrder(21);

					auto screenFiller = Sprite::create("img/bg.png");
					auto visibleSize = Director::getInstance()->getVisibleSize();
					screenFiller->setPosition(visibleSize.width / 2, visibleSize.height / 2);
					screenFiller->setColor(Color3B::BLACK);
					screenFiller->setScale(3);
					screenFiller->setOpacity(0);
					m_temporaryNode->addChild(screenFiller, -1);

					screenFiller->runAction(FadeIn::create(2));

					m_disableGame = true;
					m_character->runAction(Animate::create(m_playerAnimCache->getAnimation("idle")))->setTag(1);

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

					def->setBoolForKey("FromMenu", true);

					auto backToTitleScreen = CallFunc::create([this]() {this->menuCloseCallback(); });
					auto goToAge = CallFunc::create([this]() {
						this->nextAge();
						this->resetScene(); 
					});

					//Complete sequence of actions for the time travel animation.
					//transform, jumpPlayer and twirling are lambda functions
					timeMachinePart->runAction(Sequence::create(delay, moveTo, reachDelay, transform, transformDelay, jumpPlayer, jumpDelay, twirling, twirlingDelay, goToAge, NULL));
				}
			}
		}
	}

	if (m_isTutorial)
	{
		if (!m_isTutorialHome)
		{ 
			if (m_age == 0)
			{
				//step manager
				//Jump Highlight
				if (m_step == 0)
				{
					if (m_character->getPositionX() >= 1350 && m_character->getPositionX() <= 1420)
					{
						//nextStep();
					}
				}
				//Go Right
				else if (m_step == 1)
				{
					if (m_character->getPositionX() >= 2025 && m_character->getPositionX() <= 2100)
					{
						nextStep();
					}
				}
				//Cut Tree
				else if (m_step == 2)
				{
					if (m_character->getPositionX() >= 2750 && m_character->getPositionX() <= 2825)
					{
						nextStep();
					}
				}
				
				//Kill the bear
				else if (m_step == 3)
				{
					if (m_character->getPositionX() >= m_enemy.GetStaticEnemyPos(0).x - 300 && m_character->getPositionX() <= m_enemy.GetStaticEnemyPos(0).x + 200)
					{
						nextStep();
					}
				}

				//Take the time machine part
				else if (m_step == 4)
				{
					if (m_gameNode->getChildByName("engine"))
					{
						if (m_character->getPositionX() >= m_gameNode->getChildByName("engine")->getPosition().x - 300 && m_character->getPositionX() <= m_gameNode->getChildByName("engine")->getPosition().x + 150)
						{
							nextStep();
						}
					}
				}

				//Go Left
				else if (m_step == 5)
				{
					if (!m_gameNode->getChildByName("engine"))
					{
						nextStep();
					}
				}
				//Go left
				else if (m_step == 6)
				{
					if (m_character->getPositionX() >= 2025 && m_character->getPositionX() <= 2100)
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
						//Moving control highlight
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

						/*
						auto square1 = Sprite::create("img/ui/AttackCover.png");
						square1->setScale(0.4);
						square1->setOpacity(150);
						square1->setPosition(m_JoyStick.getMidX(), m_JoyStick.getMidY());
						square1->setName("Pad_Cover");
						m_uiNode->addChild(square1);

						auto delay2 = DelayTime::create(0.7);
						auto blink2 = Blink::create(3, 6);
						auto hide2 = Hide::create();
						square1->runAction(Sequence::create(hide2, delay2, blink2, hide2, NULL));
						*/
					}
					break;

					//Jump Button highlight + jump arrow
					case 1:
					{
						//clean up
						m_uiNode->removeChildByName("Pad_Cover");

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

						float arrowX;
						float arrowY;

						auto backgroundGroup = m_tilemap->getObjectGroup("player");
						if (backgroundGroup)
						{
							auto jumpObject = backgroundGroup->getObject("JumpIcon");
							arrowX = jumpObject["x"].asFloat();
							arrowY = jumpObject["y"].asFloat();
						}

						auto jumpArrow = Sprite::create("img/ui/tutorialArrow.png");
						jumpArrow->setPosition(arrowX, arrowY);
						jumpArrow->setScale(0.3f);
						jumpArrow->setRotation(45);
						jumpArrow->setName("JumpArrow");
						m_gameNode->addChild(jumpArrow);

						auto delay2 = DelayTime::create(0.7f);
						auto blink2 = Blink::create(3, 6);
						auto hide2 = Hide::create();
						jumpArrow->runAction(Sequence::create(hide2, delay2, blink2, hide2, NULL));

					}
					break;

					//Go right icon
					case 2:
					{
						//clean up
						m_gameNode->removeChildByName("JumpArrow");

						auto arrow = static_cast<Sprite*>(m_uiNode->getChildByName("HorizontalArrow"));

						auto blink = Blink::create(3, 3);
						auto hide = Hide::create();
						arrow->runAction(Sequence::create(hide, blink, hide, NULL));

					}
					break;

					//Cut down first tree
					case 3:
					{
						auto arrowV = Sprite::create("img/ui/tutorialArrow.png");
						arrowV->setRotation(180);
						arrowV->setOpacity(230);
						arrowV->setScale(0.2);
						arrowV->setVisible(false);
						arrowV->setPosition(m_resourceVector[0]->getPositionX(), m_resourceVector[0]->getPositionY() + 200);
						arrowV->setName("VerticalArrow");
						m_gameNode->addChild(arrowV);

						auto blink = Blink::create(3, 3);
						auto hide = Hide::create();
						arrowV->runAction(Sequence::create(blink, NULL));

						auto circle = m_uiNode->getChildByName("ButtonCover");
						circle->setPosition(m_aButtonRect.getMidX(), m_aButtonRect.getMidY());

						auto blink2 = Blink::create(3, 3);
						circle->runAction(Sequence::create(blink2, NULL));

						auto animSprite = Sprite::create();
						animSprite->setScale(1.5f);
						animSprite->setName("TreeCutAnim");
						animSprite->setPositionX(arrowV->getPositionX());
						animSprite->setPositionY(arrowV->getPositionY() - 100);
						m_gameNode->addChild(animSprite);

						auto animation = Animation::create();
						animation->addSpriteFrameWithFile("img/ui/movingIcon/cutTree01.png");
						animation->addSpriteFrameWithFile("img/ui/movingIcon/cutTree02.png");
						animation->setLoops(3.0f);
						animation->setDelayPerUnit(0.5f);
						animation->setRestoreOriginalFrame(true);

						auto animate = Animate::create(animation);
						animSprite->runAction(animate);
					}
					break;
					
					case 4:
					{
						auto circle = m_uiNode->getChildByName("ButtonCover");
						m_gameNode->removeChildByName("EngineGet");
						auto blink = Blink::create(3, 3);
						circle->runAction(blink);

						auto animSprite = Sprite::create();
						animSprite->setScale(1.5f);
						animSprite->setName("BattleIcon");
						animSprite->setPositionX(m_enemy.GetStaticEnemyPos(0).x);
						animSprite->setPositionY(m_enemy.GetStaticEnemyPos(0).y + 70.0f);
						m_gameNode->addChild(animSprite);

						auto animation = Animation::create();
						animation->addSpriteFrameWithFile("img/ui/movingIcon/battleSwords01.png");
						animation->addSpriteFrameWithFile("img/ui/movingIcon/battleSwords02.png");
						animation->setLoops(3.0f);
						animation->setDelayPerUnit(0.5f);
						animation->setRestoreOriginalFrame(true);

						auto animate = Animate::create(animation);
						animSprite->runAction(animate);
					}
					break;

					case 5:
					{
						m_gameNode->removeChildByName("TreeCutAnim");

						auto arrowV = m_gameNode->getChildByName("VerticalArrow");
						Vec2 enginePos = m_gameNode->getChildByName("engine")->getPosition();
						arrowV->setPosition(enginePos.x, enginePos.y + 100);
						auto blink = Blink::create(3, 3);
						arrowV->runAction(Sequence::create(blink, NULL));

						auto animSprite = Sprite::create();
						animSprite->setScale(1.5f);
						animSprite->setName("EngineGet");
						animSprite->setPositionX(enginePos.x);
						animSprite->setPositionY(enginePos.y + 50);
						m_gameNode->addChild(animSprite);

						auto animation = Animation::create();
						animation->addSpriteFrameWithFile("img/ui/movingIcon/timeMachGet01.png");
						animation->addSpriteFrameWithFile("img/ui/movingIcon/timeMachGet02.png");
						animation->setLoops(3.0f);
						animation->setDelayPerUnit(0.5f);
						animation->setRestoreOriginalFrame(true);

						auto animate = Animate::create(animation);
						animSprite->runAction(animate);
					}
					break;

					case 6:
					{
						m_gameNode->removeChildByName("BattleIcon");

						auto arrow = static_cast<Sprite*>(m_uiNode->getChildByName("HorizontalArrow"));
						arrow->setRotation(-90);
						auto blink = Blink::create(3, 3);
						arrow->runAction(blink);
					}
					break;

					case 7:
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
			else if (m_age == 1)
			{
				if (m_stepDone)
				{
					m_stepDone = false;
					auto visibleSize = Director::getInstance()->getVisibleSize();

					switch (m_step)
					{
					case 0:
					{
						//WIP
						auto animSprite = Sprite::create();
						animSprite->setScale(1.5f);
						animSprite->setName("EngineGet");
						animSprite->setPositionX(m_character->getPosition().x);
						animSprite->setPositionY(m_character->getPosition().y + 70);
						m_gameNode->addChild(animSprite);

						auto animation = Animation::create();
						animation->addSpriteFrameWithFile("img/ui/movingIcon/exclamation01.png");
						animation->addSpriteFrameWithFile("img/ui/movingIcon/exclamation02.png");
						animation->setLoops(3.0f);
						animation->setDelayPerUnit(0.5f);
						animation->setRestoreOriginalFrame(true);

						auto animate = Animate::create(animation);

						auto delayAnimate = DelayTime::create(0.8f);

						animSprite->runAction(Sequence::create(delayAnimate, animate, NULL));

						//shade screen anim
						auto shadeScreen = LayerColor::create(Color4B::BLACK);
						shadeScreen->setOpacity(0);
						//shadeScreen->setPosition(visibleSize.width / 2, visibleSize.height / 2);
						this->addChild(shadeScreen);

						auto waitForShade = DelayTime::create(9.0f);
						auto showShade = FadeIn::create(1.0f);
						shadeScreen->runAction(Sequence::create(waitForShade, showShade, NULL));

						//title logo appearance
						auto Delay = DelayTime::create(4.5f);
						auto shortDelay = DelayTime::create(1.0f);
						auto Fadein = FadeIn::create(3.0f);
						auto FadeOut = FadeOut::create(1.0f);
						auto goToTitle = CallFunc::create([this]() {this->menuCloseCallback(); });
						auto Sequence = Sequence::create(Delay, Fadein, Delay, FadeOut, shortDelay, goToTitle, NULL);

						auto TitleName = Sprite::create("img/ui/titleLogo.png");

						TitleName->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
						TitleName->setScale(0.8f);
						TitleName->setOpacity(0);
						TitleName->setName("Title_Logo");

						TitleName->runAction(Sequence);
						this->addChild(TitleName);

					}
					break;
					}
				}
			}
		}

		//Tutorial Home Map
		else
		{
			//Prehistoric era
			if (m_age == 0)
			{
				auto def = UserDefault::getInstance();
				if (!def->getBoolForKey("HasTimeMachinePart"))
				{
					/*
					if (m_step == 0)
					{
						auto diff = m_npcVector[0]->getPositionX() - m_character->getPositionX();
						if (abs(diff) <= 300)
						{
							nextStep();
						}
					}
					
					if (m_step == 1)
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
					}*/

					if (m_stepDone)
					{
						m_stepDone = false;
						auto visibleSize = Director::getInstance()->getVisibleSize();

						switch (m_step)
						{
						case 0:
						{
							auto animSprite = Sprite::create();
							animSprite->setScale(1.5f);
							animSprite->setName("NotWorking");
							animSprite->setPositionX(m_character->getPositionX());
							animSprite->setPositionY(m_character->getPositionY() + 70.0f);
							/*
							animSprite->setPositionX(m_gameNode->getChildByName("timeMachine")->getPositionX());
							animSprite->setPositionY(m_gameNode->getChildByName("timeMachine")->getPositionY() + 110.0f);
							*/
							m_gameNode->addChild(animSprite);

							auto animation = Animation::create();
							animation->addSpriteFrameWithFile("img/ui/movingIcon/exclamation01.png");
							animation->addSpriteFrameWithFile("img/ui/movingIcon/exclamation02.png");
							animation->setLoops(4.0f);
							animation->setDelayPerUnit(0.5f);
							animation->setRestoreOriginalFrame(true);

							auto animate = Animate::create(animation);
							animSprite->runAction(animate);

							auto delay = DelayTime::create(3);
							//auto nextStep = CallFunc::create([this]() {this->nextStep(); });
							m_gameNode->runAction(Sequence::create(delay, NULL));

						}
							break;
						case 1:
						{
							m_isUIEnabled = true;
							m_isMoveUIEnabled = true;
							m_isItemUIEnabled = true;

							auto arrow = Sprite::create("img/ui/tutorialArrow.png");
							arrow->setRotation(90);
							arrow->setOpacity(230);
							arrow->setScale(0.35);
							arrow->setPosition(visibleSize.width / 2, visibleSize.height / 1.4);
							arrow->setName("HorizontalArrow");
							arrow->setVisible(false);
							m_uiNode->addChild(arrow);

							auto delay = DelayTime::create(1.7);
							auto show = Show::create();
							auto blink = Blink::create(3, 3);
							auto hide = Hide::create();
							arrow->runAction(Sequence::create(delay, show, blink, hide, NULL));

							auto square1 = Sprite::create("img/ui/AttackCover.png");
							square1->setScale(0.4);
							square1->setOpacity(150);
							square1->setPosition(m_JoyStick.getMidX(), m_JoyStick.getMidY());
							square1->setName("Pad_Cover");
							square1->setVisible(false);
							m_uiNode->addChild(square1);

							auto delay2 = DelayTime::create(0.7);
							auto blink2 = Blink::create(3, 6);
							auto hide2 = Hide::create();
							square1->runAction(Sequence::create(hide2, delay2, delay2, delay2, blink2, hide2, NULL));

							auto joystick = m_uiNode->getChildByName("joystick");
							joystick->setOpacity(def->getFloatForKey("button"));

							auto joystickBG = m_uiNode->getChildByName("stickBack");
							joystickBG->setOpacity(def->getFloatForKey("button"));

							m_uiNode->runAction(FadeIn::create(1.0f));
						}
						break;
						default:
							break;
						}
					}
				}
				else
				{
					//Go to Villager
					if (m_step == 0)
					{
						auto diff = abs(m_npcVector[0]->getPositionX() - m_character->getPositionX());
						if (abs(diff) <= 300)
						{
							nextStep();
						}
					}
					//Go left
					/*
					else if (m_step == 1)
					{
						if (m_character->getPositionX() >= 1860 && m_character->getPositionX() <= 1950)
						{
							nextStep();
						}
					}
					*/
					//Go Time Machine
					else if (m_step == 2)
					{
						if (m_character->getPositionX() >= m_gameNode->getChildByName("timeMachine")->getPositionX() && m_character->getPositionX() <= m_gameNode->getChildByName("timeMachine")->getPositionX() + 300)
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
						{
							auto arrow = Sprite::create("img/ui/tutorialArrow.png");
							arrow->setRotation(-90);
							arrow->setOpacity(230);
							arrow->setScale(0.35);
							arrow->setPosition(visibleSize.width / 2, visibleSize.height / 1.4);
							arrow->setName("HorizontalArrow");
							m_uiNode->addChild(arrow);

							auto delay = DelayTime::create(0.7);
							auto blink = Blink::create(3, 3);
							auto hide = Hide::create();
							arrow->runAction(Sequence::create(hide, delay, blink, hide, NULL));
						}
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
							auto arrow = m_uiNode->getChildByName("HorizontalArrow");

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
			else if (m_age == 1)
			{
				auto def = UserDefault::getInstance();
				if (m_stepDone)
				{
					m_stepDone = false;
					auto visibleSize = Director::getInstance()->getVisibleSize();

					switch (m_step)
					{
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
					}
					break;
					default:
						break;
					}
				}	
			}
		}
	}

	//battle icon above enemy update
	auto battleSwords = m_gameNode->getChildByName("BattleIcon");
	if (battleSwords)
	{
		battleSwords->setPosition(m_enemy.GetStaticEnemyPos(0).x, m_enemy.GetStaticEnemyPos(0).y + 70);
	}

	//Collision with resource icons
	for (int i = 0; i < m_resourceIconVector.size(); ++i)
	{
		auto resourceIcon = m_resourceIconVector[i];
		if (m_character->getBoundingBox().intersectsRect(resourceIcon->getBoundingBox()))
		{
			//resourceIcon->setVisible(false);
			bool noSlot = true;
			bool noSpace = false;
			//bool addedOnce = false;
			int firstVoidSlot = -1;
			int addedSlot = 0;
			for (int j = 0; j < sizeof(m_inventorySlot) / sizeof(m_inventorySlot[0]); ++j)
			{
				//Put item in the same slot
				if (resourceIcon->getTag() == m_inventorySlot[j].itemType)
				{
					m_inventorySlot[j].itemAmount++;
					addedSlot = j;
					noSlot = false;
				}
				//Get first void slot
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
			//Put the item in the first void slot if there is one
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
				m_resourceIconVector.erase(m_resourceIconVector.begin() + i);
				resourceIcon->retain();
				auto newIconPos = m_gameNode->convertToWorldSpace(resourceIcon->getPosition());
				resourceIcon->removeFromParent();
				m_uiNode->addChild(resourceIcon);
				resourceIcon->setPosition(newIconPos);
				resourceIcon->release();
				resourceIcon->setScale(m_gameNode->getScale());

				auto visibleSize = Director::getInstance()->getVisibleSize();
				auto slot = noSlot ? firstVoidSlot : addedSlot;
				
				int inventoryX = (visibleSize.width / 2 - 500 + 50) + 100 * slot;
				
				auto moveToSlot = MoveTo::create(0.8f, Vec2(inventoryX, m_inventoryY));
				auto removeFunc = CallFunc::create([resourceIcon]() {resourceIcon->removeFromParent(); });
				//m_gameNode->removeChild(resourceIcon);

				if (resourceIcon)
				{
					resourceIcon->runAction(Sequence::create(moveToSlot, removeFunc, NULL));
				}

				auto audioEngine = SimpleAudioEngine::getInstance();
				audioEngine->playEffect("sound/sfx/main/resource/pon.mp3");
			}

			

			showItems(m_inventoryX, m_inventoryY);
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
				m_onGround = true;
				
				auto correctionDelta = bottomTile->getBoundingBox().getMaxY() - m_character->getBoundingBox().getMinY();
				if (correctionDelta <= 10)
				{
					
					m_character->setPositionY(m_character->getPositionY() + correctionDelta);
					if (m_speedY < 10)
					{
						if(m_speedY < 0)
							m_speedY = 0;
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
			auto def = UserDefault::getInstance();
			auto area = def->getIntegerForKey("MapArea");
			if (m_character->getBoundingBox().intersectsRect(leftArrow->getBoundingBox()))
			{
				//to the left
				
				//changeArea(false);
				if (def->getIntegerForKey("MapArea") == mapData::HOME_MAP)
				{
					resetPlayerActions();
					popUpMsg(MSG_TYPE::MSG_CHANGE_MAP);
				}
				else
				{
					popUpMsg(MSG_TYPE::MSG_CONFIRM);
				}

			}
		}
		auto rightArrow = m_gameNode->getChildByName("RightArrow");
		if (rightArrow)
		{
			auto def = UserDefault::getInstance();
			auto area = def->getIntegerForKey("MapArea");
			if (m_character->getBoundingBox().intersectsRect(rightArrow->getBoundingBox()))
			{
				//changeArea(true);
				if (area == mapData::HOME_MAP)
				{
					resetPlayerActions();
					popUpMsg(MSG_TYPE::MSG_CHANGE_MAP);
				}
				else
				{
					popUpMsg(MSG_TYPE::MSG_CONFIRM);
				}
			}
		}
	}

	m_character->setPositionY(m_character->getPositionY() + m_speedY);
	m_character->setPositionX(m_character->getPositionX() + m_speedX);
}

void Control3::playerAnimationCache()
{
	auto walking = Animation::create();
	walking->addSpriteFrameWithFileName("img/sprites/player/newCharacter/walk1.png");
	walking->addSpriteFrameWithFileName("img/sprites/player/newCharacter/walk2.png");
	walking->addSpriteFrameWithFileName("img/sprites/player/newCharacter/walk3.png");
	walking->addSpriteFrameWithFileName("img/sprites/player/newCharacter/walk4.png");
	walking->setLoops(1);
	walking->setDelayPerUnit(0.1);

	auto idle = Animation::create();
	idle->addSpriteFrameWithFileName("img/sprites/player/newCharacter/idle1.png");
	idle->addSpriteFrameWithFileName("img/sprites/player/newCharacter/idle2.png");
	idle->addSpriteFrameWithFileName("img/sprites/player/newCharacter/idle3.png");
	idle->setLoops(1);
	idle->setDelayPerUnit(0.2);

	auto swing = Animation::create();
	swing->addSpriteFrameWithFileName("img/sprites/player/newCharacter/swing2.png");
	swing->addSpriteFrameWithFileName("img/sprites/player/newCharacter/swing1.png");
	swing->addSpriteFrameWithFileName("img/sprites/player/newCharacter/swing3.png");
	swing->addSpriteFrameWithFileName("img/sprites/player/newCharacter/swing4.png");
	swing->setLoops(1);
	swing->setDelayPerUnit(0.085);

	auto jump = Animation::create();
	jump->addSpriteFrameWithFileName("img/sprites/player/newCharacter/jump3.png");
	jump->addSpriteFrameWithFileName("img/sprites/player/newCharacter/jump3.png");

	jump->setLoops(1);
	jump->setDelayPerUnit(0.2f);

	m_playerAnimCache = AnimationCache::getInstance();
	m_playerAnimCache->addAnimation(walking, "walking");
	m_playerAnimCache->addAnimation(idle, "idle");
	m_playerAnimCache->addAnimation(swing, "swing");
	m_playerAnimCache->addAnimation(jump, "jump");
}

void Control3::changeArea(int area)
{
	m_onNext = true;
	auto def = UserDefault::getInstance();
	def->setIntegerForKey("MapArea", area);
	def->setBoolForKey("FromMenu", false);
	if (area == mapData::HOME_MAP)
	{
		def->setBoolForKey("FromRight", false);
	}
	else
	{
		def->setBoolForKey("FromRight", true);
	}
	
	def->flush();
	auto areaNow = def->getIntegerForKey("MapArea");
	CCLOG("Map Area: %i", areaNow);

	m_timeOfDay += 800;

	if (m_timeOfDay >= MAX_TIME_DAY)
	{
		m_timeOfDay = m_timeOfDay - MAX_TIME_DAY;
	}

	resetScene();
}

void Control3::resetScene()
{	
	saveGame();

	auto scene = Control3::createScene();
	Director::getInstance()->replaceScene(scene);
}

void Control3::finishActionInstance()
{
	resetPlayerActions();
	m_actingInstanced = false;
	if (!m_actingWithEnemy)
	{
		if (m_actedResourceCount > 5)
		{
			if (m_actingResourceIndex > -1)
			{
				spreadResource(m_resourceVector[m_actingResourceIndex]->getName(), m_resourceVector[m_actingResourceIndex]->getPosition());
				m_gameNode->removeChild(m_resourceVector[m_actingResourceIndex]);
				m_resourceVector.erase(m_resourceVector.begin() + m_actingResourceIndex);
				m_isActing = false;
			}
			m_actedResourceCount = 0;
		}
	}

	m_swinging = false;
	m_animationInstanced = false;
	m_attackOnce = false;
}

void Control3::spreadResource(std::string name, Vec2 originPos)
{
	float movingTime = 0.5f;
	int resourceTypes[5];
	//auto name = name;

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

			resIcon[i]->setPosition(originPos);
			resIcon[i]->setScale(1.2f);
			resIcon[i]->setTag(resourceTypes[i]);
			m_gameNode->addChild(resIcon[i]);
			m_resourceIconVector.push_back(resIcon[i]);

			auto xDistance = RandomHelper::random_int(-40, 40);
			auto moveX = MoveBy::create(movingTime, Vec2(xDistance, 0));

			auto yDistance = RandomHelper::random_int(0, 40);
			auto moveY1 = MoveBy::create(movingTime / 2, Vec2(0, yDistance));
			auto floor = m_character->getBoundingBox().getMinY();
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
		m_character->runAction(RepeatForever::create(Animate::create(m_playerAnimCache->getAnimation("swing"))))->setTag(1);
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

void Control3::showItems(float invX, float invY)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float inventoryX = invX;
	float inventoryY = invY;

	for (int j = 0; j < m_itemsVector.size(); ++j)
	{
		m_foregroundNode->removeChild(m_itemsVector[j]);
		m_foregroundNode->removeChild(m_itemCountVector[j]);
	}
	//resets vectors
	m_itemsVector.clear();
	m_itemCountVector.clear();

	bool notOrganized = false;
	//organize array
	do
	{
		bool isOk = true;
		for (int i = 0; i < 9; ++i)
		{
			if (m_inventorySlot[i + 1].itemType != 0)
			{
				if (m_inventorySlot[i].itemType == 0)
				{
					notOrganized = true;
					isOk = false;
					//transfer
					m_inventorySlot[i].itemType = m_inventorySlot[i + 1].itemType;
					m_inventorySlot[i].itemAmount = m_inventorySlot[i + 1].itemAmount;

					m_inventorySlot[i + 1].itemType = 0;
					m_inventorySlot[i + 1].itemAmount = 0;
				}
			}
		}

		if (isOk)
		{
			notOrganized = false;
		}
	} while (notOrganized);
	

	auto goldIcon = m_foregroundNode->getChildByName("goldIcon");
	if (!goldIcon)
	{
		m_foregroundNode->removeChildByName("goldIcon");
		goldIcon = Sprite::create("img/ui/gold.png");
		goldIcon->setPositionX(visibleSize.width - (goldIcon->getContentSize().width/2));
		goldIcon->setPositionY(visibleSize.height / 1.28f);
		goldIcon->setName("goldIcon");
		goldIcon->setZOrder(0);
		m_foregroundNode->addChild(goldIcon);
	}

	auto goldCount = m_foregroundNode->getChildByName("goldCount");
	m_foregroundNode->removeChild(goldCount);
	std::ostringstream ostr;
	ostr << m_gold;
	goldCount = Label::createWithTTF(ostr.str(), "fonts/PixelMplus10-Regular.ttf", 40);
	goldCount->setPositionX(goldIcon->getPositionX() - 10);
	goldCount->setPositionY(goldIcon->getPositionY());
	goldCount->setName("goldCount");
	m_foregroundNode->addChild(goldCount);

	for (int i = 0; i < 10; ++i)
	{
		if (m_inventorySlot[i].itemType != 0 && m_inventorySlot[i].itemAmount != 0)
		{
			bool firstTime = true;
			//for (int j = 0; j < m_itemsVector.size(); ++j)
			/*
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
							auto newCounter = Label::createWithTTF(ostr.str(), "fonts/PixelMplus10-Regular.ttf", 17);
							newCounter->setPositionX(inventoryX + 100 * i + 17);
							newCounter->setPositionY(inventoryY - 20);
							newCounter->setZOrder(2);
							m_foregroundNode->removeChild(m_itemCountVector[i]);
							m_foregroundNode->addChild(newCounter);
							m_itemCountVector[i] = newCounter;

							itemInList->setPosition(inventoryX + 100 * i, inventoryY);
						}
						else
						{
							m_foregroundNode->removeChild(m_itemCountVector[i]);
							m_foregroundNode->removeChild(itemInList);
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
									m_foregroundNode->removeChild(m_itemsVector[j]);
									m_foregroundNode->removeChild(m_itemCountVector[j]);
								}
								//resets vectors
								m_itemsVector.clear();
								m_itemCountVector.clear();
								//recursively calls it self to update if there was a change
								showItems(visibleSize.width / 2 - 500 + 50, inventoryY);
							}
						}	
					}
				}
			}
			*/
			if (firstTime)
			{
				auto sprite = Sprite::create(resourceData::resourceIconPath[m_inventorySlot[i].itemType]);
				sprite->setPositionX(inventoryX + 100 * i);
				sprite->setPositionY(inventoryY);
				sprite->setTag(m_inventorySlot[i].itemType);
				sprite->setZOrder(1);
				sprite->setScale(1.4f);
				m_foregroundNode->addChild(sprite);
				m_itemsVector.push_back(sprite);

				std::ostringstream ostr;
				ostr << m_inventorySlot[i].itemAmount;
				auto counter = Label::createWithTTF(ostr.str(), "fonts/PixelMplus10-Regular.ttf", 17);
				counter->setPositionX(sprite->getPositionX() + 17);
				counter->setPositionY(sprite->getPositionY() - 20);
				counter->setZOrder(2);
				m_foregroundNode->addChild(counter);
				m_itemCountVector.push_back(counter);
			}
		}
		else
		{
			m_inventorySlot[i].itemType = 0;
			m_inventorySlot[i].itemAmount = 0;
		}
	}
	CCLOG("%i, %i, %i, %i, %i, %i, %i, %i, %i, %i", m_inventorySlot[0].itemAmount, m_inventorySlot[1].itemAmount, m_inventorySlot[2].itemAmount, m_inventorySlot[3].itemAmount, m_inventorySlot[4].itemAmount, m_inventorySlot[5].itemAmount, m_inventorySlot[6].itemAmount, m_inventorySlot[7].itemAmount, m_inventorySlot[8].itemAmount, m_inventorySlot[9].itemAmount);
}

void Control3::updateHPBar()
{
	auto maxHP = 1000.0f;
	float hp = m_hp;
	float hpForScale = (hp / maxHP);
	m_hpBar->setScaleX(hpForScale);
}

void Control3::enterBuildMode()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto newBuildingType = m_npcStateVector[m_talkingNPCIndex].buildingNum1;
	/*
	m_standing = true;
	if (m_onGround)
		m_animationInstanced = false;
	m_moveTouchID = -1;
	auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
	JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
	*/

	m_disableGame = true;

	m_uiNode->setVisible(false);

	// Hide unnecessary visuals
	for (auto &npc : m_npcVector)
	{
		npc->setVisible(false);
	}

	m_playerPosPreBuildMode = m_character->getPosition();
	m_character->setVisible(false);

	popUpMsg(MSG_TYPE::MSG_BUILD_MODE); 

	//create tmp sprite of completed building
	auto building = Sprite::create(buildingData::buildingPath[newBuildingType].c_str());
	building->setOpacity(200);
	building->setName("tmpBuilding");

	m_buildingModeIndex = m_buildingsVector.size() / 2;

	auto scale = (m_slotSize * buildingData::slotPerBuilding[newBuildingType]) / building->getContentSize().width;
	building->setScale(scale);
	//search the X position of the building according to its slot and the slots it takes
	building->setPositionX(m_buildingStart - ((((m_buildingModeIndex) * m_slotSize)) - ((buildingData::slotPerBuilding[newBuildingType] * m_slotSize)) / 2));
	//place the building on top of the ground
	building->setPositionY(m_groundLevel + (abs(building->getBoundingBox().getMinY() - building->getBoundingBox().getMaxY()) / 2));

	bool overlaps = false;

	for (int i = 0; i < buildingData::slotPerBuilding[newBuildingType]; ++i)
	{
		if (m_buildingsVector[m_buildingModeIndex - i].constructable == false)
		{
			overlaps = true;
		}
	}

	if (overlaps == true)
	{
		building->setColor(Color3B::RED);
	}

	m_gameNode->addChild(building);

	m_character->setPosition(building->getPosition());
}

void Control3::exitBuildMode()
{
	m_gameNode->removeChildByName("tmpBuilding");
	m_disableGame = false;

	m_uiNode->setVisible(true);

	m_character->setPosition(m_playerPosPreBuildMode);
	m_character->setVisible(true);
	// Show hidden visuals
	for (auto &npc : m_npcVector)
	{
		npc->setVisible(true);
	}
	//auto scaleUp = ScaleTo::create(1.0f, 1.5f);
	//m_gameNode->runAction(scaleUp);
}

void Control3::moveBuildMode(bool right)
{
	auto newBuildingType = m_npcStateVector[m_talkingNPCIndex].buildingNum1;
	auto building = (Sprite*)m_gameNode->getChildByName("tmpBuilding");
	
	if (!right)
	{
		if (m_buildingModeIndex < m_buildingsVector.size() - 1)
		{
			m_buildingModeIndex++;
		}
	}
	else
	{
		if (m_buildingModeIndex - buildingData::slotPerBuilding[newBuildingType] > 0)
		{
			m_buildingModeIndex--;
		}
	}

	building->setPositionX(m_buildingStart - ((((m_buildingModeIndex)* m_slotSize)) - ((buildingData::slotPerBuilding[newBuildingType] * m_slotSize)) / 2));
	m_character->setPositionX(building->getPositionX());

	bool overlaps = false;

	for (int i = 0; i < buildingData::slotPerBuilding[newBuildingType]; ++i)
	{
		if (m_buildingsVector[m_buildingModeIndex - i].constructable == false)
		{
			overlaps = true;
		}
	}

	if (overlaps == true)
	{
		building->setColor(Color3B::RED);
	}
	else
	{
		building->setColor(Color3B::WHITE);
	}
}

void Control3::confirmBuildMode()
{
	auto newBuildingType = m_npcStateVector[m_talkingNPCIndex].buildingNum1;
	int resType1 = buildingData::buildingRecepe[newBuildingType].resource1;
	int resType2 = buildingData::buildingRecepe[newBuildingType].resource2;
	int resType3 = buildingData::buildingRecepe[newBuildingType].resource3;

	//bool passed = true;
	int passCounter = 0;
	int index1 = -1;
	int index2 = -1;
	int index3 = -1;
	int passCondition = 1;

	if (resType2 != buildingData::BUILDING_NONE)
	{
		passCondition++;

		if (resType3 != buildingData::BUILDING_NONE)
		{
			passCondition++;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		if (m_inventorySlot[i].itemType == resType1)
		{
			if (m_inventorySlot[i].itemAmount >= buildingData::buildingRecepe[newBuildingType].res1Count)
			{
				passCounter++;
				index1 = i;
			}
		}

		if (resType2 != buildingData::BUILDING_NONE)
		{
			if (m_inventorySlot[i].itemType == resType2)
			{
				if (m_inventorySlot[i].itemAmount >= buildingData::buildingRecepe[newBuildingType].res2Count)
				{
					passCounter++;
					index2 = i;
				}
			}

			if (resType3 != buildingData::BUILDING_NONE)
			{
				if (m_inventorySlot[i].itemType == resType3)
				{
					if (m_inventorySlot[i].itemAmount >= buildingData::buildingRecepe[newBuildingType].res3Count)
					{
						passCounter++;
						index3 = i;
					}
				}
			}
		}
	}

	if (passCounter >= passCondition)
	{
		bool overlaps = false;

		for (int i = 0; i < buildingData::slotPerBuilding[newBuildingType]; ++i)
		{
			if (m_buildingsVector[m_buildingModeIndex - i].constructable == false)
			{
				overlaps = true;
			}
		}

		if (overlaps == true) //cannot confirm
		{
			
		}
		else //can confirm
		{

			assignEndingParameters(newBuildingType);
			m_gold += 20;
			m_inventorySlot[index1].itemAmount -= buildingData::buildingRecepe[newBuildingType].res1Count;
			if (index2 != -1)
				m_inventorySlot[index2].itemAmount -= buildingData::buildingRecepe[newBuildingType].res2Count;
			if (index3 != -1)
				m_inventorySlot[index3].itemAmount -= buildingData::buildingRecepe[newBuildingType].res3Count;

			showItems(m_inventoryX, m_inventoryY);

			//update building vector
			m_buildingsVector[m_buildingModeIndex].buildingType = newBuildingType;
			for (int i = 0; i < buildingData::slotPerBuilding[newBuildingType]; ++i)
			{
				m_buildingsVector[m_buildingModeIndex - i].constructable = false;
			}

			saveGame();
			m_npcStateVector[m_talkingNPCIndex].isBuilding = true;
			closeMsg();
		}
	}
}

void Control3::renderBuildings()
{
	auto def = UserDefault::getInstance();
	//update building vector
	for (int i = 0; i < m_buildingSlots; ++i)
	{
		if(def->getIntegerForKey(mapData::buildingMap[i].c_str()) > 0)
		{
			m_buildingsVector[i].buildingType = def->getIntegerForKey(mapData::buildingMap[i].c_str());
			for (int j = 0; j < buildingData::slotPerBuilding[m_buildingsVector[i].buildingType]; ++j)
			{
				m_buildingsVector[i - j].constructable = false;
			}
		}
	}

	int index = 0;
	//render buildings in slots
	for (auto &slot : m_buildingsVector)
	{
		if (slot.buildingType != 0)
		{
			if (m_age == 0)
			{
				if (slot.buildingType > buildingData::MAX_PREHISTORIC)
				{
					slot.buildingType -= buildingData::MAX_PREHISTORIC;
					if (slot.buildingType > buildingData::REUNION)
					{
						slot.buildingType = 0;
						slot.sprite = nullptr;
					}

					saveGame();

					//update again
					for (int i = 0; i < m_buildingSlots; ++i)
					{
						if (def->getIntegerForKey(mapData::buildingMap[i].c_str()) > 0)
						{
							
							m_buildingsVector[i].buildingType = def->getIntegerForKey(mapData::buildingMap[i].c_str());

							if (def->getIntegerForKey(mapData::buildingMap[i].c_str()) == buildingData::TIME_MACHINE01)
							{
								if (def->getBoolForKey("HasTimeMachinePart1") && def->getBoolForKey("HasTimeMachinePart2"))
								{
									m_buildingsVector[i].buildingType++;
								}
							}
							else if (def->getIntegerForKey(mapData::buildingMap[i].c_str()) == buildingData::TIME_MACHINE02)
							{
								if (def->getBoolForKey("HasTimeMachinePart3"))
								{
									m_buildingsVector[i].buildingType++;
								}
							}


							for (int j = 0; j < buildingData::slotPerBuilding[m_buildingsVector[i].buildingType]; ++j)
							{
								m_buildingsVector[i - j].constructable = false;
							}
						}
					}

					renderBuildings();
					return;
				}
			}
			else if (m_age == 1)
			{
				if (slot.buildingType < buildingData::MAX_PREHISTORIC)
				{
					if (slot.buildingType != buildingData::TIME_MACHINE01)
					{
						slot.buildingType += buildingData::MAX_PREHISTORIC;
					}

					saveGame();

					//update again
					for (int i = 0; i < m_buildingSlots; ++i)
					{
						if (def->getIntegerForKey(mapData::buildingMap[i].c_str()) > 0)
						{
							m_buildingsVector[i].buildingType = def->getIntegerForKey(mapData::buildingMap[i].c_str());
							for (int j = 0; j < buildingData::slotPerBuilding[m_buildingsVector[i].buildingType]; ++j)
							{
								m_buildingsVector[i - j].constructable = false;
							}
						}
					}
				}
			}

			if (slot.sprite == nullptr)
			{
				auto sprite = Sprite::create(buildingData::buildingPath[slot.buildingType].c_str());

				//calculate scale according to its slotPerBuilding
				auto scale = (m_slotSize * buildingData::slotPerBuilding[slot.buildingType]) / sprite->getContentSize().width;
				sprite->setScale(scale);

				//search the X position of the building according to its slot and the slots it takes
				sprite->setPositionX(m_buildingStart - ((((index)* m_slotSize)) - ((buildingData::slotPerBuilding[slot.buildingType] * m_slotSize)) / 2));
				//place the building on top of the ground
				sprite->setPositionY(m_groundLevel + (abs(sprite->getBoundingBox().getMinY() - sprite->getBoundingBox().getMaxY()) / 2));
				if (slot.buildingType == buildingData::TIME_MACHINE01 || slot.buildingType == buildingData::TIME_MACHINE02 || slot.buildingType == buildingData::TIME_MACHINE03)
				{
					sprite->setName("timeMachine");
				}
				else if (slot.buildingType == buildingData::CASTLE)
				{
					sprite->setZOrder(-100);
				}

				slot.sprite = sprite;
				m_gameNode->addChild(sprite);
			}
		}

		index++;
	}
}

void Control3::equipmentCache()
{
	auto axe = Sprite::create("img/ui/axeSwing.png");
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

void Control3::addItemsToInventory(std::vector<int> itemList, std::vector<int> itemCountList)
{
	
	bool noSpace = false;

	int firstVoidSlot = -1;
	int addedSlot = 0;

	for (int i = 0; i < 5; ++i)
	{
		bool noSlot = true;
		if (itemList[i] != RESOURCE_NONE)
		{

			for (int j = 0; j < sizeof(m_inventorySlot) / sizeof(m_inventorySlot[0]); ++j)
			{
				//Put item in the same slot
				if (itemList[i] == m_inventorySlot[j].itemType)
				{
					m_inventorySlot[j].itemAmount += itemCountList[i];
					addedSlot = j;
					noSlot = false;
				}
				//Get first void slot
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

			//Put the item in the first void slot if there is one
			if (noSlot)
			{
				if (firstVoidSlot != -1)
				{
					m_inventorySlot[firstVoidSlot].itemType = itemList[i];
					m_inventorySlot[firstVoidSlot].itemAmount += itemCountList[i];
				}
				else
				{
					noSpace = true;
				}
			}

			if (!noSpace)
			{
				//moving animation...
			}
		}
	}
}

void Control3::addItemToInventory(int item, int count)
{
	bool noSlot = true;
	bool noSpace = false;

	int firstVoidSlot = -1;
	int addedSlot = 0;

	if (item != RESOURCE_NONE)
	{
		for (int j = 0; j < sizeof(m_inventorySlot) / sizeof(m_inventorySlot[0]); ++j)
		{
			//Put item in the same slot
			if (item == m_inventorySlot[j].itemType)
			{
				m_inventorySlot[j].itemAmount += count;
				addedSlot = j;
				noSlot = false;
			}
			//Get first void slot
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

		//Put the item in the first void slot if there is one
		if (noSlot)
		{
			if (firstVoidSlot != -1)
			{
				m_inventorySlot[firstVoidSlot].itemType = item;
				m_inventorySlot[firstVoidSlot].itemAmount += count;
			}
			else
			{
				noSpace = true;
			}
		}

		if (!noSpace)
		{
			//moving animation...
		}
	}
}

void Control3::resetContractCollect()
{
	auto def = UserDefault::getInstance();
	def->setIntegerForKey("collectSlot1", 0);
	def->setIntegerForKey("collectSlot2", 0);
	def->setIntegerForKey("collectSlot3", 0);
	def->setIntegerForKey("collectSlot4", 0);
	def->setIntegerForKey("collectSlot5", 0);
	def->setIntegerForKey("collectSlotCount1", 0);
	def->setIntegerForKey("collectSlotCount2", 0);
	def->setIntegerForKey("collectSlotCount3", 0);
	def->setIntegerForKey("collectSlotCount4", 0);
	def->setIntegerForKey("collectSlotCount5", 0);

	def->flush();
}

void Control3::assignEndingParameters(int buildingType)
{
	switch (buildingType)
	{
	case buildingData::HOUSE:
	case buildingData::HOUSE2:
	{
		m_trueEnding += 2;
		m_happyEnding++;
		m_goodEnding += 2;
	}
	break;

	case buildingData::CASTLE:
	{
		m_trueEnding += 2;
		m_worstEnding++;
		m_happyEnding++;
	}
	break;

	case buildingData::CLOTH_SHOP:
	{
		m_trueEnding++;
		m_goodEnding += 2;
		m_badEnding++;
	}
	break;

	case buildingData::WAREHOUSE:
	case buildingData::WAREHOUSE2:
	{
		m_trueEnding++;
		m_happyEnding++;
		m_goodEnding++;
		m_badEnding++;
		m_worstEnding++;
	}
	break;

	case buildingData::TOWER:
	case buildingData::TOWER2:
	{
		m_trueEnding++;
		m_happyEnding++;
		m_worstEnding += 2;
	}
	break;

	case buildingData::WEAPON_SHOP:
	case buildingData::ARMOR:
	{
		m_happyEnding += 2;
	}
	break;

	case buildingData::BASE:
	case buildingData::BASE2:
	{
		UserDefault::getInstance()->setBoolForKey("HasBuiltBase", true);
		UserDefault::getInstance()->flush();
		m_happyEnding++;
		m_badEnding += 2;
		m_worstEnding += 2;
	}
	break;

	case buildingData::MARKET:
	{
		m_badEnding += 2;
		m_worstEnding++;
		m_goodEnding++;
	}
	break;

	case buildingData::TAVERN:
	{
		m_worstEnding++;
		m_badEnding += 3;
	}
	break;
	
	case buildingData::FOOD:
	{
		m_happyEnding++;
		m_goodEnding += 3;
		m_worstEnding += 2;
		m_badEnding++;
	}
	break;

	case buildingData::CHURCH:
	{
		m_goodEnding += 3;
		m_badEnding += 2;
	}
	break;

	default:
		break;
	}
}

void Control3::confirmCrafting()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto tempRes1 = m_craftingResource1;
	auto tempRes2 = m_craftingResource2;

	if (tempRes1 > tempRes2)
	{
		tempRes1 = m_craftingResource2;
		tempRes2 = m_craftingResource1;
	}
	
	//if there is at least 2 resources
	if(tempRes1 != 0)
	{
		if (tempRes2 != 0)
		{
			//check if combination is right
			for (int i = 0; i < sizeof(resourceData::craftingList) / sizeof(*resourceData::craftingList); ++i)
			{
				if (resourceData::craftingList[i].first == tempRes1)
				{
					if (resourceData::craftingList[i].second == tempRes2)
					{
						//valid combination

						addItemToInventory(resourceData::craftingList[i].result, 1);

						m_craftingResource1 = 0;
						m_craftingResource2 = 0;
						m_foregroundNode->removeChildByName("craftingResource1");
						m_foregroundNode->removeChildByName("craftingResource2");
						m_foregroundNode->removeChildByName("craftingResource3");
						showItems(m_inventoryX, visibleSize.height / 1.43f);

						saveGame();
					}
				}
			}

		}
	}
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
			if (m_npcStateVector[i].buildingNum1 > buildingData::BUILDING_NONE)
			{
				popUp(m_npcVector[i]);
			}
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
				auto posX = m_buildingStart - ((((m_buildingModeIndex)* m_slotSize)) - ((buildingData::slotPerBuilding[m_buildingsVector[m_buildingModeIndex].buildingType] * m_slotSize)) / 2);
				auto diff = m_npcVector[i]->getPositionX() - posX;
				m_npcStateVector[i].directionRight = true;
				//Determine the npc direction.
				if (diff > 0)
				{
					m_npcStateVector[i].directionRight = false;
				}

				//(diff / 40) gives the time needed to travel to the destination, in seconds. Since NPCs move at 40 units per second.
				m_npcStateVector[i].stateDuration = abs(diff / 40.0f) + 1;
				//npcWalk(m_npcStateVector[i].directionRight, m_npcVector[i]);

				auto move = MoveTo::create(abs(diff / 40.0f), Vec2(posX ,m_npcVector[i]->getPositionY()));

				m_npcVector[i]->stopAllActions();

				if (m_npcStateVector[i].directionRight) //right
				{
					m_npcVector[i]->setFlippedX(true);
					m_npcVector[i]->runAction(RepeatForever::create(move));
				}
				else //left
				{
					m_npcVector[i]->setFlippedX(false);
					m_npcVector[i]->runAction(RepeatForever::create(move));
				}

				auto frame1 = RotateTo::create(0.1, -20);
				auto frame2 = RotateTo::create(0.1, 20);
				auto sequence = Sequence::create(frame1, frame2, NULL);
				m_npcVector[i]->runAction(RepeatForever::create(sequence));

			}

			if (!m_npcStateVector[i].buildingDone)
			{
				m_npcStateVector[i].timeCounter++;

				if ((m_buildingStart - ((((m_buildingModeIndex)* m_slotSize)) - ((buildingData::slotPerBuilding[m_buildingsVector[m_buildingModeIndex].buildingType] * m_slotSize)) / 2) == m_npcVector[i]->getPositionX()))
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
		int maxBuilding = buildingData::MAX_PREHISTORIC-4;
		int minType = 0;
		int minBuilding = 1;

		if (m_age == 1)
		{
			maxType = NPC_TYPE_MEDIEVAL_MAX - 1;
			maxBuilding = buildingData::MAX_MEDIEVAL - 1;
			minType = NPC_TYPE_PREHISTORIC_MAX + 1;
			minBuilding = buildingData::MAX_PREHISTORIC + 1;
		}

		randomState.npcType = cocos2d::RandomHelper::random_int(minType, maxType);
		randomState.buildingNum1 = cocos2d::RandomHelper::random_int(minBuilding, maxBuilding);
		//WARNING: npcStateVector and npcVector has to share the same index!
		m_npcStateVector.push_back(randomState);
		m_npcVector.push_back(Sprite::create(npcTypePath[m_npcStateVector[i].npcType]));
		m_npcVector[i]->setScale(0.636f);
		m_npcVector[i]->setZOrder(1);
		m_npcVector[i]->setPositionY(m_groundLevel + (abs(m_npcVector[i]->getBoundingBox().getMinY() - m_npcVector[i]->getBoundingBox().getMaxY()) / 2));
		m_npcVector[i]->setPositionX(cocos2d::RandomHelper::random_int(0, 800) + 1050);
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
		//m_speedX += 10 * direction;
		m_speedY += 1;
		auto tintRed = TintTo::create(0.05, Color3B::RED);
		auto delay = DelayTime::create(0.1);
		auto tintBack = TintTo::create(0.05, Color3B::WHITE);
		m_character->runAction(Sequence::create(tintRed, delay, tintBack, NULL))->setTag(2);

		m_effects->ShakeNode(m_character);
		m_effects->TickScreenWithColor(1, Color3B::RED);
	}
}

void Control3::resetPlayerActions()
{
	m_character->stopAllActionsByTag(1);
	m_character->setColor(Color3B::WHITE);
	m_character->setRotation(0);
}

void Control3::createBuilding(float posX, float npcfootY)
{
	auto audioEngine = SimpleAudioEngine::getInstance();

	auto building = Sprite::create(buildingData::buildingPath[m_npcStateVector[m_talkingNPCIndex].buildingNum1]);
	building->setScale((m_slotSize * buildingData::slotPerBuilding[m_npcStateVector[m_talkingNPCIndex].buildingNum1]) / building->getContentSize().width);
	building->setOpacity(0);
	m_gameNode->addChild(building);
	building->setPosition(Vec2(posX, m_groundLevel + (abs(building->getBoundingBox().getMinY() - building->getBoundingBox().getMaxY()) / 2)));
	m_buildingsVector[m_buildingModeIndex].sprite = building;

	auto anim = Animation::create();
	anim->addSpriteFrameWithFile("img/buildings/p_a01.png");
	anim->addSpriteFrameWithFile("img/buildings/p_a02.png");
	anim->addSpriteFrameWithFile(buildingData::buildingPath[m_npcStateVector[m_talkingNPCIndex].buildingNum1]);
	anim->setDelayPerUnit(5.3f);
	anim->setLoops(1);

	auto fadeIn = FadeIn::create(1.0f);

	auto animAction = Animate::create(anim);
	building->runAction(animAction);
	building->runAction(fadeIn);

	auto effectSprite = Sprite::create();
	effectSprite->setPositionX(building->getPositionX());
	effectSprite->setPositionY(npcfootY + ((building->getContentSize().height * m_gameNode->getScaleY() * building->getScaleY()) / 6));
	effectSprite->setScale(1);
	m_gameNode->addChild(effectSprite);

	auto effectAnim = Animation::create();
	/*
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_000.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_001.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_002.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_003.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_004.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_005.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_006.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_007.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_008.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/building_effect_009.png");
	*/
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/dust1.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/dust2.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/dust3.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/dust4.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/dust5.png");
	effectAnim->addSpriteFrameWithFile("img/effects/buildingEffect/dust6.png");

	effectAnim->setDelayPerUnit(0.3f);
	effectAnim->setRestoreOriginalFrame(true);
	effectAnim->setLoops(6);

	auto effectAnimAction = Animate::create(effectAnim);

	auto buildingId = audioEngine->playEffect("sound/sfx/main/building/nokogiri.mp3", true);

	effectSprite->runAction(effectAnimAction);
	//effectSprite->runAction(playBuildingSFX);

	auto completedSprite = Sprite::create();
	completedSprite->setPositionX(building->getPositionX());
	completedSprite->setPositionY(npcfootY + (building->getContentSize().height * m_gameNode->getScaleY() * building->getScaleY()));
	m_gameNode->addChild(completedSprite);

	auto completedAnim = Animation::create();
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_000.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_001.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_002.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_003.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_004.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_005.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_006.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_007.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_008.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_009.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_010.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_011.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_012.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_013.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_014.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_015.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_016.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_017.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_018.png");
	completedAnim->addSpriteFrameWithFileName("img/effects/completedEffect/completed_building_019.png");
	completedAnim->setDelayPerUnit(0.05f);
	completedAnim->setRestoreOriginalFrame(true);
	completedAnim->setLoops(2);

	auto completedAction = Animate::create(completedAnim);
	CallFunc* nextStep = CallFunc::create([]() {});
	if (m_age == 0 && m_isTutorial)
	{
		nextStep = CallFunc::create([this]() {this->nextStep(); });
	}
	
	auto stopBuildingSFX = CallFunc::create([audioEngine, buildingId]() {audioEngine->stopEffect(buildingId); });
	auto playCompletedSFX = CallFunc::create([audioEngine]() {audioEngine->playEffect("sound/sfx/main/building/completed.mp3"); });

	auto delay = DelayTime::create(12.0f);
	completedSprite->runAction(Sequence::create(delay, stopBuildingSFX, playCompletedSFX, completedAction, nextStep, NULL));
}

void Control3::update(float dt)
{
	if (!m_disableGame)
	{
		auto damage = DamageHandler::GetInstance();
		damage->ResetDamage();
		
		resourcePopup();
		simplePhysics();
		npcAI();

		auto sword = (Sprite*)m_gameNode->getChildByName("sword");
		sword->setPosition(m_character->getPosition());

		auto axe = (Sprite*)m_gameNode->getChildByName("axe");
		axe->setPosition(m_character->getPosition());

		if (m_actingInstanced)
		{
			if (m_enemy.collideWith(sword->getBoundingBox()) && sword->isVisible() && !m_attackOnce)
			{
				auto damage = DamageHandler::GetInstance();
				damage->DealDamageToMonster(m_damage);
				if (m_enemy.isDeadNextAttack())
				{
					spreadResource(m_enemy.getActingEnemyName(), m_enemy.GetStaticEnemyPos(m_enemy.getActingEnemyIndex()));
				}
				m_attackOnce = true;
			}
		}

		

		m_timeOfDay++;

		if (m_contractState == 1)
		{
			m_contractTime++;
			if (m_contractLevel == 1)
			{
				if (m_contractTime >= 3600)
				{
					m_contractState = 2;
					m_contractTime = 0;
					saveGame();
				}
			}
			else if (m_contractLevel == 2)
			{
				if (m_contractTime >= 7200)
				{
					m_contractState = 2;
					m_contractTime = 0;
					saveGame();
				}
			}
			else if (m_contractLevel == 3)
			{
				if (m_contractTime >= 11800)
				{
					m_contractState = 2;
					m_contractTime = 0;
					saveGame();
				}
			}

		}

		//25 min reset
		if (m_timeOfDay >= MAX_TIME_DAY)
		{
			m_timeOfDay = m_timeOfDay - MAX_TIME_DAY;
		}

		if (m_gameNode->getChildByName("tutorialCameraEye"))
		{
			static float incremental = 0;
			incremental += 0.01f;
			if (incremental >= 8.400097f)
			{
				incremental = 0;
			}
			m_gameNode->getChildByName("tutorialCameraEye")->setPositionX(m_gameNode->getChildByName("tutorialCameraEye")->getPositionX() + incremental);
		}
	}

	if (m_bgEnabled)
	{
		m_background.UpdateBackground(setViewpoint(m_character->getPosition()), m_timeOfDay);
		/*
		//if the map is the last scene of the tutorial
		if (m_age == 1 && m_isTutorial && !m_isTutorialHome)
		{

			m_background.UpdateBackground(setViewpoint(m_gameNode->getChildByName("tutorialCameraEye")->getPosition()), m_timeOfDay);
		}
		else
		{
			m_background.UpdateBackground(setViewpoint(m_character->getPosition()), m_timeOfDay);
		}
		*/
	}
	else
	{
		setViewpoint(m_character->getPosition());
	}

	//dmg calc
	m_enemy.receiveDamage(false, m_effects);
	receivePlayerDamage();

	updateHPBar();
}


///ƒ^ƒbƒ`ŠJŽn
void Control3::onTouchesBegan(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (!m_disableGame)
	{
		for (auto &t : touch)
		{
			Rect touchPoint = Rect(t->getLocation().x, t->getLocation().y, 1, 1);
			if (m_isUIEnabled) 
			{
				if (m_isMoveUIEnabled)
				{
					if (touchPoint.intersectsRect(m_rightRect))
					{
						auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
						//if (!m_swinging)
						{
							walk(true, m_character);
							m_directionRight = true;
							m_standing = false;
							if (m_onGround)
								m_animationInstanced = false;
							m_moveTouchID = t->getID();
							JoyStick->setPosition(visibleSize.width / 10 * 1.8, visibleSize.height / 6);
						}
					}
					else if (touchPoint.intersectsRect(m_leftRect))
					{
						auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
						//if (!m_swinging)
						{
							walk(false, m_character);
							m_directionRight = false;
							m_standing = false;
							if (m_onGround)
								m_animationInstanced = false;
							m_moveTouchID = t->getID();
							JoyStick->setPosition(visibleSize.width / 10 * 1.2, visibleSize.height / 6);
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
				}

				if (m_isJumpUIEnabled)
				{
					if (touchPoint.intersectsRect(m_jButtonRect))
					{
						m_jButtonPressed = true;
						m_animationInstanced = false;
						m_jumpTouchID = t->getID();
					}
				}

				if (m_isActionUIEnabled)
				{
					if (touchPoint.intersectsRect(m_aButtonRect))
					{
						m_aButtonPressed = true;
						m_actionTouchID = t->getID();
					}
				}

				if (m_isItemUIEnabled)
				{
					//building icon touch
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
								auto res1 = Sprite::create(resourceData::resourceIconPath[buildingData::buildingRecepe[buildingNum].resource1]);
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
								auto resCounter1 = Label::createWithTTF(count1, "fonts/misaki_gothic.ttf", fontSize * m_gameNode->getScale());
								resCounter1->setColor(Color3B::BLACK);
								resCounter1->enableOutline(Color4B::BLACK, outline);
								resCounter1->setPosition(res1->getPositionX(), res1->getPositionY() - 30);
								resCounter1->setName(RESOURCE_COUNT_1);
								m_gameNode->addChild(resCounter1);

								Sprite* res2 = nullptr;
								Label* resCounter2 = nullptr;

								if (buildingData::buildingRecepe[buildingNum].resource2 != buildingData::BUILDING_NONE)
								{
									res2 = Sprite::create(resourceData::resourceIconPath[buildingData::buildingRecepe[buildingNum].resource2]);
									res2->setPosition(icon->getPositionX() - 60, icon->getPositionY());
									res2->setName(RESOURCE_2);
									res2->setScale(resSize);
									m_gameNode->addChild(res2);
									ostr << buildingData::buildingRecepe[buildingNum].res2Count;
									std::string count2 = "x" + ostr.str();
									ostr.str("");
									resCounter2 = Label::createWithTTF(count2, "fonts/misaki_gothic.ttf", fontSize * m_gameNode->getScale());
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
										auto res3 = Sprite::create(resourceData::resourceIconPath[buildingData::buildingRecepe[buildingNum].resource3]);
										res3->setPosition(icon->getPositionX() + 60, icon->getPositionY());
										res3->setName(RESOURCE_3);
										res3->setScale(resSize);
										m_gameNode->addChild(res3);
										ostr << buildingData::buildingRecepe[buildingNum].res3Count;
										std::string count3 = "x" + ostr.str();
										auto resCounter3 = Label::createWithTTF(count3, "fonts/misaki_gothic.ttf", fontSize * m_gameNode->getScale());
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

					//building resource icon touch
					auto buildingRes1 = m_gameNode->getChildByName(RESOURCE_1);
					if (buildingRes1)
					{
						if (buildingRes1->getOpacity() == 255 && m_npcStateVector[m_talkingNPCIndex].isTalking)
						{
							auto popUpSprite = (Sprite*)m_gameNode->getChildByName(POPUP_SPRITE);
							auto resPos = m_gameNode->convertToWorldSpace(Vec2(popUpSprite->getBoundingBox().getMinX(), popUpSprite->getBoundingBox().getMinY()));
							auto iconRect = Rect(resPos.x, resPos.y, abs(popUpSprite->getBoundingBox().getMaxX() - popUpSprite->getBoundingBox().getMinX()) * m_gameNode->getScale(), abs(popUpSprite->getBoundingBox().getMaxY() - popUpSprite->getBoundingBox().getMinY()) * m_gameNode->getScale());
							if (iconRect.intersectsRect(touchPoint))
							{
								int buildingNum = m_npcStateVector[m_talkingNPCIndex].buildingNum1;
								int resType = buildingData::buildingRecepe[buildingNum].resource1;
								int resType1 = buildingData::buildingRecepe[buildingNum].resource1;
								int resType2 = buildingData::buildingRecepe[buildingNum].resource2;
								int resType3 = buildingData::buildingRecepe[buildingNum].resource3;

								//bool passed = true;
								int passCounter = 0;
								int index1 = -1;
								int index2 = -1;
								int index3 = -1;
								int passCondition = 1;
								
								if (resType2 != buildingData::BUILDING_NONE)
								{
									passCondition++;

									if (resType3 != buildingData::BUILDING_NONE)
									{
										passCondition++;
									}
								}

								for (int i = 0; i < 10; ++i)
								{
									if (m_inventorySlot[i].itemType == resType1)
									{
										if (m_inventorySlot[i].itemAmount >= buildingData::buildingRecepe[buildingNum].res1Count)
										{
											passCounter++;
											index1 = i;
										}
									}

									if (resType2 != buildingData::BUILDING_NONE)
									{
										if (m_inventorySlot[i].itemType == resType2)
										{

											if (m_inventorySlot[i].itemAmount >= buildingData::buildingRecepe[buildingNum].res2Count)
											{
												passCounter++;
												index2 = i;
											}
										}

										if (resType3 != buildingData::BUILDING_NONE)
										{
											if (m_inventorySlot[i].itemType == resType3)
											{
												if (m_inventorySlot[i].itemAmount >= buildingData::buildingRecepe[buildingNum].res3Count)
												{
													passCounter++;
													index3 = i;
												}
											}
										}
									}
								}
								
								if (passCounter >= passCondition)
								{
									enterBuildMode();
									removePopUp();
								}
							}
						}
					}

					//building touch
					for (int i = 0; i < m_buildingsVector.size(); ++i)
					{
						if (m_buildingsVector[i].sprite)
						{
							auto sprite = m_buildingsVector[i].sprite;

							auto resPos = m_gameNode->convertToWorldSpace(Vec2(sprite->getBoundingBox().getMinX(), sprite->getBoundingBox().getMinY()));
							//auto iconRect = Rect(resPos.x, resPos.y, buildingRes1->getContentSize().width * buildingRes1->getScale() * m_gameNode->getScale(), buildingRes1->getContentSize().height * buildingRes1->getScale() * m_gameNode->getScale());
							auto buildingRect = Rect(resPos.x, resPos.y, abs(sprite->getBoundingBox().getMaxX() - sprite->getBoundingBox().getMinX()) * m_gameNode->getScale(), abs(sprite->getBoundingBox().getMaxY() - sprite->getBoundingBox().getMinY()) * m_gameNode->getScale());
							if (buildingRect.intersectsRect(touchPoint) && icon == NULL)
							{
								sprite->setColor(Color3B::GRAY);
								m_interactingBuildingType = m_buildingsVector[i].buildingType;
								popUpMsg(MSG_TYPE::MSG_BUILDING_EFFECT);
							}
							else
							{
								sprite->setColor(Color3B::WHITE);
							}
						}
					}

					//Food touch
					if (!m_isCraftingTouch)
					{
						for (int i = 0; i < m_itemsVector.size(); ++i)
						{
							auto item = m_itemsVector[i];
							//if (item->getTag() == APPLE || item->getTag() == CACTUS || item->getTag() == MUSHROOM)
							{
								if (item->getBoundingBox().intersectsRect(touchPoint))
								{
									m_isCraftingTouch = true;
									
									if (m_inventorySlot[i].itemType == APPLE || m_inventorySlot[i].itemType == CACTUS || m_inventorySlot[i].itemType == MUSHROOM)
									{
										//Healing
										m_inventorySlot[i].itemAmount--;
										if (m_inventorySlot[i].itemAmount == 0)
										{
											m_inventorySlot[i].itemType = 0;
										}

										//heal for 5 seconds,
										auto delayPerTick = DelayTime::create(0.5f);
										auto healFunc = CallFunc::create([this]() {if (this->m_hp < 2000) { this->m_hp += 100; } });
										auto healingSequence = Sequence::create(healFunc, delayPerTick, NULL);
										auto loop = Repeat::create(healingSequence, 1);
										this->runAction(loop);

										//tick effect for 5 seconds with green color
										m_effects->TickScreenWithColor(2, Color3B(10, 255, 10));

										//m_hp += 100;
										showItems(m_inventoryX, m_inventoryY);

										auto audioEngine = SimpleAudioEngine::getInstance();
										audioEngine->playEffect("sound/sfx/main/resource/eating_sound.mp3");
									}
									else if (m_inventorySlot[i].itemType == MUSHROOM2) //poison mushroom
									{
										m_inventorySlot[i].itemAmount--;
										if (m_inventorySlot[i].itemAmount == 0)
										{
											m_inventorySlot[i].itemType = 0;
										}

										//dmg
										auto delayPerTick = DelayTime::create(0.5f);
										auto healFunc = CallFunc::create([this]() {if (this->m_hp > 10) { this->m_hp -= 5; } });
										auto healingSequence = Sequence::create(healFunc, delayPerTick, NULL);
										auto loop = Repeat::create(healingSequence, 5);
										this->runAction(loop);

										//tick effect for 5 seconds with green color
										m_effects->TickScreenWithColor(2, Color3B(157, 25, 246));

										//m_hp += 100;
										showItems(m_inventoryX, m_inventoryY);

										auto audioEngine = SimpleAudioEngine::getInstance();
										audioEngine->playEffect("sound/sfx/main/resource/eating_sound.mp3");
									}
									else
									{
										m_inventorySlot[i].itemAmount--;
										if (m_inventorySlot[i].itemAmount == 0)
										{
											m_inventorySlot[i].itemType = 0;
										}

										showItems(m_inventoryX, m_inventoryY);
									}
								}
							}
						}
					}
					
				}
			}

			if (m_isCrafting)
			{
				if (!m_isCraftingTouch)
				{
					//resource icon touch for crafting
					for (int i = 0; i < m_itemsVector.size(); ++i)
					{
						if (m_inventorySlot[i].itemType != RESOURCE_NONE)
						{
							auto item = m_itemsVector[i];
							if (item->getBoundingBox().intersectsRect(touchPoint))
							{
								bool noChange = false;
								m_isCraftingTouch = true;

								if (m_craftingResource1 == 0)
								{
									m_craftingResource1 = m_inventorySlot[i].itemType;
									m_inventorySlot[i].itemAmount--;
									if (m_inventorySlot[i].itemAmount == 0)
									{
										m_inventorySlot[i].itemType = 0;
									}
								}
								else if (m_craftingResource2 == 0)
								{
									m_craftingResource2 = m_inventorySlot[i].itemType;
									m_inventorySlot[i].itemAmount--;
									if (m_inventorySlot[i].itemAmount == 0)
									{
										m_inventorySlot[i].itemType = 0;
									}
								}
								else
								{
									noChange = true;
								}

								//show the crafting resource icons in the respective frames
								if (!noChange)
								{
									//resource 1
									m_foregroundNode->removeChildByName("craftingResource1");
									if (m_craftingResource1 != 0)
									{
										auto resource1 = Sprite::create(resourceData::resourceIconPath[m_craftingResource1]);
										resource1->setPosition(visibleSize.width / 8 * 2, visibleSize.height / 2.4f);
										resource1->setName("craftingResource1");
										resource1->setScale(2.3f);
										m_foregroundNode->addChild(resource1);
									}

									//resource 2
									m_foregroundNode->removeChildByName("craftingResource2");
									if (m_craftingResource2 != 0)
									{
										auto resource2 = Sprite::create(resourceData::resourceIconPath[m_craftingResource2]);
										resource2->setPosition(visibleSize.width / 2, visibleSize.height / 2.4f);
										resource2->setName("craftingResource2");
										resource2->setScale(2.3f);
										m_foregroundNode->addChild(resource2);
									}

									m_foregroundNode->removeChildByName("craftingResource3");

									auto tempRes1 = m_craftingResource1;
									auto tempRes2 = m_craftingResource2;

									if (tempRes1 > tempRes2)
									{
										tempRes1 = m_craftingResource2;
										tempRes2 = m_craftingResource1;
									}

									//if there is at least 2 resources
									if (tempRes1 != 0)
									{
										if (tempRes2 != 0)
										{
											//check if combination is right
											for (int i = 0; i < sizeof(resourceData::craftingList) / sizeof(*resourceData::craftingList); ++i)
											{
												if (resourceData::craftingList[i].first == tempRes1)
												{
													if (resourceData::craftingList[i].second == tempRes2)
													{
														//valid combination and preview the new resource
														auto resource3 = Sprite::create(resourceData::resourceIconPath[resourceData::craftingList[i].result]);
														resource3->setPosition(visibleSize.width / 8 * 6, visibleSize.height / 2.4f);
														resource3->setName("craftingResource3");
														resource3->setScale(2.3f);
														resource3->setOpacity(140);
														m_foregroundNode->addChild(resource3);
													}
												}
											}

										}
									}

									showItems(m_inventoryX, visibleSize.height / 1.43f);
								}
							}
						}
						
					}

					auto craftRes1 = m_foregroundNode->getChildByName("craftingResource1");
					if (craftRes1)
					{
						if (craftRes1->getBoundingBox().intersectsRect(touchPoint))
						{
							addItemToInventory(m_craftingResource1, 1);
							m_foregroundNode->removeChildByName("craftingResource1");
							m_foregroundNode->removeChildByName("craftingResource3");
							m_craftingResource1 = 0;
							showItems(m_inventoryX, visibleSize.height / 1.43f);
						}
					}

					auto craftRes2 = m_foregroundNode->getChildByName("craftingResource2");
					if (craftRes2)
					{
						if (craftRes2->getBoundingBox().intersectsRect(touchPoint))
						{
							addItemToInventory(m_craftingResource2, 1);
							m_foregroundNode->removeChildByName("craftingResource2");
							m_foregroundNode->removeChildByName("craftingResource3");
							m_craftingResource2 = 0;
							showItems(m_inventoryX, visibleSize.height / 1.43f);
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
			if (m_isUIEnabled)
			{
				if (m_isMoveUIEnabled)
				{
					if (touchPoint.intersectsRect(m_rightRect))
					{
						if (m_standing)
						{
							//if (!m_swinging)
							{
								walk(true, m_character);
								m_directionRight = true;
								m_standing = false;
								if (m_onGround)
									m_animationInstanced = false;
								m_moveTouchID = t->getID();
								//ƒWƒ‡ƒCƒXƒeƒBƒbƒNposˆÚ“®
								JoyStick->setPosition(visibleSize.width / 10 * 1.8, visibleSize.height / 6);
							}
						}
					}
					else if (touchPoint.intersectsRect(m_leftRect))
					{
						if (m_standing)
						{
							//if (!m_swinging)
							{
								walk(false, m_character);
								m_directionRight = false;
								m_standing = false;
								if (m_onGround)
									m_animationInstanced = false;
								m_moveTouchID = t->getID();
								//ƒWƒ‡ƒCƒXƒeƒBƒbƒNposˆÚ“®
								JoyStick->setPosition(visibleSize.width / 10 * 1.2, visibleSize.height / 6);

							}
						}
					}
					///‚±‚±‚ÉƒXƒgƒbƒvƒ{ƒ^ƒ“’Ç‰Á
					else if (touchPoint.intersectsRect(m_StopButton))
					{
						if (m_standing == false)
						{
							m_standing = true;
							if (m_onGround)
								m_animationInstanced = false;
							m_moveTouchID = -1;
							//ƒWƒ‡ƒCƒXƒeƒBƒbƒNposˆÚ“®
							JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
						}
					}
				}
				
			}
		}
	}
}

void Control3::onTouchesEnded(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (!m_disableGame)
	{
		m_isCraftingTouch = false;
		for (auto &t : touch)
		{
			auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
			if (t->getID() == m_moveTouchID)
			{
				m_standing = true;
				if (m_onGround)
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

void Control3::popUpMsg(int msgType)
{
	if (!m_isPopUpMsg)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();

		auto darkenScreen = m_effects->GetColorScreen();
		darkenScreen->setVisible(true);
		darkenScreen->setOpacity(100);
		darkenScreen->setColor(Color3B::BLACK);
		//darkenScreen->setZOrder(-10);
		m_isPopUpMsg = true;
		m_isUIEnabled = false;
		m_standing = true;
		if (m_onGround)
			m_animationInstanced = false;
		m_moveTouchID = -1;
		auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
		JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);

		switch (msgType)
		{

		case MSG_TYPE::MSG_TUTORIAL:
		{
			//--------------BG UI---------------------------------
			auto confirmBG = m_foregroundNode->getChildByName("confirmBG");
			if (!confirmBG)
			{
				m_foregroundNode->removeChildByName("confirmBG");
				confirmBG = Sprite::create("img/ui/popUpMsg/msgBG.png");
				confirmBG->setName("confirmBG");
				confirmBG->setZOrder(0);
				confirmBG->setScale(2.0f);
				confirmBG->setOpacity(240);
				confirmBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				m_foregroundNode->addChild(confirmBG);
			}
			confirmBG->setOpacity(230);
			confirmBG->setVisible(true);

			//------------Menu (Button) UI-----------------------------------
			auto confirmMenu = m_foregroundNode->getChildByName("confirmMenu");
			if (!confirmMenu)
			{
				m_foregroundNode->removeChildByName("confirmMenu");
				auto confirmItem = MenuItemImage::create("img/ui/popUpMsg/msgConfirm.png", "img/ui/popUpMsg/msgConfirmPressed.png", [&](Ref* sender) {
					this->closeMsg();
					this->nextStep();
				});
				confirmItem->setPositionX(visibleSize.width / 2);
				confirmItem->setPositionY((visibleSize.height / 2) - confirmBG->getContentSize().height + 100);
				confirmItem->setName("confirmItem");
				confirmItem->setEnabled(true);

				confirmMenu = Menu::create(confirmItem, NULL);
				confirmMenu->setName("confirmMenu");
				confirmMenu->setScale(1.5f);
				confirmMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(confirmMenu);
			}

			confirmMenu->setOpacity(200);
			auto item = (MenuItemImage*)confirmMenu->getChildByName("confirmItem");
			item->setEnabled(true);
			confirmMenu->setVisible(true);

			//-----------MSG Text UI-----------------------------------------------
			auto msgText = m_foregroundNode->getChildByName("msgText");
			if (!msgText)
			{
				m_foregroundNode->removeChildByName("msgText");
				msgText = Label::createWithTTF("タイムマシーンのパーツが各時代に落ちてしまったようだ！　なんとか取り戻さなくてはならない。。。", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
				msgText->setPosition(visibleSize.width / 2, (visibleSize.height / 1.85f));
				msgText->setName("msgText");
				m_foregroundNode->addChild(msgText);
			}

			msgText->setVisible(true);

		}
		break;

		case MSG_TYPE::MSG_CONFIRM:
		{
			//--------------BG UI---------------------------------
			auto confirmBG = m_foregroundNode->getChildByName("confirmBG");
			if (!confirmBG)
			{
				m_foregroundNode->removeChildByName("confirmBG");
				confirmBG = Sprite::create("img/ui/popUpMsg/msgBG.png");
				confirmBG->setName("confirmBG");
				confirmBG->setScale(2.0f);
				confirmBG->setZOrder(0);
				confirmBG->setOpacity(200);
				confirmBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				m_foregroundNode->addChild(confirmBG);
			}
			confirmBG->setOpacity(230);
			confirmBG->setVisible(true);

			//------------Menu (Button) UI-----------------------------------
			auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
			if (!cancelMenu)
			{
				m_foregroundNode->removeChildByName("cancelMenu");
				auto cancelItem = MenuItemImage::create("img/ui/popUpMsg/msgCancel.png", "img/ui/popUpMsg/msgCancelPressed.png", [&](Ref* sender) {
					this->closeMsg();
				});
				cancelItem->setPositionX(visibleSize.width / 1.67f);
				cancelItem->setPositionY(visibleSize.height / 3.4);
				cancelItem->setScale(0.7f);
				cancelItem->setName("cancelItem");
				cancelItem->setEnabled(true);

				cancelMenu = Menu::create(cancelItem, NULL);
				cancelMenu->setName("cancelMenu");
				//cancelMenu->setScale(1.5f);
				cancelMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(cancelMenu);
			}
			cancelMenu->setTag(1); //Yes character knock back
			cancelMenu->setOpacity(230);
			auto itemCancel = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
			itemCancel->setEnabled(true);
			itemCancel->setScale(1.7f);
			itemCancel->setPositionX((visibleSize.width / 2) + 110);
			itemCancel->setPositionY(visibleSize.height / 3.4);
			cancelMenu->setVisible(true);

			auto confirmMenu = m_foregroundNode->getChildByName("confirmMenu");
			//if (!confirmMenu)
			{
				m_foregroundNode->removeChildByName("confirmMenu");
				auto confirmItem = MenuItemImage::create("img/ui/popUpMsg/msgConfirm.png", "img/ui/popUpMsg/msgConfirmPressed.png", [&](Ref* sender) {this->changeArea(mapData::HOME_MAP); });
				confirmItem->setPositionX(visibleSize.width / 2);
				confirmItem->setPositionY((visibleSize.height / 2) - confirmBG->getContentSize().height + 100);
				confirmItem->setName("confirmItem");
				confirmItem->setEnabled(true);

				confirmMenu = Menu::create(confirmItem, NULL);
				confirmMenu->setName("confirmMenu");
				//confirmMenu->setScale(1.5f);
				confirmMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(confirmMenu);
			}

			confirmMenu->setOpacity(230);
			auto itemConfirm = (MenuItemImage*)confirmMenu->getChildByName("confirmItem");
			itemConfirm->setEnabled(true);
			itemConfirm->setScale(1.7);
			itemConfirm->setPositionX((visibleSize.width / 2) - 110);
			itemConfirm->setPositionY(visibleSize.height / 3.4);
			confirmMenu->setVisible(true);

			//-----------MSG Text UI-----------------------------------------------
			auto confirmText = m_foregroundNode->getChildByName("confirmText");
			if (!confirmText)
			{
				m_foregroundNode->removeChildByName("confirmText");
				confirmText = Label::createWithTTF("街に行きますか？", "fonts/PixelMplus10-Regular.ttf", 50);
				confirmText->setPosition(visibleSize.width / 2, (visibleSize.height / 2));
				confirmText->setName("confirmText");
				m_foregroundNode->addChild(confirmText);
			}

			confirmText->setVisible(true);
		}
		break;

		case MSG_TYPE::MSG_CHANGE_MAP:
		{
			//--------------BG UI---------------------------------
			auto confirmBG = m_foregroundNode->getChildByName("confirmBG");
			if (!confirmBG)
			{
				m_foregroundNode->removeChildByName("confirmBG");
				confirmBG = Sprite::create("img/ui/popUpMsg/msgBG.png");
				confirmBG->setName("confirmBG");
				confirmBG->setScale(2.0f);
				confirmBG->setZOrder(0);
				confirmBG->setOpacity(230);
				confirmBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				m_foregroundNode->addChild(confirmBG);
			}
			confirmBG->setOpacity(230);
			confirmBG->setVisible(true);

			//------------Menu (Button) UI-----------------------------------
			auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
			//if (!cancelMenu)
			{
				m_foregroundNode->removeChildByName("cancelMenu");
				auto cancelItem = MenuItemImage::create("img/ui/popUpMsg/msgCancel.png", "img/ui/popUpMsg/msgCancelPressed.png", [&](Ref* sender) {
					this->closeMsg();
					if (m_fromContract)
					{
						//this->m_contractLevel = 0;
						this->m_fromContract = false;
						//this->popUpMsg(MSG_TYPE::MSG_CONTRACT);
					}
				});
				cancelItem->setPositionX(visibleSize.width / 1.67f);
				cancelItem->setPositionY((visibleSize.height / 2) + confirmBG->getContentSize().height - 86);
				cancelItem->setScale(0.7f);
				cancelItem->setName("cancelItem");
				cancelItem->setEnabled(true);

				cancelMenu = Menu::create(cancelItem, NULL);
				cancelMenu->setName("cancelMenu");
				//cancelMenu->setScale(1.5f);
				cancelMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(cancelMenu);
			}
			cancelMenu->setTag(1); //Yes character knock back
			cancelMenu->setOpacity(200);
			auto item = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
			item->setEnabled(true);
			item->setScale(1.7f);
			item->setPositionX(visibleSize.width / 1.57f);
			item->setPositionY(visibleSize.height / 3.4f);
			cancelMenu->setVisible(true);

			//-----------MSG Text UI-----------------------------------------------
			auto areaText = m_foregroundNode->getChildByName("areaText");
			//if (!areaText)
			{
				m_foregroundNode->removeChildByName("areaText");
				if (m_fromContract)
				{
					areaText = Label::createWithTTF("派遣するエリア選択", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
				}
				else
				{
					areaText = Label::createWithTTF("エリア選択", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
				}
				areaText->setPosition(visibleSize.width / 2, (visibleSize.height / 1.38f));
				areaText->setName("areaText");
				m_foregroundNode->addChild(areaText);
			}

			areaText->setVisible(true);

			//-----------Scroll View UI---------------------------------------------
			auto areaScroll = (ScrollView*)m_foregroundNode->getChildByName("areaScroll");
			areaScroll = ScrollView::create();
			areaScroll->setContentSize(Size(visibleSize.width / 2.2, visibleSize.height / 3));
			areaScroll->setInnerContainerSize(Size(visibleSize.width / 2, 120 * m_accesibleAreas.size()));
			areaScroll->setAnchorPoint(Vec2(0.5f, 0.5f));
			areaScroll->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.95));
			areaScroll->setDirection(ScrollView::Direction::VERTICAL);
			areaScroll->setBackGroundColorType(LayoutBackGroundColorType::SOLID);
			areaScroll->setBackGroundColor(Color3B::BLACK);
			areaScroll->setBackGroundColorOpacity(0);
			areaScroll->setBounceEnabled(true);
			areaScroll->setName("areaScroll");
			areaScroll->setScrollBarEnabled(true);
			areaScroll->setScrollBarAutoHideEnabled(false);
			areaScroll->setScrollBarOpacity(230);
			areaScroll->setScrollBarColor(Color3B(149, 54, 39));
			m_foregroundNode->addChild(areaScroll);
			int areaButtonCounter = 0;
			for (auto &area : m_accesibleAreas)
			{
				auto areaButtton = Button::create("img/ui/popUpMsg/areaButton.png");
				areaButtton->setScale(2.0f);
				areaButtton->setPosition(Vec2(areaScroll->getContentSize().width/2, areaScroll->getInnerContainerSize().height - (areaButtonCounter * 68 * 1.5f) - (areaButtton->getContentSize().height)));
				Label* areaName;
				
				areaName = Label::createWithTTF(mapData::mapName[area], "fonts/PixelMplus10-Regular.ttf", 39);
				if (m_fromContract)
				{
					areaButtton->addClickEventListener([&](Ref* sender) {
						m_contractState = 1;
						m_contractTime = 0;
						m_contractArea = area;
						this->saveGame();
						m_fromContract = false;
						this->closeMsg();
						if (m_contractLevel == 1)
						{
							m_gold -= 10;
						}
						else if (m_contractLevel == 2)
						{
							m_gold -= 20;
						}
						else if (m_contractLevel == 3)
						{
							m_gold -= 30;
						}
						this->popUpMsg(MSG_TYPE::MSG_BUILDING_EFFECT);
					});
				}
				else
				{
					areaButtton->addClickEventListener([&](Ref* sender) {this->changeArea(area); });
				}


				areaName->setPosition(Vec2(areaScroll->getContentSize().width / 2, areaScroll->getInnerContainerSize().height - (areaButtonCounter * 68 * 1.5f) - (areaButtton->getContentSize().height)));
				
				areaScroll->addChild(areaButtton);
				areaScroll->addChild(areaName);
				areaButtonCounter++;
			}

			//-----
		}
		break;

		case MSG_TYPE::MSG_BUILD_MODE:
		{
			darkenScreen->setOpacity(0);

			showItems(m_inventoryX, m_inventoryY + 1000);

			//------------Menu (Button) UI-----------------------------------
			auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
			//if (!cancelMenu)
			{
				m_foregroundNode->removeChildByName("cancelMenu");
				auto cancelItem = MenuItemImage::create("img/ui/popUpMsg/msgCancel.png", "img/ui/popUpMsg/msgCancelPressed.png", [&](Ref* sender) {
					this->closeMsg();
				});
				cancelItem->setPositionX(visibleSize.width / 1.67f);
				//cancelItem->setPositionY((visibleSize.height / 2) + confirmBG->getContentSize().height - 86);
				cancelItem->setScale(0.7f);
				cancelItem->setName("cancelItem");
				cancelItem->setEnabled(true);

				cancelMenu = Menu::create(cancelItem, NULL);
				cancelMenu->setName("cancelMenu");
				//cancelMenu->setScale(1.5f);
				cancelMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(cancelMenu);
			}
			cancelMenu->setTag(0); //No character knock back
			cancelMenu->setOpacity(200);
			auto itemCancel = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
			itemCancel->setEnabled(true);
			itemCancel->setScale(1.5f);
			itemCancel->setPositionX((visibleSize.width / 2) + 150);
			itemCancel->setPositionY(visibleSize.height / 6);
			cancelMenu->setVisible(true);

			auto confirmMenu = m_foregroundNode->getChildByName("confirmMenu");
			//if (!confirmMenu)
			{
				m_foregroundNode->removeChildByName("confirmMenu");
				auto confirmItem = MenuItemImage::create("img/ui/popUpMsg/msgConfirm.png", "img/ui/popUpMsg/msgConfirmPressed.png", [&](Ref* sender) {
					this->confirmBuildMode();
				});
				confirmItem->setPositionX(visibleSize.width / 2);
				//confirmItem->setPositionY((visibleSize.height / 2) - confirmBG->getContentSize().height + 100);
				confirmItem->setName("confirmItem");
				confirmItem->setEnabled(true);

				confirmMenu = Menu::create(confirmItem, NULL);
				confirmMenu->setName("confirmMenu");
				//confirmMenu->setScale(1.5f);
				confirmMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(confirmMenu);
			}

			confirmMenu->setOpacity(200);
			auto itemConfirm = (MenuItemImage*)confirmMenu->getChildByName("confirmItem");
			itemConfirm->setEnabled(true);
			itemConfirm->setScale(1.5f);
			itemConfirm->setPositionX((visibleSize.width / 2) - 150);
			itemConfirm->setPositionY(visibleSize.height / 6);
			confirmMenu->setVisible(true);

			//-----------Arrow (Menu) UI-------------------------------------------

			auto arrowMenu = m_foregroundNode->getChildByName("arrowMenu");
			if (!arrowMenu)
			{
				m_foregroundNode->removeChildByName("arrowMenu");
				auto arrowLeftItem = MenuItemImage::create("img/ui/popUpMsg/green-arrow.png", "img/ui/popUpMsg/green-arrowPressed.png", [&](Ref* sender) {
					this->moveBuildMode(false);
				});
				arrowLeftItem->setPositionX(visibleSize.width / 5);
				arrowLeftItem->setPositionY(visibleSize.height / 6);
				arrowLeftItem->setScale(0.7f);
				arrowLeftItem->setRotation(-90);
				arrowLeftItem->setName("arrowLeftItem");
				arrowLeftItem->setEnabled(true);

				auto arrowRightItem = MenuItemImage::create("img/ui/popUpMsg/green-arrow.png", "img/ui/popUpMsg/green-arrowPressed.png", [&](Ref* sender) {
					this->moveBuildMode(true);
				});
				arrowRightItem->setPositionX(visibleSize.width / 1.3f);
				arrowRightItem->setPositionY(visibleSize.height / 6);
				arrowRightItem->setScale(0.7f);
				arrowRightItem->setName("arrowRightItem");
				arrowRightItem->setRotation(90);
				arrowRightItem->setEnabled(true);

				arrowMenu = Menu::create(arrowLeftItem, arrowRightItem, NULL);
				arrowMenu->setName("arrowMenu");
				//arrowMenu->setScale(1.5f);
				arrowMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(arrowMenu);
			}

			arrowMenu->setOpacity(240);
			auto itemRight = (MenuItemImage*)arrowMenu->getChildByName("arrowRightItem");
			itemRight->setEnabled(true);
			auto itemLeft = (MenuItemImage*)arrowMenu->getChildByName("arrowLeftItem");
			itemLeft->setEnabled(true);
			arrowMenu->setVisible(true);

			//-----------MSG Text UI-----------------------------------------------
			auto buildText = m_foregroundNode->getChildByName("buildText");
			if (!buildText)
			{
				m_foregroundNode->removeChildByName("buildText");
				buildText = Label::createWithTTF("建物の配置を決めてください", "fonts/PixelMplus10-Regular.ttf", 50);
				buildText->setPosition(visibleSize.width / 2, (visibleSize.height / 1.13f));
				buildText->setName("buildText");
				m_foregroundNode->addChild(buildText);
			}

			buildText->setVisible(true);
		}
		break;

		case MSG_TYPE::MSG_BUILDING_EFFECT:
		{
			auto buildingName = m_foregroundNode->getChildByName("buildingName");
			m_foregroundNode->removeChild(buildingName);
			buildingName = Label::createWithTTF(buildingData::buildingName[m_interactingBuildingType], "fonts/PixelMplus10-Regular.ttf", 50);
			buildingName->setName("buildingName");
			buildingName->setZOrder(2);
			m_foregroundNode->addChild(buildingName);

			switch (m_interactingBuildingType)
			{
			case buildingData::BASE: 
			case buildingData::BASE2:
			{
				//--------------BG UI---------------------------------
				auto confirmBG = m_foregroundNode->getChildByName("confirmBG");
				if (!confirmBG)
				{
					m_foregroundNode->removeChildByName("confirmBG");
					confirmBG = Sprite::create("img/ui/popUpMsg/msgBG.png");
					confirmBG->setName("confirmBG");
					confirmBG->setZOrder(0);
					confirmBG->setScale(2.0f);
					confirmBG->setOpacity(200);
					confirmBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
					m_foregroundNode->addChild(confirmBG);
				}
				confirmBG->setOpacity(230);
				confirmBG->setVisible(true);

				//------------Menu (Button) UI-----------------------------------
				auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
				//if (!cancelMenu)
				{
					m_foregroundNode->removeChildByName("cancelMenu");
					auto cancelItem = MenuItemImage::create("img/ui/popUpMsg/msgCancel.png", "img/ui/popUpMsg/msgCancelPressed.png", [&](Ref* sender) {
						this->closeMsg();
					});
					cancelItem->setPositionX(visibleSize.width / 1.67f);
					cancelItem->setPositionY((visibleSize.height / 2) - confirmBG->getContentSize().height - 86);
					cancelItem->setScale(0.7f);
					cancelItem->setName("cancelItem");
					cancelItem->setEnabled(true);

					cancelMenu = Menu::create(cancelItem, NULL);
					cancelMenu->setName("cancelMenu");
					//cancelMenu->setScale(1.5f);
					cancelMenu->setPosition(Vec2::ZERO);

					m_foregroundNode->addChild(cancelMenu);
				}
				cancelMenu->setOpacity(200);
				cancelMenu->setTag(0); //No character knock back
				auto item = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
				item->setEnabled(true);
				item->setScale(1.7f);
				item->setPositionX(visibleSize.width / 1.47f);
				item->setPositionY(visibleSize.height / 3.4f);
				cancelMenu->setVisible(true);

				//--------------Building Name UI-----------------------------------
				buildingName->setPosition(visibleSize.width / 2, (visibleSize.height / 1.38f));

				//--------------Craft and Contract UI------------------------------
				auto craftButton = (Button*)m_foregroundNode->getChildByName("craftButton");
				if (!craftButton)
				{
					m_foregroundNode->removeChild(craftButton);
					craftButton = Button::create("img/ui/popUpMsg/areaButton.png");
					craftButton->setName("craftButton");
					craftButton->setScale(2.0f);
					craftButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2.45f));
					craftButton->addClickEventListener([&](Ref* sender) {
						this->closeMsg();
						this->popUpMsg(MSG_TYPE::MSG_CRAFTING);
					});

					Label* craftText;
					craftText = Label::createWithTTF("素材合成", "fonts/PixelMplus10-Regular.ttf", 39);
					craftText->setName("craftText");
					craftText->setPosition(Vec2(craftButton->getPosition()));

					m_foregroundNode->addChild(craftButton);
					m_foregroundNode->addChild(craftText);
				}
				craftButton->setVisible(true);
				m_foregroundNode->getChildByName("craftText")->setVisible(true);

				auto contractButton = (Button*)m_foregroundNode->getChildByName("contractButton");
				if (!contractButton)
				{
					m_foregroundNode->removeChild(contractButton);
					contractButton = Button::create("img/ui/popUpMsg/areaButton.png");
					contractButton->setName("contractButton");
					contractButton->setScale(2.0f);
					contractButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.70f));
					contractButton->addClickEventListener([&](Ref* sender) {
						this->closeMsg();
						this->popUpMsg(MSG_TYPE::MSG_CONTRACT);
					});

					Label* contractText;
					contractText = Label::createWithTTF("派遣", "fonts/PixelMplus10-Regular.ttf", 39);
					contractText->setName("contractText");
					contractText->setPosition(Vec2(contractButton->getPosition()));

					m_foregroundNode->addChild(contractButton);
					m_foregroundNode->addChild(contractText);
				}
				contractButton->setVisible(true);
				m_foregroundNode->getChildByName("contractText")->setVisible(true);
			}
			break;

			case buildingData::TIME_MACHINE01:
			case buildingData::TIME_MACHINE02:
			case buildingData::TIME_MACHINE03:
			{
				darkenScreen->setOpacity(180);
				m_uiNode->setVisible(false);

				showItems(m_inventoryX, m_inventoryY + 1000);

				auto def = UserDefault::getInstance();

				bool hasTM1 = def->getBoolForKey("HasTimeMachinePart1");
				bool hasTM2 = def->getBoolForKey("HasTimeMachinePart2");
				bool hasTM3 = def->getBoolForKey("HasTimeMachinePart3");

				CCLOG("age: %i", m_age);

				//time machine portrait
				auto timeMachine = (Sprite*)m_foregroundNode->getChildByName("timeMachinePortrait");
				if (!timeMachine)
				{
					m_foregroundNode->removeChild(timeMachine);

					if (def->getBoolForKey("HasTimeMachinePart3"))
					{
						timeMachine = Sprite::create("img/timeMachine/TimeMachine4.png");
					}
					else if (def->getBoolForKey("HasTimeMachinePart2"))
					{
						timeMachine = Sprite::create("img/timeMachine/TimeMachine3.png");
					}
					else
					{
						timeMachine = Sprite::create("img/timeMachine/TimeMachine2.png");
					}
					

					timeMachine->setName("timeMachinePortrait");
					timeMachine->setPosition(visibleSize.width / 4, visibleSize.height / 2.4);
					timeMachine->setScale(1.9f);
					m_foregroundNode->addChild(timeMachine);
				}

				timeMachine->setVisible(true);

				// time machine part 1
				auto timeMachinePart1 = (Sprite*)m_foregroundNode->getChildByName("timeMachinePart1");
				if (!timeMachinePart1)
				{
					m_foregroundNode->removeChild(timeMachinePart1);
					timeMachinePart1 = Sprite::create("img/timeMachine/TimeMachinePart1.png");
					timeMachinePart1->setName("timeMachinePart1");
					if (!def->getBoolForKey("HasTimeMachinePart1"))
					{
						timeMachinePart1->setColor(Color3B(30, 30, 30));
					}
					timeMachinePart1->setPosition(visibleSize.width / 8 * 2, visibleSize.height / 4 * 3);
					timeMachinePart1->setScale(1.8f);
					m_foregroundNode->addChild(timeMachinePart1);
				}

				timeMachinePart1->setVisible(true);

				// time machine part 2

				auto timeMachinePart2 = (Sprite*)m_foregroundNode->getChildByName("timeMachinePart2");
				if (!timeMachinePart2)
				{
					m_foregroundNode->removeChild(timeMachinePart2);
					timeMachinePart2 = Sprite::create("img/timeMachine/TimeMachinePart2.png");
					timeMachinePart2->setName("timeMachinePart2");
					if (!def->getBoolForKey("HasTimeMachinePart2"))
					{
						timeMachinePart2->setColor(Color3B(30, 30, 30));
					}
					timeMachinePart2->setPosition(visibleSize.width / 8 * 4, visibleSize.height / 4 * 3);
					timeMachinePart2->setScale(1.8f);
					m_foregroundNode->addChild(timeMachinePart2);
				}

				timeMachinePart2->setVisible(true);

				// time machine part 3

				auto timeMachinePart3 = (Sprite*)m_foregroundNode->getChildByName("timeMachinePart3");
				if (!timeMachinePart3)
				{
					m_foregroundNode->removeChild(timeMachinePart3);
					timeMachinePart3 = Sprite::create("img/timeMachine/TimeMachinePart3.png");
					timeMachinePart3->setName("timeMachinePart3");
					if (!def->getBoolForKey("HasTimeMachinePart3"))
					{
						timeMachinePart3->setColor(Color3B(30, 30, 30));
					}
					timeMachinePart3->setPosition(visibleSize.width / 8 * 6, visibleSize.height / 4 * 3);
					timeMachinePart3->setScale(1.8f);
					m_foregroundNode->addChild(timeMachinePart3);
				}

				timeMachinePart3->setVisible(true);

				//------------Menu (Button) UI-----------------------------------
				auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
				//if (!cancelMenu)
				{
					m_foregroundNode->removeChildByName("cancelMenu");
					auto cancelItem = MenuItemImage::create("img/ui/popUpMsg/msgCancel.png", "img/ui/popUpMsg/msgCancelPressed.png", [&](Ref* sender) {
						this->closeMsg();
					});
					cancelItem->setPositionX(visibleSize.width / 1.67f);
					cancelItem->setPositionY(visibleSize.height / 2);
					cancelItem->setScale(0.7f);
					cancelItem->setName("cancelItem");
					cancelItem->setEnabled(true);

					cancelMenu = Menu::create(cancelItem, NULL);
					cancelMenu->setName("cancelMenu");
					//cancelMenu->setScale(1.5f);
					cancelMenu->setPosition(Vec2::ZERO);

					m_foregroundNode->addChild(cancelMenu);
				}
				cancelMenu->setOpacity(255);
				cancelMenu->setTag(0); //No character knock back
				auto item = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
				item->setEnabled(true);
				item->setScale(1.7f);
				item->setPositionX(visibleSize.width / 8 * 5);
				item->setPositionY(visibleSize.height / 5.9f);
				cancelMenu->setVisible(true);

				//--------------Building Name UI-----------------------------------
				buildingName->setPosition(visibleSize.width / 2, (visibleSize.height / 1.1f));

				//--------------next Age and go back UI------------------------------
				auto nextAge = (Button*)m_foregroundNode->getChildByName("nextAge");
				if (!nextAge)
				{
					m_foregroundNode->removeChild(nextAge);
					nextAge = Button::create("img/ui/popUpMsg/areaButton.png");
					nextAge->setName("nextAge");
					nextAge->setScale(2.0f);
					nextAge->setPosition(Vec2(visibleSize.width / 8 * 5, visibleSize.height / 2.0f));
					if (m_age == 0)
					{
						if (hasTM1 && hasTM2)
						{
							nextAge->addClickEventListener([&](Ref* sender) {
								this->m_age++;
								this->changeArea(0);
							});
							
						}
					}
					else if (m_age == 1)
					{
						if (hasTM3)
						{
							nextAge->addClickEventListener([&](Ref* sender) {
								//this->m_age++;
								Director::getInstance()->replaceScene(Ending::createScene());
							});
						}
					}
					

					Label* nextAgeText;
					nextAgeText = Label::createWithTTF("次の時代に行く", "fonts/PixelMplus10-Regular.ttf", 39);
					nextAgeText->setName("nextAgeText");
					nextAgeText->setPosition(Vec2(nextAge->getPosition()));

					m_foregroundNode->addChild(nextAge);
					m_foregroundNode->addChild(nextAgeText);
				}
				nextAge->setVisible(true);
				m_foregroundNode->getChildByName("nextAgeText")->setVisible(true);

				auto pastAge = (Button*)m_foregroundNode->getChildByName("pastAge");
				if (!pastAge)
				{
					m_foregroundNode->removeChild(pastAge);
					pastAge = Button::create("img/ui/popUpMsg/areaButton.png");
					pastAge->setName("pastAge");
					pastAge->setScale(2.0f);
					pastAge->setPosition(Vec2(visibleSize.width / 8 * 5, visibleSize.height / 2.9f));
					if (m_age == 0)
					{
						pastAge->setColor(Color3B::GRAY);
					}
					if (m_age == 1)
					{
						pastAge->addClickEventListener([&](Ref* sender) {
							m_age--;
							changeArea(0);
						});
					}

					Label* pastAgeText;
					pastAgeText = Label::createWithTTF("過去の時代に行く", "fonts/PixelMplus10-Regular.ttf", 39);
					pastAgeText->setName("pastAgeText");
					pastAgeText->setPosition(Vec2(pastAge->getPosition()));

					m_foregroundNode->addChild(pastAge);
					m_foregroundNode->addChild(pastAgeText);
				}
				pastAge->setVisible(true);
				m_foregroundNode->getChildByName("pastAgeText")->setVisible(true);
			}
			break;

			default:
			{
				//m_gold++;
				closeMsg();
			}
				break;
			}
		}
		break;

		case MSG_TYPE::MSG_CRAFTING:
		{
			darkenScreen->setOpacity(180);
			m_uiNode->setVisible(false);
			m_isCrafting = true;
			/*
			m_isUIEnabled = true;
			m_isMoveUIEnabled = false;
			m_isActionUIEnabled = false;
			m_isItemUIEnabled = true;
			m_isJumpUIEnabled = false;
			*/

			//-------------Inventory slots UI---------------------------------
			auto inventorySlots = m_foregroundNode->getChildByName("inventorySlots");
			if (!inventorySlots)
			{
				m_foregroundNode->removeChildByName("inventorySlots");
				inventorySlots = Sprite::create("img/ui/inventory.png");
				inventorySlots->setPosition(visibleSize.width / 2, visibleSize.height / 1.43f);
				inventorySlots->setName("inventorySlots");
				inventorySlots->setZOrder(0);
				m_foregroundNode->addChild(inventorySlots);
			}
			inventorySlots->setVisible(true);

			showItems(m_inventoryX, inventorySlots->getPositionY());

			//------------Crafting frames UI---------------------------------
			//frame + frame = frame

			auto framePosY = visibleSize.height / 2.4f;

			auto frame01 = m_foregroundNode->getChildByName("frame01");
			if (!frame01)
			{
				m_foregroundNode->removeChildByName("frame01");
				frame01 = Sprite::create("img/ui/popUpMsg/frame.png");
				frame01->setPosition((visibleSize.width / 8 * 2), framePosY);
				frame01->setName("frame01");
				frame01->setScale(1.5f);
				m_foregroundNode->addChild(frame01);
			}
			frame01->setVisible(true);

			auto frame02 = m_foregroundNode->getChildByName("frame02");
			if (!frame02)
			{
				m_foregroundNode->removeChildByName("frame02");
				frame02 = Sprite::create("img/ui/popUpMsg/frame.png");
				frame02->setPosition(visibleSize.width / 2, framePosY);
				frame02->setName("frame02");
				frame02->setScale(1.5f);
				m_foregroundNode->addChild(frame02);
			}
			frame02->setVisible(true);

			auto frame03 = m_foregroundNode->getChildByName("frame03");
			if (!frame03)
			{
				m_foregroundNode->removeChildByName("frame03");
				frame03 = Sprite::create("img/ui/popUpMsg/frame.png");
				frame03->setPosition(visibleSize.width / 8 * 6, framePosY);
				frame03->setName("frame03");
				frame03->setScale(1.5f);
				m_foregroundNode->addChild(frame03);
			}
			frame03->setVisible(true);

			auto plusMark = m_foregroundNode->getChildByName("plusMark");
			if (!plusMark);
			{
				m_foregroundNode->removeChildByName("plusMark");
				plusMark = Sprite::create("img/ui/popUpMsg/+.png");
				plusMark->setPosition(visibleSize.width / 8 * 3, framePosY);
				plusMark->setName("plusMark");
				plusMark->setScale(1.5f);
				m_foregroundNode->addChild(plusMark);
			}
			plusMark->setVisible(true);

			auto equalMark = m_foregroundNode->getChildByName("equalMark");
			if (!plusMark);
			{
				m_foregroundNode->removeChildByName("equalMark");
				equalMark = Sprite::create("img/ui/popUpMsg/=.png");
				equalMark->setPosition(visibleSize.width / 8 * 5, framePosY);
				equalMark->setName("equalMark");
				equalMark->setScale(1.5f);
				m_foregroundNode->addChild(equalMark);
			}
			equalMark->setVisible(true);

			//------------Crafting confirm UI----------------------------------
			auto craftConfirm = (Button*)m_foregroundNode->getChildByName("craftConfirm");
			if (!craftConfirm)
			{
				m_foregroundNode->removeChild(craftConfirm);
				craftConfirm = Button::create("img/ui/popUpMsg/craft.png");
				craftConfirm->setName("craftConfirm");
				craftConfirm->setScale(2.0f);
				craftConfirm->setPosition(Vec2(visibleSize.width / 8 * 3, visibleSize.height / 7.0f));
				craftConfirm->addClickEventListener([&](Ref* sender) {
					this->confirmCrafting();
				});
				m_foregroundNode->addChild(craftConfirm);
			}

			craftConfirm->setVisible(true);

			//------------Menu (Button) UI-----------------------------------
			auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
			//if (!cancelMenu)
			{
				m_foregroundNode->removeChildByName("cancelMenu");
				auto cancelItem = MenuItemImage::create("img/ui/popUpMsg/msgCancel.png", "img/ui/popUpMsg/msgCancelPressed.png", [&](Ref* sender) {
					this->closeMsg();
					this->popUpMsg(MSG_TYPE::MSG_BUILDING_EFFECT);
				});
				cancelItem->setPositionX(visibleSize.width / 1.67f);
				cancelItem->setPositionY(visibleSize.height / 5);
				cancelItem->setScale(0.7f);
				cancelItem->setName("cancelItem");
				cancelItem->setEnabled(true);

				cancelMenu = Menu::create(cancelItem, NULL);
				cancelMenu->setName("cancelMenu");
				//cancelMenu->setScale(1.5f);
				cancelMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(cancelMenu);
			}
			cancelMenu->setOpacity(255);
			cancelMenu->setTag(0); //No character knock back
			auto item = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
			item->setEnabled(true);
			item->setScale(2.0f);
			item->setPositionX(visibleSize.width / 1.58f);
			item->setPositionY(visibleSize.height / 7.0f);
			cancelMenu->setVisible(true);
		}
		break;

		case MSG_TYPE::MSG_CONTRACT:
		{
			darkenScreen->setOpacity(180);
			m_uiNode->setVisible(false);

			showItems(m_inventoryX, m_inventoryY + 1000);

			//------------Contract boy---------------------------------------
			auto contractBoy = (Sprite*)m_foregroundNode->getChildByName("contractBoy");
			if (!contractBoy)
			{
				if (m_age == 0)
				{
					contractBoy = Sprite::create(npcTypePath[NPC_TYPE::NPC_TYPE_CAVE_BOY1].c_str());
				}
				else
				{
					contractBoy = Sprite::create(npcTypePath[NPC_TYPE::NPC_TYPE_MID_BOY1].c_str());
				}
				
				contractBoy->setPosition(visibleSize.width / 4, visibleSize.height / 2);
				contractBoy->setName("contractBoy");
				contractBoy->setScale(3.5f);
				contractBoy->setFlippedX(true);
				m_foregroundNode->addChild(contractBoy);
			}

			if (m_contractState == 1) //in contract
			{
				contractBoy->setOpacity(0);
				contractBoy->setScale(1.0f);
			}
			else
			{
				contractBoy->setPosition(visibleSize.width / 4, visibleSize.height / 2);
				contractBoy->setScale(3.5f);
				contractBoy->setOpacity(255);
			}

			contractBoy->setVisible(true);


			if (m_contractState == 0)
			{
				//------------contract price buttons-------------------------------

				//contract button 01------------------------------------------------
				auto contractButton01 = (Button*)m_foregroundNode->getChildByName("contractButton01");
				if (!contractButton01)
				{
					m_foregroundNode->removeChild(contractButton01);
					contractButton01 = Button::create("img/ui/popUpMsg/go01.png");
					contractButton01->setName("contractButton01");
					contractButton01->setScale(2.0f);
					contractButton01->setPosition(Vec2(visibleSize.width / 8 * 5, visibleSize.height / 4 * 3));
					contractButton01->addClickEventListener([&](Ref* sender) {
						if (m_gold >= 10)
						{
							this->m_contractLevel = 1;
							this->m_fromContract = true;
							this->closeMsg();
							this->popUpMsg(MSG_TYPE::MSG_CHANGE_MAP);
						}
					});
					m_foregroundNode->addChild(contractButton01);
				}

				contractButton01->setVisible(true);

				auto goldIcon01 = (Sprite*)m_foregroundNode->getChildByName("goldIcon01");
				if (!goldIcon01)
				{
					m_foregroundNode->removeChild(goldIcon01);
					goldIcon01 = Sprite::create("img/ui/coin.png");
					goldIcon01->setName("goldIcon01");
					goldIcon01->setPositionX(contractButton01->getPositionX() + 150);
					goldIcon01->setPositionY(contractButton01->getPositionY() - 5);
					m_foregroundNode->addChild(goldIcon01);
				}
				goldIcon01->setVisible(true);

				auto goldCount01 = m_foregroundNode->getChildByName("goldCount01");
				if (!goldCount01)
				{
					m_foregroundNode->removeChild(goldCount01);
					std::ostringstream ostr;
					ostr << "10";
					goldCount01 = Label::createWithTTF(ostr.str(), "fonts/PixelMplus10-Regular.ttf", 40);
					goldCount01->setPositionX(goldIcon01->getPositionX() - 50);
					goldCount01->setPositionY(goldIcon01->getPositionY());
					goldCount01->setName("goldCount01");
					m_foregroundNode->addChild(goldCount01);
				}
				goldCount01->setVisible(true);

				//contract button 02------------------------------------------------
				auto contractButton02 = (Button*)m_foregroundNode->getChildByName("contractButton02");
				if (!contractButton02)
				{
					m_foregroundNode->removeChild(contractButton02);
					contractButton02 = Button::create("img/ui/popUpMsg/go02.png");
					contractButton02->setName("contractButton02");
					contractButton02->setScale(2.0f);
					contractButton02->setPosition(Vec2(visibleSize.width / 8 * 5, visibleSize.height / 4 * 2));
					contractButton02->addClickEventListener([&](Ref* sender) {
						if (m_gold >= 20)
						{
							this->m_contractLevel = 2;
							this->m_fromContract = true;
							this->closeMsg();
							this->popUpMsg(MSG_TYPE::MSG_CHANGE_MAP);
						}
					});
					m_foregroundNode->addChild(contractButton02);
				}

				contractButton02->setVisible(true);

				auto goldIcon02 = (Sprite*)m_foregroundNode->getChildByName("goldIcon02");
				if (!goldIcon02)
				{
					m_foregroundNode->removeChild(goldIcon02);
					goldIcon02 = Sprite::create("img/ui/coin.png");
					goldIcon02->setName("goldIcon02");
					goldIcon02->setPositionX(contractButton02->getPositionX() + 150);
					goldIcon02->setPositionY(contractButton02->getPositionY() - 5);
					m_foregroundNode->addChild(goldIcon02);
				}
				goldIcon02->setVisible(true);

				auto goldCount02 = m_foregroundNode->getChildByName("goldCount02");
				if (!goldCount02)
				{
					m_foregroundNode->removeChild(goldCount02);
					std::ostringstream ostr;
					ostr << "20";
					goldCount02 = Label::createWithTTF(ostr.str(), "fonts/PixelMplus10-Regular.ttf", 40);
					goldCount02->setPositionX(goldIcon02->getPositionX() - 50);
					goldCount02->setPositionY(goldIcon02->getPositionY());
					goldCount02->setName("goldCount02");
					m_foregroundNode->addChild(goldCount02);
				}
				goldCount02->setVisible(true);

				//contract button 03------------------------------------------------
				auto contractButton03 = (Button*)m_foregroundNode->getChildByName("contractButton03");
				if (!contractButton03)
				{
					m_foregroundNode->removeChild(contractButton03);
					contractButton03 = Button::create("img/ui/popUpMsg/go03.png");
					contractButton03->setName("contractButton03");
					contractButton03->setScale(2.0f);
					contractButton03->setPosition(Vec2(visibleSize.width / 8 * 5, visibleSize.height / 4 * 1));
					contractButton03->addClickEventListener([&](Ref* sender) {
						if (m_gold >= 30)
						{
							this->m_contractLevel = 3;
							this->m_fromContract = true;
							this->closeMsg();
							this->popUpMsg(MSG_TYPE::MSG_CHANGE_MAP);
						}
					});
					m_foregroundNode->addChild(contractButton03);
				}

				contractButton03->setVisible(true);

				auto goldIcon03 = (Sprite*)m_foregroundNode->getChildByName("goldIcon03");
				if (!goldIcon03)
				{
					m_foregroundNode->removeChild(goldIcon03);
					goldIcon03 = Sprite::create("img/ui/coin.png");
					goldIcon03->setName("goldIcon03");
					goldIcon03->setPositionX(contractButton03->getPositionX() + 150);
					goldIcon03->setPositionY(contractButton03->getPositionY() - 5);
					m_foregroundNode->addChild(goldIcon03);
				}
				goldIcon03->setVisible(true);

				auto goldCount03 = m_foregroundNode->getChildByName("goldCount03");
				if (!goldCount03)
				{
					m_foregroundNode->removeChild(goldCount03);
					std::ostringstream ostr;
					ostr << "30";
					goldCount03 = Label::createWithTTF(ostr.str(), "fonts/PixelMplus10-Regular.ttf", 40);
					goldCount03->setPositionX(goldIcon03->getPositionX() - 50);
					goldCount03->setPositionY(goldIcon03->getPositionY());
					goldCount03->setName("goldCount03");
					m_foregroundNode->addChild(goldCount03);
				}
				goldCount03->setVisible(true);
			}

			else if (m_contractState == 1)
			{
				auto progressSlider = (Slider*)m_foregroundNode->getChildByName("progressSlider");
				if (!progressSlider)
				{
					m_foregroundNode->removeChild(progressSlider);
					progressSlider = Slider::create();
					progressSlider->loadBarTexture("img/ui/popUpMsg/sliderBar.png");
					progressSlider->loadProgressBarTexture("img/ui/popUpMsg/sliderProgress.png");
					if (m_age == 0)
					{
						progressSlider->loadSlidBallTextures("img/ui/popUpMsg/slidBall01.png", "img/ui/popUpMsg/slidBall01.png", "img/ui/popUpMsg/slidBall01.png");
					}
					else
					{
						progressSlider->loadSlidBallTextures("img/ui/popUpMsg/slidBall02.png", "img/ui/popUpMsg/slidBall02.png", "img/ui/popUpMsg/slidBall02.png");
					}

					progressSlider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
					progressSlider->setPercent(0);
					progressSlider->setName("progressSlider");
					progressSlider->setEnabled(false);
					progressSlider->setTouchEnabled(false);
					
					m_foregroundNode->addChild(progressSlider);
				}

				int duration = 1;
				if (m_contractLevel == 1)
				{
					duration = 3600;
				}
				else if (m_contractLevel == 2)
				{
					duration = 7200;
				}
				else if (m_contractLevel == 3)
				{
					duration = 11800;
				}

				progressSlider->setPercent(m_contractTime * 100 / duration);
				progressSlider->setVisible(true);

				auto villageIcon = m_foregroundNode->getChildByName("villageIcon");
				if (!villageIcon)
				{
					if (m_age == 0)
					{
						villageIcon = Sprite::create("img/ui/popUpMsg/house.png");
					}
					else
					{
						villageIcon = Sprite::create("img/ui/popUpMsg/house.png");
					}
					
					villageIcon->setPosition(visibleSize.width / 8 * 5.9, visibleSize.height / 2);
					villageIcon->setScale(0.3f);
					villageIcon->setName("villageIcon");
					m_foregroundNode->addChild(villageIcon);
				}

				villageIcon->setVisible(true);
			}

			else if(m_contractState == 2)
			{
				auto def = UserDefault::getInstance();

				//generate items once if empty
				if (def->getIntegerForKey(m_contractCollectSave[0].c_str()) == 0)
				{
					std::vector<int> resourceList;
					
					resourceList.push_back(mapData::mapCollect[m_contractArea].resource1);
					resourceList.push_back(mapData::mapCollect[m_contractArea].resource2);
					resourceList.push_back(mapData::mapCollect[m_contractArea].resource3);
					resourceList.push_back(mapData::mapCollect[m_contractArea].resource4);
					resourceList.push_back(mapData::mapCollect[m_contractArea].resource5);

					std::vector<int> resourceCountList;
					
					resourceCountList.push_back(mapData::mapCollect[m_contractArea].res1Count);
					resourceCountList.push_back(mapData::mapCollect[m_contractArea].res2Count);
					resourceCountList.push_back(mapData::mapCollect[m_contractArea].res3Count);
					resourceCountList.push_back(mapData::mapCollect[m_contractArea].res4Count);
					resourceCountList.push_back(mapData::mapCollect[m_contractArea].res5Count);

					for (int i = 0; i < 3; ++i)
					{
						auto rand = RandomHelper::random_int(0, 4);
						def->setIntegerForKey(m_contractCollectSave[i].c_str(), resourceList[rand]);
						def->setIntegerForKey(m_contractCollectCountSave[i].c_str(), resourceCountList[rand]);
						def->flush();
					}
				}

				std::vector<int> itemsToAdd;
				std::vector<int> itemsToAddCount;
				for (int i = 0; i < 5; ++i)
				{
					itemsToAdd.push_back(def->getIntegerForKey(m_contractCollectSave[i].c_str()));
					itemsToAddCount.push_back(def->getIntegerForKey(m_contractCollectCountSave[i].c_str()));
				}
				
				auto item1 = def->getIntegerForKey(m_contractCollectSave[0].c_str());
				CCLOG("item1: %i", item1);

				auto receiveContract = (Button*)m_foregroundNode->getChildByName("receiveContract");
				if (!receiveContract)
				{
					m_foregroundNode->removeChild(receiveContract);
					receiveContract = Button::create("img/ui/popUpMsg/areaButton.png");
					receiveContract->setName("receiveContract");
					receiveContract->setScale(2.0f);
					receiveContract->setPosition(Vec2(visibleSize.width / 8 * 5, visibleSize.height / 4 * 2));
					receiveContract->addClickEventListener([&, itemsToAdd, itemsToAddCount](Ref* sender) {
						
						this->addItemsToInventory(itemsToAdd, itemsToAddCount);
						
						this->resetContractCollect();

						this->m_contractLevel = 0;
						this->m_fromContract = false;
						this->closeMsg();
						this->m_contractState = 0;
						//this->popUpMsg(MSG_TYPE::MSG_BUILDING_EFFECT);
					});
					m_foregroundNode->addChild(receiveContract);
				}
				receiveContract->setVisible(true);
				
				auto itemIcon1 = m_foregroundNode->getChildByName("itemIcon1");
				if (!itemIcon1)
				{
					itemIcon1 = Sprite::create(resourceData::resourceIconPath[def->getIntegerForKey(m_contractCollectSave[0].c_str())]);
					itemIcon1->setName("itemIcon1");
					itemIcon1->setPosition(visibleSize.width / 8 * 4, visibleSize.height / 4 * 3);
					itemIcon1->setScale(2.0f);
					m_foregroundNode->addChild(itemIcon1);
				}

				itemIcon1->setVisible(true);

				auto itemIcon2 = m_foregroundNode->getChildByName("itemIcon2");
				if (!itemIcon2)
				{
					itemIcon2 = Sprite::create(resourceData::resourceIconPath[def->getIntegerForKey(m_contractCollectSave[1].c_str())]);
					itemIcon2->setName("itemIcon2");
					itemIcon2->setPosition(visibleSize.width / 8 * 5, visibleSize.height / 4 * 3);
					itemIcon2->setScale(2.0f);
					m_foregroundNode->addChild(itemIcon2);
				}

				itemIcon2->setVisible(true);

				auto itemIcon3 = m_foregroundNode->getChildByName("itemIcon3");
				if (!itemIcon3)
				{
					itemIcon3 = Sprite::create(resourceData::resourceIconPath[def->getIntegerForKey(m_contractCollectSave[2].c_str())]);
					itemIcon3->setName("itemIcon3");
					itemIcon3->setPosition(visibleSize.width / 8 * 6, visibleSize.height / 4 * 3);
					itemIcon3->setScale(2.0f);
					m_foregroundNode->addChild(itemIcon3);
				}

				itemIcon3->setVisible(true);

				auto receiveText = m_foregroundNode->getChildByName("receiveText");
				if (!receiveText)
				{
					m_foregroundNode->removeChild(receiveText);
					std::ostringstream ostr;
					ostr << "受け取る";
					receiveText = Label::createWithTTF(ostr.str(), "fonts/PixelMplus10-Regular.ttf", 40);
					receiveText->setPositionX(receiveContract->getPositionX());
					receiveText->setPositionY(receiveContract->getPositionY());
					receiveText->setName("receiveText");
					m_foregroundNode->addChild(receiveText);
				}
				receiveText->setVisible(true);
			}

			//------------Menu (Button) UI-----------------------------------
			auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
			//if (!cancelMenu)
			{
				m_foregroundNode->removeChildByName("cancelMenu");
				auto cancelItem = MenuItemImage::create("img/ui/popUpMsg/msgCancel.png", "img/ui/popUpMsg/msgCancelPressed.png", [&](Ref* sender) {
					this->closeMsg();
					//this->popUpMsg(MSG_TYPE::MSG_BUILDING_EFFECT);
				});

				cancelItem->setPositionX(visibleSize.width / 8 * 2);
				cancelItem->setPositionY(visibleSize.height / 5);
				cancelItem->setScale(0.7f);
				cancelItem->setName("cancelItem");
				cancelItem->setEnabled(true);

				cancelMenu = Menu::create(cancelItem, NULL);
				cancelMenu->setName("cancelMenu");
				//cancelMenu->setScale(1.5f);
				cancelMenu->setPosition(Vec2::ZERO);

				m_foregroundNode->addChild(cancelMenu);
			}

			cancelMenu->setOpacity(255);
			cancelMenu->setTag(0); //No character knock back
			auto item = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
			item->setEnabled(true);
			item->setScale(2.0f);
			if (m_contractState == 1)
			{
				item->setPositionX(visibleSize.width / 8 * 4);
			}
			else
			{
				item->setPositionX(visibleSize.width / 8 * 2);
			}
			
			item->setPositionY(visibleSize.height / 8.0f);
			cancelMenu->setVisible(true);
		}
		break;

		default:
			break;
		}
	}
}

void Control3::closeMsg()
{
	m_isPopUpMsg = false;
	m_isUIEnabled = true;
	m_isCrafting = false;

	m_uiNode->setVisible(true);

	auto darkenScreen = m_effects->GetColorScreen();
	darkenScreen->setVisible(false);

	//-------------tutorial ui
	auto confirmBG = m_foregroundNode->getChildByName("confirmBG");
	if (confirmBG)
	{
		confirmBG->setVisible(false);
	}

	auto confirmMenu = m_foregroundNode->getChildByName("confirmMenu");
	if (confirmMenu)
	{
		auto item = (MenuItemImage*)confirmMenu->getChildByName("confirmItem");
		item->setEnabled(false);
		confirmMenu->setVisible(false);
	}

	auto msgText = m_foregroundNode->getChildByName("msgText");
	if (msgText)
	{
		msgText->setVisible(false);
	}

	//-------------change map ui
	auto cancelMenu = m_foregroundNode->getChildByName("cancelMenu");
	if (cancelMenu)
	{
		if (cancelMenu->isVisible())
		{
			auto item = (MenuItemImage*)cancelMenu->getChildByName("cancelItem");
			item->setEnabled(false);
			cancelMenu->setVisible(false);

			if (cancelMenu->getTag() == 1)
			{
				if (m_directionRight)
				{
					m_directionRight = false;
					m_character->setPositionX(m_character->getPositionX() - 20);
					m_character->setFlippedX(m_directionRight);
				}
				else
				{
					m_directionRight = true;
					m_character->setPositionX(m_character->getPositionX() + 20);
					m_character->setFlippedX(m_directionRight);
				}
			}
			
			Size visibleSize = Director::getInstance()->getVisibleSize();
			m_standing = true;
			if (m_onGround)
				m_animationInstanced = false;
			m_moveTouchID = -1;
			auto JoyStick = (Sprite*)m_uiNode->getChildByTag(JOYSTICK_SPRITE_NUM);
			JoyStick->setPosition(visibleSize.width / 10 * 1.5, visibleSize.height / 6);
		}
	}

	auto areaText = m_foregroundNode->getChildByName("areaText");
	if (areaText)
	{
		areaText->setVisible(false);
	}

	auto areaScroll = m_foregroundNode->getChildByName("areaScroll");
	if (areaScroll)
	{
		m_foregroundNode->removeChild(areaScroll);
	}
	
	// confirm ui
	auto confirmText = m_foregroundNode->getChildByName("confirmText");
	if (confirmText)
	{
		confirmText->setVisible(false);
	}

	// build mode ui
	auto arrowMenu = m_foregroundNode->getChildByName("arrowMenu");
	if (arrowMenu)
	{
		if (arrowMenu->isVisible())
		{
			auto itemRight = (MenuItemImage*)arrowMenu->getChildByName("arrowRightItem");
			itemRight->setEnabled(false);

			auto itemLeft = (MenuItemImage*)arrowMenu->getChildByName("arrowLeftItem");
			itemLeft->setEnabled(false);

			exitBuildMode();

			arrowMenu->setVisible(false);
		}
	}

	auto buildText = m_foregroundNode->getChildByName("buildText");
	if (buildText)
	{
		buildText->setVisible(false);
	}

	//building effect ui
	auto buildingName = m_foregroundNode->getChildByName("buildingName");
	if (buildingName)
	{
		buildingName->setVisible(false);
	}

	auto craftButton = m_foregroundNode->getChildByName("craftButton");
	if (craftButton)
	{
		craftButton->setVisible(false);
		m_foregroundNode->getChildByName("craftText")->setVisible(false);
	}

	auto contractButton = m_foregroundNode->getChildByName("contractButton");
	if (contractButton)
	{
		contractButton->setVisible(false);
		m_foregroundNode->getChildByName("contractText")->setVisible(false);
	}

	auto nextAge = m_foregroundNode->getChildByName("nextAge");
	if (nextAge)
	{
		nextAge->setVisible(false);
		m_foregroundNode->getChildByName("nextAgeText")->setVisible(false);
	}

	auto pastAge = m_foregroundNode->getChildByName("pastAge");
	if (pastAge)
	{
		pastAge->setVisible(false);
		m_foregroundNode->getChildByName("pastAgeText")->setVisible(false);
	}

	auto timeMachinePortrait = m_foregroundNode->getChildByName("timeMachinePortrait");
	if (timeMachinePortrait)
	{
		timeMachinePortrait->setVisible(false);
		m_foregroundNode->getChildByName("timeMachinePart1")->setVisible(false);
		m_foregroundNode->getChildByName("timeMachinePart2")->setVisible(false);
		m_foregroundNode->getChildByName("timeMachinePart3")->setVisible(false);
	}

	//craft ui
	auto inventorySlots = m_foregroundNode->getChildByName("inventorySlots");
	if (inventorySlots)
	{
		if (inventorySlots->isVisible())
		{
			inventorySlots->setVisible(false);
			m_foregroundNode->getChildByName("frame01")->setVisible(false);
			m_foregroundNode->getChildByName("frame02")->setVisible(false);
			m_foregroundNode->getChildByName("frame03")->setVisible(false);
			m_foregroundNode->getChildByName("plusMark")->setVisible(false);
			m_foregroundNode->getChildByName("equalMark")->setVisible(false);
			m_foregroundNode->getChildByName("craftConfirm")->setVisible(false);
			m_foregroundNode->removeChildByName("craftingResource1");
			m_foregroundNode->removeChildByName("craftingResource2");
			m_foregroundNode->removeChildByName("craftingResource3");
			if (m_craftingResource1 != 0)
			{
				addItemToInventory(m_craftingResource1, 1);
			}

			if (m_craftingResource2 != 0)
			{
				addItemToInventory(m_craftingResource2, 1);
			}

			m_craftingResource1 = 0;
			m_craftingResource2 = 0;
		}
	}

	//contract ui
	auto contractBoy = m_foregroundNode->getChildByName("contractBoy");
	if (contractBoy)
	{
		if (contractBoy->isVisible())
		{
			
			//m_fromContract = false;
			//m_contractLevel = 0;
			contractBoy->setVisible(false);
			if (m_contractState == 0)
			{
				m_foregroundNode->getChildByName("contractButton01")->setVisible(false);
				m_foregroundNode->getChildByName("contractButton02")->setVisible(false);
				m_foregroundNode->getChildByName("contractButton03")->setVisible(false);
				m_foregroundNode->getChildByName("goldIcon01")->setVisible(false);
				m_foregroundNode->getChildByName("goldCount01")->setVisible(false);
				m_foregroundNode->getChildByName("goldIcon02")->setVisible(false);
				m_foregroundNode->getChildByName("goldCount02")->setVisible(false);
				m_foregroundNode->getChildByName("goldIcon03")->setVisible(false);
				m_foregroundNode->getChildByName("goldCount03")->setVisible(false);
			}
			else if(m_contractState == 1)
			{
				m_foregroundNode->getChildByName("progressSlider")->setVisible(false);
				m_foregroundNode->getChildByName("villageIcon")->setVisible(false);
			}
			else if (m_contractState == 2)
			{
				if (m_foregroundNode->getChildByName("progressSlider"))
				{
					if (m_foregroundNode->getChildByName("progressSlider")->isVisible())
					{
						m_foregroundNode->getChildByName("progressSlider")->setVisible(false);
						m_foregroundNode->getChildByName("villageIcon")->setVisible(false);
					}
					else
					{
						m_foregroundNode->getChildByName("receiveContract")->setVisible(false);
						m_foregroundNode->getChildByName("receiveText")->setVisible(false);
						m_foregroundNode->getChildByName("itemIcon1")->setVisible(false);
						m_foregroundNode->getChildByName("itemIcon2")->setVisible(false);
						m_foregroundNode->getChildByName("itemIcon3")->setVisible(false);
					}
				}
				else
				{
					m_foregroundNode->getChildByName("receiveContract")->setVisible(false);
					m_foregroundNode->getChildByName("receiveText")->setVisible(false);
					m_foregroundNode->getChildByName("itemIcon1")->setVisible(false);
					m_foregroundNode->getChildByName("itemIcon2")->setVisible(false);
					m_foregroundNode->getChildByName("itemIcon3")->setVisible(false);
				}
			}
		}
	}


	showItems(m_inventoryX, m_inventoryY);
}

void Control3::nextAge()
{
	auto def = UserDefault::getInstance();
	if (m_age == 0)
		m_age++;
	else
		m_age--;

	def->setIntegerForKey("AgeNumber", m_age);
	def->flush();
}

void Control3::nextStep()
{
	m_stepDone = true;
	m_step++;
}

void Control3::saveGame()
{
	auto def = UserDefault::getInstance();

	def->setBoolForKey("SavedGame", true);

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

	def->setIntegerForKey("gold", m_gold);
	def->setIntegerForKey("contractState", m_contractState);
	def->setIntegerForKey("contractCount", m_contractTime);
	def->setIntegerForKey("contractLevel", m_contractLevel);
	def->setIntegerForKey("contractArea", m_contractArea);

	def->setIntegerForKey("timeOfDay", m_timeOfDay);
	def->setIntegerForKey("AgeNumber", m_age);

	def->setIntegerForKey("trueEnding", m_trueEnding);
	def->setIntegerForKey("happyEnding", m_happyEnding);
	def->setIntegerForKey("goodEnding", m_goodEnding);
	def->setIntegerForKey("badEnding", m_badEnding);
	def->setIntegerForKey("worstEnding", m_worstEnding);

	def->setIntegerForKey("hp", m_hp);

	for (int i = 0; i < m_buildingSlots; ++i)
	{
		def->setIntegerForKey(mapData::buildingMap[i].c_str(), m_buildingsVector[i].buildingType);
	}

	def->flush();
}

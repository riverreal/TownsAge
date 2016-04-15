#include "ControlTesting02.h"
#include "TitleScene.h"

USING_NS_CC;

Scene* Control2::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Control2::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Control2::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Control2::menuCloseCallback, this));
	closeItem->setScale(1);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getBoundingBox().getMaxX() ,
                                origin.y + closeItem->getBoundingBox().getMaxY()));
	

	auto aButton = MenuItemImage::create("img/a-button.png", "img/a-button.png");
	aButton->setPosition(visibleSize.width / 5 * 3.5, visibleSize.height / 6);
	aButton->setScale(0.4);
	aButton->setOpacity(100);

	auto bButton = MenuItemImage::create("img/b-button.png", "img/b-button.png");
	bButton->setPosition(visibleSize.width / 5 * 4.5, visibleSize.height / 5);
	bButton->setScale(0.3);
	bButton->setOpacity(100);
	
	
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, aButton, bButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label


	//facing right by default
	m_directionRight = true;

	auto bg = Sprite::create("img/bg.png");
	bg->setScale(2.5);
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg);

	auto tilemap = TMXTiledMap::create("img/tilemap/TileMap.tmx");
	tilemap->setVisible(true);
	this->addChild(tilemap);

	m_character = Sprite::create("img/sprite.png");
	m_character->setScale(0.8);
	m_character->setPosition(visibleSize.width / 2, visibleSize.height / 2.8);
	this->addChild(m_character);

	auto leftButton = Sprite::create("img/dpad.png");
	leftButton->setPosition(visibleSize.width / 10 * 1, visibleSize.height / 6);
	leftButton->setScale(1);
	leftButton->setRotation(-90);
	leftButton->setOpacity(100);
	this->addChild(leftButton);

	auto rightButton = Sprite::create("img/dpad.png");
	rightButton->setPosition(visibleSize.width / 10 * 3, visibleSize.height / 6);
	rightButton->setScale(1);
	rightButton->setRotation(90);
	rightButton->setOpacity(100);
	this->addChild(rightButton);

	m_leftRect = leftButton->getBoundingBox();
	m_rightRect = rightButton->getBoundingBox();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Control2::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Control2::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Control2::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void Control2::walk(bool directionRight)
{
	auto moveRight = MoveBy::create(0.5, Vec2(90, 0));
	auto moveLeft = MoveBy::create(0.5, Vec2(-90, 0));

	m_character->stopAllActions();

	if (directionRight) //right
	{
		m_character->setFlipX(true);
		m_character->runAction(RepeatForever::create(moveRight));
	}
	else //left
	{
		m_character->setFlipX(false);
		m_character->runAction(RepeatForever::create(moveLeft));
	}

	auto frame1 = RotateTo::create(0.1, -20);
	auto frame2 = RotateTo::create(0.1, 20);
	auto sequence = Sequence::create(frame1, frame2, NULL);
	m_character->runAction(RepeatForever::create(sequence));
}

void Control2::update()
{
}

bool Control2::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Rect rightHalf = Rect(visibleSize.width*0.5, 0, visibleSize.width, visibleSize.height);
	Rect leftHalf = Rect(0, 0, visibleSize.width*0.5, visibleSize.height);
	Rect touchPoint = Rect(touch->getLocation().x, touch->getLocation().y, 2, 2);
	if (touchPoint.intersectsRect(m_rightRect))
	{
		walk(true);
		m_directionRight = true;
	}
	else if (touchPoint.intersectsRect(m_leftRect))
	{
		walk(false);
		m_directionRight = false;
	}

	return true;
}

void Control2::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* eventt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Rect rightHalf = Rect(visibleSize.width*0.5, 0, visibleSize.width, visibleSize.height);
	Rect leftHalf = Rect(0, 0, visibleSize.width*0.5, visibleSize.height);
	Rect touchPoint = Rect(touch->getLocation().x, touch->getLocation().y, 2, 2);
	if (touchPoint.intersectsRect(m_rightRect))
	{
		if (!m_directionRight)
		{
			m_character->stopAllActions();
			walk(true);
			m_directionRight = true;
		}
		
	}
	else if (touchPoint.intersectsRect(m_leftRect))
	{
		if (m_directionRight)
		{
			m_character->stopAllActions();
			walk(false);
			m_directionRight = false;
		}
	}
}

void Control2::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* eventt)
{
	m_character->stopAllActions();
	m_character->setRotation(0);
}

void Control2::menuCloseCallback(Ref* pSender)
{
	auto scene = TitlleScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

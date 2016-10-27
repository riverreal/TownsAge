#include "Ending.h"
#include "TitleScene.h"


USING_NS_CC;

Scene* Ending::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Ending::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Ending::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto def = UserDefault::getInstance();

	auto trueEnding = def->getIntegerForKey("trueEnding");
	auto happyEnding = def->getIntegerForKey("happyEnding");
	auto goodEnding = def->getIntegerForKey("goodEnding");
	auto badEnding = def->getIntegerForKey("badEnding");
	auto worstEnding = def->getIntegerForKey("worstEnding");

	int result;

	//get the result
	if (trueEnding > happyEnding)
	{
		result = trueEnding;
	}
	else
	{
		result = happyEnding;
	}

	if (result < goodEnding)
	{
		result = goodEnding;
	}

	if (result < badEnding)
	{
		result = badEnding;
	}

	if (result < worstEnding)
	{
		result = worstEnding;
	}

	if (result == trueEnding)
	{
		result = 0;
	}
	else if (result == happyEnding)
	{
		result = 1;
	}
	else if (result == goodEnding)
	{
		result = 2;
	}
	else if (result == badEnding)
	{
		result = 3;
	}
	else if (result == worstEnding)
	{
		result = 4;
	}

	Sprite* sprite;
	Label* label;

	if (result == 0)//true
	{
		label = Label::createWithTTF(" ロボットと共存！！一番良い道を見つけた！最高の未来だ！", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
		sprite = Sprite::create("img/ending/true.png");
	}
	else if (result == 1)//happy
	{
		label = Label::createWithTTF("ロボットを従えさせた！良い未来にはできたけどこれでいいのかな…？", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
		sprite = Sprite::create("img/ending/happy.png");
	}
	else if (result == 2)//good
	{
		label = Label::createWithTTF("人間だけになった！未来を変えることに成功したけどこれでいいのかな？", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
		sprite = Sprite::create("img/ending/good.png");
	}
	else if (result == 3)//bad
	{
		label = Label::createWithTTF("ロボットに支配された！未来は変わらなかった…もっと他にあったんじゃないのか…", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
		sprite = Sprite::create("img/ending/bad.png");
	}
	else if (result == 4)//worst
	{
		label = Label::createWithTTF("ロボットだけになった…　最悪の未来だ、なんでこんなことに…", "fonts/PixelMplus10-Regular.ttf", 50, Size(600, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
		sprite = Sprite::create("img/ending/worst.png");
	}

	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);

 
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite, 0);

	def->setBoolForKey("SavedGame", false);

	auto delay = DelayTime::create(7.0f);
	auto titleScene = CallFunc::create([this]() {Director::getInstance()->replaceScene(TitlleScene::createScene()); });
	this->runAction(Sequence::create(delay, titleScene, NULL));

	return true;
}


void Ending::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

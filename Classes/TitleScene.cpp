#include "TitleScene.h"
#include "ControlTesting03.h"
#include "GameData.h"
#include "Option.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* TitlleScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TitlleScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TitlleScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	UserDefault* def = UserDefault::getInstance();
	//‰Šú‰»

	m_skyNode = Node::create();
	this->addChild(m_skyNode);

	m_backgroundNode = Node::create();
	this->addChild(m_backgroundNode);

	m_foregroundNode = Node::create();
	this->addChild(m_foregroundNode);

	m_gameNode = Node::create();
	this->addChild(m_gameNode);

	m_gameNode->setScale(1.3);

	//ƒ}ƒbƒv“Ç‚Ýž‚Ý
	m_tilemap = TMXTiledMap::create("img/tilemap/tutorial.tmx");
	m_tilemap->getLayer("col")->setVisible(false);
	m_gameNode->addChild(m_tilemap, -1);



	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_DayTime = 3600;


	//‹ó‚ÌƒXƒvƒ‰ƒCƒg(ƒJƒƒ‰—p)
	auto empty_Sprite = Sprite::create();
	auto spawn_obj = m_tilemap->getObjectGroup("player")->getObject("Spawn");
	empty_Sprite->setPosition(spawn_obj["x"].asFloat(), spawn_obj["y"].asFloat());
	empty_Sprite->setName("empty_Sprite");
	m_gameNode->addChild(empty_Sprite);

	//Ž‹“_  
	m_background.Init(empty_Sprite->getPosition(), m_DayTime, visibleSize, m_gameNode, 7200, 3, 3000);

	//background i‰_‚Æ‚©j
	auto backgroundGroup = m_tilemap->getObjectGroup("bg");
	if (backgroundGroup)
	{
		auto backgroundVector = backgroundGroup->getObjects();
		for (auto &bg : backgroundVector)
		{
			if (bg.asValueMap()["name"].asString() == "montain01")
			{
				auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
				m_background.AddParallaxLayer("img/bg/staticBG_forest01.png", 0.2f, bgPos, m_gameNode->getScale() * 1.4f, m_background.DEPTH_LEVEL_FAR);
			}
			else if (bg.asValueMap()["name"].asString() == "montain02")
			{
				auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
				m_background.AddParallaxLayer("img/bg/staticBG_forest02.png", 0.11f, bgPos, m_gameNode->getScale() * 1.2f, m_background.DEPTH_LEVEL_FAR + 1);
			}
			else if (bg.asValueMap()["name"].asString() == "montain03")
			{
				auto bgPos = Vec2(bg.asValueMap()["x"].asFloat(), bg.asValueMap()["y"].asFloat());
				m_background.AddParallaxLayer("img/bg/staticBG_forest03.png", 0.015f, bgPos, m_gameNode->getScale() * 1.0f, m_background.DEPTH_LEVEL_FAR + 2);
			}
		}
	}

	m_backgroundNode->addChild(m_background.GetBackgroundNode());
	m_skyNode->addChild(m_background.GetSkyNode());
	m_foregroundNode->addChild(m_background.GetShadeSprite());

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	auto Delay = DelayTime::create(0.5f);
	auto Fadein = FadeIn::create(1.5f);
	auto Sequence = Sequence::create(Delay, Fadein, NULL);

	auto TitleName = Sprite::create("img/ui/titleLogo.png");
	//“§–¾“x
	TitleName->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
	TitleName->setScale(0.8f);
	TitleName->setName("Title_Logo");
	TitleName->setOpacity(0);
	//ƒtƒF[ƒhƒCƒ“
	TitleName->runAction(Sequence);
	this->addChild(TitleName);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(TitlleScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	closeItem->setVisible(false);
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto label = Label::createWithTTF("TOWNS AGE", "fonts/Marker Felt.ttf", 36);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	//this->addChild(label, 1);


	def->setIntegerForKey("MapArea", 0);

	def->setBoolForKey("FromRight", true);
	for (int i = 0; i < 14; ++i)
	{
		def->setIntegerForKey(mapData::leftMap[i].c_str(), 1);
		def->setIntegerForKey(mapData::rightMap[i].c_str(), 1);
	}

	for (int i = 0; i < 10; ++i)
	{
		auto randomArea = RandomHelper::random_int(1, 1);
		def->setIntegerForKey(mapData::cicleMap[i].c_str(), randomArea);
	}

	def->setIntegerForKey(mapData::cicleMap[0].c_str(), 0);

	def->setBoolForKey("tutorial", true);

	//def->setIntegerForKey(mapData::rightMap[2].c_str(), 3);

	def->setIntegerForKey("inventorySlotType01", 0);
	def->setIntegerForKey("inventorySlotType02", 0);
	def->setIntegerForKey("inventorySlotType03", 0);
	def->setIntegerForKey("inventorySlotType04", 0);
	def->setIntegerForKey("inventorySlotType05", 0);
	def->setIntegerForKey("inventorySlotType06", 0);
	def->setIntegerForKey("inventorySlotType07", 0);
	def->setIntegerForKey("inventorySlotType08", 0);
	def->setIntegerForKey("inventorySlotType09", 0);
	def->setIntegerForKey("inventorySlotType10", 0);

	def->setIntegerForKey("inventorySlotAmount01", 0);
	def->setIntegerForKey("inventorySlotAmount02", 0);
	def->setIntegerForKey("inventorySlotAmount03", 0);
	def->setIntegerForKey("inventorySlotAmount04", 0);
	def->setIntegerForKey("inventorySlotAmount05", 0);
	def->setIntegerForKey("inventorySlotAmount06", 0);
	def->setIntegerForKey("inventorySlotAmount07", 0);
	def->setIntegerForKey("inventorySlotAmount08", 0);
	def->setIntegerForKey("inventorySlotAmount09", 0);
	def->setIntegerForKey("inventorySlotAmount10", 0);
	def->setIntegerForKey("hp", 1000);

	def->setBoolForKey("HasTimeMachinePart", false);
	def->setIntegerForKey("AgeNumber", 0);
	def->setBoolForKey("FromMenu", true);

	def->flush();

	//auto control1 = MenuItemFont::create("Half Screen", CC_CALLBACK_1(TitlleScene::menuTransitionCallback, this, 0)); //index 0
	//control1->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height*4/6);
	//auto control2 = MenuItemFont::create("Classic", CC_CALLBACK_1(TitlleScene::menuTransitionCallback, this, 1)); //index 1
	//control2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 3 / 6);
	auto control3 = MenuItemFont::create("TUTORIAL", CC_CALLBACK_1(TitlleScene::menuTransitionCallback, this, 2)); //index 2
	control3->setPosition(origin.x + visibleSize.width / 2, visibleSize.height / 2);

	auto controlMenu = Menu::create(control3, NULL);
	controlMenu->setPosition(Vec2::ZERO);
	//this->addChild(controlMenu);
	ButtonCreate();

	this->scheduleUpdate();

	option();

	//ƒIƒvƒVƒ‡ƒ“‚ÌƒL[
	//ƒIƒvƒVƒ‡ƒ“”²‚¯‚½‚ç‰¹—ÊÁ‚µ
	_Volume = def->getFloatForKey("volume");
	_ButtonA = def->getFloatForKey("button");
	_ButtonSwap = def->getBoolForKey("ButtonSwap");

	return true;
}

void TitlleScene::update(float dt)
{
	//ˆê“ú
	m_DayTime++;
	if (m_DayTime >= 7200)
	{
		m_DayTime = 0;
	}
	m_background.UpdateBackground(setViewpoint(m_gameNode->getChildByName("empty_Sprite")->getPosition()), m_DayTime);
}
//ƒJƒƒ‰@empty_sprite‚ð’†S‚É‚µ‚Ä‚é
Vec2 TitlleScene::setViewpoint(cocos2d::Vec2 position)
{
	auto visibleSize = Director::getInstance()->getWinSize();

	int x = (visibleSize.width / 2.0f) - position.x;
	int y = (visibleSize.height / 2.8f) - position.y;

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

void TitlleScene::ButtonCreate()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto set_posX = (m_gameNode->getChildByName("empty_Sprite")->getPosition().x + visibleSize.width / 2);
	//auto button = ui::Button::create("img/ui/new.png");
	//ƒ{ƒ^ƒ“‚Ì‰æ‘œB‰¼‰æ‘œ
	//”z—ñ
	ui::Button* button[]
	{
		/*
		ui::Button::create("img/ui/new.png"),
		ui::Button::create("img/ui/option.png"),
		ui::Button::create("img/ui/new.png"),
		ui::Button::create("img/ui/continue.png")
		*/
		ui::Button::create("img/ui/title/start2.png"),
		ui::Button::create("img/ui/title/option.png"),
		ui::Button::create("img/ui/title/new.png"),
		ui::Button::create("img/ui/title/continue.png")
	};
	for (int i = 0; i < 4; i++)
	{

		/*
		auto tiltingFunc = CallFunc::create([button]()
		{
		auto quickFadeIn = FadeIn::create(0.8f);
		auto pause = DelayTime::create(1.0f);
		auto quickFadeOut = FadeOut::create(0.8f);
		auto shortPause = DelayTime::create(0.5f);
		auto tilting = RepeatForever::create(Sequence::create(quickFadeOut, shortPause, quickFadeIn, pause, NULL));
		button->runAction(tilting);
		});*/

		auto Delay = DelayTime::create(1.2f);
		auto Fadein = FadeIn::create(1.0f);
		auto Sequence = Sequence::create(Delay, Fadein,/* tiltingFunc,*/ NULL);

		//“§–¾“x
		button[i]->setOpacity(0);
		//ƒAƒNƒVƒ‡ƒ“
		button[i]->runAction(Sequence);

		button[i]->setName("new");

		button[i]->setScale(0.5f);

		m_gameNode->addChild(button[i]);
		//ƒ^ƒO•t
		button[i]->setTag(i);

		//ƒ^ƒbƒ`‚ð—LŒø‚É
		button[i]->addTouchEventListener(CC_CALLBACK_2(TitlleScene::touchEvent, this));

		//‹ô”‚ª¶‚ÅŠï”‚ª‰Eiƒ{ƒ^ƒ“‚ÌˆÊ’uj
		if (i == 0)
		{
			button[i]->setPosition(Vec2(set_posX / 2.5, visibleSize.height / 4.8));
		}
		else if (i == 1)
		{
			button[i]->setPosition(Vec2(set_posX / 1.5, visibleSize.height / 4.8));
		}

		else if (i == 2)
		{
			button[i]->setPosition(Vec2((set_posX / 2.5) + visibleSize.width * 1.3, visibleSize.height / 1.75));
		}
		else if (i == 3)
		{
			button[i]->setPosition(Vec2((set_posX / 1.5) + visibleSize.width * 1.3, visibleSize.height / 1.75));
			button[i]->setColor(Color3B(100, 100, 100));
		}
	}
}



void TitlleScene::option()
{
	//‰¹—Êƒo[
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//node‚ÌŽq‚È‚Ì‚ÅˆÊ’u‚ª‚¸‚ê‚Ä‚éB‚È‚Ì‚ÅposC³
	auto empty_Sprite_Pos = m_gameNode->getChildByName("empty_Sprite")->getPosition();

	auto sizeY = (visibleSize.height - visibleSize.height / 2.8f) + empty_Sprite_Pos.y;
	auto sizeX = empty_Sprite_Pos.x;

	auto Bar_set_posX = ((sizeX + visibleSize.width / 2) / 3);

	auto Vslider = ui::Slider::create();
	//ƒo[ ‚Ü‚¾‰¼‘fÞ
	Vslider->loadBarTexture("img/ui/option/slider.png");
	//‚Â‚Ü‚Ý@‚Ü‚¾‰¼‘fÞ
	Vslider->loadSlidBallTextures("img/ui/option/cursor.png");

	//ƒ|ƒWƒVƒ‡ƒ“
	Vslider->setPosition(Vec2(Bar_set_posX *2.1, sizeY + visibleSize.height / 1.4));
	//ƒXƒ‰ƒCƒ_[‚ÉƒCƒxƒ“ƒg“K‰ž
	Vslider->addEventListener(CC_CALLBACK_2(TitlleScene::sliderEvent, this, true));
	//‚Â‚Ü‚Ý‚ÌˆÊ’u
	Vslider->setPercent(static_cast<int>(def->getFloatForKey("volume") * 100));
	m_gameNode->addChild(Vslider);

	//ƒ‰ƒxƒ‹
	auto Vlabel = Label::createWithTTF("サウンド", "fonts/misaki_gothic.ttf", 36);
	Vlabel->setPosition(Vec2(Bar_set_posX * 2.1, sizeY + visibleSize.height / 1.3));
	m_gameNode->addChild(Vlabel);


	//ƒ{ƒ^ƒ““§–¾“xƒo[

	//“§–¾“xƒo[
	auto Bslider = ui::Slider::create();
	//ƒo[ ‚Ü‚¾‰¼‘fÞ
	Bslider->loadBarTexture("img/ui/option/slider.png");
	//‚Â‚Ü‚Ý@‚Ü‚¾‰¼‘fÞ
	Bslider->loadSlidBallTextures("img/ui/option/cursor.png");
	//ƒ|ƒWƒVƒ‡ƒ“
	Bslider->setPosition(Vec2(Bar_set_posX * 2.1, sizeY + visibleSize.height / 1.9));
	//ƒXƒ‰ƒCƒ_[‚ÉƒCƒxƒ“ƒg“K‰ž
	Bslider->addEventListener(CC_CALLBACK_2(TitlleScene::sliderEvent, this, false));
	//‚Â‚Ü‚Ý‚ÌˆÊ’uB
	Bslider->setPercent(static_cast<int>(def->getFloatForKey("button") / 255 * 100));

	m_gameNode->addChild(Bslider);

	//ƒ‰ƒxƒ‹
	auto Blabel = Label::createWithTTF("ボタン透明度", "fonts/misaki_gothic.ttf", 36);
	Blabel->setPosition(Vec2(Bar_set_posX * 2.1, sizeY + visibleSize.height / 1.73));
	m_gameNode->addChild(Blabel);



	/*auto checkBox = ui::CheckBox::create(
	"img/ui/appleOrignal.png",//”ñ‘I‘ðŽž
	"img/ui/appleOrignal.png",//”ñ‘I‘ðiƒ{ƒ^ƒ“~‰ºŽžj
	"img/ui/stoneIcon.png",//‘I‘ðŽž
	"img/ui/appleOrignal.png",//”ñ‘I‘ðŽžiƒ{ƒ^ƒ“–³Œøj
	"img/ui/appleOrignal.png");//‘I‘ðŽžiƒ{ƒ^ƒ“–³Œøj
	*/
	//ƒ`ƒFƒbƒNƒ{ƒbƒNƒX
	auto checkBox = ui::CheckBox::create(
		"img/ui/option/checkbox.png",//”ñ‘I‘ðŽž
		"img/ui/option/checkbox.png",//”ñ‘I‘ðiƒ{ƒ^ƒ“~‰ºŽžj
		"img/ui/option/check.png",//‘I‘ðŽž
		"img/ui/option/checkbox.png",//”ñ‘I‘ðŽžiƒ{ƒ^ƒ“–³Œøj
		"img/ui/option/checkbox.png");//‘I‘ðŽžiƒ{ƒ^ƒ“–³Œøj

									  //ƒ|ƒWƒVƒ‡ƒ“
	checkBox->setPosition(Vec2(Bar_set_posX * 2.1, sizeY + visibleSize.height / 1.08));
	m_gameNode->addChild(checkBox);
	checkBox->setScale(0.9f);
	checkBox->addEventListener(CC_CALLBACK_2(TitlleScene::selectedEvent, this));
	checkBox->setSelected(def->getBoolForKey("ButtonSwap"));
	//ƒ‰ƒxƒ‹
	auto BPlabel = Label::createWithTTF("ボタン切り替え", "fonts/misaki_gothic.ttf", 36);
	BPlabel->setPosition(Vec2(Bar_set_posX * 2.1, sizeY + visibleSize.height / 1));
	m_gameNode->addChild(BPlabel);

	//ƒ{ƒ^ƒ“‚Ì‰æ‘œB‰¼‰æ‘œ
	auto button = ui::Button::create("img/ui/option/backButton.png");
	//ƒ|ƒWƒVƒ‡ƒ“Ý’è
	//button->setPosition(Vec2(visibleSize.width / 12.1, visibleSize.height / 1.1));
	button->setScale(0.4f);
	button->setPosition(Vec2(Bar_set_posX * 1.05, sizeY + (visibleSize.height / 2) * 1.3));
	m_gameNode->addChild(button);
	//ƒ^ƒO•t
	button->setTag(option_back_button);
	//ƒ^ƒbƒ`‚ð—LŒø‚É
	button->addTouchEventListener(CC_CALLBACK_2(TitlleScene::touchEvent, this));

}


void TitlleScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void TitlleScene::menuTransitionCallback(cocos2d::Ref* pSender, int index)
{

	Scene* scene = nullptr;
	switch (index)
	{
	case 0: //index 0
			//scene = Control1::createScene();
		break;
	case 1: //index 1
			//scene = Control2::createScene();
		break;
	case 2: //index 2
		scene = Control3::createScene();
		break;
	default:
		break;
	}

	if (scene != nullptr)
	{
		Director::getInstance()->replaceScene(scene);
	}
}

void TitlleScene::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto spr = (CCSprite *)pSender;
	auto tag = spr->getTag();
	auto empty_sprite = m_gameNode->getChildByName("empty_Sprite");
	//type‚Åƒ^ƒbƒ`ƒCƒxƒ“ƒgŽw’è
	switch (type)
	{
		//ƒ^ƒbƒ`ŠJŽn
	case ui::Widget::TouchEventType::BEGAN:
		//ƒ^ƒO‚ðƒQƒbƒg‚µAŠ„‚èU‚é
		switch (tag)
		{
		case	0:
			//Žn‚ß‚éƒ{ƒ^ƒ“
			//‰¡‚ÉƒJƒƒ‰ˆÚ“®
		{
			auto sizeX = empty_sprite->getPosition().x + visibleSize.width *0.7;
			auto move_up = MoveBy::create(0.7f, Vec2(sizeX, visibleSize.height / 2.8));
			empty_sprite->runAction(move_up);
		}

		break;
		case	1:
			//ƒIƒvƒVƒ‡ƒ“
		{
			//ƒƒSk¬‚ÆƒJƒƒ‰ˆÚ“®
			auto sizeY = (visibleSize.height - visibleSize.height / 2.8f) + empty_sprite->getPosition().y + visibleSize.height / 2.8;
			auto move_up = MoveBy::create(0.7f, Vec2(0, sizeY));

			auto title_logo = this->getChildByName("Title_Logo");

			auto Title_scale = ScaleTo::create(0.7f, 0.6f);
			auto Title_move = MoveTo::create(0.7f, Vec2(visibleSize.width / 4, (visibleSize.height / 4) * 3));

			title_logo->runAction(Spawn::create(Title_scale, Title_move, NULL));
			empty_sprite->runAction(move_up);

		}
		break;
		case	2:
			//‰‚ß‚©‚ç
			Director::getInstance()->replaceScene(Control3::createScene());
			break;
		case	3:
			//‘±‚«‚©‚ç
			break;
		case	4:
			//ƒIƒvƒVƒ‡ƒ“‚Ì–ß‚éƒ{ƒ^ƒ“
		{
			//ƒXƒ|[ƒ“ƒIƒuƒWƒF‚Ü‚Åempty‚ðŽ‚Á‚Ä‚­‚¤‚“
			auto spawn_obj = m_tilemap->getObjectGroup("player")->getObject("Spawn");
			auto sizeY = (spawn_obj["x"].asFloat(), spawn_obj["y"].asFloat());
			//auto sizeY = (visibleSize.height - visibleSize.height / 2.8f) + empty_sprite->getPosition().y + visibleSize.height / 2.8;
			auto move_up = MoveTo::create(0.7f, Vec2(empty_sprite->getPosition().x, sizeY));
			empty_sprite->runAction(move_up);
			//ƒL[•Û‘¶
			def->setFloatForKey("volume", _Volume);
			def->setFloatForKey("button", _ButtonA);
			def->setBoolForKey("ButtonSwap", _ButtonSwap);
			//ƒ^ƒCƒgƒ‹‚ÌŠg‘å
			auto title_logo = this->getChildByName("Title_Logo");
			auto Title_scale = ScaleTo::create(0.7f, 0.8f);
			auto Title_move = MoveTo::create(0.7f, Vec2(visibleSize.width / 2, (visibleSize.height / 1.5)));
			title_logo->runAction(Spawn::create(Title_scale, Title_move, NULL));
			title_logo->setOpacity(255);
		}

		break;

		default:
			break;
		}
		//‚±‚±‚Ü‚Åƒ{ƒ^ƒ“
		break;

	default:
		break;
	}
	//ƒ^ƒbƒ`I—¹
}

void TitlleScene::sliderEvent(Ref* pSender, cocos2d::ui::Slider::EventType type, bool volume)
{
	//ƒXƒ‰ƒCƒ_[‚ð0|100‚ÅŽæ‚é
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		if (volume)
		{
			//ƒXƒ‰ƒCƒ_[‚ÌˆÊ’u‡‚í‚¹‚é
			auto slider = dynamic_cast<ui::Slider*>(pSender);
			_Volume = slider->getPercent();
			_Volume = _Volume / 100;
			SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(_Volume);
			CCLOG("%f", _Volume);
		}
		else
		{
			//“§–¾“x
			auto slider = dynamic_cast<ui::Slider*>(pSender);
			auto Title = this->getChildByName("Title_Logo");
			_ButtonA = slider->getPercent();
			_ButtonA = (_ButtonA / 100) * 255;
			Title->setOpacity(_ButtonA);
			//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(_ButtonA);
			CCLOG("%f", _ButtonA);

		}
	}
}

void TitlleScene::selectedEvent(Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
	auto def = UserDefault::getInstance();
	auto spr = (CCSprite *)pSender;

	//ƒ`ƒFƒbƒNƒ{ƒbƒNƒX
	switch (type)
	{
	case ui::CheckBox::EventType::SELECTED:
		//‘I‘ðŽž
		_ButtonSwap = true;
		break;
	case ui::CheckBox::EventType::UNSELECTED:
		//”ñ‘I‘ðŽž
		_ButtonSwap = false;
		break;
	default:
		break;
	}
}


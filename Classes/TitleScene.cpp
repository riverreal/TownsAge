#include "TitleScene.h"
#include "ControlTesting03.h"
#include "GameData.h"

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

	auto Delay = DelayTime::create(0.5f);
	auto Fadein = FadeIn::create(1.5f);
	auto Sequence = Sequence::create(Delay, Fadein, NULL);

	auto TitleName = Sprite::create("img/ui/titleLogo.png");
	//透明度
	TitleName->setOpacity(0);
	TitleName->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
	TitleName->setScale(0.8f);
	//フェードイン
	TitleName->runAction(Sequence);
	this->addChild(TitleName);

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(TitlleScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

	closeItem->setVisible(false);
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto label = Label::createWithTTF("TOWNS AGE", "fonts/Marker Felt.ttf", 36);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);

	UserDefault* def = UserDefault::getInstance();
	//def->setIntegerForKey("AreaATM", 0);
	def->setIntegerForKey("MapArea", 1);
	def->setBoolForKey("FromRight", true);
	for (int i = 0; i < 14; ++i)
	{
		def->setIntegerForKey(mapData::leftMap[i].c_str(), 2);
		def->setIntegerForKey(mapData::rightMap[i].c_str(), 3);
	}
	def->setIntegerForKey(mapData::rightMap[1].c_str(), 2);

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

	char* messageTbl[] =
	{
		"  ",
		"  ",
		"  ",
		"  "

	};
	//テーブルの要素数だけ回す　ボタン追加するなら数字も追加
	for (int i = 0; i < 1; i++)
	{	//どのボタンが何番でどこにあるのか
		//偶数が右で奇数が左（ボタンの位置）
		if (i % 2 == 0)
		{
			ButtonCreate("", i, Vec2(visibleSize.width / 2, visibleSize.height / 3.7 - 22 * (i)));
		}
		else
		{
			ButtonCreate(messageTbl[i], i, Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2.5 * (i - 1)));
		}
	}

	

    return true;
}

void TitlleScene::ButtonCreate(char* name, int num, Vec2 pos)
{

	auto button = ui::Button::create("img/ui/new.png");


	auto tiltingFunc = CallFunc::create([button]() {
		auto quickFadeIn = FadeIn::create(0.8f);
		auto pause = DelayTime::create(1.0f);
		auto quickFadeOut = FadeOut::create(0.8f);
		auto shortPause = DelayTime::create(0.5f);
		auto tilting = RepeatForever::create(Sequence::create(quickFadeOut, shortPause, quickFadeIn, pause, NULL));
		button->runAction(tilting);
	});

	auto Delay = DelayTime::create(1.2f);
	auto Fadein = FadeIn::create(1.0f);
	auto Sequence = Sequence::create(Delay, Fadein, tiltingFunc, NULL);

	//ボタンの画像。仮画像
	
	//ポジション設定
	button->setPosition(pos);
	//9分割して引き伸ばし。文字がきれいに見える
	button->setScale9Enabled(true);
	//ラベル
	button->setTitleText(name);
	//フォントサイズ
	button->setTitleFontSize(30);
	//透明度
	button->setOpacity(0);
	//アクション
	button->runAction(Sequence);

	button->setName("new");

	this->addChild(button);
	//タグ付
	button->setTag(num);
	//タッチを有効に
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
	auto spr = (CCSprite *)pSender;
	auto tag = spr->getTag();
	//typeでタッチイベント指定
	switch (type)
	{
		//タッチ開始
	case ui::Widget::TouchEventType::BEGAN:
		//タグをゲットし、割り振る
		switch (tag)
		{
		case	0:
			Director::getInstance()->replaceScene(Control3::createScene());
			break;
		case	1:
			//ゲーム画面に遷移
			//Director::getInstance()->replaceScene(GameScene::createScene());
			break;
		case	2:
			//オプション画面に遷移
			//Director::getInstance()->pushScene(Opsion::createScene());
			break;
		case	3:
			//ED一覧に遷移
			//Director::getInstance()->replaceScene(Collection::createScene());
			break;

		default:
			break;
		}
		//ここまでボタン
		break;

	default:
		break;
	}
	//タッチ終了
}

void TitlleScene::Action()
{
	//フェードインとかまとめてこっちで管理したい|дﾟ)


}
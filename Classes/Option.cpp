#include "Option.h"
//SimpleAudioEngine用
using namespace CocosDenshion;

USING_NS_CC;

Scene* Option::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Option::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Option::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//ボタン配置と音量 
	//_volumeをセーブで全部管理できる便利！
	BGM_Control();
	text();
	BackButton();
	Button_Opacity();
	Button_Placement();
 	auto audio = SimpleAudioEngine::getInstance();
	//BGMデフォ 0.0f～1.0fまで 止める処理を後で入れる
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Test.mp3");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Test.mp3");
	//SEデフォ 0.0f～1.0f

	auto def = UserDefault::getInstance();
	_Volume = def->getFloatForKey("volume");
	_ButtonA = def->getFloatForKey("button");
	_ButtonSwap = def->getBoolForKey("ButtonSwap");
	CCLOG("volume: %f",_Volume);
		

    return true;
}
void Option::text()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto txt = CCLabelTTF::create("option", "arial", 100);
	txt->setPosition(Vec2(visibleSize.width + 500,visibleSize.height /2));
	txt->setColor(Color3B::WHITE);
	this->addChild(txt);
}

//volume設定
void Option::BGM_Control()
{
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	auto slider = ui::Slider::create();
	//バー まだ仮素材
	slider->loadBarTexture("img/ui/bar2.png");
	//つまみ　まだ仮素材
	slider->loadSlidBallTextures("img/ui/woodIcon.png");

	//ポジション
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 7.5));
	//スライダーにイベント適応
	slider->addEventListener(CC_CALLBACK_2(Option::sliderEvent, this, true));
	//つまみの位置
	slider->setPercent(static_cast<int>(def->getFloatForKey("volume") * 100));
	this->addChild(slider);

	//ラベル
	auto label = Label::createWithTTF("Volume", "fonts/misaki_gothic.ttf", 36);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5.7));
	this->addChild(label);


	
}
//ボタン設定
void Option::Button_Opacity()
{
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//透明度バー
	auto slider = ui::Slider::create();
	//バー まだ仮素材
	slider->loadBarTexture("img/ui/bar2.png");
	//つまみ　まだ仮素材
	slider->loadSlidBallTextures("img/ui/appleOrignal.png");
	//ポジション
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
	//スライダーにイベント適応
	slider->addEventListener(CC_CALLBACK_2(Option::sliderEvent, this,false));
	//つまみの位置。
	slider->setPercent(static_cast<int>(def->getFloatForKey("button")/255 * 100));
	
	this->addChild(slider);
	
	//ラベル
	auto label = Label::createWithTTF("透明度", "fonts/misaki_gothic.ttf", 36);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3.2));
	this->addChild(label);

}
//ボタン配置
void Option::Button_Placement()
{
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto checkBox = ui::CheckBox::create(
		"img/ui/appleOrignal.png",//非選択時
		"img/ui/appleOrignal.png",//非選択（ボタン降下時）
		"img/ui/stoneIcon.png",//選択時
		"img/ui/appleOrignal.png",//非選択時（ボタン無効）
		"img/ui/appleOrignal.png");//選択時（ボタン無効）
	
	//ポジション
	checkBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2.0));
	this->addChild(checkBox);
	checkBox->addEventListener(CC_CALLBACK_2(Option::selectedEvent, this));
	checkBox->setSelected(def->getBoolForKey("ButtonSwap"));

	auto label = Label::createWithTTF("切り替え", "fonts/misaki_gothic.ttf", 36);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.8));
	this->addChild(label);

}
//スライダーのイベント
void Option::sliderEvent(Ref* pSender, cocos2d::ui::Slider::EventType type,bool volume)
{ 
	//スライダーを0－100で取る
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		if (volume)
		{
			//スライダーの位置合わせる
			auto slider = dynamic_cast<ui::Slider*>(pSender);
			_Volume = slider->getPercent();
			_Volume = _Volume /100;
			SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(_Volume);
			CCLOG("%f", _Volume);
		}
		else
		{
			//透明度
			
			auto slider = dynamic_cast<ui::Slider*>(pSender);
			
			_ButtonA = slider->getPercent();
			_ButtonA = (_ButtonA / 100) *255;
			//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(_ButtonA);
			CCLOG("%f", _ButtonA);
			

		}
	}


}
void Option::BackButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//ボタンの画像。仮画像
	auto button = ui::Button::create("img/ui/woodIcon.png");
	//ポジション設定
	button->setPosition(Vec2(visibleSize.width / 12.1,visibleSize.height / 1.1));
	
	this->addChild(button);
	//タグ付
	button->setTag(1);
	//タッチを有効に
	button->addTouchEventListener(CC_CALLBACK_2(Option::touchEvent, this));


}
void Option::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	auto def = UserDefault::getInstance();
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
			//バックボタン
			case 1:
				def->setFloatForKey("volume",_Volume);
				def->setFloatForKey("button", _ButtonA);
				def->setBoolForKey("ButtonSwap", _ButtonSwap);
				Director::getInstance()->popScene();
			break;

		default:
			break;
		}
	
	}
	
}
void Option::selectedEvent(Ref *pSender, cocos2d::ui::CheckBox::EventType type)
{
	auto def = UserDefault::getInstance();
	auto spr = (CCSprite *)pSender;

	//チェックボックス
	switch (type)
	{
	case ui::CheckBox::EventType::SELECTED:
		//選択時
		_ButtonSwap = true;
		break;
	case ui::CheckBox::EventType::UNSELECTED:
		//非選択時
		_ButtonSwap = false;
		break;
	default:
		break;
	}
}



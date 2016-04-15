#include "TitleScene.h"
#include "ControlTesting01.h"
#include "ControlTesting02.h"
#include "ControlTesting03.h"

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
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(TitlleScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Control Testing", "fonts/Marker Felt.ttf", 36);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	auto control1 = MenuItemFont::create("Half Screen", CC_CALLBACK_1(TitlleScene::menuTransitionCallback, this, 0)); //index 0
	control1->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height*4/6);
	auto control2 = MenuItemFont::create("Classic", CC_CALLBACK_1(TitlleScene::menuTransitionCallback, this, 1)); //index 1
	control2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 3 / 6);
	auto control3 = MenuItemFont::create("Sliding", CC_CALLBACK_1(TitlleScene::menuTransitionCallback, this, 2)); //index 2
	control3->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 / 6);

	auto controlMenu = Menu::create(control1, control2, control3, NULL);
	controlMenu->setPosition(Vec2::ZERO);
	this->addChild(controlMenu);

    return true;
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
		scene = Control1::createScene();
		break;
	case 1: //index 1
		scene = Control2::createScene();
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

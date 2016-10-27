#include "MangaCutScene.h"
#include "ControlTesting03.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

cocos2d::Scene * MangaScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MangaScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MangaScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	m_mangaNode = Node::create();
	m_mangaNode->setCascadeOpacityEnabled(true);
	m_timeMachineNode = Node::create();
	this->addChild(m_mangaNode);
	this->addChild(m_timeMachineNode);

	auto audioEngine = SimpleAudioEngine::getInstance();
	audioEngine->preloadEffect("sound/sfx/manga/control.mp3");
	audioEngine->preloadEffect("sound/sfx/manga/robot_voice.wav");
	audioEngine->preloadEffect("sound/sfx/manga/break.mp3");
	audioEngine->preloadEffect("sound/sfx/manga/men_voice.wav");
	audioEngine->preloadEffect("sound/sfx/manga/gacha.mp3");
	audioEngine->preloadEffect("sound/sfx/manga/completed.mp3");
	audioEngine->preloadEffect("sound/sfx/manga/time_machine1.mp3");

	runCutScene();
	runTimeTravel();

	return true;
}

void MangaScene::update(float dt)
{

}

void MangaScene::runCutScene()
{
	auto audioEngine = SimpleAudioEngine::getInstance();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto whiteBG = LayerColor::create(Color4B::WHITE);
	whiteBG->setName("whiteBG");
	this->addChild(whiteBG, -10);

	//Pre Load Data------------------------------------------

	auto initialPos = m_mangaNode->convertToNodeSpace(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//initialPos.x += 3000;

	auto slice01 = Sprite::create("img/manga/slice01.png");
	slice01->setPosition(initialPos);
	slice01->setVisible(false);
	m_mangaNode->addChild(slice01);

	auto slice02 = Sprite::create("img/manga/slice02.png");
	slice02->setPosition(slice01->getPosition());
	slice02->setVisible(false);
	m_mangaNode->addChild(slice02);

	auto slice03 = Sprite::create("img/manga/slice03.png");
	slice03->setPosition(slice01->getPosition());
	slice03->setVisible(false);
	m_mangaNode->addChild(slice03);

	auto slice04 = Sprite::create("img/manga/slice04.png");
	slice04->setPosition(slice01->getPosition());
	slice04->setVisible(false);
	m_mangaNode->addChild(slice04);

	auto slice05 = Sprite::create("img/manga/slice05.png");
	slice05->setPosition(slice01->getPosition());
	slice05->setVisible(false);
	m_mangaNode->addChild(slice05);

	auto slice06 = Sprite::create("img/manga/slice06.png");
	slice06->setPosition(slice01->getPosition());
	slice06->setVisible(false);
	m_mangaNode->addChild(slice06);

	auto slice07 = Sprite::create("img/manga/slice07.png");
	slice07->setPosition(slice01->getPosition());
	slice07->setVisible(false);
	m_mangaNode->addChild(slice07);

	auto slice08 = Sprite::create("img/manga/slice08.png");
	slice08->setPosition(slice01->getPosition());
	slice08->setVisible(false);
	m_mangaNode->addChild(slice08);

	auto slice09 = Sprite::create("img/manga/slice09.png");
	slice09->setPosition(slice01->getPosition());
	slice09->setVisible(false);
	m_mangaNode->addChild(slice09);

	//repositioning
	slice02->setPositionX(slice02->getPositionX() - slice02->getContentSize().width / 2 - slice01->getContentSize().width / 2 - 10);
	slice03->setPositionX(slice02->getPositionX() - slice03->getContentSize().width / 2 - slice02->getContentSize().width / 2 + 80);
	slice04->setPositionX(slice04->getPositionX() - (slice04->getContentSize().width / 2 - slice01->getContentSize().width / 2));
	slice04->setPositionY(slice04->getPositionY() - slice01->getContentSize().height / 2 - slice04->getContentSize().height / 2 - 10);
	slice06->setPositionX(slice04->getPositionX() - (slice06->getContentSize().width/2 - slice04->getContentSize().width/2));
	slice06->setPositionY(slice04->getPositionY() - 630);
	slice07->setPositionX(slice03->getPositionX() + (slice07->getContentSize().width/2 - slice03->getContentSize().width/2) - 10 );
	slice07->setPositionY(slice06->getPositionY() + (slice07->getContentSize().height/2 - slice06->getContentSize().height/2));
	slice08->setPositionX(slice04->getPositionX() - (slice08->getContentSize().width/2 - slice04->getContentSize().width/2));
	slice08->setPositionY(slice06->getPositionY() - slice08->getContentSize().height/2 - slice06->getContentSize().height/2 - 10);
	slice09->setPositionX(slice03->getPositionX() + (slice09->getContentSize().width / 2 - slice03->getContentSize().width / 2));
	slice09->setPositionY(slice08->getPositionY());
	//-------------------------------------------------------------------
	//Cut Scene animation------------------------------------------------
	//-------------------------------------------------------------------

	//Slice 01---------------------------------------------- Total Duration : 2.3f
	slice01->setOpacity(0);
	slice01->setVisible(true);
	
	auto slice01MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(550, 0))); };

	auto slice01SFX = [this, audioEngine]() {audioEngine->playEffect("sound/sfx/manga/control.mp3"); };

	auto slice01Actions = CallFunc::create([slice01, slice01MangaMoveFunc, slice01SFX]()
	{
		auto slice01Delay = DelayTime::create(0.6f);
		auto slice01fadeIn = FadeIn::create(0.3f);
		auto slice01AfterDelay = DelayTime::create(1.1f);
		auto slice01Rotate1 = RotateBy::create(0.15f, -20);
		auto slice01Rotate2 = RotateBy::create(0.15f, 20);
		auto slice01NextSlice = CallFunc::create(slice01MangaMoveFunc);
		auto slice01SoundEffect = CallFunc::create(slice01SFX);

		auto slice01Sequence = Sequence::create(slice01Delay, slice01fadeIn, slice01AfterDelay, slice01Rotate1, slice01SoundEffect, slice01Rotate2, slice01NextSlice, NULL);

		slice01->runAction(slice01Sequence); 
	});

	//Slice 02---------------------------------------------- Total Duration : 4.05f
	slice02->setOpacity(0);
	slice02->setVisible(true);
	
	auto slice02MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(400, 0))); };

	auto slice02SFX = [this, audioEngine]() {audioEngine->playEffect("sound/sfx/manga/robot_voice.wav"); };


	auto slice02Actions = CallFunc::create([slice02, slice02MangaMoveFunc, slice02SFX]()
	{
		auto slice02Delay = DelayTime::create(2.3f);
		auto slice02FadeIn = FadeIn::create(0.05f);
		auto slice02ZoomIn = ScaleBy::create(0.8f, 1.5f);
		auto slice02AfterDelay = DelayTime::create(0.8f);
		auto slice02MoveDiagonal = MoveBy::create(0.8f, Vec2(-300, 200));
		auto slice02ZoomMove = Spawn::create(slice02ZoomIn, slice02MoveDiagonal, NULL);
		auto slice02ZoomOut = ScaleBy::create(0.8f, 0.66f);
		auto slice02MoveBack = MoveBy::create(0.8f, Vec2(300, -200));
		auto slice02ZoomMoveBack = Spawn::create(slice02ZoomOut, slice02MoveBack, NULL);
		auto slice02NextSlice = CallFunc::create(slice02MangaMoveFunc);
		auto slice02SoundEffect = CallFunc::create(slice02SFX);

		auto slice02Sequence = Sequence::create(slice02Delay, slice02FadeIn, slice02AfterDelay, slice02SoundEffect, slice02ZoomMove, slice02AfterDelay, slice02ZoomMoveBack, slice02AfterDelay, slice02NextSlice, NULL);

		slice02->runAction(slice02Sequence);
	});

	//Slice 03---------------------------------------------- Total Duration : 1.7f
	slice03->setOpacity(0);
	slice03->setVisible(true);
	slice03->setScale(2.5f);

	auto slice03MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(-900, 350))); };

	auto slice03SFX = [this, audioEngine]() {audioEngine->playEffect("sound/sfx/manga/break.mp3"); };

	auto slice03Actions = CallFunc::create([slice03, slice03MangaMoveFunc, slice03SFX]()
	{
		auto slice03Delay = DelayTime::create(6.35f);
		auto slice03ScaleBack = ScaleTo::create(0.3f, 1.0f);
		auto slice03FadeIn = FadeIn::create(0.3f);
		auto slice03FadeScale = Spawn::create(slice03ScaleBack, slice03FadeIn, NULL);
		auto slice03Shake01 = MoveBy::create(0.05f, Vec2(8, 7));
		auto slice03Shake02 = MoveBy::create(0.05f, Vec2(5, -6));
		auto slice03Shake03 = MoveBy::create(0.05f, Vec2(-9, 5));
		auto slice03Shake04 = MoveBy::create(0.05f, Vec2(6, -8));
		auto slice03Shake05 = MoveBy::create(0.05f, Vec2(-5, 7));
		auto slice03Shake06 = MoveBy::create(0.05f, Vec2(8, -6));
		auto slice03Shake07 = MoveBy::create(0.05f, Vec2(-7, 7));
		auto slice03Shake08 = MoveBy::create(0.05f, Vec2(9, 9));
		auto slice03Shake09 = MoveBy::create(0.05f, Vec2(-15, -15));
		auto slice03ShakeSequence = Sequence::create(slice03Shake01, slice03Shake02, slice03Shake03, slice03Shake04, slice03Shake05, slice03Shake06, slice03Shake07, slice03Shake08, slice03Shake09, 
			slice03Shake01, slice03Shake02, slice03Shake03, slice03Shake04, slice03Shake05, slice03Shake06, slice03Shake07, slice03Shake08, slice03Shake09,
			NULL);
		auto slice03SmallDelay = DelayTime::create(0.5f);
		auto slice03NextSlice = CallFunc::create(slice03MangaMoveFunc);

		auto slice03SoundEffect = CallFunc::create(slice03SFX);

		auto slice03Sequence = Sequence::create(slice03Delay, slice03FadeScale, slice03SoundEffect, slice03ShakeSequence, slice03SmallDelay, slice03NextSlice, NULL);

		slice03->runAction(slice03Sequence);
	});

	//Slice 04---------------------------------------------- Total Duration : 0.5f
	auto slice04MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(0, 330))); };

	auto slice04SFX = [this, audioEngine]() {audioEngine->playEffect("sound/sfx/manga/men_voice.wav"); };

	auto slice04Actions = CallFunc::create([slice04, slice04MangaMoveFunc, slice04SFX]()
	{
		auto slice04Delay = DelayTime::create(8.45f);
		auto slice04Show = Show::create();
		auto slice04Shake01 = MoveBy::create(0.05f, Vec2(20, 0));
		auto slice04Shake02 = MoveBy::create(0.05f, Vec2(-23, 0));
		auto slice04Shake03 = MoveBy::create(0.05f, Vec2(22, 0));
		auto slice04Shake04 = MoveBy::create(0.05f, Vec2(-16, 0));
		auto slice04Shake05 = MoveBy::create(0.05f, Vec2(-3, 0));
		auto slice04ShakeSequence = Sequence::create(slice04Shake01, slice04Shake02, slice04Shake03, slice04Shake04, slice04Shake05, 
			slice04Shake01, slice04Shake02, slice04Shake03, slice04Shake04, slice04Shake05, NULL);
		auto slice04AfterDelay = DelayTime::create(0.5f);
		auto slice04NextSlice = CallFunc::create(slice04MangaMoveFunc);
		auto slice04SoundEffect = CallFunc::create(slice04SFX);

		auto slice04Sequence = Sequence::create(slice04Delay, slice04Show, slice04SoundEffect, slice04ShakeSequence, slice04AfterDelay, slice04NextSlice, NULL);

		slice04->runAction(slice04Sequence);
	});

	//Slice 05---------------------------------------------- Total Duration : 1.4f
	slice05->setVisible(true);
	slice05->setOpacity(0);

	auto slice05MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(0, 350))); };
	auto slice05NewPos = slice04->getPosition();
	slice05NewPos.x -= (slice05->getContentSize().width / 2 - slice04->getContentSize().width / 2);
	slice05NewPos.y -= 268;
	
	auto slice05Actions = CallFunc::create([slice05, slice05MangaMoveFunc, slice05NewPos]()
	{
		auto slice05Delay = DelayTime::create(10.1f);
		auto slice05FadeIn = FadeIn::create(0.4f);
		auto slice05MoveDown = MoveTo::create(0.4f, Vec2(slice05NewPos));
		auto slice05FadeMove = Spawn::create(slice05FadeIn, slice05MoveDown, NULL);
		auto slice05AfterDelay = DelayTime::create(1.0f);
		auto slice05NextSlice = CallFunc::create(slice05MangaMoveFunc);
		auto slice05Sequence = Sequence::create(slice05Delay, slice05FadeMove, slice05AfterDelay, slice05NextSlice, NULL);
		slice05->runAction(slice05Sequence);

	});

	//Slice 06---------------------------------------------- Total Duration : 2.55
	slice06->setVisible(true);
	slice06->setOpacity(0);
	
	auto slice06MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(800, -285))); };

	auto slice06SFX = [this, audioEngine]() {audioEngine->playEffect("sound/sfx/manga/gacha.mp3"); };

	auto slice06Actions = CallFunc::create([slice06, slice06MangaMoveFunc, slice06SFX]()
	{
		
		auto slice06Delay = DelayTime::create(11.9f);
		auto slice06FadeIn = FadeIn::create(0.6f);
		auto slice06RotateL = RotateTo::create(0.15f, -6);
		auto slice06RotateR = RotateTo::create(0.15f, 6);
		auto slice06RotateBack = RotateTo::create(0.15f, 0);
		auto slide06RotationSequence = Sequence::create(slice06RotateL, slice06RotateR, slice06RotateL, slice06RotateR, slice06RotateL, slice06RotateR, slice06RotateL, slice06RotateR,
			slice06RotateL, slice06RotateR, slice06RotateL, slice06RotateR, slice06RotateBack,
			NULL);

		auto slice06NextSlice = CallFunc::create(slice06MangaMoveFunc);

		auto slice06SoundEffect = CallFunc::create(slice06SFX);

		auto slice06Sequence = Sequence::create(slice06Delay, slice06SoundEffect, slice06FadeIn, slide06RotationSequence, slice06NextSlice, NULL);

		slice06->runAction(slice06Sequence);

	});

	//Slice 07---------------------------------------------- Total Duration : 1.3f
	slice07->setVisible(true);
	slice07->setOpacity(0);
	slice07->setScale(0.3f);

	auto slice07MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(-800, 665))); };
	auto slice07MangaZoomOut = [this]() {
		this->m_mangaNode->runAction(ScaleBy::create(0.4f, 0.6f));
		this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(-100, -165)));
	};

	auto slice08MangaZoomIn = [this]() {
		this->m_mangaNode->runAction(ScaleBy::create(0.4f, 1.36f));
		this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(100, 165)));
	};

	auto slice07SFX = [this, audioEngine]() {audioEngine->stopAllEffects(); audioEngine->playEffect("sound/sfx/manga/completed.mp3"); };

	auto slice07Actions = CallFunc::create([slice07, slice07MangaMoveFunc, slice07MangaZoomOut, slice08MangaZoomIn, slice07SFX]()
	{
		auto slice07ZoomOut = CallFunc::create(slice07MangaZoomOut);
		auto slice07Delay = DelayTime::create(14.45f);
		auto slice07FadeIn = FadeIn::create(0.5f);
		auto slice07Enlarge = ScaleTo::create(0.5f, 1.0f);
		auto slice07Rotate = RotateBy::create(0.3f, 360);
		auto slice07FadeEnglarge = Spawn::create(slice07FadeIn, slice07Enlarge, slice07Rotate, NULL);
		auto slice07AfterDelay = TintTo::create(1.5f, Color3B::WHITE);
		auto slice08FixZoom = CallFunc::create(slice08MangaZoomIn);
		auto slice07NextSlice = CallFunc::create(slice07MangaMoveFunc);

		auto slice07SoundEffect = CallFunc::create(slice07SFX);

		auto slice07Sequence = Sequence::create(slice07Delay, slice07SoundEffect, slice07ZoomOut, slice07FadeEnglarge, slice07AfterDelay, slice08FixZoom, slice07NextSlice, NULL);

		slice07->runAction(slice07Sequence);
	});

	//Slice 08---------------------------------------------- Total Duration : 1.6f
	slice08->setVisible(true);
	slice08->setOpacity(0);
	slice08->setPositionY(slice08->getPositionY() + 300);

	auto slice08MangaMoveFunc = [this]() {this->m_mangaNode->runAction(MoveBy::create(0.3f, Vec2(800, 0))); };

	auto slice08Actions = CallFunc::create([slice08, slice08MangaMoveFunc, slice08MangaZoomIn]()
	{
		auto slice08ZoomIn = CallFunc::create(slice08MangaZoomIn);
		auto slice08Delay = DelayTime::create(16.55f);
		auto slice08FadeIn = FadeIn::create(0.3f);
		auto slice08MoveDown = MoveBy::create(0.3f, Vec2(0, -300));
		auto slice08FadeMove = Spawn::create(slice08FadeIn, slice08MoveDown, NULL);
		auto slice08AfterDelay = TintTo::create(1.3f, Color3B::WHITE);
		auto slice08NextSlice = CallFunc::create(slice08MangaMoveFunc);

		auto slice08Sequence = Sequence::create(slice08Delay, slice08FadeMove, slice08AfterDelay, slice08NextSlice, NULL);

		slice08->runAction(slice08Sequence);
	});

	//Slice 09---------------------------------------------- Total Duration : 
	slice09->setPositionY(slice09->getPositionY() - 400);
	slice09->setVisible(true);
	slice09->setOpacity(0);

	auto slice09Actions = CallFunc::create([slice09]()
	{
		auto slice09Delay = DelayTime::create(18.65f);
		auto slice09FadeIn = FadeIn::create(0.4f);
		auto slice09MoveUp = MoveBy::create(0.4f, Vec2(0, 400));
		auto slice09FadeMove = Spawn::create(slice09FadeIn, slice09MoveUp, NULL);

		auto slice09Sequence = Sequence::create(slice09Delay, slice09FadeMove, NULL);

		slice09->runAction(slice09Sequence);
	});

	//all animations
	m_mangaNode->runAction(Sequence::create(slice01Actions, slice02Actions, slice03Actions, slice04Actions, slice05Actions, slice06Actions, slice07Actions, slice08Actions, slice09Actions, NULL));
}

void MangaScene::runTimeTravel()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto whiteBG = this->getChildByName("whiteBG");
	auto delayForWhite = DelayTime::create(21.0f);
	auto fadeOutWhite = FadeOut::create(2.0f);
	whiteBG->runAction(Sequence::create(delayForWhite, fadeOutWhite, NULL));

	auto mangaFadeOut = FadeOut::create(2.0f);
	auto mangaDelay = DelayTime::create(20.5f);
	m_mangaNode->runAction(Sequence::create(mangaDelay, mangaFadeOut, NULL));

	auto timeMachineFinal = Sprite::create("img/timeMachine/TimeMachine3.png");
	timeMachineFinal->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	timeMachineFinal->setOpacity(0);
	timeMachineFinal->setScale(2.5f);
	m_timeMachineNode->addChild(timeMachineFinal);

	auto timeMachinePart1 = Sprite::create("img/timeMachine/TimeMachinePart1.png");
	timeMachinePart1->setOpacity(0);
	timeMachinePart1->setScale(2.4f);
	timeMachinePart1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	m_timeMachineNode->addChild(timeMachinePart1);

	auto timeMachinePart2 = Sprite::create("img/timeMachine/TimeMachinePart2.png");
	timeMachinePart2->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	timeMachinePart2->setScale(2.4f);
	timeMachinePart2->setOpacity(0);
	m_timeMachineNode->addChild(timeMachinePart2);

	auto timeMachinePart3 = Sprite::create("img/timeMachine/TimeMachinePart3.png");
	timeMachinePart3->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	timeMachinePart3->setScale(2.4f);
	timeMachinePart3->setOpacity(0);
	m_timeMachineNode->addChild(timeMachinePart3);

	auto timeTravelBG = Sprite::create("img/timeMachine/trick-art2.png");
	timeTravelBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	timeTravelBG->setScale(2.0f);
	timeTravelBG->setOpacity(0);
	m_timeMachineNode->addChild(timeTravelBG, -10);

	auto trickArtBG = Sprite::create("img/timeMachine/travel-bg.png");
	trickArtBG->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	trickArtBG->setScale(1.0f);
	trickArtBG->setOpacity(0);
	//trickArtBG->setVisible(false);
	m_timeMachineNode->addChild(trickArtBG, -2);

	auto travelEffect = ParticleSystemQuad::create("img/effects/timeTravelEffect/timeTravel04.plist");
	travelEffect->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	travelEffect->setVisible(false);
	travelEffect->setOpacity(180);
	m_timeMachineNode->addChild(travelEffect, -1);

	//-----------------------------------------------------------------------------------
	//time travel anim

	//---------------------------Time Machine----------------------------------------------------------

	auto timeMachineDelay = DelayTime::create(22.0f);
	auto timeMachineFadeIn = FadeIn::create(2.5f);

	auto timeMachinePath1 = MoveBy::create(0.2f, Vec2(-30, 20));
	auto timeMachinePath2 = MoveBy::create(0.6f, Vec2(50, 40));
	auto timeMachinePath3 = MoveBy::create(0.4f, Vec2(-50, -45));
	auto timeMachinePath4 = MoveBy::create(0.5f, Vec2(70, -55));
	auto timeMachinePath5 = MoveBy::create(0.9f, Vec2(-40, 40));
	auto timeMachinePathSequence = Sequence::create(timeMachinePath1, timeMachinePath2, timeMachinePath3, timeMachinePath4, timeMachinePath5, NULL);
	auto timeMachineRepeat = Repeat::create(timeMachinePathSequence, 20);

	auto timeMachineRotDelay = DelayTime::create(3.5f);
	auto timeMachineRot1 = RotateBy::create(0.2f, -45.0f);
	auto timeMachineRot2 = RotateBy::create(0.2f, 90.0f);
	auto timeMachineRot3 = RotateBy::create(0.2f, -80.0f);
	auto timeMachineRot4 = RotateBy::create(0.2f, 93.0f);
	auto timeMachineRot5 = RotateBy::create(0.2f, -98.0f);
	auto timeMachineRot6 = RotateBy::create(0.2f, 80.0f);
	auto timeMachineRot7 = RotateBy::create(0.2f, -80.0f);
	auto timeMachineRot8 = RotateBy::create(0.2f, 70.0f);
	auto timeMachineRot9 = RotateTo::create(0.2f, 0.0f);
	auto timeMachineDropPart = CallFunc::create([timeMachinePart1, timeMachinePart2, timeMachinePart3]() {

		static int repeatCounter = 0;

		auto fadeIn = FadeIn::create(0.1f);

		int side = RandomHelper::random_int(0, 1);
		if (side == 0)
			side = -1;

		auto moveX = MoveBy::create(0.8f, Vec2(side * 400.0f, 0.0f));
		auto moveY1 = MoveBy::create(0.4f, Vec2(0, 170.0f));
		auto moveY2 = MoveBy::create(0.4f, Vec2(0, -230.0f));
		auto moveY = Sequence::create(moveY1, moveY2, NULL);
		auto move = Spawn::create(moveX, moveY, NULL);

		auto fadeOut = FadeOut::create(2.5f);

		auto sequence = Sequence::create(fadeIn, move, fadeOut, NULL);
		CCLOG("repeat Counter: %i", repeatCounter);
		switch (repeatCounter)
		{
		case 0:
			timeMachinePart1->runAction(sequence);
			break;
		case 1:
			timeMachinePart2->runAction(sequence);
			break;
		case 2:
			timeMachinePart3->runAction(sequence);
			break;
		default:
			//timeMachinePart1->runAction(sequence);
			break;
		}

		repeatCounter++;
		
	});

	auto timeMachineRotSequence = Sequence::create(timeMachineRotDelay, timeMachineRot1, timeMachineRot2, timeMachineRot3, timeMachineRot4, timeMachineRot5, timeMachineRot6, timeMachineRot7, timeMachineRot8, timeMachineRot9, timeMachineDropPart, NULL);
	auto timeMachineRotRep = Repeat::create(timeMachineRotSequence, 3);

	auto timeMachineAnim = Spawn::create(timeMachineRepeat, timeMachineRotRep, NULL);


	auto timeMachineSoundEffect = CallFunc::create([this]() {SimpleAudioEngine::getInstance()->playEffect("sound/sfx/manga/time_machine1.mp3"); });

	timeMachineFinal->runAction(Sequence::create(timeMachineDelay, timeMachineSoundEffect, timeMachineFadeIn, timeMachineAnim, NULL));

	auto ttBGDelay = DelayTime::create(24.0f);
	auto ttBGFadeIn = FadeIn::create(1.0f);
	auto ttBGRotate = RotateBy::create(10.0f, 360);
	auto ttBGRepeat = Repeat::create(ttBGRotate, 5);
	timeTravelBG->runAction(Sequence::create(ttBGDelay, ttBGFadeIn, ttBGRepeat, NULL));

	auto taBGDelay = DelayTime::create(25.0f);
	auto taBGShow = FadeIn::create(0.3f);
	auto taBGPause = DelayTime::create(0.5f);
	auto taBGHide = FadeOut::create(0.3f);
	//auto taRepeat = Repeat::create(Sequence::create(taBGShow, taBGPause, taBGHide, taBGPause, NULL), 20);
	trickArtBG->runAction(Sequence::create(taBGDelay, taBGShow, NULL));

	auto effectDelay = DelayTime::create(25.0f);
	auto effectShow = Show::create();
	travelEffect->runAction(Sequence::create(effectDelay, effectShow, NULL));


	//--------Next Scene jump----------------------------------------------
	auto nextSceneDelay = DelayTime::create(45.5f);
	auto startNextScene = CallFunc::create([this]() {this->startTutorial(); });
	
	
	auto audioEngine = SimpleAudioEngine::getInstance();

	auto unloadEffects = CallFunc::create([audioEngine]() {
		audioEngine->unloadEffect("sound/sfx/manga/control.mp3");
		audioEngine->unloadEffect("sound/sfx/manga/robot_voice.wav");
		audioEngine->unloadEffect("sound/sfx/manga/break.mp3");
		audioEngine->unloadEffect("sound/sfx/manga/men_voice.wav");
		audioEngine->unloadEffect("sound/sfx/manga/gacha.mp3");
		audioEngine->unloadEffect("sound/sfx/manga/completed.mp3");
		audioEngine->unloadEffect("sound/sfx/manga/time_machine1.mp3");
	});

	this->runAction(Sequence::create(nextSceneDelay, unloadEffects, startNextScene, NULL));
}

void MangaScene::startTutorial()
{
	auto tutorialScene = Control3::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, tutorialScene));
}

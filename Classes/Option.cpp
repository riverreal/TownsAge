#include "Option.h"
//SimpleAudioEngine�p
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
	//�{�^���z�u�Ɖ��� 
	//_volume���Z�[�u�őS���Ǘ��ł���֗��I
	BGM_Control();
	text();
	BackButton();
	Button_Opacity();
	Button_Placement();
 	auto audio = SimpleAudioEngine::getInstance();
	//BGM�f�t�H 0.0f�`1.0f�܂� �~�߂鏈������œ����
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Test.mp3");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Test.mp3");
	//SE�f�t�H 0.0f�`1.0f

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

//volume�ݒ�
void Option::BGM_Control()
{
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	auto slider = ui::Slider::create();
	//�o�[ �܂����f��
	slider->loadBarTexture("img/ui/bar2.png");
	//�܂݁@�܂����f��
	slider->loadSlidBallTextures("img/ui/woodIcon.png");

	//�|�W�V����
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 7.5));
	//�X���C�_�[�ɃC�x���g�K��
	slider->addEventListener(CC_CALLBACK_2(Option::sliderEvent, this, true));
	//�܂݂̈ʒu
	slider->setPercent(static_cast<int>(def->getFloatForKey("volume") * 100));
	this->addChild(slider);

	//���x��
	auto label = Label::createWithTTF("Volume", "fonts/misaki_gothic.ttf", 36);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5.7));
	this->addChild(label);


	
}
//�{�^���ݒ�
void Option::Button_Opacity()
{
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//�����x�o�[
	auto slider = ui::Slider::create();
	//�o�[ �܂����f��
	slider->loadBarTexture("img/ui/bar2.png");
	//�܂݁@�܂����f��
	slider->loadSlidBallTextures("img/ui/appleOrignal.png");
	//�|�W�V����
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
	//�X���C�_�[�ɃC�x���g�K��
	slider->addEventListener(CC_CALLBACK_2(Option::sliderEvent, this,false));
	//�܂݂̈ʒu�B
	slider->setPercent(static_cast<int>(def->getFloatForKey("button")/255 * 100));
	
	this->addChild(slider);
	
	//���x��
	auto label = Label::createWithTTF("�����x", "fonts/misaki_gothic.ttf", 36);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3.2));
	this->addChild(label);

}
//�{�^���z�u
void Option::Button_Placement()
{
	auto def = UserDefault::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto checkBox = ui::CheckBox::create(
		"img/ui/appleOrignal.png",//��I����
		"img/ui/appleOrignal.png",//��I���i�{�^���~�����j
		"img/ui/stoneIcon.png",//�I����
		"img/ui/appleOrignal.png",//��I�����i�{�^�������j
		"img/ui/appleOrignal.png");//�I�����i�{�^�������j
	
	//�|�W�V����
	checkBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2.0));
	this->addChild(checkBox);
	checkBox->addEventListener(CC_CALLBACK_2(Option::selectedEvent, this));
	checkBox->setSelected(def->getBoolForKey("ButtonSwap"));

	auto label = Label::createWithTTF("�؂�ւ�", "fonts/misaki_gothic.ttf", 36);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.8));
	this->addChild(label);

}
//�X���C�_�[�̃C�x���g
void Option::sliderEvent(Ref* pSender, cocos2d::ui::Slider::EventType type,bool volume)
{ 
	//�X���C�_�[��0�|100�Ŏ��
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		if (volume)
		{
			//�X���C�_�[�̈ʒu���킹��
			auto slider = dynamic_cast<ui::Slider*>(pSender);
			_Volume = slider->getPercent();
			_Volume = _Volume /100;
			SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(_Volume);
			CCLOG("%f", _Volume);
		}
		else
		{
			//�����x
			
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

	//�{�^���̉摜�B���摜
	auto button = ui::Button::create("img/ui/woodIcon.png");
	//�|�W�V�����ݒ�
	button->setPosition(Vec2(visibleSize.width / 12.1,visibleSize.height / 1.1));
	
	this->addChild(button);
	//�^�O�t
	button->setTag(1);
	//�^�b�`��L����
	button->addTouchEventListener(CC_CALLBACK_2(Option::touchEvent, this));


}
void Option::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	auto def = UserDefault::getInstance();
	auto spr = (CCSprite *)pSender;
	auto tag = spr->getTag();
	//type�Ń^�b�`�C�x���g�w��
	switch (type)
	{
		//�^�b�`�J�n
	case ui::Widget::TouchEventType::BEGAN:
		//�^�O���Q�b�g���A����U��
		switch (tag)
		{
			//�o�b�N�{�^��
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

	//�`�F�b�N�{�b�N�X
	switch (type)
	{
	case ui::CheckBox::EventType::SELECTED:
		//�I����
		_ButtonSwap = true;
		break;
	case ui::CheckBox::EventType::UNSELECTED:
		//��I����
		_ButtonSwap = false;
		break;
	default:
		break;
	}
}



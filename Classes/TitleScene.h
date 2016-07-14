#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
//SimpleAudioEngine—p
#include "SimpleAudioEngine.h"
//ƒXƒ‰ƒCƒ_[—p
#include "ui/CocosGUI.h"
#include"ParallaxBackground.h"
#pragma execution_character_set("utf-8");

#include"Option.h"

class TitlleScene : public cocos2d::Layer
{
private:
	cocos2d::TMXTiledMap*m_tilemap;
	cocos2d::Node*m_gameNode;

	ParallaxBackground m_background;
	cocos2d::Node* m_backgroundNode;
	cocos2d::Node* m_skyNode;
	cocos2d::Node* m_foregroundNode;
	//ŽžŠÔ@0`7200
	int m_DayTime;
	//ƒIƒvƒVƒ‡ƒ“ŠÖ˜A
	float _Volume = 0.5;
	float _ButtonA = 100;
	bool _ButtonSwap = false;
	const int option_back_button = 4;

public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float dt);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void menuTransitionCallback(cocos2d::Ref* pSender, int index);

	void ButtonCreate();
	void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Action();

	void option();
	void sliderEvent(Ref* pSender, cocos2d::ui::Slider::EventType type, bool volume);
	void selectedEvent(Ref *pSender, cocos2d::ui::CheckBox::EventType type);

	cocos2d::Vec2 setViewpoint(cocos2d::Vec2 position);


	// implement the "static create()" method manually
	CREATE_FUNC(TitlleScene);

	/*private:
	Option option;
	*/
};

#endif // __TITLE_SCENE_H__

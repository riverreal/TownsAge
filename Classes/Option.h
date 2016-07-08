#ifndef __OPTION_H__
#define __OPTION_H__

#include "TitleScene.h"

#include "cocos2d.h"
//SimpleAudioEngine用
#include "SimpleAudioEngine.h"
//スライダー用
#include "ui/CocosGUI.h"



class Option : public cocos2d::Layer
{	
public:

	
	
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void text();
	void BGM_Control();
	void sliderEvent(Ref* pSender, cocos2d::ui::Slider::EventType type, bool volume);
	void BackButton();
	void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void selectedEvent(Ref *pSender, cocos2d::ui::CheckBox::EventType type);
	void Button_Opacity();
	void Button_Placement();

    // implement the "static create()" method manually
    CREATE_FUNC(Option);

private:
	float _Volume = 0.5;
	float _ButtonA = 100;
	bool _ButtonSwap = false;
};

#endif // __HELLOWORLD_SCENE_H__

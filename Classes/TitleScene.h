#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#pragma execution_character_set("utf-8");

#include"Option.h"

class TitlleScene : public cocos2d::Layer
{

public:

    static cocos2d::Scene* createScene();

    virtual bool init();
    

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	void menuTransitionCallback(cocos2d::Ref* pSender, int index);

	void ButtonCreate();
	void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Action();

    
	

    // implement the "static create()" method manually
    CREATE_FUNC(TitlleScene);

/*private:
	Option option;
	*/
};

#endif // __TITLE_SCENE_H__

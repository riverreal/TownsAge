#pragma once

#include "cocos2d.h"

#pragma execution_character_set("utf-8");

class Ending : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Ending);
};

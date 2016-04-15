#ifndef __Control1_H__
#define __Control1_H__

#include "cocos2d.h"

class Control1 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update();

	void walk(bool directionRight);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	//controls
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* eventt);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* eventt);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Control1);

private:
	cocos2d::Sprite* m_character;
	bool m_directionRight;
};

#endif // __Control1_H__

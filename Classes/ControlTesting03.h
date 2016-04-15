#ifndef __Control3_H__
#define __Control3_H__

#include "cocos2d.h"

class Control3 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float dt);

	void walk(bool directionRight);
	void setViewpoint(cocos2d::Vec2 position);
	cocos2d::Vec2 convertToTilePosition(cocos2d::Vec2 position);
	void simplePhysics(); //will go inside update

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	//controls
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touch, cocos2d::Event* eventt);

    // implement the "static create()" method manually
    CREATE_FUNC(Control3);

private:
	enum TOUCH_INDEX
	{
		FIRST_TOUCH,
		SECOND_TOUCH,
		THRID_TOUCH,
		FORTH_TOUCH
	};

	cocos2d::Sprite* m_character;
	cocos2d::TMXTiledMap* m_tilemap;
	cocos2d::TMXLayer* m_collisionLayer;
	bool m_directionRight;

	//Control related
	cocos2d::Rect m_rightRect;
	cocos2d::Rect m_leftRect;
	bool m_aButtonPressed;
	cocos2d::Rect m_aButtonRect;
	int m_moveTouchID;
	int m_actionTouchID;

	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_uiNode;

	//---------------------
	//----physics variables/constants
	//---------------------

	//gravity acceleration for character
	const float GRAVITY = -0.2;
	
	//max speed for character
	const float MAX_SPEED = 3;

	//max jump capacity
	const float MAX_JUMP = 5;

	//player states
	bool m_standing;
	bool m_onGround;
	bool m_jumpInstanced;

	//player variables
	float m_speedX;
	float m_speedY;
	float m_accelerationX;
	float m_frictionX;
	float m_jumpForce;

};

#endif // __Control3_H__

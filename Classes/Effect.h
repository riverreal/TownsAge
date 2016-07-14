#pragma once

#include "cocos2d.h"

class Effect
{
public:
	Effect();
	~Effect();

	//Shade a Node 5 times 0.05 seconds each shake (total of 0.25 seconds).
	void ShakeNode(cocos2d::Node* node);
	//Tick healing/dmg effect on screen
	//Every tick lasts 0.1 seconds. And has a 0.4 seconds delay (0.5 seconds total from tick to tick).
	//eg. tickCount = 10 -> tick effect lasts for 5 seconds.
	void TickScreenWithColor(unsigned int tickCount, cocos2d::Color3B tickColor);

	cocos2d::LayerColor* GetColorScreen();

private:
	cocos2d::LayerColor* m_onScreenEffect;
};
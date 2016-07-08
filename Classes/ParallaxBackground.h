#pragma once

#include "cocos2d.h"
#define PI 3.1415926

class ParallaxBackground
{
public:
	ParallaxBackground();
	~ParallaxBackground();

	//Initializes class getting the ground position and current game time.
	void Init(cocos2d::Vec2 initialPos, float currentTime, float visibleSizeX, cocos2d::Node* gameNode);
	//Adds a new layer of background to the background/data vector.
	void AddParallaxLayer(std::string filePath, float scrollVel, cocos2d::Vec2 initialPos, float scale);
	//Updates each background with it's respective modifications.
	void UpdateBackground(cocos2d::Vec2 cameraPos, int timeOfDay);

	cocos2d::Node* GetBackgroundNode();
	cocos2d::Node* GetSkyNode();
	cocos2d::Sprite* GetShadeSprite();

	float Interpolate(float x1, float x2, float x3, float y1, float y3);
	float Clamp(float toClamp, float maxValue, float minValue);
private:
	
	void UpdateSky(int timeOfDay);
	void UpdateSunMoon(int timeOfDay);

	cocos2d::Color4B GetSkyColorTopByTime(float timeOfDay);
	cocos2d::Color4B GetSkyColorBottomByTime(float timeOfDay);
	float GetAngleByTime(float timeOfDay, bool sun);
	float GetShadeAlphaByTime(float timeOfDay);

	struct BackgroundData
	{
		BackgroundData()
		{
		};

		float scrollVel;
	};

private:
	//Node that will be added to the scene. This Node is the parent of all the backgrounds.
	cocos2d::Node* m_backgroundNode;
	//Node that will be added to the scene. This Node is the parent of all celestial bodies. (sky, sun, moon)
	cocos2d::Node* m_skyNode;

	float m_visibleSizeX;

	//max seconds in a day
	const int MAX_DAY_TIME = 3600; //25 min in frames = 90 000 (60 frames per sec)

	//radius for celestials
	float RADIUS = 700.0f;

	bool m_isParallaxBG;

	//Sky color gradient. Updated by time.
	cocos2d::LayerGradient* m_skyGradient;
	cocos2d::Sprite* m_sun;
	cocos2d::Sprite* m_moon;

	//Night Shade. Updates by time changing its shade.
	cocos2d::LayerColor* m_nightShade;
	cocos2d::Sprite* m_gameNodeCopy;
	cocos2d::ClippingNode* m_shadeClipping;

	//A vector that stores backgorunds.
	std::vector<cocos2d::Node*> m_backgroundVector;
	//A vector that that stores data for each background.
	std::vector<BackgroundData> m_backgroundDataVector;
	
	//Initial position value as reference for scrolling.
	cocos2d::Vec2 m_initialPosition;
};
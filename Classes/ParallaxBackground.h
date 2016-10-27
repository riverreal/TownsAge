#pragma once

#include "cocos2d.h"
#define PI 3.1415926

class ParallaxBackground
{
public:
	ParallaxBackground();
	~ParallaxBackground();

	//Initializes class getting the ground position and current game time.
	void Init(cocos2d::Vec2 initialPos, float currentTime, cocos2d::Size visibleSize, cocos2d::Node* gameNode, int maxDayTime, int cloudDensity, float maxMapWidth, int weatherType);
	//Adds a new layer of background to the background/data vector.
	void AddParallaxLayer(std::string filePath, float scrollVel, cocos2d::Vec2 initialPos, float scale, int depthLevel);
	//Updates each background with it's respective modifications.
	void UpdateBackground(cocos2d::Vec2 cameraPos, int timeOfDay);

	void SetCloudLevelRange(int min, int max);

	cocos2d::Node* GetBackgroundNode();
	cocos2d::Node* GetFrontNode();
	cocos2d::Node* GetSkyNode();
	cocos2d::Sprite* GetShadeSprite();

	float Interpolate(float x1, float x2, float x3, float y1, float y3);
	float Clamp(float toClamp, float maxValue, float minValue);

	//Depth level that controls each layer's shade throughout time.
	enum DEPTH_LEVEL
	{
		DEPTH_LEVEL_NEAR,
		DEPTH_LEVEL_MIDDLE,
		DEPTH_LEVEL_FAR,
		DEPTH_LEVEL_BACKGROUND
	};
private:
	//Initializes clouds.
	void InitClouds();

	//Reset parameters to re-generate.
	void ResetCloudParameters();

	void UpdateSky(int timeOfDay);
	void UpdateSunMoon(int timeOfDay);
	void UpdateClouds(int timeOfDay, cocos2d::Vec2 cameraPos);
	void UpdateParallaxBG(int timeOfDay, cocos2d::Vec2 cameraPos);

	cocos2d::Color4B GetSkyColorTopByTime(float timeOfDay);
	cocos2d::Color4B GetSkyColorBottomByTime(float timeOfDay);
	float GetAngleByTime(float timeOfDay, bool sun);
	float GetShadeAlphaByTime(float timeOfDay, bool stars);

	struct BackgroundData
	{
		BackgroundData(float scrolVel)
			:scrollVel(scrolVel)
		{
		};

		float scrollVel;
	};

private:
	//Node that will be added to the scene. This Node is the parent of all the backgrounds.
	cocos2d::Node* m_backgroundNode;
	//Node that will be added to the scene. This Node is the parent of all celestial bodies. (sky, sun, moon)
	cocos2d::Node* m_skyNode;
	//Noda that will be added to the scene. This Node does show on top of everything, covering thing behind it.
	cocos2d::Node* m_frontNode;

	float m_visibleSizeX;
	float m_visibleSizeY;

	int m_sfxID;

	//0 - rain, 1 - sand storm
	int m_weatherType;

	//max seconds in a day
	int m_maxDayTime; //(60 frames per sec)

	//radius for celestials
	float RADIUS = 700.0f;

	//flag whether it should update the parallax bg or not.
	bool m_isParallaxBG;

	//------------------------------------------------------------------
	// Cloud Related
	//------------------------------------------------------------------

	//max size X of the map to determine limits
	float m_maxMapWidth;

	//A path list of cloud imgs.
	const std::string m_cloudPathList[5] = {
		//"img/bg/clouds/cloud01.png",
		//"img/bg/clouds/cloud02.png",
		"img/bg/clouds/cloud03.png",
		"img/bg/clouds/cloud04.png",
		"img/bg/clouds/cloud05.png",
		"img/bg/clouds/cloud06.png",
		"img/bg/clouds/cloud07.png"
	};

	//Cloud density which desides amount of clouds in the sky
	int m_cloudDensity;

	//Number of clouds depending on the cloud density
	int m_cloudNumber;

	//cloud range for random cloud level generation
	int m_cloudLevelMin;
	int m_cloudLevelMax;

	//Cloud node (moves along with the camera)
	//Each cloud level has it's own alpha to create cloud illusion.
	//Near cloud Node
	cocos2d::Node* m_cloudNodeNear;
	//Middle Cloud Node
	cocos2d::Node* m_cloudNodeMiddle;
	//Far Cloud Node
	cocos2d::Node* m_cloudNodeFar;

	//Cloud Sprite vector
	std::vector<cocos2d::Sprite*> m_cloudSpriteVector;

	//Sprite name for cloud level. Each cloud is given its level as its name.
	const std::string CLOUD_LEVEL_NEAR = "NearCloud";
	const std::string CLOUD_LEVEL_MIDDLE = "MiddleCloud";
	const std::string CLOUD_LEVEL_FAR = "FarCloud";

	//------------------------------------------------------------------
	// Celestials
	//------------------------------------------------------------------
	//Sky color gradient. Updated by time.
	cocos2d::LayerGradient* m_skyGradient;
	cocos2d::Sprite* m_sun;
	cocos2d::Sprite* m_moon;
	cocos2d::Sprite* m_stars;
	cocos2d::ParticleSystemQuad* m_weather;

	//Night Shade. Updates by time changing its shade.
	cocos2d::LayerColor* m_nightShade;
	cocos2d::Node* m_gameNodeCopy;
	cocos2d::ClippingNode* m_shadeClipping;

	//A vector that stores backgorunds.
	std::vector<cocos2d::Node*> m_backgroundVector;
	//A vector that that stores data for each background.
	std::vector<BackgroundData> m_backgroundDataVector;
	
	//Initial position value as reference for scrolling.
	cocos2d::Vec2 m_initialPosition;
};
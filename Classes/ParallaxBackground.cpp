#include "ParallaxBackground.h"
#include <math.h>
#include "SimpleAudioEngine.h"
#include "DamageHandler.h"

using namespace cocos2d;
using namespace std;

ParallaxBackground::ParallaxBackground()
{
}

ParallaxBackground::~ParallaxBackground()
{
}

void ParallaxBackground::Init(Vec2 initialPos, float currentTime, Size visibleSize, Node* gameNode, int maxDayTime, int cloudDensity, float maxMapWidth, int weatherType)
{
	m_maxDayTime = maxDayTime;
	m_weatherType = weatherType;//Clamp(weatherType, 0, 1);
	m_cloudDensity = Clamp(cloudDensity, 5, 0);
	m_isParallaxBG = false;
	m_visibleSizeX = visibleSize.width;
	m_visibleSizeY = visibleSize.height;
	m_initialPosition = initialPos;
	m_backgroundNode = Node::create();
	m_skyNode = Node::create();
	m_frontNode = Node::create();
	m_maxMapWidth = maxMapWidth;
	m_cloudLevelMin = 0;
	m_cloudLevelMax = 0;
	m_sfxID = -1;

	//Sky init ---------------------
	m_skyGradient = LayerGradient::create(GetSkyColorTopByTime(currentTime), GetSkyColorBottomByTime(currentTime), Vec2(0, -1));
	m_skyNode->addChild(m_skyGradient);

	//Sun/Moon init
	m_sun = Sprite::create("img/bg/sun.png");
	auto x = (m_visibleSizeX * 0.5f) + cosf(GetAngleByTime(currentTime, true)) * RADIUS;
	auto y = sinf(GetAngleByTime(currentTime, true)) * RADIUS;
	m_sun->setPosition(x, y);
	m_sun->setZOrder(100);
	m_skyNode->addChild(m_sun);

	m_moon = Sprite::create("img/bg/moon.png");
	auto x2 = (m_visibleSizeX * 0.5f) + cosf(GetAngleByTime(currentTime, false)) * RADIUS;
	auto y2 = sinf(GetAngleByTime(currentTime, false)) * RADIUS;
	m_moon->setPosition(x2, y2);
	m_moon->setZOrder(100);
	m_skyNode->addChild(m_moon);

	//Stars init
	m_stars = Sprite::create("img/bg/stars.png");
	auto x3 = m_visibleSizeX * 0.5f;
	m_stars->setScale(3.2f);
	m_stars->setPositionX(x3);
	m_skyNode->addChild(m_stars);

	m_nightShade = LayerColor::create(Color4B(0, 0, 25, GetShadeAlphaByTime(currentTime, false)));

	CCLOG("weather type2: %i", m_weatherType);
	//weather init
	if (m_weatherType == 0)
	{
		m_weather = ParticleSystemQuad::create("img/effects/rainEffect/rain.plist");
		m_weather->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		m_backgroundNode->addChild(m_weather);
	}
	else if (m_weatherType == 1)
	{
		m_weather = ParticleSystemQuad::create("img/effects/sandstormEffect/sandstorm.plist");
		m_weather->setPosition(visibleSize.width / 2, visibleSize.height / 3.5f);
		m_frontNode->addChild(m_weather);
		
	}
	m_weather->setCascadeOpacityEnabled(true);
	m_weather->setOpacity(0);
	m_weather->setVisible(false);
	
	InitClouds();

	//preload sound effects
	auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	audioEngine->preloadEffect("sound/sfx/main/weather/rain.mp3");
}

void ParallaxBackground::AddParallaxLayer(string filePath, float scrollVel, Vec2 initialPos, float scale, int depthLevel)
{
	m_isParallaxBG = true;

	//Create Sprite
	auto sprite = Sprite::create(filePath);
	auto newScale = scale;
	sprite->setPosition(Vec2(initialPos.x * scale, initialPos.y * scale));
	sprite->setScale(newScale);

	auto bgNode = Node::create();
	bgNode->addChild(sprite);
	bgNode->setTag(depthLevel);
	bgNode->setZOrder(-depthLevel * 100);
	bgNode->setCascadeColorEnabled(true);
	m_backgroundNode->addChild(bgNode);

	//Store data
	auto bgData = BackgroundData(scrollVel);
	bgData.scrollVel = scrollVel;

	//Assign in respective vectors
	m_backgroundVector.push_back(bgNode);
	m_backgroundDataVector.push_back(bgData);
}

void ParallaxBackground::UpdateBackground(Vec2 cameraPos, int timeOfDay)
{
	UpdateSky(timeOfDay);
	UpdateSunMoon(timeOfDay);
	UpdateParallaxBG(timeOfDay, cameraPos);
	UpdateClouds(timeOfDay, cameraPos);
}

void ParallaxBackground::SetCloudLevelRange(int min, int max)
{
	m_cloudLevelMin = Clamp(min, 5, 0);
	m_cloudLevelMax = Clamp(max, 5, 0);
}

Node * ParallaxBackground::GetBackgroundNode()
{
	return m_backgroundNode;
}

cocos2d::Node * ParallaxBackground::GetFrontNode()
{
	return m_frontNode;
}

cocos2d::Node * ParallaxBackground::GetSkyNode()
{
	return m_skyNode;
}

cocos2d::Sprite * ParallaxBackground::GetShadeSprite()
{
	return (Sprite*)m_nightShade;
}

float ParallaxBackground::Interpolate(float x1, float x2, float x3, float y1, float y3)
{
	//linear interpolation formula: 
	//         (x2 - x1) (y3 - y1)
	//  y2 =  _____________________ + y1
	//              (x3 - x1)

	//x1, x3 are the range, x2 is the fluctuating value

	float y2 = (((x2 - x1) * (y3 - y1)) / (x3 - x1)) + y1;

	return y2;
}

float ParallaxBackground::Clamp(float toClamp, float maxValue, float minValue)
{
	auto firstResult = toClamp <= maxValue ? toClamp : maxValue;
	return firstResult >= minValue ? firstResult : minValue;
}

void ParallaxBackground::InitClouds()
{
	//Initializes cloud nodes
	m_cloudNodeNear = Node::create();
	m_cloudNodeNear->setZOrder(-210);
	m_cloudNodeNear->setCascadeColorEnabled(true);
	m_cloudNodeNear->setCascadeOpacityEnabled(true);
	m_backgroundNode->addChild(m_cloudNodeNear);

	m_cloudNodeMiddle = Node::create();
	m_cloudNodeMiddle->setZOrder(-220);
	m_cloudNodeMiddle->setCascadeColorEnabled(true);
	m_cloudNodeMiddle->setCascadeOpacityEnabled(true);
	m_backgroundNode->addChild(m_cloudNodeMiddle);

	m_cloudNodeFar = Node::create();
	m_cloudNodeFar->setZOrder(-302);
	m_cloudNodeFar->setCascadeColorEnabled(true);
	m_cloudNodeFar->setCascadeOpacityEnabled(true);
	m_backgroundNode->addChild(m_cloudNodeFar);

	//---------------------------------------------------------
	//Create cloud sprites depending on the density (m_cloudDensity) x**2
	m_cloudNumber = m_cloudDensity * m_cloudDensity;

	//Create them for the first time.
	ResetCloudParameters();

	m_cloudNodeNear->setOpacity(255);
	m_cloudNodeMiddle->setOpacity(255);
	m_cloudNodeFar->setOpacity(255);
}

void ParallaxBackground::ResetCloudParameters()
{
	m_cloudSpriteVector.clear();

	m_cloudNodeNear->removeAllChildren();
	m_cloudNodeMiddle->removeAllChildren();
	m_cloudNodeFar->removeAllChildren();

	m_cloudNodeNear->setOpacity(0);
	m_cloudNodeMiddle->setOpacity(0);
	m_cloudNodeFar->setOpacity(0);

	CCLOG("weather type3: %i", m_weatherType);

	auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

	if (m_sfxID != -1)
	{
		audioEngine->stopEffect(m_sfxID);
	}

	//Show or hide rain
	if (m_weatherType == 0)
	{
		if (m_cloudDensity == 5)
		{
			m_weather->setVisible(true);
			m_weather->runAction(FadeIn::create(3.0f));
			
			m_sfxID = audioEngine->playEffect("sound/sfx/main/weather/rain.mp3", true);
		}
		else
		{
			auto hide = Hide::create();
			m_weather->runAction(Sequence::create(FadeOut::create(1.0f), hide, NULL));
		}
	}
	else if (m_weatherType == 1)
	{
		if (m_cloudDensity == 0)
		{
			m_weather->setVisible(true);
			m_weather->runAction(FadeIn::create(3.0f));
		}
		else
		{
			auto hide = Hide::create();
			m_weather->runAction(Sequence::create(FadeOut::create(1.0f), hide, NULL));
		}
	}

	//cloud number is the power of cloud density.
	m_cloudNumber = 2 * m_cloudDensity * m_cloudDensity;

	for (int i = 0; i < m_cloudNumber; ++i)
	{
		//------------------------------------------------------------
		//    Near clouds
		//------------------------------------------------------------
		float sizeX = RandomHelper::random_int(-2, 10);
		sizeX = (sizeX / 10.0f) + 1.3f + (m_cloudDensity * 2) / 5;//set random number as the decimal of the size.

		float sizeY = RandomHelper::random_int(-2, 6);
		sizeY = (sizeY / 10.0f) + 1.0f + (m_cloudDensity * 2) / 5; //set random number as the decimal of the size.

		//place clouds in a set place with a little deviation.
		float x = (m_maxMapWidth / m_cloudNumber) * i;
		x += RandomHelper::random_int(-100, 100);
		//y for near clouds. with deviation.
		float y = (m_visibleSizeY * 0.9f) + RandomHelper::random_int(-50, 150);

		//Generate Random Cloud Type from cloud file paths
		int cloudType = RandomHelper::random_int(0, 4);
		auto sprite = Sprite::create(m_cloudPathList[cloudType]);
		sprite->setPosition(x, y);
		sprite->setScaleX(sizeX);
		sprite->setScaleY(sizeY);
		sprite->setName(CLOUD_LEVEL_NEAR);
		sprite->setZOrder(-10);
		//sprite->setOpacity(240);
		m_cloudNodeNear->addChild(sprite);

		m_cloudSpriteVector.push_back(sprite);

		//------------------------------------------------------------
		//    Middle clouds
		//------------------------------------------------------------
		sizeX = RandomHelper::random_int(-2, 3);
		sizeX = (sizeX / 10.0f) + 1.0f + (m_cloudDensity * 1.5f) / 5; //set random number as the decimal of the size.

		sizeY = RandomHelper::random_int(-2, 3);
		sizeY = (sizeY / 10.0f) + 0.8f + (m_cloudDensity * 1.5f) / 5; //set random number as the decimal of the size.

		//place clouds in a set place with a little deviation.
		x = ((m_maxMapWidth * 0.8f) / m_cloudNumber) * i;
		x += RandomHelper::random_int(-130, 130);
		//y for near clouds. with deviation.
		y = (m_visibleSizeY * 0.76f) + RandomHelper::random_int(-40, 40);

		//Generate Random Cloud Type from cloud file paths
		cloudType = RandomHelper::random_int(0, 4);
		sprite = Sprite::create(m_cloudPathList[cloudType]);
		sprite->setPosition(x, y);
		sprite->setScaleX(sizeX);
		sprite->setScaleY(sizeY);
		sprite->setZOrder(-20);
		sprite->setColor(Color3B(200, 200, 200));
		sprite->setName(CLOUD_LEVEL_MIDDLE);
		sprite->setOpacity(230);

		//25% chance to miss (to be ignored, not created)
		auto missingChance = RandomHelper::random_int(0, 4);
		if (missingChance == 0)
		{
			sprite->setVisible(false);
		}
		else
		{
			m_cloudNodeMiddle->addChild(sprite);
			m_cloudSpriteVector.push_back(sprite);
		}
		
		//------------------------------------------------------------
		//    Far clouds
		//------------------------------------------------------------
		sizeX = RandomHelper::random_int(-3, 3);
		sizeX = (sizeX / 10.0f) + 0.8f + (m_cloudDensity * 1) / 5; //set random number as the decimal of the size.

		sizeY = RandomHelper::random_int(-1, 2);
		sizeY = (sizeY / 10.0f) + 0.4f + (m_cloudDensity * 1) / 5; //set random number as the decimal of the size.

		//place clouds in a set place with a little deviation.
		x = ((m_maxMapWidth * 0.5f) / m_cloudNumber) * i;
		x += RandomHelper::random_int(-80, 80);
		//y for near clouds. with deviation.
		y = (m_visibleSizeY * 0.63f) + RandomHelper::random_int(-10, 10);

		//Generate Random Cloud Type from cloud file paths
		cloudType = RandomHelper::random_int(0, 4);
		sprite = Sprite::create(m_cloudPathList[cloudType]);
		sprite->setPosition(x, y);
		sprite->setScaleX(sizeX);
		sprite->setScaleY(sizeY);
		sprite->setZOrder(-30);
		sprite->setName(CLOUD_LEVEL_FAR);
		sprite->setColor(Color3B(150, 150, 150));
		sprite->setOpacity(200);

		//50% chance to miss (to be ignored; not created)
		missingChance = RandomHelper::random_int(0, 1);

		if (missingChance == 0)
		{
			sprite->setVisible(false);
		}
		else
		{
			m_cloudNodeFar->addChild(sprite);
			m_cloudSpriteVector.push_back(sprite);
		}
	}
}

void ParallaxBackground::UpdateSky(int timeOfDay)
{
	//skycolor update
	auto startColor = GetSkyColorTopByTime(timeOfDay);
	auto endColor = GetSkyColorBottomByTime(timeOfDay);
	m_skyGradient->setStartColor(Color3B(startColor.r, startColor.g, startColor.b));
	m_skyGradient->setEndColor(Color3B(endColor.r, endColor.g, endColor.b));

	//stars update
	m_stars->setOpacity(GetShadeAlphaByTime(timeOfDay, true));
	static float s_angle = 0;
	s_angle+= 0.005f;
	m_stars->setRotation(s_angle);

	//shade update
	m_nightShade->setOpacity(GetShadeAlphaByTime(timeOfDay, false));
}

void ParallaxBackground::UpdateSunMoon(int timeOfDay)
{
	auto x = (m_visibleSizeX * 0.5f) + cosf(GetAngleByTime(timeOfDay, true)) * RADIUS;
	auto y = sinf(GetAngleByTime(timeOfDay, true)) * RADIUS;
	m_sun->setPosition(x, y);

	auto x2 = (m_visibleSizeX * 0.5f) + cosf(GetAngleByTime(timeOfDay, false)) * RADIUS;
	auto y2 = sinf(GetAngleByTime(timeOfDay, false)) * RADIUS;
	m_moon->setPosition(x2, y2);
}

void ParallaxBackground::UpdateClouds(int timeOfDay, Vec2 cameraPos)
{
	auto cloudShade = GetShadeAlphaByTime(timeOfDay, true);
	cloudShade = Clamp(cloudShade, 255, 0);

	static float nearCloudMovingOffset = 0;
	nearCloudMovingOffset += 0.215f;
	
	m_cloudNodeNear->setPosition(Vec2((cameraPos.x * 0.3f) - nearCloudMovingOffset, cameraPos.y * 0.1f));
	auto nearShade = Clamp((255 - cloudShade) + 120.0f, 255, 0);
	m_cloudNodeNear->setColor(Color3B(nearShade, nearShade, nearShade));

	static float middleCloudMovingOffset = 0;
	middleCloudMovingOffset += 0.1f;

	auto middleShade = Clamp((255 - cloudShade) + 80.0f, 255, 0);
	m_cloudNodeMiddle->setPosition(Vec2((cameraPos.x * 0.2f) - middleCloudMovingOffset, cameraPos.y * 0.06f));
	m_cloudNodeMiddle->setColor(Color3B(middleShade, middleShade, middleShade));

	static float farCloudMovingOffset = 0;
	farCloudMovingOffset += 0.05f;

	auto farShade = Clamp((255 - cloudShade) + 40.0f, 255, 0);
	m_cloudNodeFar->setPosition(Vec2((cameraPos.x * 0.1f) - farCloudMovingOffset, cameraPos.y * 0.005));
	m_cloudNodeFar->setColor(Color3B(farShade, farShade, farShade));

	if (nearCloudMovingOffset >= 1000)
	{
		static bool fadedOnce = false;

		if (!fadedOnce)
		{
			auto fadeOutNear = FadeOut::create(1.0f);
			auto fadeOutMid = FadeOut::create(2.0f);
			auto fadeOutFar = FadeOut::create(3.0f);

			m_cloudNodeNear->runAction(fadeOutNear);
			m_cloudNodeMiddle->runAction(fadeOutMid);
			m_cloudNodeFar->runAction(fadeOutFar);

			fadedOnce = true;
		}

		if (nearCloudMovingOffset >= 1040)
		{
			m_cloudDensity = RandomHelper::random_int(m_cloudLevelMin, m_cloudLevelMax);

			ResetCloudParameters();

			auto fadeInNear = FadeIn::create(1.0f);
			auto fadeInMid = FadeIn::create(2.0f);
			auto fadeInFar = FadeIn::create(3.0f);

			m_cloudNodeNear->runAction(fadeInNear);
			m_cloudNodeMiddle->runAction(fadeInMid);
			m_cloudNodeFar->runAction(fadeInFar);

			fadedOnce = false;

			nearCloudMovingOffset = 0;
			middleCloudMovingOffset = 0;
			farCloudMovingOffset = 0;
		}
		
	}
}

void ParallaxBackground::UpdateParallaxBG(int timeOfDay, cocos2d::Vec2 cameraPos)
{
	if (m_isParallaxBG)
	{
		for (int i = 0; i < m_backgroundDataVector.size(); ++i)
		{
			m_backgroundVector[i]->setPosition(Vec2(cameraPos.x * m_backgroundDataVector[i].scrollVel, cameraPos.y * m_backgroundDataVector[i].scrollVel));

			if (m_backgroundVector[i]->getTag() >= DEPTH_LEVEL_BACKGROUND)
			{
				m_backgroundVector[i]->setPosition(Vec2(cameraPos.x * m_backgroundDataVector[i].scrollVel, 0));
			}

			float blacknessLevel = GetShadeAlphaByTime(timeOfDay, true);
			switch (m_backgroundVector[i]->getTag())
			{
			case DEPTH_LEVEL_NEAR:
				blacknessLevel -= 120.0f;
				break;
			case DEPTH_LEVEL_MIDDLE:
				blacknessLevel -= 80.0f;
				break;
			case DEPTH_LEVEL_FAR:
				blacknessLevel -= 40.0f;
				break;
			default:
				break;
			}
			blacknessLevel = Clamp(blacknessLevel, 255, 0);
			m_backgroundVector[i]->setColor(Color3B(255 - blacknessLevel, 255 - blacknessLevel, 255 - blacknessLevel));
		}

		if (m_weatherType == 1)
		{
			if (m_cloudDensity == 0)
			{
				static int dmgCounter = 0;
				dmgCounter++;
				CCLOG("dmgCounter: %i", dmgCounter);
				if (dmgCounter >= 60)
				{
					dmgCounter = 0;
					auto dmg = DamageHandler::GetInstance();
					dmg->DealDamageToPlayer(5);
				}
			}
		}
	}
}

cocos2d::Color4B ParallaxBackground::GetSkyColorTopByTime(float timeOfDay)
{
	Color4B topColorList[24] =
	{
		Color4B(0, 0, 12, 255), Color4B(2, 1, 17, 255), Color4B(2, 1, 17, 255),
		Color4B(2, 1, 17, 255), Color4B(32, 32, 44, 255), Color4B(64, 64, 92, 255),
		Color4B(74, 73, 105, 255), Color4B(117, 122, 191, 255), Color4B(130, 173, 219, 255),
		Color4B(148, 197, 248, 255), Color4B(183, 234, 255, 255), Color4B(155, 226, 254, 255),
		Color4B(144, 223, 254, 255), Color4B(87, 193, 235, 255), Color4B(15, 45, 145, 255),
		Color4B(36, 115, 171, 255), Color4B(30, 82, 142, 255), Color4B(30, 82, 142, 255),
		Color4B(21, 66, 119, 255), Color4B(22, 60, 82, 255), Color4B(7, 17, 38, 255),
		Color4B(1, 10, 16, 255), Color4B(9, 4, 1, 255), Color4B(0, 0, 12, 255)
	};

	auto step = m_maxDayTime / 24;
	
	int index = (timeOfDay / step);
	Color4B previousColor = topColorList[index];
	int nextIndex = index+1;
	if (nextIndex > 23)
	{
		nextIndex = 0;
	}
	Color4B nextColor = topColorList[nextIndex];
	//get value from 0 to step
	float delta = abs(timeOfDay - (index * step));

	int red = floor(Interpolate(0, delta, step, previousColor.r, nextColor.r));
	int blue = floor(Interpolate(0, delta, step, previousColor.b, nextColor.b));
	int green = floor(Interpolate(0, delta, step, previousColor.g, nextColor.g));

	return cocos2d::Color4B(red, green, blue, 255);
}

cocos2d::Color4B ParallaxBackground::GetSkyColorBottomByTime(float timeOfDay)
{
	Color4B bottomColorList[24] =
	{
		Color4B(0, 0, 12, 255), Color4B(25, 22, 33, 255), Color4B(32, 32, 44, 255),
		Color4B(58, 58, 82, 255), Color4B(81, 81, 117, 255), Color4B(138, 118, 171, 255),
		Color4B(205, 130, 160, 255), Color4B(234, 176, 209, 255), Color4B(235, 178, 177, 255),
		Color4B(177, 181, 234, 255), Color4B(148, 223, 255, 255), Color4B(103, 209, 251, 255),
		Color4B(56, 163, 209, 255), Color4B(36, 111, 168, 255), Color4B(30, 82, 142, 255),
		Color4B(91, 121, 131, 255), Color4B(157, 166, 113, 255), Color4B(233, 206, 93, 255),
		Color4B(178, 99, 57, 255), Color4B(183, 73, 15, 255), Color4B(138, 59, 18, 255), 
		Color4B(89, 35, 11, 255), Color4B(75, 29, 6, 255), Color4B(21, 8, 0, 255)
	};

	auto step = m_maxDayTime / 24;

	int index = (timeOfDay / step);
	Color4B previousColor = bottomColorList[index];
	int nextIndex = index+1;
	if (nextIndex > 23)
	{
		nextIndex = 0;
	}
	Color4B nextColor = bottomColorList[nextIndex];

	//get value from 0 to 62.5
	float delta = abs(timeOfDay - (index * step));

	//interpolate colors to get sky color in between and gradually changing by time.
	int red = floor(Interpolate(0, delta, step, previousColor.r, nextColor.r));
	int blue = floor(Interpolate(0, delta, step, previousColor.b, nextColor.b));
	int green = floor(Interpolate(0, delta, step, previousColor.g, nextColor.g));

	return cocos2d::Color4B(red, green, blue, 255);
}

float ParallaxBackground::GetAngleByTime(float timeOfDay, bool sun)
{
	//Angle values
	// 0  ------ 360 (max angle)
	//Time values
	// 0  ------ MAX_DAY_TIME

	//Formula:
	//angle = (360 * timeOfDay) / MAX_DAY_TIME

	//degrees to radian conversion and angle correction included
	// the - (minus) represents the direction for the rotation.
	float angle;

	if (sun)
	{
		angle = -((((360 * timeOfDay) / m_maxDayTime) * (PI / 180)) + (PI / 2));
	}
	else
	{
		angle = -((((360 * timeOfDay) / m_maxDayTime) * (PI / 180) - (PI / 2)));
	}

	return angle;
}

float ParallaxBackground::GetShadeAlphaByTime(float timeOfDay, bool stars)
{	
	//from 0 to 255 (255 being black)
	float shadeLevel = 0;
	//255 for the stars, 100 for the shading
	float darkestValue = stars ? 255 : 120;

	auto realShadeTime = timeOfDay;
	auto step = m_maxDayTime / 24;
	//determine hour (24 hour) for convinience
	float hour = (realShadeTime/ step);
	
	if (stars)
	{
		if (hour >= 18)
		{
			shadeLevel = 255 - Interpolate(18, hour, 24, darkestValue, 0);
		}
		else if (hour <= 6)
		{
			shadeLevel = 255 - Interpolate(6, hour, 0, darkestValue, 0);
		}
		else
		{
			shadeLevel = 0;
		}
	}
	else
	{
		auto cloudContribution = m_cloudDensity * 10.0f;

		//not dark hours
		if (hour >= 3 && hour <= 21)
		{
			float interpolatingValue = 12 / hour;
			if (hour > 12)
			{
				interpolatingValue = 12 / (24 - hour);
			}

			shadeLevel = 255 - Interpolate(1, interpolatingValue, 4, darkestValue, 0);
			shadeLevel -= 135.0f;
			shadeLevel += cloudContribution;
		}
		//dark hours
		else
		{
			shadeLevel = darkestValue + cloudContribution;
		}
	}

	//return the clamped value (clamp to color values: 255 - 0)
	return Clamp(shadeLevel, 255, 0);
}

#include "ParallaxBackground.h"
#include <math.h>

using namespace cocos2d;
using namespace std;

ParallaxBackground::ParallaxBackground()
{
}

ParallaxBackground::~ParallaxBackground()
{
}

void ParallaxBackground::Init(Vec2 initialPos, float currentTime, float visibleSizeX, Node* gameNode)
{
	m_isParallaxBG = false;
	m_visibleSizeX = visibleSizeX;
	m_initialPosition = initialPos;
	m_backgroundNode = Node::create();
	m_skyNode = Node::create();

	//Sky init ---------------------
	m_skyGradient = LayerGradient::create(GetSkyColorTopByTime(currentTime), GetSkyColorBottomByTime(currentTime), Vec2(0, -1));
	m_skyNode->addChild(m_skyGradient);

	//Sun/Moon init
	m_sun = Sprite::create("img/bg/sun.png");

	auto x = (visibleSizeX * 0.5f) + cosf(GetAngleByTime(currentTime, true)) * RADIUS;
	auto y = sinf(GetAngleByTime(currentTime, true)) * RADIUS;
	m_sun->setPosition(x, y);
	m_skyNode->addChild(m_sun);

	m_moon = Sprite::create("img/bg/moon.png");

	auto x2 = (visibleSizeX * 0.5f) + cosf(GetAngleByTime(currentTime, false)) * RADIUS;
	auto y2 = sinf(GetAngleByTime(currentTime, false)) * RADIUS;
	m_moon->setPosition(x2, y2);
	m_skyNode->addChild(m_moon);

	//Shading ----------------------
	//gameNodeCopy as the stencil for the clipping mask
	//m_gameNodeCopy = (Sprite*)gameNode;
	
	//create clipper with gameNode as stencil
	//m_shadeClipping = ClippingNode::create(m_gameNodeCopy);

	m_nightShade = LayerColor::create(Color4B(0, 0, 0, GetShadeAlphaByTime(currentTime)));
	//m_shadeClipping->addChild(m_nightShade);
}

void ParallaxBackground::AddParallaxLayer(string filePath, float scrollVel, Vec2 initialPos, float scale)
{
	m_isParallaxBG = true;

	//Create Sprite
	auto sprite = Sprite::create(filePath);
	auto newScale = scale * 0.7;
	sprite->setPosition(Vec2(initialPos.x * scale, initialPos.y * scale));
	sprite->setScale(newScale);
	m_backgroundNode->addChild(sprite);

	//Store data
	auto bgData = BackgroundData();
	bgData.scrollVel = scrollVel;

	//Assign in respective vectors
	m_backgroundVector.push_back(sprite);
	m_backgroundDataVector.push_back(bgData);
}

void ParallaxBackground::UpdateBackground(Vec2 cameraPos, int timeOfDay)
{
	UpdateSky(timeOfDay);
	UpdateSunMoon(timeOfDay);

	if (m_isParallaxBG)
	{
		for (int i = 0; i < m_backgroundDataVector.size(); ++i)
		{
			if (m_backgroundNode != NULL)
			{
				m_backgroundNode->setPosition(Vec2(cameraPos.x * m_backgroundDataVector[0].scrollVel, cameraPos.y * m_backgroundDataVector[0].scrollVel));	
			}
		}
	}
}

Node * ParallaxBackground::GetBackgroundNode()
{
	return m_backgroundNode;
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

void ParallaxBackground::UpdateSky(int timeOfDay)
{
	//skycolor update
	auto startColor = GetSkyColorTopByTime(timeOfDay);
	auto endColor = GetSkyColorBottomByTime(timeOfDay);
	m_skyGradient->setStartColor(Color3B(startColor.r, startColor.g, startColor.b));
	m_skyGradient->setEndColor(Color3B(endColor.r, endColor.g, endColor.b));

	//shade update
	m_nightShade->setOpacity(GetShadeAlphaByTime(timeOfDay));
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

	auto step = MAX_DAY_TIME / 24;
	
	int index = (timeOfDay / step);
	Color4B previousColor = topColorList[index];
	int nextIndex = index+1;
	if (nextIndex > 23)
	{
		nextIndex = 0;
	}
	Color4B nextColor = topColorList[nextIndex];
	//CCLOG("previous Color Red: %i , next color red: %i", previousColor.r, nextColor.r);
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

	auto step = MAX_DAY_TIME / 24;

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
		angle = -((((360 * timeOfDay) / MAX_DAY_TIME) * (PI / 180)) + (PI / 2));
	}
	else
	{
		angle = -((((360 * timeOfDay) / MAX_DAY_TIME) * (PI / 180) - (PI / 2)));
	}

	return angle;
}

float ParallaxBackground::GetShadeAlphaByTime(float timeOfDay)
{	
	//from 0 to 255 (255 being black)
	float shadeLevel = 0;
	float darkestValue = 140;

	auto realShadeTime = timeOfDay;
	auto step = MAX_DAY_TIME / 24;
	//determine hour (24 hour) for convinience
	float hour = (realShadeTime/ step);
	
	//not dark hours
	if (hour >= 3 && hour <= 21)
	{
		float interpolatingValue = 12 / hour;
		if (hour > 12)
		{
			interpolatingValue = 12 / (24 - hour);
		}

		shadeLevel = 255 - Interpolate(1, interpolatingValue, 4, darkestValue, 0);
	}
	//dark hours
	else
	{
		shadeLevel = darkestValue;
	}

	return Clamp(shadeLevel, darkestValue, 0);
}

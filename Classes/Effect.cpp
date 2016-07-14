#include "Effect.h"

using namespace cocos2d;

Effect::Effect()
{
	m_onScreenEffect = LayerColor::create(Color4B(255, 0, 0, 255));
	m_onScreenEffect->setOpacity(20);
	m_onScreenEffect->setVisible(false);
}

Effect::~Effect()
{
}

void Effect::ShakeNode(cocos2d::Node * node)
{
	node->stopActionByTag(10);

	float x, y;
	//x = node->getPositionX();
	//y = node->getPositionY();
	auto pos1 = MoveBy::create(0.05f, Vec2(5, -5));
	auto pos2 = MoveBy::create(0.05f, Vec2(-5, -7));
	auto pos3 = MoveBy::create(0.05f, Vec2(7, 4));
	auto pos4 = MoveBy::create(0.05f, Vec2(-5, 5));
	auto pos5 = MoveTo::create(0.05f, Vec2( - 8,  - 5));
	auto pos6 = MoveTo::create(0.05f, Vec2( - 15,  + 12));
	auto pos7 = MoveTo::create(0.05f, Vec2( + 3,  - 8));
	auto pos8 = MoveTo::create(0.05f, Vec2( - 4,  + 10));
	auto pos9 = MoveBy::create(0.05f, Vec2(-2, 3));

	//node->runAction(Sequence::create(pos1, pos2, pos3, pos4, pos5, pos6, pos7, pos8, pos9, NULL));
	auto action = Sequence::create(pos1, pos2, pos3, pos4, pos9, NULL);
	action->setTag(10);
	node->runAction(action);
}

void Effect::TickScreenWithColor(unsigned int tickCount, cocos2d::Color3B tickColor)
{
	m_onScreenEffect->setColor(tickColor);
	auto delayPerTick = DelayTime::create(0.4f);
	auto tickDuration = DelayTime::create(0.1f);
	//auto tickEffectFunc = CallFunc::create([this]() {this->m_onScreenEffect->});
	auto show = Show::create();
	auto hide = Hide::create();
	auto sequence = Sequence::create(show, tickDuration, hide, delayPerTick, NULL);
	auto repeat = Repeat::create(sequence, tickCount);
	m_onScreenEffect->runAction(repeat);
}

cocos2d::LayerColor * Effect::GetColorScreen()
{
	return m_onScreenEffect;
}
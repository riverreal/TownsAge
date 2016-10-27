#pragma once

#include "cocos2d.h"
#pragma execution_character_set("utf-8");

class MangaScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float dt);

	//run manga cut scene
	void runCutScene();
	//run time travel scene
	void runTimeTravel();

	void startTutorial();

	// implement the "static create()" method manually
	CREATE_FUNC(MangaScene);

private:
	cocos2d::Node* m_mangaNode;
	cocos2d::Node* m_timeMachineNode;
};

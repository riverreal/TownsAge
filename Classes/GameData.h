#pragma once
#include<string>

enum NPC_STATE_ID
{
	NPC_STATE_IDLE,
	NPC_STATE_WALKING,
	NPC_STATE_MAX
};

enum NPC_TYPE
{
	NPC_TYPE_BEARD,
	NPC_TYPE_BALD,
	NPC_TYPE_PREHISTORIC_MAX
};

const std::string npcTypePath[] =
{
	"img/sprites/caveman01.png",
	"img/sprites/caveman02.png"
};
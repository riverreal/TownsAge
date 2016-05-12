#pragma once
#include<string>
#include<vector>

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

namespace mapData
{
	enum MAP_ID
	{
		HOME_MAP,
		FOREST01
	};

	const std::string mapPath[] = 
	{
		"img/tilemap/test.tmx",
		"img/tilemap/forest01.tmx"
	};

	const std::string leftMap[] =
	{
		"LeftMap01",
		"LeftMap02",
		"LeftMap03",
		"LeftMap04",
		"LeftMap05",
		"LeftMap06",
		"LeftMap07",
		"LeftMap08",
		"LeftMap09",
		"LeftMap10",
		"LeftMap11",
		"LeftMap12",
		"LeftMap13",
		"LeftMap14"
	};

	const std::string rightMap[] = 
	{
		"RightMap01",
		"RightMap02",
		"RightMap03",
		"RightMap04",
		"RightMap05",
		"RightMap06",
		"RightMap07",
		"RightMap08",
		"RightMap09",
		"RightMap10",
		"RightMap11",
		"RightMap12",
		"RightMap13",
		"RightMap14"
	};
}

namespace buildingData
{
	enum BUILDING_ID
	{
		BUILDING_NONE,
		HOUSE,
		TOWER,
		MAX_PREHISTORIC
	};

	const std::string buildingPath[] = 
	{
		"",
		"img/ui/houseIcon.png",
		"img/ui/towerIcon.png"
	};

	

	struct building
	{
		building(std::string name, int rec1, int count1, int rec2, int count2, int rec3, int count3)
			:resource1(rec1),
			res1Count(count1),
			resource2(rec2),
			res2Count(count2),
			resource3(rec3),
			res3Count(count3)
		{}

		std::string name;
		int resource1;
		int res1Count;
		int resource2;
		int res2Count;
		int resource3;
		int res3Count;
	};

	enum RESOURCE_ID
	{
		RESOURCE_NONE,
		WOOD,
		STONE
	};

	const std::string resourcePath[] =
	{
		"",
		"img/ui/woodIcon.png",
		"img/ui/stoneIcon.png"
	};

	const building buildingRecepe[] =
	{
		building("", BUILDING_NONE, 0, BUILDING_NONE, 0, BUILDING_NONE, 0),
		building("house", WOOD, 5, STONE, 3, WOOD, 30),
		building("tower", WOOD, 10, STONE, 5, BUILDING_NONE, 0)
	};
	
	
}



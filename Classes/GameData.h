#pragma once
#include<string>
#include<vector>

#pragma execution_character_set("utf-8");

enum NPC_STATE_ID
{
	NPC_STATE_IDLE,
	NPC_STATE_WALKING,
	NPC_STATE_BUILDING,
	NPC_STATE_MAX
};

enum NPC_TYPE
{
	NPC_TYPE_CAVE_GIRL1,
	NPC_TYPE_CAVE_GIRL2,
	NPC_TYPE_CAVE_GIRL3,
	NPC_TYPE_CAVE_GIRL4,
	NPC_TYPE_CAVE_BOY1,
	NPC_TYPE_CAVE_BOY2,
	NPC_TYPE_CAVE_BOY3,
	NPC_TYPE_CAVE_BOY4,
	NPC_TYPE_PREHISTORIC_MAX,
	NPC_TYPE_MID_GIRL1,
	NPC_TYPE_MID_GIRL2,
	NPC_TYPE_MID_GIRL3,
	NPC_TYPE_MID_GIRL4,
	NPC_TYPE_MID_BOY1,
	NPC_TYPE_MID_BOY2,
	NPC_TYPE_MID_BOY3,
	NPC_TYPE_MID_BOY4,
	NPC_TYPE_MEDIEVAL_MAX
};

const std::string npcTypePath[] =
{
	"img/sprites/new/caveman/caveman1.png",
	"img/sprites/new/caveman/caveman2.png",
	"img/sprites/new/caveman/caveman3.png",
	"img/sprites/new/caveman/caveman4.png",
	"img/sprites/new/caveman/caveman5.png",
	"img/sprites/new/caveman/caveman6.png",
	"img/sprites/new/caveman/caveman7.png",
	"img/sprites/new/caveman/caveman8.png",
	"",
	"img/sprites/new/medieval/medieval1.png",
	"img/sprites/new/medieval/medieval2.png",
	"img/sprites/new/medieval/medieval3.png",
	"img/sprites/new/medieval/medieval4.png",
	"img/sprites/new/medieval/medieval5.png",
	"img/sprites/new/medieval/medieval6.png",
	"img/sprites/new/medieval/medieval7.png",
	"img/sprites/new/medieval/medieval8.png",
	""
};

enum RESOURCE_ID
{
	RESOURCE_NONE,
	WOOD,
	STONE,
	APPLE,
	CACTUS,
	CARBON,
	EMERALD,
	MUSHROOM,
	MUSHROOM2,
	STEEL,
	STRAW,
	NENDO,
	SAND,
	STICK,
	BRICK,
	CLAW,
	CLOTH,
	GLASS,
	SHARP,
	SHIELD,
	STEEL2,
	STRING,
	WOOD2,
	STONE2,
	STICK2,
	CLOTH2,
	STRAW2
};

namespace mapData
{
	enum MAP_ID
	{
		HOME_MAP,
		FOREST01,
		FOREST02,
		SWAMP01,
		CAVE01,
		DESERT01,
		OASIS01,
		SWAMP02,
		CAVE02,
		MONTAIN01,
		MONTAIN02,
		MONTAIN03,
		DESERT02
	};

	struct collect
	{
		collect(std::string name, int rec1, int count1, int rec2, int count2, int rec3, int count3, int rec4, int count4, int rec5, int count5)
			:resource1(rec1),
			res1Count(count1),
			resource2(rec2),
			res2Count(count2),
			resource3(rec3),
			res3Count(count3),
			resource4(rec4),
			res4Count(count4),
			resource5(rec5),
			res5Count(count5)
		{}

		std::string name;
		int resource1;
		int res1Count;
		int resource2;
		int res2Count;
		int resource3;
		int res3Count;
		int resource4;
		int res4Count;
		int resource5;
		int res5Count;
	};

	const std::string mapName[] =
	{
		"街",
		"始まりの森",
		"旅立ちの森",
		"偽りの沼地",
		"迷いの洞窟",
		"地獄の砂漠",
		"オアシス",
		"ドロドロの沼地",
		"迷路の洞窟",
		"蒼天山（登り）",
		"蒼天山（降り）",
		"蒼天山（頂上）",
		"マース砂丘"
	};

	const collect mapCollect[] =
	{
		collect("街", RESOURCE_NONE, 0, RESOURCE_NONE, 0, RESOURCE_NONE, 0, RESOURCE_NONE, 0, RESOURCE_NONE, 0),
		collect("始まりの森", WOOD, 2, STONE, 2, MUSHROOM, 1, APPLE, 1, WOOD, 2),
		collect("旅立ちの森", WOOD, 2, STONE, 2, MUSHROOM, 1, APPLE, 1, STRAW, 2),
		collect("偽りの沼地", WOOD, 1, MUSHROOM, 1, MUSHROOM2, 1, WOOD, 1, MUSHROOM, 1),
		collect("迷いの洞窟", STONE, 1, CARBON, 2, EMERALD, 1, STEEL, 2, CARBON, 1),
		collect("地獄の砂漠", CACTUS, 1, WOOD, 1, CACTUS, 1, CACTUS, 1, CACTUS, 1),
		collect("オアシス", CACTUS, 1, WOOD, 1, CACTUS, 1, CACTUS, 1, CACTUS, 1),
		collect("ドロドロの沼地", WOOD, 1, NENDO, 1, MUSHROOM2, 1, WOOD, 1, MUSHROOM, 1),
		collect("迷路の洞窟", STONE, 1, CARBON, 2, EMERALD, 1, STEEL, 2, CARBON, 1),
		collect("蒼天山（登り）", WOOD, 2, STONE, 2, MUSHROOM, 1, APPLE, 1, STRAW, 2),
		collect("蒼天山（降り）", WOOD, 2, STONE, 2, MUSHROOM, 1, APPLE, 1, STRAW, 2),
		collect("蒼天山（頂上）", WOOD, 2, STONE, 2, MUSHROOM, 1, APPLE, 1, STRAW, 2),
		collect("マース砂丘", CACTUS, 1, WOOD, 1, CACTUS, 1, CACTUS, 1, CACTUS, 1)
	};

	const std::string mapPath[] =
	{
		"img/tilemap/home.tmx",
		"img/tilemap/maps/forest01.tmx",
		"img/tilemap/maps/newMap/forest02.tmx",
		"img/tilemap/maps/newMap/swamp01.tmx",
		"img/tilemap/maps/newMap/cave01.tmx",
		"img/tilemap/maps/newMap/desert01.tmx",
		"img/tilemap/maps/newMap/oasis.tmx",
		"img/tilemap/maps/newMap/swamp02.tmx",
		"img/tilemap/maps/newMap/cave02.tmx",
		"img/tilemap/maps/newMap/montain01.tmx",
		"img/tilemap/maps/newMap/montain02.tmx",
		"img/tilemap/maps/newMap/montain03.tmx",
		"img/tilemap/maps/newMap/desert02.tmx"
	};

	const int minLevel[] =
	{
		1,
		0,
		0,
		4,
		0,
		0,
		0,
		3,
		0,
		0,
		0,
		0,
		0
	};

	const int maxLevel[] =
	{
		5,
		2,
		1,
		5,
		0,
		2,
		1,
		5,
		0,
		5,
		5,
		0,
		0
	};

	const int weatherType[] =
	{
		0,
		0,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		1
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

	const std::string cicleMap[] =
	{
		"CycleMap00",
		"CycleMap01",
		"CycleMap02",
		"CycleMap03",
		"CycleMap04",
		"CycleMap05",
		"CycleMap06",
		"CycleMap07",
		"CycleMap08",
		"CycleMap09"
	};


	const std::string buildingMap[] =
	{
		"BuildingMap01",
		"BuildingMap02",
		"BuildingMap03",
		"BuildingMap04",
		"BuildingMap05",
		"BuildingMap06",
		"BuildingMap07",
		"BuildingMap08",
		"BuildingMap09",
		"BuildingMap10",
		"BuildingMap11",
		"BuildingMap12",
		"BuildingMap13",
		"BuildingMap14",
		"BuildingMap15",
		"BuildingMap16",
		"BuildingMap17",
		"BuildingMap18",
		"BuildingMap19",
		"BuildingMap20",
		"BuildingMap21",
		"BuildingMap22",
		"BuildingMap23",
		"BuildingMap24",
		"BuildingMap25",
		"BuildingMap26",
		"BuildingMap27",
		"BuildingMap28",
		"BuildingMap29",
		"BuildingMap30",
		"BuildingMap31",
		"BuildingMap32",
		"BuildingMap33",
		"BuildingMap34",
		"BuildingMap35",
		"BuildingMap36",
		"BuildingMap37",
		"BuildingMap38",
		"BuildingMap39",
		"BuildingMap40"
	};
}

namespace buildingData
{
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

	enum BUILDING_ID
	{
		BUILDING_NONE,
		HOUSE,
		TOWER,
		CULT,
		BASE,
		WAREHOUSE,
		REUNION,
		TIME_MACHINE01,
		TIME_MACHINE02,
		TIME_MACHINE03,
		MAX_PREHISTORIC,

		HOUSE2,
		TOWER2,
		CHURCH,
		BASE2,
		WAREHOUSE2,
		TAVERN,
		WEAPON_SHOP,
		CLOTH_SHOP,
		CASTLE,
		MARKET,
		FOOD,
		ARMOR,
		MAX_MEDIEVAL
	};

	const std::string buildingName[] =
	{
		"",
		"家",
		"見晴台",
		"祭壇",
		"基地",
		"倉庫",
		"集合所",
		"タイムマシン",
		"タイムマシン",
		"タイムマシン",
		"",

		"家",
		"時計塔",
		"教会",
		"基地",
		"倉庫",
		"酒場",
		"武器屋",
		"服屋",
		"城",
		"交換所",
		"食べ物屋",
		"防具屋",
		""
	};

	const int slotPerBuilding[] = 
	{
		0, //BUILDING_NONE
		3, //HOUSE
		2, //TOWER
		3, //CULT
		4, //BASE
		3, //WAREHOUSE
		3, //REUNION
		2, //TIME_MACHINE01
		2,
		2,
		0,

		3, //HOUSE2
		2, //TOWER2
		3, //CHURCH
		4, //BASE2
		3, //WAREHOUSE
		3, //TAVRN
		2, //WEAPON_SHOP
		2, //CLOTH_SHOP
		3, //CASTLE
		3, //MARKET
		4, //FOOD
		3, //ARMOR
		0
	};

	const std::string buildingPath[] = 
	{
		"",
		"img/buildings/stone/house.png",
		"img/buildings/stone/tower.png",
		"img/buildings/stone/cult.png",
		"img/buildings/stone/base.png",
		"img/buildings/stone/warehouse.png",
		"img/buildings/stone/reunion.png",
		"img/timeMachine/TimeMachine2.png",
		"img/timeMachine/TimeMachine3.png",
		"img/timeMachine/TimeMachine4.png",
		"",

		"img/buildings/medieval/house.png",
		"img/buildings/medieval/tower.png",
		"img/buildings/medieval/church.png",
		"img/buildings/medieval/base.png",
		"img/buildings/medieval/warehouse.png",
		"img/buildings/medieval/tavern.png",
		"img/buildings/medieval/weapon.png",
		"img/buildings/medieval/cloth.png",
		"img/buildings/medieval/castle.png",
		"img/buildings/medieval/market.png",
		"img/buildings/medieval/food.png",
		"img/buildings/medieval/armor.png",
		""
	};

	const building buildingRecepe[] =
	{
		building("", BUILDING_NONE, 0, BUILDING_NONE, 0, BUILDING_NONE, 0),
		building("house", WOOD, 5, STRAW2, 3, BUILDING_NONE, 0),
		building("tower", STICK2, 5, WOOD2, 5, BUILDING_NONE, 0),
		building("cult", STONE2, 3, CLOTH, 1, BUILDING_NONE, 0),
		building("base", STONE, 5, STICK, 3, WOOD, 2),
		building("warehouse", CLOTH, 5, STICK, 7, BUILDING_NONE, 0),
		building("reunion", STONE2, 3, CARBON, 1, WOOD2, 3),
		building("", BUILDING_NONE, 0, BUILDING_NONE, 0, BUILDING_NONE, 0),
		building("", BUILDING_NONE, 0, BUILDING_NONE, 0, BUILDING_NONE, 0),
		building("", BUILDING_NONE, 0, BUILDING_NONE, 0, BUILDING_NONE, 0),
		building("", BUILDING_NONE, 0, BUILDING_NONE, 0, BUILDING_NONE, 0),

		building("house", BRICK, 5, WOOD2, 3, GLASS, 4),
		building("tower", BRICK, 8, STEEL2, 5, BUILDING_NONE, 0),
		building("church", WOOD2, 2, BRICK, 5, GLASS, 5),
		building("base", BRICK, 5, WOOD2, 3, GLASS, 4),
		building("warehouse", BRICK, 5, WOOD2, 3, BUILDING_NONE, 0),
		building("tavern", WOOD2, 3, BRICK, 6, STRAW2, 5),
		building("weapon", WOOD2, 3, BRICK, 6, CLAW, 5),
		building("cloth", WOOD2, 3, BRICK, 6, CLOTH, 5),
		building("castle", BRICK, 10, GLASS, 5, BUILDING_NONE, 0),
		building("market", WOOD2, 3, BRICK, 6, EMERALD, 3),
		building("food", WOOD2, 5, APPLE, 3, CACTUS, 10),
		building("armor", WOOD2, 3, BRICK, 6, STEEL2, 5),
		building("", BUILDING_NONE, 0, BUILDING_NONE, 0, BUILDING_NONE, 0)
	};

	const std::string resourcePath[] =
	{
		"",
		"img/ui/woodIcon.png",
		"img/ui/stoneIcon.png",
		"img/ui/apple.png"
	};

	
}

namespace resourceData
{


	const std::string resourceIconPath[] =
	{
		"",
		"img/ui/resourceIcon/wood01.png",
		"img/ui/resourceIcon/stone03.png",
		"img/ui/resourceIcon/apple02.png",
		"img/ui/resourceIcon/cactus.png",
		"img/ui/resourceIcon/carbon01.png",
		"img/ui/resourceIcon/emerald.png",
		"img/ui/resourceIcon/mushroom01.png",
		"img/ui/resourceIcon/mushroom02.png",
		"img/ui/resourceIcon/steel01.png",
		"img/ui/resourceIcon/straw.png",
		"img/ui/resourceIcon/nendo.png",
		"img/ui/resourceIcon/sand02.png",
		"img/ui/resourceIcon/stick01.png",
		"img/ui/resourceIcon/brick.png",
		"img/ui/resourceIcon/claw.png",
		"img/ui/resourceIcon/cloth.png",
		"img/ui/resourceIcon/glass.png",
		"img/ui/resourceIcon/sharp.png",
		"img/ui/resourceIcon/shield.png",
		"img/ui/resourceIcon/steel02.png",
		"img/ui/resourceIcon/string.png",
		"img/ui/resourceIcon/wood02.png",
		"img/ui/resourceIcon/stone02.png",
		"img/ui/resourceIcon/stick02.png",
		"img/ui/resourceIcon/cloth.png",
		"img/ui/resourceIcon/straw2.png"
	};

	struct spreadIcons
	{
		spreadIcons(std::string n, int i1, int i2, int i3, int i4, int i5)
			:name(n),
			icon1(i1),
			icon2(i2),
			icon3(i3),
			icon4(i4),
			icon5(i5)
		{
		}

		std::string name;
		int icon1;
		int icon2;
		int icon3;
		int icon4;
		int icon5;
	};

	//name as on tilemaps
	const spreadIcons resourceSpread[] =
	{
		spreadIcons("", RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("tree", WOOD, WOOD, WOOD, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("appletree", WOOD, WOOD, APPLE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("cactus", CACTUS, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("mush1", MUSHROOM, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("mush2", MUSHROOM2, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("carbon", CARBON, CARBON, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("emerald", EMERALD, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("steel", STEEL, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("stone1", STONE, STONE, STONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("stone2", STONE, STONE, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("straw", STRAW, STRAW, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("nendo", NENDO, NENDO, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("sand", SAND, SAND, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("sharp", SHARP, SHARP, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("stick", STICK, STICK, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("bear", CLAW, CLAW, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("spider", STRING, STRING, STRING, RESOURCE_NONE, RESOURCE_NONE),
		spreadIcons("snake", SHARP, SHARP, RESOURCE_NONE, RESOURCE_NONE, RESOURCE_NONE)
	};

	struct CraftingItem
	{
		CraftingItem(int firstRes, int secondRes, int resultRes)
			:first(firstRes),
			second(secondRes),
			result(resultRes)
		{
		}

		int first;
		int second;
		int result;
	};

	const CraftingItem craftingList[] =
	{
		CraftingItem(WOOD, SHARP, WOOD2),
		CraftingItem(STONE, SHARP, STONE2),
		CraftingItem(STICK, STICK, STICK2),
		CraftingItem(STRAW, STRAW, STRAW2),
		CraftingItem(STRING, STRING, CLOTH),
		CraftingItem(CARBON, NENDO, BRICK),
		//CraftingItem(CLAW, WOOD, BRICK), //!
		//CraftingItem(STEEL, WOOD, BRICK), //!
		//CraftingItem(APPLE, WOOD, BRICK), //!
		//CraftingItem(CACTUS, WOOD, BRICK), //!
		CraftingItem(STEEL, SHARP, STEEL2),
		//CraftingItem(STEEL2, WOOD, BRICK), //!
		CraftingItem(CARBON, SAND, GLASS)
	};
}

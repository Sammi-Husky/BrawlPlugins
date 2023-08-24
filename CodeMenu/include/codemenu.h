#pragma once

#include <gf/gf_archive.h>
#include <string.h>
#include "page.h"

#define CM_HEAPSIZE   0xF0000    //Test heap size
#define CM_HEAPSTART  0x93600000 //Test start point
#define CM_HEAPMEM    1		     //Use MEM-2 (1)
#define CM_HEAPREGLOC 0x80494D28 // 80494D18 has Syringe, 80494D08 should have BrawlX
#define CM_HEAPID     61		 //Heap Number

struct playerPort
{
		char charID;
		bool infiniteShield;
		float selectPercent;
		bool dpadSelectPercent;
		bool disableDpad;
		double tagHex;
};
	
struct flightSettings
{
		float MaxHorizSpeed[2];
		float MaxVertSpeed[2];
		float HorzAccel[2];
		float VertAccel[2];	
};	

struct debugSettings
{
		bool debugFreeze;
		bool hitboxDisplay;
		bool collisionDisplay;
		bool stageCollisionDisplay;
		bool cameraLock;
		bool drawDI;
		bool displayFPS;
		bool displayHUD;	
};	

class mu_CodeMenu {
	public:		
		static void Initialize(), Destroy(), Update();
		static int* cmArcAccess(ARCNodeType fileType, int fileIndex = 0, int fileGroup = 0, void* packageLoc = 0);
};



#pragma once

#include <gf/gf_archive.h>

#define CM_HEAPSIZE   0x10000    //Test heap size
#define CM_HEAPSTART  0x93600000 //Test start point
#define CM_HEAPMEM    1		     //Use MEM-2 (1)
#define CM_HEAPREGLOC 0x80494D28 // 80494D18 has Syringe, 80494D08 should have BrawlX
#define CM_HEAPID     61		 //Heap Number

	

class mu_CodeMenu {
	public:
		
		static void Initialize();	
		static int* quickArcAccess(void* packageLoc, ARCNodeType fileType, int fileIndex = 0, int fileGroup = 0);
};



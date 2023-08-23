#include <CXCompression.h>
#include <OS/OSCache.h>
#include <gf/gf_archive.h>
#include <gf/gf_file_io.h>
#include <memory.h>
#include <modules.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <sy_core.h>
#include <types.h>
#include <vector.h>

#include "codemenu.h"

using namespace nw4r::g3d;

void mu_CodeMenu::Initialize()
{
	register const char* heapname = "CodeMenu";
	register int heapStart; heapStart = CM_HEAPSTART;
	register int heapSize; heapSize = CM_HEAPSIZE;
	register int heapMem; heapMem = CM_HEAPMEM;
	register int heapRegLoc; heapRegLoc = CM_HEAPREGLOC;
	
	//create the Code Menu heap
	asm {
		mr r3, heapStart;
		mr r4, heapSize;
		mr r5, heapname;
		mr r6, heapRegLoc;
		mr r7, heapMem;
		stw heapname, 0x0(heapRegLoc);
		stw heapStart, 0x4(heapRegLoc);
		stw heapSize, 0x8(heapRegLoc);
		stw heapMem, 0xC(heapRegLoc);
		bla 0x0259A4;		// create/gfMemoryPool
		li r4, CM_HEAPID;
		lwz r3,-0x43e8(r13);
		bla 0x026C78;		// setup heap 
	}
	
	// load in the pac file containing the contents
	gfFileIOHandle packageLoad;
	packageLoad.read("menu3/CodeMenu.pac",Heaps::CodeMenu,0);
	register void* packageLoc = packageLoad.getBuffer();
	register int* packagePart = 0;
	
	//gfArchive* package = new (Heaps::CodeMenu) gfArchive;

	//ARCNodeType fileType;
	//fileType = Data_Type_Misc;
	packagePart = mu_CodeMenu::quickArcAccess(packageLoc,Data_Type_Misc,1,3);

	
	asm {
			lis r4, 0x8049;
			stw packageLoc, 0x4D38(r4); 
			stw packagePart, 0x4D3C(r4);
	}
	
	packageLoad.release();	
};

int* mu_CodeMenu::quickArcAccess(void* packageLoc, ARCNodeType fileType, int fileIndex, int fileGroup)
{
	void* pointerToPart;
	gfArchive* package = new (Heaps::CodeMenu) gfArchive;
	package->setFileImage(packageLoc,0x10000,Heaps::CodeMenu);	
	pointerToPart = package->getData(fileGroup,fileType,fileIndex,0xFFFE);
	delete package;
	return (int*)pointerToPart;
};


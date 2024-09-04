#include <CX.h>
#include <OS/OSCache.h>
#include <gf/gf_archive.h>
#include <gf/gf_file_io.h>
#include <memory.h>
#include <modules.h>
#include <ms/ms_message.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <sy_core.h>
#include <types.h>
#include <vector.h>

#include "codemenu.h"
#include "pagemanager.h"

using namespace nw4r::g3d;

//CODE MENU FUNCTIONS ACCESSIBLE GAME-WIDE

void * cmFunctions[2] =
{
	mu_CodeMenu::cmArcAccess,	
	mu_CodeMenu::Update
};
void * cmModels[32];

void * cmi_DrawDI;

//CODE MENU INFO

char cmi_General[3];// = {0,0,0};
char cmi_Match[2];
char cmi_Extra[2];
char cmi_MatchFinish[4];
char cmi_GameMechanics[32];


playerPort cmi_Player1, cmi_Player2, cmi_Player3, cmi_Player4;
debugSettings cmi_Debug;
flightSettings cmi_Flight = { {2.0, 2.0},{2.0,2.0},{1.0,1.0},{1.0,1.0}};


void * cmInfo[12] =
{
	&cmi_General,
	&cmi_Match,
	&cmi_Extra,
	&cmi_MatchFinish,
	&cmi_Player1,
	&cmi_Player2,
	&cmi_Player3,
	&cmi_Player4,
	&cmi_GameMechanics,
	&cmi_Flight,
	&cmi_Debug,
	&cmi_DrawDI
};

void * cmComponents[4] =
{
	0,
	&cmFunctions,
	&cmModels,
	&cmInfo
};

mu_cmPageManager* pagemanager;

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
	cmComponents[0] = packageLoad.getBuffer();

	register void* cmSetupLoc = &cmComponents;
		asm {
			lis r4, 0x8049;
			stw cmSetupLoc, 0x4D38(r4);
	}
	pagemanager = new (Heaps::CodeMenu) mu_cmPageManager;
	
	pagemanager->init(&cmComponents);
	
	packageLoad.release();

	SyringeCore::syInlineHookRel(0x6910, reinterpret_cast<void*>(mu_CodeMenu::Update), Modules::SORA_MENU_SEL_CHAR);
	
};

void mu_CodeMenu::Destroy()
{
	delete pagemanager;	
};

int* mu_CodeMenu::cmArcAccess(ARCNodeType fileType, int fileIndex, int fileGroup, void* packageLoc)
{
	void* pointerToPart;
	if (packageLoc == 0)
	{
		packageLoc = cmComponents[0]; //Default to the Code Menu but allow this code to be plastic enough to work with other archives!
	};
	gfArchive* package = new (Heaps::CodeMenu) gfArchive;
	package->setFileImage(packageLoc,0x10000,Heaps::CodeMenu);	
	pointerToPart = package->getData(fileGroup,fileType,fileIndex,0xFFFE);
	delete package;
	return (int*)pointerToPart;
};

void mu_CodeMenu::Update()
{
	pagemanager->update();
	
};

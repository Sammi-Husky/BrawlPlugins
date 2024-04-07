#include "codemenu.h"
#include "pagemanager.h"

#include <memory.h>
#include <mu/mu_object.h>
#include <nw4r/g3d/g3d_resfile.h>

void mu_cmPageManager::update()
{
	mu_cmPageManager::timeIdle++;
	

	int  (*cmiConv)[1] = (int(*)[1])reinterpret_cast<int*>(cmInfo);
	int* cmiConv2 = cmiConv[3];		//Terrifyingly hacky way that happens to work.
	cmiConv2 = (int*)cmiConv2[0];
	char* cmiGeneral = (char*)cmiConv2[0];
	//char array access from void*
	//A thousand coders cried out in anguish reading the above few lines.
	
	if (cmiGeneral[0] && !cmActive)
	{
		cmiGeneral[2]++;
		mu_cmPageManager::cmActive = true;
		cmPageObjectTest = new MuObject;
		
		
		void* arcModel = mu_CodeMenu::cmArcAccess(Data_Type_Model,0,0);
		nw4r::g3d::ResFile::Init(&arcModel);
		cmPageObjectTest->create((nw4r::g3d::ResFile**)&arcModel,"Test",0x6A,0,Heaps::MenuInstance);
		//getData/gfArchive 80015DDC
		//pointer to result stored from r3 returned from previous function
		//Init/nw4r3g3d7ResFileFv 8018EA08
		//r3 = pointer to result of 80015DDC
		//r4 = 
		//cmPageObjectTest->create(<pointerToData>,<modelname>,<flag>,0,Heaps::CodeMenu);
		
		//node is relevant to muobject?
	}
	else if (!cmiGeneral[0] && cmActive)
	{
		mu_cmPageManager::cmActive = false;
		delete cmPageObjectTest;
	}
};

void mu_cmPageManager::init(void* cmAccess)
{
	mu_cmPageManager::cmInfo = cmAccess;
};
#include "codemenu.h"
#include "pagemanager.h"

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
	}
	else if (!cmiGeneral[0] && cmActive)
	{
		mu_cmPageManager::cmActive = false;
	}
	
	/*
	
	
	if (cmiActive[0] && !cmActive)	//is the code menu active but no models are rendered?
	{
		reinterpret_cast<char*>cmiActive[2]++;
		mu_cmPageManager::cmActive = true;
	}
	else if (!cmiActive[0] && cmActive)	//is the code menu deactivated but models for the menu are still rendering?
	{
		mu_cmPageManager::cmActive = false;
	}
	else
	{
		
	}
	*/

};

void mu_cmPageManager::init(void* cmAccess)
{
	mu_cmPageManager::cmInfo = cmAccess;
};
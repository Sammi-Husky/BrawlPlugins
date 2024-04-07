#pragma once

#include <mu/mu_object.h>
#include "codemenu.h"

#define MIN_OPTION_TIME 20.0f	//The amount of time spent at a minimum on a single selection option

class mu_cmPageManager
{
	float timeIdle;
	bool cmActive;
	MuObject* cmPageObjectTest;
	
	static int pageNo;
	static int currentLayer;
	static int prevPage[4];	// allows layers up to 5 deep
	static int prevOption[4];	// allows layers up to 5 deep

	static int optionNo;
	static int optionCount;
	void* cmInfo;
	mu_CodeMenu* cmMain;
	
public:
	void init(void* cmAccess);	
	void update();
	
};	
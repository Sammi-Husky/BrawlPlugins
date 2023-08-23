#pragma once

#define MIN_OPTION_TIME 20.0f	//The amount of time spent at a minimum on a single selection option

class mu_cmPageManager
{
	float timeIdle;
	bool cmActive;
	
	static int pageNo;
	static int currentLayer;
	static int prevPage[4];	// allows layers up to 5 deep
	static int prevOption[4];	// allows layers up to 5 deep

	static int optionNo;
	static int optionCount;
	void* cmInfo;
	
public:
	void init(void* cmAccess);	
	void update();
	
};	
#pragma once

class cmPage 
{
	union fourByte
	{
		char b[4];
		int i;
		float f;
	};
	
	int entryCount;
};
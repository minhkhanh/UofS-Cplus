// HookCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "HookCore.h"


// This is an example of an exported variable
HOOKCORE_API int nHookCore=0;

// This is an example of an exported function.
HOOKCORE_API int fnHookCore(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see HookCore.h for the class definition
CHookCore::CHookCore()
{
	return;
}

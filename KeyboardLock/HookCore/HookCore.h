// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HOOKCORE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HOOKCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HOOKCORE_EXPORTS
#define HOOKCORE_API __declspec(dllexport)
#else
#define HOOKCORE_API __declspec(dllimport)
#endif

// This class is exported from the HookCore.dll
class HOOKCORE_API CHookCore {
public:
	CHookCore(void);
	// TODO: add your methods here.
};

extern HOOKCORE_API int nHookCore;

HOOKCORE_API int fnHookCore(void);

#define CINTERFACE

#include <windows.h>

#include "d3d9.h"

// DLLエントリポイント
BOOL APIENTRY DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		d3d9_initialize(hinst, reason, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


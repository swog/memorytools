#pragma once

#define MEM_LOCATE_PATTERNS (1 << 0)
#define MEM_LOCATE_STRINGS (1 << 1)

/*
MemLocate - Locate a pattern in memory
*/
SIZE_T 
MemLocate( 
	LPBYTE Base,
	DWORD Size,
	LPCSTR Pattern,
	LPCSTR Mask,
	LPVOID* List,
	SIZE_T CountOfList,
	DWORD Flags
);

/*
MemLocate - Locate a pattern in memory inside a module region
*/
SIZE_T
MemLocate(
	LPCSTR ModuleName,
	LPCSTR Pattern,
	LPCSTR Mask,
	LPVOID* List,
	SIZE_T CountOfList,
	DWORD Flags
);

/*
MemModuleInfo - Module information
*/
LPBYTE
MemModuleInfo(
	LPCSTR Name,
	LPDWORD Size
);

/*
MemProc - GetProcAddress
*/
LPVOID
MemProc(
	LPCSTR ModuleName,
	LPCSTR Name
);

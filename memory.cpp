#include <Windows.h>
#include "memory.h"

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
)
{
	ZeroMemory( List, CountOfList * sizeof( LPVOID ) );

	if ( !Flags )
	{
		Flags = Mask ? MEM_LOCATE_PATTERNS : MEM_LOCATE_STRINGS;
	}

	SIZE_T Count = 0;
	LPBYTE Byte = 0;
	SIZE_T Len = Mask ? strlen( Mask ) : strlen( Pattern );

	if ( Flags & MEM_LOCATE_PATTERNS && Mask )
	{
		SIZE_T i;

		for ( Byte = Base; Byte <= Base + Size - Len; Byte++ )
		{
			for ( i = 0; i < Len; i++ )
			{
				if ( Mask[i] == 'x' && Pattern[i] != Byte[i] )
				{
					break;
				}
			}

			if ( i == Len )
			{
				if ( Count < CountOfList )
				{
					List[Count] = Byte;
				}

				Count++;
			}
		}
	}

	if ( Flags & MEM_LOCATE_STRINGS )
	{
		for ( Byte = Base; Byte <= Base + Size - Len; Byte++ )
		{
			if ( !strncmp( (LPCSTR)Byte, Pattern, Len ) )
			{
				if ( Count < CountOfList )
				{
					List[Count] = Byte;
				}

				Count++;
			}
		}
	}

	return Count;
}

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
)
{
	ZeroMemory( List, CountOfList * sizeof( LPVOID ) );

	DWORD SizeOfImage;
	LPBYTE Base = MemModuleInfo( ModuleName, &SizeOfImage );

	if ( Base )
	{
		return MemLocate( Base, SizeOfImage, Pattern, Mask, List, CountOfList, Flags );
	}

	return 0;
}

/*
MemModuleInfo - Module information
*/
LPBYTE
MemModuleInfo( 
	LPCSTR Name, 
	LPDWORD Size 
)
{
	HMODULE Module = GetModuleHandle( Name );
	MODULEINFO mi;

	if ( !Module )
	{
		goto NotFound;
	}

	if ( GetModuleInformation( GetCurrentProcess(), Module, &mi, sizeof( mi ) ) )
	{
		if ( Size )
		{
			*Size = mi.SizeOfImage;
		}

		return (LPBYTE)mi.lpBaseOfDll;
	}

	NotFound:

	if ( Size )
	{
		*Size = 0;
	}

	return NULL;
}

/*
MemProc - GetProcAddress
*/
LPVOID 
MemProc(
	LPCSTR ModuleName, LPCSTR Name
)
{
	HMODULE Module = GetModuleHandle( ModuleName );

	if ( !Module )
	{
		return NULL;
	}

	return GetProcAddress( Module, Name );
}

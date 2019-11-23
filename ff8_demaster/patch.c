#include <stdio.h>
#include <Windows.h>

BOOL modPage(DWORD address, int size)
{
	PDWORD lastProtect = 0;
	DWORD failure = VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &lastProtect);
	if (failure == 0)
	{
		DWORD myError = GetLastError();
		printf("err %08X", myError);
		return FALSE;
	}
	return TRUE;
}
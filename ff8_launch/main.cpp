#include <windows.h>

int main()
{
	HMODULE mod= LoadLibraryA("FFVIII_JP.dll");
	FARPROC prc = GetProcAddress(mod, "runGame");
	prc();
	return 0;
}
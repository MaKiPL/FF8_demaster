#include <stdio.h>
#include <Windows.h>

#define EXPORT __declspec(dllexport)
#pragma warning(disable:4996)

//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest()
{
	OutputDebugStringA("DEMASTER ENGINE LOADED!- we will try to repair what some company screwed up... ;-;\n");
	return;
}

static __int64 IMAGE_BASE;

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

#pragma region WM_UV_PATCH

BOOL uvPatchModMnemonic(DWORD address)
{
	if (!modPage(address, 4))
		return FALSE;
	*((BYTE*)address + 3) = 0;
	return TRUE;
}

unsigned int uvPatchAddresses[] =
{
	0x8A2CB2,
	0x8A2CB8,
	0x8A2D17,
	0x8A2D1D,
	0x8A2D6D,
	0x8A2D73,
	0x8A2DC5,
	0x8A2DCB,
	0x8A2E1B,
	0x8A2E21,
	0x8A2E6F,
	0x8A2E75
};

void ApplyUVPatch()
{
	BYTE* firstMnemonic = IMAGE_BASE + 0x8A2CB2;
	DWORD addMnemonic = *(DWORD*)firstMnemonic & 0xFFFFFF; //get mnemonic without parameter
	if (addMnemonic != 0x084680)
	{
		OutputDebugStringA("DEMASTER::ApplyUvPatch::The addresses are wrong! We found no ADD byte ptr[esi+8] at given rel call\n");
		return;
	}
	for (int i = 0; i < 12; i++)
		if (!uvPatchModMnemonic(IMAGE_BASE + uvPatchAddresses[i]))
			return;
}
#pragma endregion


#pragma region DirectIO

const char* DIRECT_IO_EXPORT_DIR = "EXP\\";
DWORD DIRECT_IO_EXPORT_DIR_LEN;

DWORD IO_backAddress = 0;
DWORD IO_backAddress2 = 0;
DWORD IO_backAddress3 = 0;
DWORD filePathBuffer, filePathStrlen;
char IO_backlogFilePath[256];

void directIO_fopenReroute()
{
	__asm
	{

		NOP //reconstruct stack notifier
		POP EBX //see below
		POP EBP //this is because Visual Studio doing it's shit

		//let's save our regs, because VS compiler will probably destroy them
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX

		MOV filePathBuffer, EDX //save buffer
		MOV filePathStrlen, ECX //and strlen
	}
	
	strcpy(IO_backlogFilePath, DIRECT_IO_EXPORT_DIR); //VS automatically does the ESP math
	strcpy(IO_backlogFilePath + DIRECT_IO_EXPORT_DIR_LEN, filePathBuffer); //same for this, no local vars so no ESP--
	OutputDebugStringA(IO_backlogFilePath);
	OutputDebugStringA("\n");


	__asm
	{
		//restore our regs
		POP EDX
		POP ECX
		POP EBX
		POP EAX
		//original code here
		PUSH ECX
		PUSH EDX
		LEA ECX, [ebp - 2Ch]
		JMP IO_backAddress
	}
}

const char rb[] = "rb";

void directIO_fopenReroute2()
{
	__asm
	{
		NOP //reconstruct stack notifier
		POP EBX //see below
		POP EBP //this is because Visual Studio doing it's shit

		PUSH OFFSET rb //too much hustle to get original value based on calcs without using any regs
		PUSH OFFSET IO_backlogFilePath
		JMP IO_backAddress2
	}
}
int loc00 = 0;
FILE* fd;
void directIO_fopenReroute3()
{
	__asm
	{
		NOP //reconstruct stack notifier
		POP EBX
		POP ECX
		POP EBP


		MOV EAX, ESI
		MOV ECX, [EBP - 0x0C]
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX
	}
	FILE* fd = fopen(IO_backlogFilePath, "rb");
	fseek(fd, 0, 2); //back
	loc00 = ftell(fd);
	fclose(fd);

	__asm
	{
		POP EDX
		POP ECX
		POP EBX
		POP EAX

		PUSH EBX
		MOV EBX, [loc00]
		ADD EAX, 0x10
		MOV [EAX], EBX
		SUB EAX, 0x10
		POP EBX

		JMP IO_backAddress3
	}
}


void ApplyDirectIO()
{
	//to not f-up things let's first see if the exp dir exists
	DIRECT_IO_EXPORT_DIR_LEN = strlen(DIRECT_IO_EXPORT_DIR);
	DWORD attrib = GetFileAttributesA(DIRECT_IO_EXPORT_DIR);
	if (attrib == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebugStringA("There is no export directory, so it looks like you didn't export the files from zzz files. Not applying patch\n");
		return;
	}


		//Patch fopen method
	//patch JMP to directIO_fopenReroute
	BYTE* fopenPatchMnemonic = IMAGE_BASE + 0x15D41EB;
	IO_backAddress = fopenPatchMnemonic + 5;
	DWORD jmpParam = (DWORD)directIO_fopenReroute - (DWORD)fopenPatchMnemonic - 5;
	modPage(fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;

	//patch JMP to directIO_fopenReroute2
	fopenPatchMnemonic = IMAGE_BASE + 0x15D4797;
	IO_backAddress2 = fopenPatchMnemonic + 5;
	jmpParam = (DWORD)directIO_fopenReroute2 - (DWORD)fopenPatchMnemonic - 5;
	modPage(fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
	*(fopenPatchMnemonic + 5) = 0x90; //NOP

		//patch FSEEKs
	//First fseek
	fopenPatchMnemonic = IMAGE_BASE + 0x15D47D8;
	modPage(fopenPatchMnemonic, 3); //PUSH + NOP
	*fopenPatchMnemonic = 0x6A;		//PUSH
	*(fopenPatchMnemonic+1) = 0x00;	//	PUSH->0
	*(fopenPatchMnemonic+2) = 0x90;	//NOP

	//Second fseek
	fopenPatchMnemonic = IMAGE_BASE + 0x15D4947;
	modPage(fopenPatchMnemonic, 3); //XOR EAX, EAX + NOP
	*fopenPatchMnemonic = 0x31;		//XOR EAX
	*(fopenPatchMnemonic + 1) = 0xc0;	//	XOR EAX->EAX
	*(fopenPatchMnemonic + 2) = 0x90;	//NOP

	//Now we need to fix fd struct filelen for modifications of RAW files
	//see fopen_archivePrepareSeek- we need to update struct before return
	//EAX contains struct for FD open archive
	fopenPatchMnemonic = IMAGE_BASE + 0x15D42B7; //MOV EAX, ESI; MOV ECX, [EBP-0ch]   [//8BC6 8B4DF4]
	IO_backAddress3 = fopenPatchMnemonic + 5;
	modPage(fopenPatchMnemonic, 5); //JMP
	jmpParam = (DWORD)directIO_fopenReroute3 - (DWORD)fopenPatchMnemonic - 5;
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
}

#pragma endregion

#pragma region TextureUpscaleMod

const int battleTextureUpscale = 0x600; //768 [600=2048]

void ApplyTextureUpscaleMod()
{
	int mnemonicPatchPlace = IMAGE_BASE + 0x15AABAB;
	modPage(mnemonicPatchPlace, 5);
	BYTE* b = mnemonicPatchPlace;
	*b = 0xB9; //MOV ECX
	*(DWORD*)(b + 1) = battleTextureUpscale;
}
#pragma endregion



BOOL WINAPI DllMain(

	HINSTANCE hinstDLL, // handle to DLL module

	DWORD fdwReason, // reason for calling function

	LPVOID lpReserved) // reserved

{
	if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
		return 0;

	IMAGE_BASE = GetModuleHandleA("FFVIII_EFIGS");

	InitTest();


	//LET'S GET THE HACKING DONE
	ApplyUVPatch();
	ApplyDirectIO();
	ApplyTextureUpscaleMod();


	//HACKING DONE, WE CAN GTFO
	return 1; //all success
}


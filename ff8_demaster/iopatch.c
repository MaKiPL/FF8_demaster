#include <stdio.h>
#include <Windows.h>
#include "coreHeader.h"



DWORD IO_backAddress = 0;
DWORD IO_backAddress2 = 0;
DWORD IO_backAddress3 = 0;
DWORD filePathBuffer, filePathStrlen;
char IO_backlogFilePath[256];

const DWORD IO_FUNC1 = 0x364CC;//first: 0x15D41EB
const DWORD IO_FUNC2 = 0x36B37;//first: 0x15D4797;
const DWORD IO_FUNC3 = 0x36B78;//first: 0x15D47D8;
const DWORD IO_FUNC4 = 0x36D37;//first: 0x15D4947;
const DWORD IO_FUNC5 = 0x365F8;//first: 0x15D42B7;
const DWORD IO_FUNC6 = 0x3656A;//first: 0x15D42B7;



//CREATES PATH
__declspec(naked) void directIO_fopenReroute()
{
	__asm
	{
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

	attr = GetFileAttributesA(IO_backlogFilePath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		sprintf(IO_backlogFilePath, "%stextures\\null.png", DIRECT_IO_EXPORT_DIR);


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
		LEA ECX, [esp+0x38] //first: [ebp - 2Ch]
		JMP IO_backAddress
	}
}

const char rb[] = "rb";

//PUSHES THE PATH TO READER
__declspec(naked) void directIO_fopenReroute2()
{
	__asm
	{
		PUSH OFFSET rb //too much hustle to get original value based on calcs without using any regs
		PUSH OFFSET IO_backlogFilePath
		JMP IO_backAddress2
	}
}
int loc00 = 0;
FILE* fd;

//SETS FILESIZE BASED ON REAL HDD ENTRY INSTEAD OF VTABLE OF WEEP ARCHIVE
__declspec(naked) void directIO_fopenReroute3()
{
	__asm
	{
		//JNE $+0xC //I can't do JE IO_backAddress3, probably because VS treats it's a DWORD PTR
		MOV EAX, ESI
		MOV ECX, [ESP + 0x50]
		TEST ESI, ESI //
		JNZ validEsi
		JMP IO_backAddress3
		validEsi :
		PUSH EAX
			PUSH EBX
			PUSH ECX
			PUSH EDX
	}
	attr = GetFileAttributesA(IO_backlogFilePath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		loc00 = -1;
	else
	{
		fd = fopen(IO_backlogFilePath, "rb");
		fseek(fd, 0, 2); //back
		loc00 = ftell(fd);
		fclose(fd);
	}

	__asm
	{
		POP EDX
		POP ECX
		POP EBX
		POP EAX

		PUSH EBX
		MOV EBX, [loc00]
		ADD EAX, 0x10
		MOV[EAX], EBX
		SUB EAX, 0x10
		POP EBX

		JMP IO_backAddress3
	}
}


void ApplyDirectIO()
{
	//let's see if the exp dir exists
	DIRECT_IO_EXPORT_DIR_LEN = strlen(DIRECT_IO_EXPORT_DIR);
	DWORD attrib = GetFileAttributesA(DIRECT_IO_EXPORT_DIR);
	if (attrib == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebugStringA("There is no export directory, so it looks like you didn't export the files from zzz files. Not applying patch\n");
		DIRECT_IO = FALSE;
		return;
	}


	//Patch fopen method
//patch JMP to directIO_fopenReroute
	BYTE* fopenPatchMnemonic = IMAGE_BASE + IO_FUNC1;
	IO_backAddress = fopenPatchMnemonic + 6;
	DWORD jmpParam = (DWORD)directIO_fopenReroute - (DWORD)fopenPatchMnemonic - 5;
	modPage(fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;

	//patch JMP to directIO_fopenReroute2
	fopenPatchMnemonic = IMAGE_BASE + IO_FUNC2;
	IO_backAddress2 = fopenPatchMnemonic + 5;
	jmpParam = (DWORD)directIO_fopenReroute2 - (DWORD)fopenPatchMnemonic - 5;
	modPage(fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
	*(fopenPatchMnemonic + 5) = 0x90; //NOP

		//patch FSEEKs
	//First fseek
	fopenPatchMnemonic = IMAGE_BASE + IO_FUNC3;
	modPage(fopenPatchMnemonic, 3); //PUSH + NOP
	*fopenPatchMnemonic = 0x6A;		//PUSH
	*(fopenPatchMnemonic + 1) = 0x00;	//	PUSH->0
	*(fopenPatchMnemonic + 2) = 0x90;	//NOP

	//Second fseek
	fopenPatchMnemonic = IMAGE_BASE + IO_FUNC4;
	modPage(fopenPatchMnemonic, 3); //XOR EAX, EAX + NOP
	*fopenPatchMnemonic = 0x31;		//XOR EAX
	*(fopenPatchMnemonic + 1) = 0xc0;	//	XOR EAX->EAX
	*(fopenPatchMnemonic + 2) = 0x90;	//NOP

	//Now we need to fix fd struct filelen for modifications of RAW files
	//see fopen_archivePrepareSeek- we need to update struct before return
	//EAX contains struct for FD open archive
	fopenPatchMnemonic = IMAGE_BASE + IO_FUNC5; //MOV EAX, ESI; MOV ECX, [EBP-0ch]   [//8BC6 8B4DF4]
	IO_backAddress3 = fopenPatchMnemonic + 6;
	modPage(fopenPatchMnemonic, 5); //JMP
	jmpParam = (DWORD)directIO_fopenReroute3 - (DWORD)fopenPatchMnemonic - 5;
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;

	//patch additional security that checks for weepTable
	fopenPatchMnemonic = IMAGE_BASE + IO_FUNC6;
	modPage(fopenPatchMnemonic, 2);
	*(WORD*)fopenPatchMnemonic = 0x9090;		//NOP NOP
}

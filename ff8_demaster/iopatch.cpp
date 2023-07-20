#include "coreHeader.h"

#include <string>

#include "config.h"

DWORD IO_backAddress = 0;
DWORD IO_backAddress2 = 0;
DWORD IO_backAddress3 = 0;
DWORD IO_backAddress4 = 0;
DWORD IO_seekAddresses[2];
DWORD IO_originalBytes[2];
WORD IO_originalBytes6;
DWORD filePathBuffer, filePathStrlen;
DWORD IO_SEEKASM1 = 0xFF90006A;// 6A0090FF;
DWORD IO_SEEKASM2 = 0x8B90C031;// 31C0908B;
char IO_backlogFilePath[256]{ 0 };

const std::string nullPath = std::string(DIRECT_IO_EXPORT_DIR) + "textures\\null.png";
const char * nullPathC = nullPath.c_str();

bool bInvalidFile = false;

//CREATES PATH
__declspec(naked) void directIO_fopenReroute()
{
	__asm
	{
		MOV filePathBuffer, EDX //save buffer
		MOV filePathStrlen, ECX //and strlen
	}
	if(bInvalidFile==true) //check state if was invalid file before
	{
		*reinterpret_cast<DWORD*>(IO_seekAddresses[0]) = IO_SEEKASM1;
		*reinterpret_cast<DWORD*>(IO_seekAddresses[1]) = IO_SEEKASM2;
		*reinterpret_cast<WORD*>(IO_backAddress4) = 0x9090;
	}

	bInvalidFile = false;

	strcpy(IO_backlogFilePath, DIRECT_IO_EXPORT_DIR); //VS automatically does the ESP math
	strcpy(IO_backlogFilePath + DIRECT_IO_EXPORT_DIR_LEN, (char*)filePathBuffer); //same for this, no local vars so no ESP--

	if (GetFileAttributesA(IO_backlogFilePath) == INVALID_FILE_ATTRIBUTES)
	{
#if DEBUG_LOG_IO
		OutputDebug("%s: %s, %s\n", __func__, IO_backlogFilePath, "file not found");
#endif
		strcpy(IO_backlogFilePath, (char*)filePathBuffer);
		bInvalidFile = true;
		*reinterpret_cast<DWORD*>(IO_seekAddresses[0]) = IO_originalBytes[0];
		*reinterpret_cast<DWORD*>(IO_seekAddresses[1]) = IO_originalBytes[1];
		*reinterpret_cast<WORD*>(IO_backAddress4) = IO_originalBytes6;

		//rewrite functions to original one
	}
#if DEBUG_LOG_IO
	else
		OutputDebug("%s: %s\n", __func__, IO_backlogFilePath);
#endif // DEBUG_LOG_IO
	

	__asm
	{
		//original code here
		//CMP bInvalidFile, 0
		//JE _normalpush
		//XOR ECX, ECX
		
		//_normalpush:
		MOV ECX, filePathStrlen
		MOV EDX, filePathBuffer
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
		CMP bInvalidFile, 0
		JE _normal
		PUSH EDI //EDI contains original filename 
		JMP IO_backAddress2
		_normal:
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
		MOV EAX, ESI
		MOV ECX, [ESP + 0x50]
		CMP bInvalidFile, 0
		JE _proceed
		JMP IO_backAddress3
		_proceed:
	}

	//if(bInvalidFile) //this destroys EAX
	//__asm {JMP IO_backAddress3}

	__asm
	{
		//JNE $+0xC //I can't do JE IO_backAddress3, probably because VS treats it's a DWORD PTR
		TEST ESI, ESI
		JNE validEsi
		JMP IO_backAddress3
		validEsi :
		PUSH EAX
			PUSH EBX
			PUSH ECX
			PUSH EDX
	}
	if (GetFileAttributesA(IO_backlogFilePath) == INVALID_FILE_ATTRIBUTES)
		loc00 = -1;
	else
	{
		fd = fopen(IO_backlogFilePath, "rb");
		fseek(fd, 0, SEEK_END); //back
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
	OutputDebug("Applying DIRECT_IO\n");
	//let's see if the exp dir exists
	DIRECT_IO_EXPORT_DIR_LEN = strlen(DIRECT_IO_EXPORT_DIR);
	if (GetFileAttributesA(DIRECT_IO_EXPORT_DIR) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("There is no export directory, so it looks like you didn't export the files from zzz files. Not applying patch\n");
		DIRECT_IO = FALSE;
		return;
	}
	const DWORD IO_FUNC1 = GetAddress(IOFUNC1);
	const DWORD IO_FUNC2 = GetAddress(IOFUNC2);
	const DWORD IO_FUNC3 = GetAddress(IOFUNC3);
	const DWORD IO_FUNC4 = GetAddress(IOFUNC4);
	const DWORD IO_FUNC5 = GetAddress(IOFUNC5);
	const DWORD IO_FUNC6 = GetAddress(IOFUNC6);

	//Patch fopen method
//patch JMP to directIO_fopenReroute

	
	BYTE* fopenPatchMnemonic = (BYTE*)(IMAGE_BASE + IO_FUNC1);
	IO_backAddress = (DWORD)fopenPatchMnemonic + 6;
	DWORD jmpParam = (DWORD)directIO_fopenReroute - (DWORD)fopenPatchMnemonic - 5;
	ModPage((DWORD)fopenPatchMnemonic, 5);
 	*fopenPatchMnemonic = 0xE9; //JMP [DW]
 	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;

	//patch JMP to directIO_fopenReroute2


	fopenPatchMnemonic = (BYTE*)(IMAGE_BASE + IO_FUNC2);
	IO_backAddress2 = (DWORD)fopenPatchMnemonic + 5;
	jmpParam = (DWORD)directIO_fopenReroute2 - (DWORD)fopenPatchMnemonic - 5;
	ModPage((DWORD)fopenPatchMnemonic, 5);
 	*fopenPatchMnemonic = 0xE9; //JMP [DW]
 	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
 	*(fopenPatchMnemonic + 5) = 0x90; //NOP

		//patch FSEEKs
	//First fseek

	fopenPatchMnemonic = (BYTE*)(IMAGE_BASE + IO_FUNC3);
	ModPage((DWORD)fopenPatchMnemonic, 4); //PUSH + NOP
	IO_originalBytes[0] = *((DWORD*)fopenPatchMnemonic);
	IO_seekAddresses[0] = (DWORD)fopenPatchMnemonic;
	*((DWORD*)fopenPatchMnemonic) = IO_SEEKASM1; //PUSH 0 NOP PUSH...
	//FF 75 10 FF

	//Second fseek


	fopenPatchMnemonic = (BYTE*)(IMAGE_BASE + IO_FUNC4);
	ModPage((DWORD)fopenPatchMnemonic, 4); //XOR EAX, EAX + NOP
	IO_seekAddresses[1] = (DWORD)fopenPatchMnemonic;
	IO_originalBytes[1] = *((DWORD*)fopenPatchMnemonic);
	*((DWORD*)fopenPatchMnemonic) = IO_SEEKASM2; //XOR EAX, EAX; NOP; PUSH...

	//Now we need to fix fd struct filelen for modifications of RAW files
	//see fopen_archivePrepareSeek- we need to update struct before return
	//EAX contains struct for FD open archive
	//

	fopenPatchMnemonic = (BYTE*)(IMAGE_BASE + IO_FUNC5); //MOV EAX, ESI; MOV ECX, [EBP-0ch]   [//8BC6 8B4DF4]
	IO_backAddress3 = (DWORD)fopenPatchMnemonic + 6;
	ModPage((DWORD)fopenPatchMnemonic, 5); //JMP
	jmpParam = (DWORD)directIO_fopenReroute3 - (DWORD)fopenPatchMnemonic - 5;
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;

	//patch additional security that checks for weepTable


	fopenPatchMnemonic = (BYTE*)(IMAGE_BASE + IO_FUNC6);
	ModPage((DWORD)fopenPatchMnemonic, 2);
	IO_backAddress4 = (DWORD)fopenPatchMnemonic;
	IO_originalBytes6 = *((WORD*)fopenPatchMnemonic);
	*(WORD*)fopenPatchMnemonic = 0x9090;		//NOP NOP
}

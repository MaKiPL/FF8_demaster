#include "coreHeader.h"

BYTE TexFuncCharaSegment(int this__, int row, int aIndex, int bIndex)
{
	//THIS IS NON-NAKED FUNCTION, SO WE CAN UTILIZE NEW LOCALS
	char tempPath[256];
	char tempSprint[256];
	sprintf(texPath, "%stextures\\field.fs\\field_hd", DIRECT_IO_EXPORT_DIR);
	strcpy(tempPath, texPath); //exp\\tex\\fieldfs\\fielhd


	if (aIndex - 97 > 0xF9F)
	{
		//bla bla not field model texture bla bla
	}
	if (aIndex >= 0xC19)
		sprintf(tempSprint, "\\%s%03u_%u", "p", aIndex - 3097, bIndex);
	else if (aIndex < 0x831)
	{
		if (aIndex < 0x449)
			sprintf(tempSprint, "\\%s%03u_%u", "d", aIndex - 97, bIndex);
		else
			sprintf(tempSprint, "\\%s%03u_%u", "n", aIndex - 1097, bIndex);
	}
	else
		sprintf(tempSprint, "\\%s%03u_%u", "o", aIndex - 2097, bIndex);


	char testPath[256];
	sprintf(testPath, "%s%s.png", tempPath, tempSprint);
	attr = GetFileAttributesA(testPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		sprintf(testPath, "%s_new%s.png", tempPath, tempSprint);
	attr = GetFileAttributesA(testPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		sprintf(testPath, "%s_new\\d000_0.png", tempPath); //ERROR !!!!

	strcpy(texPath, testPath); //establish path


	//we now need to create the atlas- it's normally one tex on top and one below
	//but of course we can easily tweak it because I ported whole texture function
	//- vanilla dev (that company starting at D) made every size hardcoded
	//like create gl tex with 768x768 or subtex at 384
	int width, height, channels;
	char n[256];
	lastrgbBuffer = stbi_load(texPath, &width, &height, &channels, 4);
	sprintf(n, "TEX::CHARAFIELD- %s %dx%d\n", texPath, width, height);
	OutputDebugStringA(n);
	lastWidth = width;
	lastHeight = height;

	return 0;
}
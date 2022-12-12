// ReSharper disable CppDefaultCaseNotHandledInSwitchStatement
#include "config.h"
#include "coreHeader.h"
DWORD _asm_WorldRetAddr1;
DWORD _asm_WorldRetAddr2;
DWORD _asm_WorldRetAddr3;
DWORD _asm_WorldRetAddr4;


struct worldTextureStructure
{
	char localPath[256]{ 0 };
	DWORD tpage{};
	SafeBimg buffer{ SafeBimgInit() };
	int width{ -1 };
	int height{ -1 };
	int channels{ -1 };
	bool bActive{ false };
	worldTextureStructure(DWORD in_tpage) noexcept
		: tpage{ in_tpage }
	{

	}
};
//Maki- are you alright Maki?
worldTextureStructure ws[] =
{
{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},{17},{18},{19},{20},
	{21},   // TRAIN TRACKS
	{22},   // CLOUDS
	{23},   // MAP
	{24},   // moon texture + fx
	{25},   // WATER
	{26},   // characters
	{27},   // 16: textype=18 ???
	{28},	// 0:  textype=0
	{29},	// 13: textype=0 full screen wm, sm wm, clouds
	{30},	// 15: textype=0 sm wm
	{31},	// 20: textype=0
	{32},	// 22: textype=0 sm wm
	{33},	// 28: textype=0 sm wm
	{34},	// 29: textype=0
};

int GetTextureIndex();

//you can't just create new folder because >WEEP< - too lazy to find the cause
DWORD lastKnownTextureId{ 0 };
bool LoadImageIntoWorldStruct(size_t texIndex, const char* const localn, int tPage, int palette)
{
	//If you want to be able to swap out a texture at a given index you could set bActive to false and it would force load it.
	if (ws[texIndex].bActive && ws[texIndex].buffer)//texture loaded skip
	{
		if (strcmp(ws[texIndex].localPath, localn) != 0) //the texture has a different name!
			PLOG_WARNING << "attempt to load texture with new name:\n\t" << localn << "\n\t!=\n\t" << ws[texIndex].
localPath;
		return true;
	}
	SafeBimg img = LoadImageFromFile(localn);
	if (!img)
		return false;
	ws[texIndex].width = img->m_width;
	ws[texIndex].height = img->m_height;
	ws[texIndex].channels = img->m_hasAlpha ? 4 : 3;
	ws[texIndex].buffer = std::move(img);
	ws[texIndex].bActive = true;
	strcpy(ws[texIndex].localPath, localn);
	
	PLOG_VERBOSE << "Loaded texture EAX: " << *reinterpret_cast<DWORD*>(IMAGE_BASE + 0x1780f88) << " Tpage: "
	<< tPage << " Palette: " << palette << " TexIndex: " << texIndex << " localn: " << localn;
	PLOG_VERBOSE << "w: " << ws[texIndex].width << " h: " << ws[texIndex].height << " channels: " << ws[texIndex].channels;
	return true;
}
void WtpGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	int textureType = gl_textures[48];
	char localn[256]{ 0 };

	DWORD unk = *(DWORD*)(IMAGE_BASE + 0x17424B4);
	size_t texIndex = lastKnownTextureId;

	int getTexIndex = GetTextureIndex();
	if (getTexIndex <= 0) return;

	if (textureType == 18 && texIndex == getTexIndex)
		//if (getTexIndex < 20 && (tPage > 14 && tPage < 26) )
	{
		if (DDSorPNG(localn, 256, "%stextures\\world\\dat\\texl\\texl_%03d_%d", DIRECT_IO_EXPORT_DIR, /*tPage*/texIndex,
		             palette))
			if (DDSorPNG(localn, 256, "%stextures\\world\\dat\\texl\\texl_%03d_%d", DIRECT_IO_EXPORT_DIR, /*tPage*/
			             texIndex, 0))
			{
				PLOG_ERROR << "Failed to Load: " << localn << " getTexIndex: " << getTexIndex << " tPage: " << tPage <<
 " texIndex: " << texIndex << " palette: " << palette;
				return;
			}

		LoadImageIntoWorldStruct(texIndex, localn, tPage, palette);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		RenderTexture(ws[texIndex].buffer.get());

		return;
	}
	if (textureType == 18)
	{
		if (DDSorPNG(localn, 256, "%stextures\\world\\dat\\wmset\\wmset_%03d_%d", DIRECT_IO_EXPORT_DIR, getTexIndex, palette))
			if (DDSorPNG(localn, 256, "%stextures\\world\\dat\\wmset\\wmset_%03d_%d", DIRECT_IO_EXPORT_DIR, getTexIndex, 0))
			{
				PLOG_ERROR << "Failed to Load: " << localn << " getTexIndex: " << getTexIndex << " palette: " << palette;
				return;
			}
		int wmStructPointer = 21;
		switch (getTexIndex)
		{
			case 13: //moon texture + fx
				wmStructPointer = 24;
				break;
			case 14: //clouds or atleast is used to be.
				wmStructPointer = 22;
				break;
			case 15: //characters and vehicles
				wmStructPointer = 26;
				break;
			case 16: //???
				wmStructPointer = 27;
				break;
			case 24: //???
				wmStructPointer = 23;
				break;
			case 28: // water
				wmStructPointer = 25;
				break;
			case 29: //train tracks
				wmStructPointer = 21;
				break;
		}
		if (LoadImageIntoWorldStruct(wmStructPointer, localn, tPage, palette))
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			RenderTexture(ws[wmStructPointer].buffer.get());
		}
	}
	else if (textureType == 0)
	{
		if (DDSorPNG(localn, 256, R"(%stextures\world\dat\wmset\wmset0_%03d_%d)", DIRECT_IO_EXPORT_DIR, getTexIndex, palette))
			if (DDSorPNG(localn, 256, R"(%stextures\world\dat\wmset\wmset0_%03d_%d)", DIRECT_IO_EXPORT_DIR, getTexIndex, 0))
			{
				PLOG_ERROR << "Failed to Load: " << localn << " getTexIndex: " << getTexIndex << " palette: " << palette;
				return;
			}
		int wmStructPointer = 28;
		switch (getTexIndex)
		{
			case 0: //
				wmStructPointer = 28;
				break;
			case 13: //full screen wm, sm wm, clouds
				wmStructPointer = 28;
				break;
			case 15: //sm wm
				wmStructPointer = 30;
				break;
			case 20: //
				wmStructPointer = 31;
				break;
			case 22: //sm wm
				wmStructPointer = 32;
				break;
			case 28: //sm wm
				wmStructPointer = 33;
				break;
			case 29: //
				wmStructPointer = 34;
				break;
		}
		if (LoadImageIntoWorldStruct(wmStructPointer, localn, tPage, palette))
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			RenderTexture(ws[wmStructPointer].buffer.get());
		}
	}
	return;
}

int GetTextureIndex()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	int textureType = gl_textures[48];
	const DWORD texA = *reinterpret_cast<DWORD*>(IMAGE_BASE + GetAddress(WORLDGETTEXIDX0)) + 1;
	const DWORD texB = *reinterpret_cast<DWORD*>(IMAGE_BASE + GetAddress(WORLDGETTEXIDX1)) + 1;
	const DWORD texC = *reinterpret_cast<DWORD*>(IMAGE_BASE + GetAddress(WORLDGETTEXIDX2)) + 1;
	const DWORD texD = *reinterpret_cast<DWORD*>(IMAGE_BASE + GetAddress(WORLDGETTEXIDX3)) + 1;

	if (textureType == 18)
	{
		lastKnownTextureId = 0;
		switch (tPage)
		{
		case 20:
			lastKnownTextureId = texA;
			return texA;
		case 22:
			lastKnownTextureId = texB;
			return texB;
		case 24:
			lastKnownTextureId = texC;
			return texC;
		case 26:
			lastKnownTextureId = texD;
			return texD;
		case 13: //moon texture + fx
		case 14: //clouds or atleast used to be
		case 28: //water textures
		case 29: //tracks, bridge, vehicle
		//case 16: //??? I see it trying to load this and don't see anything using it.
		//case 15: //vehicles + character
				 //this one is special the game mixes different textures into one and they overlap.
			// character texture is top left. some cars are top left.
			//Maki- looks like I need to find pre-atlas generator for this
			return tPage;
		}
		//OutputDebug("%s::tPage: %d\n", __func__, tPage);
	}
	else if (textureType == 0) //map
		switch (tPage)
		{
			case 13: //full screen wm, sm wm, clouds
			case 15: // sm wm
			case 22: // sm wm
			case 28: // sm wm
				// when small map up 15,22,28 swaps / flickers with 13
				//return 13;
				return -1; //disabled to due to cloulds colliding with minimap
			case 0: //??? probably more minimaps 
			case 20: //???
			case 29: //???
				return tPage;
			default:
				PLOG_VERBOSE << "tPage: " << tPage << " textureType: " << textureType << " palette: " << palette;
				break;
		}
	else
		PLOG_VERBOSE << "tPage: " << tPage << " textureType: " << textureType << " palette: " << palette;
	return -1;
}

DWORD LOCALDEB = 0;

DWORD WtpCheck()
{
	if (!WORLD_TEXTURES)
		return 0;

	const int textureType = gl_textures[48];
	if (textureType != 18 && textureType != 0) //we want only world textures
		return 0;

	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];

	char localn[256]{ 0 };
	const int textureIndex = GetTextureIndex();
	
	PLOG_VERBOSE << "tPage: " << tPage << " textureType: " << textureType << " palette: " << palette;

	if (textureIndex < 0) //disabled textures are set to -1;
		return 0;
	if (textureType == 18 && lastKnownTextureId == textureIndex)//(textureIndex < 20 && (tPage > 14 && tPage < 26))
	{
		if (DDSorPNG(localn, 256, R"(%stextures\world\dat\texl\texl_%03d_%d)", DIRECT_IO_EXPORT_DIR, textureIndex, palette))
			DDSorPNG(localn, 256, R"(%stextures\world\dat\texl\texl_%03d_%d)", DIRECT_IO_EXPORT_DIR, textureIndex, 0);
	}
	else if (textureType == 18)
	{
		if (DDSorPNG(localn, 256, R"(%stextures\world\dat\wmset\wmset_%03d_%d)", DIRECT_IO_EXPORT_DIR, textureIndex, palette))
			DDSorPNG(localn, 256, R"(%stextures\world\dat\wmset\wmset_%03d_%d)", DIRECT_IO_EXPORT_DIR, textureIndex, 0);
	}
	else if (textureType == 0)
	{
		//giving different name to avoid collisions as some of these have the same textureIndex.
		// I do wonder if the texture index does anything for this part. Minimap seems to use many different ones.
		if (DDSorPNG(localn, 256, R"(%stextures\world\dat\wmset\wmset0_%03d_%d)", DIRECT_IO_EXPORT_DIR, textureIndex, palette))
			DDSorPNG(localn, 256, R"(%stextures\world\dat\wmset\wmset0_%03d_%d)", DIRECT_IO_EXPORT_DIR, textureIndex, 0);

		//return 1; //force attempt to override
	}

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		//OutputDebug("%s::Failed to Load: %s\n", __func__, localn); //spamming message!
		return 0;
	}
	return 1;
}

void ApplyWorldPatch()
{
}
#undef CRASHLOG
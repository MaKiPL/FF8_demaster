#include "coreHeader.h"

#define CRASHLOG OutputDebug("%s::%d::%s\n", __FILE__, __LINE__,__func__)

DWORD _bspBackAdd1;
DWORD _bspBackAdd2;



DWORD bAlreadyFreed;

unsigned char* buffer16;
unsigned char* buffer17;
unsigned char* buffer18;
unsigned char* buffer19;
unsigned char* buffer20;
unsigned char* buffer21;
unsigned char* buffer22;
DWORD lastStage;

struct battleSceneryPathandTexture
{
	std::string localPath{};
	safe_bimg buffer{ safe_bimg_init() };
	operator bool() const noexcept
	{
		return !localPath.empty() && buffer;
	}
	operator bimg::ImageContainer* () const noexcept
	{
		return buffer.get();
	}
};

struct battleSceneryStructure
{
	DWORD tpage{};
	std::vector<battleSceneryPathandTexture> buffer{}; //use a vector to store animation frames.
	int width{ -1 };
	int height{ -1 };
	int channels{ -1 };
	bool bActive{ false };
	int nextFrame{ 0 };
	battleSceneryStructure(DWORD in_tpage) noexcept
		: tpage{ in_tpage }
	{
	}
	auto& current() const noexcept
	{
		const auto currentFrame = nextFrame - 1;
		if (static_cast<size_t>(currentFrame) < buffer.size())
			return buffer[currentFrame];
		static const auto dummy = battleSceneryPathandTexture{};
		return dummy;
	}

	void update(battleSceneryPathandTexture&& new_img)
	{
		OutputDebug("%s::%d::Updating: tpage: %d, buffer count: \n", __func__, __LINE__, tpage, buffer.size());

		const auto currentFrame = nextFrame - 1;
		if (static_cast<size_t>(currentFrame) < buffer.size())
		{
			OutputDebug("\tcurrent frame: %d\n\told path: %s\n\tnew path: %s\n", currentFrame, buffer[currentFrame].localPath.c_str(), new_img.localPath.c_str());
			buffer[currentFrame] = std::move(new_img);
			return;
		}
		while (static_cast<size_t>(currentFrame) > buffer.size()) //encase the size() is too small.
			buffer.emplace_back(battleSceneryPathandTexture{});
		buffer.emplace_back(std::move(new_img));
	}
};

battleSceneryStructure bss[] =
{
	{16},
	{17},
	{18},
	{19},
	{20},
	{21},
	{22},
};

bool LoadImageIntoBattleSceneryStruct(const size_t index, const DWORD tPage, const char* const localn)
{
	if (bss[index].current() && bss[index].current().localPath == localn) //prevent loading an image that is loaded.
		return true;
	int palette = tex_header[52];
	//spamming so i put it here so it only logs when loading.
	OutputDebug("%s::Stage: %d, Tpage: %d, Palette: %d\n", __func__, currentStage, tPage, palette);

	safe_bimg img = LoadImageFromFile(localn);
	if (!img)
	{
		OutputDebug("%s::%d::Fail to load texture, clearing buffer vector: tpage: %d, buffer count: \n", __func__, __LINE__, tPage, bss[index].buffer.size());
		bss[index].bActive = false;
		bss[index].buffer.clear();
		return false;
	}

	bss[index].bActive = true;

	bss[index].width = img->m_width;

	bss[index].height = img->m_width;

	bss[index].channels = img->m_hasAlpha ? 4 : 3;

	bss[index].update({ localn, std::move(img) });

	bss[index].tpage = tPage;
	OutputDebug("\t%s::Battle texture for page: %d", __func__, tPage);
	OutputDebug("\t%s::w: %d; h: %d; channels: %d\n", __func__, bss[index].width, bss[index].height, bss[index].channels);
	return true;
}
void _bspGl()
{
	DWORD tPage = gl_textures[50];
	char localn[256]{ 0 };
	if (DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage, bss[tPage - 16].nextFrame++))
	{
		if (DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage, 0))
			if (!DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage) && bss[tPage - 16].buffer.size() > 1)
				bss[tPage - 16].buffer.clear();
		if (bss[tPage - 16].nextFrame != 1)
			bss[tPage - 16].nextFrame = 1; // prevents using the wrong frame number for the base texture
										   // or resets if current frame number too high.
	}

	if (LoadImageIntoBattleSceneryStruct(tPage - 16, tPage, localn))
	{
		RenderTexture(bss[tPage - 16].current());
	}
	return;
}

DWORD _bspCheck()
{
	int textureType = gl_textures[48];
	if (textureType != 11) //we want only battle textures
		return 0;
	if (currentStage == -1)
		return 0;
	DWORD tPage = gl_textures[50];
	if (tPage < 16)
		return 0;
	if (tPage > 21)
		return 0;
	char localn[256]{ 0 };
	if (DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage))
	{
		OutputDebug("%s FAILED ON TEXTURE!; Expected: a0stg%03d_%d.(dds|png)\n", __func__, currentStage, tPage);
		return 0;
	}
	OutputDebug("%s: Happy at a0stg%03d_%d.(dds|png)\n", __func__, currentStage, tPage);
	return 1;
}

DWORD** ds_free;
DWORD** ds_teximg;

//this function hooks the pre-upload to VRAM data. Add new cases below for other modules
GLint MINFILTER, MAGFILTER;
__declspec(naked) void _bsp()
{
	//glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &MINFILTER);
	//glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &MAGFILTER);
	__asm
	{
		MOV DWORD PTR[ebp + 0x8], EAX
		MOV texturesPtr, EAX
		MOV EAX, [EBP + 0x10]
		MOV pixelsPtr, EAX
		MOV bAlreadyFreed, 0

		CALL _bspCheck
		TEST EAX, EAX
		JNZ _bspOk

		CALL _wtpCheck //WORLD MODULE
		TEST EAX, EAX
		JNZ _wtpOk



	_original:
		PUSH DWORD PTR[EBP + 0x10]
			PUSH 0x1401
			PUSH 0x80E1
			PUSH 0
			PUSH ESI
			PUSH EDI
			PUSH 0x8058
			PUSH 0
			PUSH 0xDE1
			MOV EAX, ds_teximg
			MOV EAX, [EAX]
			CALL EAX
			_out :
		JMP _bspBackAdd1

			_wtpOk :
		MOV EAX, OFFSET IMAGE_BASE
			MOV EAX, [EAX]
			ADD EAX, 0x166B2A8
			PUSH DWORD PTR[EBP + 0x10]
			CALL[EAX]
			//CALL DWORD PTR DS:0x1166B2A8
			MOV bAlreadyFreed, 1
			CALL _wtpGl
			JMP _out

			JMP _original

			_bspOk :
		PUSH DWORD PTR[EBP + 0x10]
			MOV EAX, ds_free
			MOV EAX, [EAX]
			CALL EAX
			MOV bAlreadyFreed, 1

			CALL _bspGl
			JMP _out

	}
}

__declspec(naked) void _bspFree()
{
	__asm
	{
		CMP bAlreadyFreed, 1
		JE _out
		push   DWORD PTR[ebp + 0x10]
		MOV EAX, ds_free
		MOV EAX, [EAX]
		CALL EAX
		_out :
		JMP _bspBackAdd2
	}

}

void ApplyBattleFieldPatch()
{
	OutputDebug("Applying battle field patch\n");
	ds_free = (DWORD**)(IMAGE_BASE + 0x166b2a8);
	ds_teximg = (DWORD**)(IMAGE_BASE + 0x166b4a0);
	OutputDebug("ApplyBattleFieldPatch():ds_free is at: %08X and ds_teximg is at: %08X\n", ds_free, ds_teximg);
	_bspBackAdd1 = (DWORD)InjectJMP(IMAGE_BASE + 0x1573AFF, (DWORD)_bsp, 38);
	_bspBackAdd2 = (DWORD)InjectJMP(IMAGE_BASE + 0x1573B54, (DWORD)_bspFree, 9);

	//this disables textureLimit for resolution
	InjectJMP(IMAGE_BASE + 0x156CED4, (DWORD)(IMAGE_BASE + 0x156D30B), 6);
}
#undef CRASHLOG
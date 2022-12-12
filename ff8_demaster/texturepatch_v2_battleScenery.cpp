#include "coreHeader.h"
#include <chrono>

#include "config.h"

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
	SafeBimg buffer{ SafeBimgInit() };
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
private:
	int m_nextFrame{ 0 };
	std::chrono::steady_clock::time_point m_timestamp{ std::chrono::steady_clock::now() };
	//constexpr static std::chrono::milliseconds m_delay { 100 };
public:
	battleSceneryStructure(DWORD in_tpage) noexcept
		: tpage{ in_tpage }
	{
	}
	void restart_animation() noexcept
	{
		m_nextFrame = 1;
	}
	int currentFrame() const noexcept
	{
		return m_nextFrame - 1;
	}
	auto get_current_frame_number_and_iterate() noexcept
	{
		const auto current_time = std::chrono::steady_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - m_timestamp);
		if (duration > std::chrono::milliseconds(BATTLE_STAGE_ANIMATION_DELAY))
		{
			if (m_nextFrame != 1 || !buffer.empty())
				PLOG_VERBOSE << "Frame Update: duration: " << duration.count() << " ms, Current animation frame: " << currentFrame();
			m_timestamp = current_time;
			++m_nextFrame;
		}
		return currentFrame();
	}

	[[nodiscard]] auto& current() const noexcept
	{
		const int current_frame = currentFrame();
		if (current_frame >= 0 && static_cast<size_t>(current_frame) < buffer.size())
			return buffer[current_frame];
		static const auto dummy = battleSceneryPathandTexture{};
		return dummy;
	}

	void update(battleSceneryPathandTexture&& new_img)
	{
		PLOG_VERBOSE << "Updating: tpage: " << tpage << ", buffer count: " << buffer.size();

		const auto current_frame = currentFrame();
		if (static_cast<size_t>(current_frame) < buffer.size())
		{
			PLOG_VERBOSE << "\tcurrent frame: " << current_frame << "\n\told path: " << buffer[current_frame].localPath << "\n\tnew path: " << new_img.localPath;
			buffer[current_frame] = std::move(new_img);
			return;
		}
		while (static_cast<size_t>(current_frame) > buffer.size()) //encase the size() is too small.
			buffer.emplace_back(battleSceneryPathandTexture{});
		buffer.emplace_back(std::move(new_img));
	}
};

battleSceneryStructure bss[] =
{
	{16},{17},{18},{19},{20},{21},{22},
};

bool LoadImageIntoBattleSceneryStruct(const size_t index, const DWORD tPage, const char* const localn)
{
	
	if (!static_cast<bool>(BATTLE_STAGE_FORCE_RELOAD) && bss[index].current() && bss[index].current().localPath == localn) //prevent loading an image that is loaded.
		return true;
	int palette = tex_header[52];
	//spamming so i put it here so it only logs when loading.
	PLOG_VERBOSE << "Stage: " << currentStage << ", Tpage: " << tPage << ", Palette: " << palette;

	SafeBimg img = LoadImageFromFile(localn);
	if (!img)
	{
		PLOG_ERROR << "Fail to load texture, clearing buffer vector: tpage: " << tPage << ", buffer count: " << bss[index].buffer.size();
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
	PLOG_VERBOSE << "Loaded battle texture for page: " << tPage;
	PLOG_VERBOSE << "w: " << bss[index].width << "; h: " << bss[index].height << "; channels: " << bss[index].channels;
	return true;
}
void BspGl()
{
	DWORD tPage = gl_textures[50];
	char localn[256]{ 0 };
	if (DDSorPNG(localn, 256, R"(%stextures\battle.fs\hd_new\a0stg%03d_%d_%d)", DIRECT_IO_EXPORT_DIR, currentStage, tPage, bss[tPage - 16].get_current_frame_number_and_iterate()))
	{
		if (DDSorPNG(localn, 256, R"(%stextures\battle.fs\hd_new\a0stg%03d_%d_%d)", DIRECT_IO_EXPORT_DIR, currentStage,
		             tPage, 0))
			if (!DDSorPNG(localn, 256, R"(%stextures\battle.fs\hd_new\a0stg%03d_%d)", DIRECT_IO_EXPORT_DIR,
			              currentStage, tPage) && bss[tPage - 16].buffer.size() > 1)
				bss[tPage - 16].buffer.clear();
		bss[tPage - 16].restart_animation(); // prevents using the wrong frame number for the base texture
		// or resets if current frame number too high.
	}

	if (LoadImageIntoBattleSceneryStruct(tPage - 16, tPage, localn))
		RenderTexture(bss[tPage - 16].current());
}

DWORD BspCheck()
{
	const int textureType = gl_textures[48];
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
	if (!DDSorPNG(localn, 256, R"(%stextures\battle.fs\hd_new\a0stg%03d_%d)", DIRECT_IO_EXPORT_DIR, currentStage, tPage)
		|| !DDSorPNG(localn, 256, R"(%stextures\battle.fs\hd_new\a0stg%03d_%d_0)", DIRECT_IO_EXPORT_DIR, currentStage, tPage))
	{
		PLOG_VERBOSE << "BspCheck success at: " << localn;
		return 1;
	}
	PLOG_ERROR << "BspCheck failed at stage: " << currentStage << ", tpage: " << tPage;
	return 0;
}

DWORD FbgCheck()
{
	if(!FIELD_BACKGROUND)
		return 0;
	return 0; //WIP
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

		CALL BspCheck
		TEST EAX, EAX
		JNZ _bspOk

		CALL WtpCheck //WORLD MODULE
		TEST EAX, EAX
		JNZ _wtpOk

		CALL FbgCheck //Field background module
		TEST EAX, EAX
		JNZ _fbgOk



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
			PUSH DWORD PTR[EBP + 0x10]
			MOV EAX, ds_free
			MOV EAX, [EAX]
			CALL EAX
			MOV bAlreadyFreed, 1
			CALL WtpGl
			JMP _out

			JMP _original

		_bspOk :
			PUSH DWORD PTR[EBP + 0x10]
				MOV EAX, ds_free
				MOV EAX, [EAX]
				CALL EAX
				MOV bAlreadyFreed, 1

				CALL BspGl
				JMP _out

		_fbgOk: //FIELD BACKGROUND
			PUSH DWORD PTR[EBP + 0x10]
			MOV EAX, ds_free
			MOV EAX, [EAX]
			CALL EAX
			MOV bAlreadyFreed, 1
			CALL FbgGl
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
	PLOG_INFO << "Applying battle field patch";
	ds_free = reinterpret_cast<DWORD**>(IMAGE_BASE + GetAddress(DS_FREE));
	ds_teximg = reinterpret_cast<DWORD**>(IMAGE_BASE + GetAddress(DS_TEXIMG));
	PLOG_INFO << "ds_free is at: " << ds_free << " and ds_teximg is at: " << ds_teximg;
	_bspBackAdd1 = reinterpret_cast<DWORD>(InjectJMP(IMAGE_BASE + GetAddress(_BSPBACKADD1)
		, reinterpret_cast<DWORD>(_bsp), 38));
	_bspBackAdd2 = reinterpret_cast<DWORD>(InjectJMP(IMAGE_BASE + GetAddress(_BSPBACKADD2)
		, reinterpret_cast<DWORD>(_bspFree), 9));

	//fixes balamb low-res and library low-res for example @ 15.08.2022
	DWORD bgPatchWidth = IMAGE_BASE + GetAddress(BGRESPATCH1);
	DWORD bgPatchHeight = IMAGE_BASE + GetAddress(BGRESPATCH1) + 0xC;
	ModPage(bgPatchWidth, 1);
	ModPage(bgPatchHeight, 1);
	*reinterpret_cast<BYTE*>(bgPatchWidth) = 0xE0; //SHL instead of SHR
	*reinterpret_cast<BYTE*>(bgPatchHeight) = 0xE0; //SHL instead of SHR

	//this disables textureLimit for resolution
	InjectJMP(IMAGE_BASE + GetAddress(BATTLEJMPPATCH1), (DWORD)(IMAGE_BASE + GetAddress(BATTLEJMPPATCH2)), 6);
}
#undef CRASHLOG
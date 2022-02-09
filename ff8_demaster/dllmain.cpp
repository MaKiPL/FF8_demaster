#include "coreHeader.h"
#include "kiero.h"
#include <GL/GL.h>
#include "GLFW/include/GLFW/glfw3.h"
#include "minhook/include/MinHook.h"

#define CRASHLOG OutputDebug("%s::%d::%s\n", __FILE__, __LINE__,__func__)

/*
Thomas the tank Engine
*/

/*
*
*
*/
DWORD IMAGE_BASE = 0;
DWORD IMAGE_SIZE = 0;
DWORD THIS_BASE = 0;
DWORD THIS_SIZE = 0;
DWORD OPENGL_HANDLE = 0;
const char* DIRECT_IO_EXPORT_DIR = "DEMASTER_EXP\\";
std::unique_ptr<FILE, decltype(&fclose)> logFile{ nullptr, fclose };
void checkGlew();
DWORD* tex_header = 0;
DWORD attr = -1;
DWORD DIRECT_IO_EXPORT_DIR_LEN = -1;
DWORD* tex_struct = 0;
DWORD* gl_textures = 0;
DWORD pixelsPtr = 0;
DWORD texturesPtr = 0;
DWORD TEX_TYPE = 0;
int currentStage = -1;
DWORD* langIdent_ESI;
BOOL UVPATCH, DIRECT_IO, TEXTURE_PATCH, DEBUG_PATCH, LOG;
BOOL BATTLE_CHARA, FIELD_ENTITY, BATTLE_HOOK, FIELD_BACKGROUND, WORLD_TEXTURES;
BOOL LINEAR_PATCH, OPENGL_HOOK;
int BATTLE_STAGE_ANIMATION_DELAY;
BOOL BATTLE_STAGE_FORCE_RELOAD;

static bool glewInitialized = false;

static float frames = 0.0f;

void OutputDebug(const char* fmt, ...)
{
    
#if _DEBUG && !JAPANESE_PATCH
      //DWORD fmtPtr = (DWORD)fmt;
      //if (
      //   IMAGE_BASE <= fmtPtr && fmtPtr <= IMAGE_BASE + IMAGE_SIZE
      //   ||
      //   THIS_BASE <= fmtPtr && fmtPtr <= THIS_BASE + THIS_SIZE
      //   )
      //{
      //   return;
      //}
    //static StackWalker sw;
   // sw.ShowCallstack();
    if (IsBadReadPtr(fmt, 4)) return;
      std::string fmtString = std::string(fmt);
      if (fmtString.substr(0, 7) == std::string("GLERROR"))
         return;
      va_list args;
      char tmp_str[1024];

      va_start(args, fmt);
      vsnprintf(tmp_str, sizeof(tmp_str), fmt, args);
      va_end(args);

      printf(tmp_str);

      if (logFile)
      {
	      fwrite(tmp_str, sizeof(char), strlen(tmp_str), logFile.get());
	      fflush(logFile.get());
      }
#endif
}

DWORD _dllmainBackAddr1;
DWORD _dllmainBackAddr2;

#if JAPANESE_PATCH
const char* windowTitle = "ファイナルファンタジーVIII ディマスター by Maki [1.3]";
#else
const char* windowTitle = "FINAL FANTASY VIII Remastered - Demaster patch by Maki [1.3]";
#endif

__declspec(naked) void _asm_ReplaceWindowTitle()
{
   __asm
   {
      PUSH windowTitle
      JMP _dllmainBackAddr2
   }
}


typedef GLFWwindow*(*oGlfwCreateWindow)(int width, int height, const char* title, GLFWwindow* monitor, GLFWwindow* share);

static GLFWwindow* ffWindow;
static LPVOID glfwWindowTrampoline;
GLFWwindow* hookGlfwWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    return ffWindow = ((GLFWwindow*(*)(int,int,const char*, GLFWmonitor*, GLFWwindow*))glfwWindowTrampoline)(width, height, title, monitor, share);
        
}

typedef void __stdcall _glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

static LPVOID oglViewport;

void * __stdcall hookGlViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    checkGlew();
    static bool backdoorUsed = false;
    if(glewInitialized && !backdoorUsed)
    {
        LPVOID test1 = __glewBufferData;
        LPVOID test2 = glBufferData;
        //Maki: PUT BACKDOORS FOR EXTENSIONS HERE
        
        backdoorUsed = true;
    }

    return ((void* (__stdcall*)(GLint, GLint, GLsizei, GLsizei))oglViewport)(x, y, width, height);
}

static LPVOID oglSwaPbuffers;

#define DEMFUNC(name,...) void * __stdcall hook##name##(##__VA_ARGS__##)

DEMFUNC(GlSwap, int);




void GetWindow()
{
    unsigned int wndTitle = IMAGE_BASE + GetAddress(WINDOWTITLE);
   _dllmainBackAddr2 = (DWORD)InjectJMP(wndTitle, (DWORD)_asm_ReplaceWindowTitle, 5);
   uint wndGlfw = wndTitle + 0x12 + (*(DWORD*)(IMAGE_BASE + GetAddress(WINDOWTITLE) + 0x12)) + 4;
   //MH_CreateHook((LPVOID)wndTitle, hookGlfwWindow, glfwWindowTrampoline);
   MH_CreateHook((LPVOID)wndGlfw, hookGlfwWindow, &glfwWindowTrampoline); //Maki: We need trampoline here!
   MH_STATUS hookApiViewport = MH_CreateHookApi(L"OPENGL32", "glViewport", hookGlViewport, &oglViewport);
}

//typedef void APIENTRY glClear(GLbitfield);
//typedef void (__stdcall * glClear)(GLbitfield);
//typedef void __stdcall * glClear(GLbitfield mask);
typedef void(__stdcall* oglClear)(GLbitfield);
typedef void(__stdcall* oglClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void(__stdcall* oglBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

static oglClear _oglClear;
static oglClearColor _oglClearColor;
static oglBufferData _oglBufferData;

void __stdcall kglClear(GLbitfield mask)
{
    _oglClear(mask);
}

void __stdcall kglClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    //_oglClearColor(red, green, 1.0f, 1.0f); //Maki: Try it :D
    _oglClearColor(red, green, blue, alpha);

}

void __stdcall koglBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
    _oglBufferData(target, size, data, usage);
}

static LPVOID* oglBindBuffer;


//Maki: probably to be replaced by manual hook/minhook - it's just additional layer of doing the same things
//Maki: btw- this works on independent thread- so screw your context xD although they should share it naturally
int kieroThread()
{
    if (kiero::init(kiero::RenderType::OpenGL) == kiero::Status::Success)
    {
        // define KIERO_USE_MINHOOK must be 1
        // the index of the required function can be found in the METHODSTABLE.txt
			//kiero::bind(10, (void**)&_oglClear, kglClear); //Maki: This makes Nsight crash
			//kiero::bind(12, (void**)&_oglClearColor, kglClearColor);
        //kiero::bind(336, (void**)&_oglBufferData, koglBufferData);
        //glewInit();
        //auto testing = glBufferData;
        //LPVOID orig;
        //MH_CreateHook(glBufferData, koglBufferData, &orig);
        //MH_EnableHook(MH_ALL_HOOKS);


       // MH_CreateHook(bufferTest, koglBufferData, oglBindBuffer);
        MH_EnableHook(MH_ALL_HOOKS);


        // If you just need to get the function address you can use the kiero::getMethodsTable function
        //_oglClear = (oglClear)kiero::getMethodsTable()[10];
    }

    return 0;
}

//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest()
{
   OutputDebug("DEMASTER ENGINE LOADED!\n");
   return;
}

DWORD lastJMP;

__declspec(naked) void nullsub()
{
   __asm
   {
      ret
   }
}

//force GL_LINEAR with non-fixed UV was a d*k move ... ~Maki
void ApplyFilteringPatch()
{
   InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH1) + 1, GL_NEAREST);
   InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH2) + 1, GL_NEAREST);
   InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH3) + 1, GL_NEAREST);
   InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH4) + 1, GL_NEAREST);
}

void ReadConfigFile()
{
   if (GetFileAttributesA(DEMASTER_CONF) == INVALID_FILE_ATTRIBUTES)
   {
      OutputDebug("File " DEMASTER_CONF " not found- all failed\n");
      return;
   }
   OutputDebug("Reading config file " DEMASTER_CONF "\n");
   //ini_t* conf = ini_load(DEMASTER_CONF);

   INIReader conf(DEMASTER_CONF);

   UVPATCH = conf.GetInteger("", "UV_PATCH", 0);
   DIRECT_IO = conf.GetInteger("", "DIRECT_IO", 0);
   LOG = conf.GetInteger("", "LOG", 0);
   BATTLE_CHARA = conf.GetInteger("", "BATTLE_CHARACTER", 0);
   FIELD_ENTITY = conf.GetInteger("", "FIELD_ENTITY", 0);
   BATTLE_HOOK = conf.GetInteger("", "BATTLE_HOOK_MONSTER_FIELDS", 0);
   FIELD_BACKGROUND = conf.GetInteger("", "FIELD_BACKGROUND", 0);
   WORLD_TEXTURES = conf.GetInteger("", "WORLD_TEXTURES", 0);
   TEXTURE_PATCH = conf.GetInteger("", "TEXTURE_PATCH", 1); //this one lacks actual demaster.conf so default to 1
   LINEAR_PATCH = conf.GetInteger("", "LINEAR_PATCH", 1);
   DEBUG_PATCH = conf.GetInteger("", "DEBUG_PATCH", 0);
   OPENGL_HOOK = conf.GetInteger("", "OPENGL_HOOK", 0);
   BATTLE_STAGE_ANIMATION_DELAY = conf.GetInteger("", "BATTLE_STAGE_ANIMATION_DELAY", 100);
   BATTLE_STAGE_FORCE_RELOAD = conf.GetInteger("", "BATTLE_STAGE_FORCE_RELOAD", 0);
}


//Maki: Why this doesn't work?
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* ep)
{
   DemasteredStackWalker sw;

   OutputDebug("*** Exception 0x%x, address 0x%x ***\n", ep->ExceptionRecord->ExceptionCode, ep->ExceptionRecord->ExceptionAddress);
   sw.ShowCallstack(
      GetCurrentThread(),
      ep->ContextRecord
   );

   // show cursor in case it was hidden
   ShowCursor(true);

   MessageBoxA(0, "Oops! Something very bad happened.\n\nPlease provide a copy of demasterlog.txt when reporting this error at https://github.com/MaKiPL/FF8_demastered/issues.\n", "Error", MB_OK);

   // let OS handle the crash
   SetUnhandledExceptionFilter(0);
   return EXCEPTION_CONTINUE_EXECUTION;
}
safe_bimg safe_bimg_init(bimg::ImageContainer* img)
{
	return {img, bimg::imageFree};
}



void checkGlew()
{
    if (glewInitialized) return;

        glewInitialized = true;
        GLenum err = glewInit();
        if (GLEW_OK != err)
            /* Problem: glewInit failed, something is seriously wrong. */
            OutputDebug("%s - GLEW Error: %s\n", __func__, glewGetErrorString(err));
    
}

safe_bimg LoadImageFromFile(const char* const filename)
{
    checkGlew();

   char msg[1024]{ 0 };

   OutputDebug("%s - Opening File: %s\n", __func__, filename);

   auto fp = std::fstream(filename, std::ios::in | std::ios::binary);

   if (!fp.is_open())
      return safe_bimg_init();

   fp.seekg(0, std::ios::end);

   auto filesize = fp.tellg();

   auto buffer = std::make_unique<char[]>(static_cast<size_t>(filesize));

   if (!buffer)
      return safe_bimg_init();

   fp.seekg(0, std::ios::beg);

   fp.read(buffer.get(), filesize);

   // REQUIRED BY BIMG FILE DECODING
   static bx::DefaultAllocator texAllocator{};
   bx::Error error{};
   constexpr static auto format{ bimg::TextureFormat::Enum::Count };
   auto* img = bimg::imageParse(&texAllocator, buffer.get(), static_cast<uint32_t>(filesize), format, &error);
   if (error.isOk())
      return safe_bimg_init(img);

   OutputDebug("%s::%d::bimg error: %s ", __func__, __LINE__, error.getMessage());
   return safe_bimg_init();
}
//appends DDS checks if file exists and then checks for PNG. returns false if atleast one exists. true on failure.
bool DDSorPNG(char* buffer, size_t in_size, const char* fmt, ...)
{
   const size_t size = in_size - 4U; //for extension
   va_list args;
   va_start(args, fmt);
   vsnprintf(buffer, size, fmt, args);
   strcat(buffer, ".dds");
   va_end(args);
   if (GetFileAttributesA(buffer) == INVALID_FILE_ATTRIBUTES)
   {
      va_start(args, fmt);
      vsnprintf(buffer, size, fmt, args);
      strcat(buffer, ".png");
      va_end(args);
      return GetFileAttributesA(buffer) == INVALID_FILE_ATTRIBUTES;
   }
   return false;
}
void RenderTexture(bimg::ImageContainer* img)
{
   TextureFormatInfo& texInfo = s_textureFormat[img->m_format];
   if (isCompressed(img->m_format))
	   RenderCompressedTexture(img, texInfo);
   else
	   RenderUncompressedTexture(img, texInfo);
}

void RenderUncompressedTexture(bimg::ImageContainer* img, TextureFormatInfo& texInfo)
{
    checkGlew();
   uint32_t width = img->m_width;
   uint32_t height = img->m_height;
   uint32_t depth = img->m_depth;
   const uint8_t startLod = bx::min<uint8_t>(0, img->m_numMips - 1);

   for (uint8_t lod = 0, num = img->m_numMips; lod < num; ++lod)
   {
      width = bx::uint32_max(1, width);
      height = bx::uint32_max(1, height);
      depth = 1;

      bimg::ImageMip mip;
      if (imageGetRawData(*img, 0, lod + startLod, img->m_data, img->m_size, mip))
         glTexImage2D(GL_TEXTURE_2D, lod, texInfo.m_internalFmt, img->m_width, img->m_height, 0, texInfo.m_fmt, texInfo.m_type, mip.m_data);
   }
}

void RenderCompressedTexture(bimg::ImageContainer* img, TextureFormatInfo& texInfo)
{
   if (GLEW_ARB_texture_compression)
   {
      uint32_t width = img->m_width;
      uint32_t height = img->m_height;
      uint32_t depth = img->m_depth;
      const uint8_t startLod = bx::min<uint8_t>(0, img->m_numMips - 1);

      for (uint8_t lod = 0, num = img->m_numMips; lod < num; ++lod)
      {
         width = bx::uint32_max(1, width);
         height = bx::uint32_max(1, height);
         depth = 1;

         bimg::ImageMip mip;
         if (bimg::imageGetRawData(*img, 0, lod + startLod, img->m_data, img->m_size, mip))
            glCompressedTexImage2D(GL_TEXTURE_2D, lod, texInfo.m_internalFmt, img->m_width, img->m_height, 0, mip.m_size, mip.m_data);
      }
   }
   else
      OutputDebug("Texture is compressed, but compression is not supported on your GPU. Skipping draw.");
}

BOOL WINAPI DllMain(

   HINSTANCE hinstDLL, // handle to DLL module
   DWORD fdwReason, // reason for calling function
   LPVOID lpReserved) // reserved
{
   if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
      return 0;

   SetUnhandledExceptionFilter(ExceptionHandler);

   CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)kieroThread, NULL, 0, NULL);

   AllocConsole();
   (void)freopen("CONOUT$", "w", stdout);
   (void)freopen("CON", "r", stdin);
   InitTest();
   ReadConfigFile();
   if (LOG) logFile = decltype(logFile){ fopen("demasterlog.txt", "wb"), fclose };

#if JAPANESE_PATCH
   HMODULE IMG_BASE = GetModuleHandleA("FFVIII_JP");
#else
   HMODULE IMG_BASE = GetModuleHandleA("FFVIII_EFIGS");
#endif
   IMAGE_BASE = (long long)IMG_BASE;

   MODULEINFO modinfo = {};
   K32GetModuleInformation(GetCurrentProcess(), IMG_BASE, &modinfo, sizeof(MODULEINFO));
   IMAGE_SIZE = modinfo.SizeOfImage;

   THIS_BASE = (DWORD)hinstDLL;
   K32GetModuleInformation(GetCurrentProcess(), hinstDLL, &modinfo, sizeof(MODULEINFO));
   THIS_SIZE = modinfo.SizeOfImage;

   OutputDebug("IMAGE_BASE at: %lX; OPENGL at: %lX\n", IMAGE_BASE, OPENGL_HANDLE);

   MH_Initialize();
   GetWindow();

   //LET'S GET THE HACKING DONE
   if (DIRECT_IO)
      ApplyDirectIO();
   if (UVPATCH)
      ApplyUVPatch();
   if (TEXTURE_PATCH && DIRECT_IO)
      ReplaceTextureFunction();
   if (DEBUG_PATCH) //it's just one func ;-;
       InjectJMP(IMAGE_BASE + GetAddress(NULLSUB_DEBUG), (DWORD)OutputDebug);
   if (LINEAR_PATCH)
      ApplyFilteringPatch();
   if (OPENGL_HOOK)
      HookOpenGL();


   MH_EnableHook(MH_ALL_HOOKS);
   return 1; //all success
}
#undef CRASHLOG
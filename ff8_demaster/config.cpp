#include "config.h"
#include <INIReader.h>
#include "debug.h"
#include "minhook/include/MinHook.h"

void ReadConfigFile()
{
    if (GetFileAttributesA(DEMASTER_CONF) == INVALID_FILE_ATTRIBUTES)
        exit(ERROR_FILE_NOT_FOUND);

    const INIReader conf(DEMASTER_CONF);

    UVPATCH = conf.GetInteger("", "UV_PATCH", 0);
    DIRECT_IO = conf.GetInteger("", "DIRECT_IO", 0);
    FILE_LOG = conf.GetInteger("", "LOG_TO_FILE", 0);
    LOG_FILENAME = conf.Get("", "LOG_FILENAME", "demaster.txt");
    LOG_SEVERITY = conf.GetInteger("", "LOG_SEVERITY", plog::Severity::info);
    BATTLE_CHARA = conf.GetInteger("", "BATTLE_CHARACTER", 0);
    FIELD_ENTITY = conf.GetInteger("", "FIELD_ENTITY", 0);
    BATTLE_HOOK = conf.GetInteger("", "BATTLE_HOOK_MONSTER_FIELDS", 0);
    FIELD_BACKGROUND = conf.GetInteger("", "FIELD_BACKGROUND", 0);
    WORLD_TEXTURES = conf.GetInteger("", "WORLD_TEXTURES", 0);
    TEXTURE_PATCH = conf.GetInteger("", "TEXTURE_PATCH", 1); //this one lacks actual demaster.conf so default to 1
    FORCE_NEAREST_FILTERING = conf.GetInteger("", "FORCE_NEAREST_FILTERING", 0);
    LINEAR_PATCH = conf.GetInteger("", "LINEAR_PATCH", 1);
    DEBUG_PATCH = conf.GetInteger("", "DEBUG_PATCH", 0);
    OPENGL_HOOK = conf.GetInteger("", "OPENGL_HOOK", 0);
    BATTLE_STAGE_ANIMATION_DELAY = conf.GetInteger("", "BATTLE_STAGE_ANIMATION_DELAY", 100);
    BATTLE_STAGE_FORCE_RELOAD = conf.GetInteger("", "BATTLE_STAGE_FORCE_RELOAD", 0);
	HASH_ENABLED = conf.GetInteger("", "HASH_ENABLED", 1);
    HASH_OUTPUT = conf.GetInteger("", "HASH_OUTPUT", 0);
    HASH_OUTPUT_EXT = conf.GetInteger("", "HASH_OUTPUT_EXT", 0);
    HASH_LOAD_HD = conf.GetInteger("", "HASH_LOAD_HD", 1);
    HASH_LOAD_HD_EXT = conf.GetInteger("", "HASH_LOAD_HD_EXT", 1);
}

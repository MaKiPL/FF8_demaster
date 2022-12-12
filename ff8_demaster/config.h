#pragma once

#define DEMASTER_CONF "demaster.conf"
#include <string>

inline int UVPATCH;
inline int DIRECT_IO;
inline int TEXTURE_PATCH;
inline int FORCE_NEAREST_FILTERING;
inline int DEBUG_PATCH;
inline int FILE_LOG;
inline std::string LOG_FILENAME;
inline int LOG_SEVERITY;
inline int BATTLE_CHARA;
inline int FIELD_ENTITY;
inline int BATTLE_HOOK;
inline int FIELD_BACKGROUND;
inline int WORLD_TEXTURES;
inline int LINEAR_PATCH;
inline int OPENGL_HOOK;
inline int BATTLE_STAGE_FORCE_RELOAD;
inline int HASH_ENABLED;
inline int HASH_OUTPUT;
inline int HASH_LOAD_HD;

inline int BATTLE_STAGE_ANIMATION_DELAY;
inline int HASH_OUTPUT_EXT;
inline int HASH_LOAD_HD_EXT;

void ReadConfigFile();
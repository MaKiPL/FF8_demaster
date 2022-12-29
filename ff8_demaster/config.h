#pragma once

#define DEMASTER_CONF "demaster.conf"

inline int UVPATCH;
inline int DIRECT_IO;
inline int TEXTURE_PATCH;
inline int DEBUG_PATCH;
inline int IMGUI_DEBUG;
inline int LOG;
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
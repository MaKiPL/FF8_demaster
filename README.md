# FF8_demastered
This is injectable DLL that acts as an import-dependency for FFVIII_EFIGS.dll hot-patching the gamecode and allowing future super-fast-development mods

## Usage:
1. Just drop to your folder of Final Fantasy VIII Remastered installation.
Usually:<br/>
_D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII Remastered_<br/>
2. Extract your main.zzz and other.zzz to new folder called "EXP". This DLL was compiled to use unpacked .zzz files in folder called "EXP".
If user doesn't have such folder the patch wouldn't be applied to not crash the application.<br/>
Tools to extract zzz files: [qt.zzz](https://github.com/myst6re/qt-zzz/releases), [zzzDeArchive](https://github.com/Sebanisu/zzzDeArchive/releases)



## Troubleshooting
### Usage:
`FFVIII_LAUNCER.exe` crashes, and won't let you start the game.<br/>
  - [Use an alternate launcher](https://github.com/Sebanisu/FFVIII_LAUNCHER-Alternative/releases) - now comes with the release.
### Compiling:
`Error	MSB8020	The build tools for v142 (Platform Toolset = 'v142') cannot be found.`<br/>
  - Open properties (right click _ff8_demaster_ and click properties) and change `Platform Toolset` to something you have installed.

`Error	MSB8036	The Windows SDK version 10.0 was not found.`<br/>
  - Open properties (right click _ff8_demaster_ and click properties) and change `Windows SDK Version` to something you have installed.

`Error	MSB3191	Unable to create directory "D:\FINAL FANTASY VIII REMASTERED\".`<br/>
  - Open properties (right click _ff8_demaster_ and click properties) and change `Output Directory` to your _FINAL FANTASY VIII Remastered_ folder.<br/>
Default steam install: `C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered`


![Properties Page in Visual Studio 2017](https://cdn.discordapp.com/attachments/622172784667394052/628892957327753216/unknown.png)

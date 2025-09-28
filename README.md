# ff8_demaster
FF8_Demaster is unofficial patch for Final Fantasy VIII Remastered game by SquareEnix. It works as DLL-dependency and has custom launcher with brand new functions. Patch makes it possible to load new textures, use non-fixed-resolution textures and will aim in tweaking issues

## Usage:
1. Just drop to your folder of Final Fantasy VIII Remastered installation.
Usually:<br/>
_D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII Remastered_<br/>
2. If it's your first run- please open FFVIII_Demaster_Manager.exe to unpacking files

## SteamDeck / Proton:
[https://www.protondb.com/app/1026680#nyIcYa992E](https://www.protondb.com/app/1026680#nyIcYa992E)

Basically run this with:
`export WINEDLLOVERRIDES="dinput8=n,b"; %command%`

## Troubleshooting
### Compiling:
`Error	MSB8020	The build tools for v142 (Platform Toolset = 'v142') cannot be found.`<br/>
  - Open properties (right click _ff8_demaster_ and click properties) and change `Platform Toolset` to something you have installed.

`Error	MSB8036	The Windows SDK version 10.0 was not found.`<br/>
  - Open properties (right click _ff8_demaster_ and click properties) and change `Windows SDK Version` to something you have installed.

`Error	MSB3191	Unable to create directory "D:\FINAL FANTASY VIII REMASTERED\".`<br/>
  - Open properties (right click _ff8_demaster_ and click properties) and change `Output Directory` to your _FINAL FANTASY VIII Remastered_ folder.<br/>
Default steam install: `C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered`


![Properties Page in Visual Studio 2017](https://cdn.discordapp.com/attachments/622172784667394052/628892957327753216/unknown.png)

## Third Party Libraries

| Library                                                      | License               |
| ------------------------------------------------------------ |:---------------------:|
| [BX](https://github.com/bkaradzic/bx)                        | BSD 2-Clause          |
| [BIMG](https://github.com/bkaradzic/bimg)                    | BSD 2-Clause          |
| [GLEW](https://github.com/nigels-com/glew)                   | Khronos License (MIT) |
| [inih](https://github.com/jtilly/inih)                       | New BSD               |
| [StackWalker](https://github.com/JochenKalmbach/StackWalker) | BSD-2-Clause          |

## Third party

HEXT based on HEXT format from [HEXT Tools](https://forums.qhimm.com/index.php?topic=13574.0) by Daniel L. P. Burke

## Compilation

1. Install vcpkg
2. Do a `vcpkg integrate install`
3. That's all. VS/Rider should automatically utilize the vcpkg.json file to add ports
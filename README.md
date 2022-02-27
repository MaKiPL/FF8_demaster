# IMPORTANT!
I, **Maki**, the **creator** of **ff8_demastered**, officially support **Ukraine** in military conflict. My country is one of the biggest ally of Ukraine and we are all ready to respond along with **NATO** and **USA** with all the power. The fight is real and **anonymous** hacker group is really actively cyber-attacking the government of Russia (they overtook the television, media, Internet access and official websites along with leaking gov documents). The **leader of Russia** has **blood on his hands** of all the **soldiers** of both **Ukraine** and **Russia**, also **civilians** which include **children** (yes, **they bombed kindergarten**). **Dear Russian developers**- we are **not against you**, but the **leader of your country**. We **don't want** to lose our **families** as well as you **don't want** to **lose** your **friends**, **fathers**, **husbands**. Please **stand against the dictator**, it's not a shame to object the **lunatic** who **destroys your country**, your **economy**- this is **you who feel the pain**, this is **you who is hit with sanctions**, not this psycho. This is **not a leader**, this is just a crazy person with power that is **using all of you** for his own wet dream. I know **several great developers from Russia** and **I want to remain your friend.** I keep **fingers crossed** to **neutralise the lunatic** and **get back to normal life**. 

![USA flag](https://mgomulak.pl/f/flag22_64.jpg)**I officially want to provide any kind of my knowledge of software hacking and engineering to help with cyber-aspect of the war. Please contact me directly, I'll be happy to help you.** 

![Ukrainian flag](https://mgomulak.pl/f/flag46_64.jpg)**Я хочу надати свої знання програмного забезпечення. Будь ласка, зв'яжіться зі мною. Я більш ніж радий допомогти вам!**

![Polish flag](https://mgomulak.pl/f/flag70_64.jpg)**Chciałbym oficjalnie użyczyć swoich zdolności dot. hakowania i zabezpieczania oprogramowania oraz jego produkcji celem pomocy w cyberbezpieczeństwie podczas konfliktu wojennego. Pomoc będzie dla mnie zaszczytem.** 
### #FreeUkraine
### #gamedevFreeUkraine
Under above tag I'm collecting gamedev companies that share support for UKR and also those that don't. I'm collecting the entries on **Twitter**. 

### Dotemu- a french game porting developer that made Final Fantasy VIII 2013 and Remaster- did not make a statement about the conflict despite being located in France and is part of European Union as of 27.02.2021@17:00

# ff8_demastered
FF8_Demastered is unofficial patch for Final Fantasy VIII Remastered game by SquareEnix. It works as DLL-dependency and has custom launcher with brand new functions. Patch makes it possible to load new textures, use non-fixed-resolution textures and will aim in tweaking issues

## Usage:
1. Just drop to your folder of Final Fantasy VIII Remastered installation.
Usually:<br/>
_D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII Remastered_<br/>
2. If it's your first run- please open FFVIII_Demaster_Manager.exe to unpacking files

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
| [TinySTL](https://github.com/mendsley/tinystl)               | BSD-2-Clause          |<!-- no longer used?-->

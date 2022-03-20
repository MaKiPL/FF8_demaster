## naming conventions

### methods:

*static:*

```cpp
static void PublicMethod(void* arg1, const char * arg2); 
```

- UpperCamelCase

*class public method:*

```cpp
void PublicMethod(int arg1, int arg2, BYTE* arg3); 
```

- UpperCamelCase

*class private method:*

```cpp
int get_internal_idx(int arg1, char arg2);
```

- lower_case

* * *

### variables:

```cpp
int lowerCamelCase;
unsigned char lowerCamelCaseByte;
#define DEFINE_NAME
enum exampleEnum : unsigned byte
{
    one,
    two,
    three
}
struct exampleStruct
{
    int exampleVariable;
    int mixed,example,variables;
}
```

* * *

### filenames:

```
main.cpp
main.h
patch.cpp
modules.cpp
multinamepath.cpp
foldername/example.cpp
foobar/foobarexample.cpp
nested/folder/namesexample.cpp
```

## Name shortage:

|FullName|Short|Example|
| --- | --- | --- |
| Index | id |**id**Character|
| |idx|**idx**Vehicle|
| WorldMap | wm |patch**wm**|
| Menu | menu|module**menu**.cpp|
| Menu Start | start | module**start**.cpp|
| Battle | btl | module**btl**.cpp|
| Field | fld | module**fld**.h|
| Card Game | card | module**card**.h|
| Text | txt | patch**txt**.cpp|
| Return | ret | **ret**Address |
| Absolute Pointer | p | **p**CreateWindow
| Bool | b | **b**Enabled |
| String | s | **s**Title |
| Count Of | cnt | **cnt**Patches |
| Texture | tex | texChara |
| Assembler | asm | asmPatch**IO** |
| Input/Output | IO | patch**io**.cpp |

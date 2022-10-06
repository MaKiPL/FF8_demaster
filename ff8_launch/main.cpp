#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <windows.h>
#include <filesystem>


int main()
{
	std::filesystem::path pt("FFVIII.exe");
	if(!std::filesystem::exists(pt))
	{
		MessageBoxW(NULL, L"FFVIII.exe ファイルが見つかりません", 
			L"エラー", MB_OK);
		return ERROR_FILE_NOT_FOUND;
	}
	std::ifstream ifs;
	ifs.open(L"FFVIII.exe", std::ios::binary | std::ios::in);
	if(!ifs.is_open())
	{
		MessageBoxW(NULL, L"FFVIII.exe ファイルは存在しますが開けません", 
			L"エラー", MB_OK);
		return ERROR_FILE_CORRUPT;
	}

	std::vector<unsigned char> buffer;
	buffer.assign(std::istreambuf_iterator<char>(ifs),
		std::istreambuf_iterator<char>());

	ifs.close();

	buffer[0x94B] = 0x89;
	buffer[0x94B+1] = 0xC8;
	buffer[0x94B+2] = 0x90;

	std::ofstream ofs;
	ofs.open(L"FFVIII_JP.exe", std::ios::binary | std::ios::out | std::ios::trunc);
	ofs.write((const char*) & buffer[0], buffer.size());
	ofs.close();
	
	MessageBoxW(NULL, L"やった！ パッチは「FFVIII_JP.exe」という名前の新しいファイルを作成しました. ファイルを実行してみてください", L"YATTA!", MB_OK);

	return 0;
}
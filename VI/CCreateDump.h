#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
class CCreateDump
{
public:
	CCreateDump();
	~CCreateDump(void);
	static CCreateDump* Instance();
	static long __stdcall UnhandleExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);

	void DeclarDumpFile(std::string dmpFileName = "");
private:
	static std::string    strDumpFile;
	static CCreateDump*    __instance;
};

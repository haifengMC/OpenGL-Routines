#ifndef __TOOLS_H_
#define __TOOLS_H_

#ifdef _WIN32

void toANSI(const std::string& utf8Str, std::string& ansiStr)
{
	using namespace std;
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
	WCHAR* unicodeString = new WCHAR[bufferSize];
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, unicodeString, bufferSize);
	bufferSize = WideCharToMultiByte(CP_ACP, 0, unicodeString, -1, nullptr, 0, nullptr, nullptr);
	CHAR* ansiString = new CHAR[bufferSize];
	WideCharToMultiByte(CP_ACP, 0, unicodeString, -1, ansiString, bufferSize, nullptr, nullptr);
	ansiStr.clear();
	ansiStr.append(ansiString);
	delete[] ansiString;
	delete[] unicodeString;
}

void toUTF8(const std::string& ansiStr, std::string& utf8Str)
{
	using namespace std;
	int bufferSize = MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, nullptr, 0);
	WCHAR* unicodeString = new WCHAR[bufferSize];
	MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, unicodeString, bufferSize);
	bufferSize = WideCharToMultiByte(CP_UTF8, 0, unicodeString, -1, nullptr, 0, nullptr, nullptr);
	CHAR* utf8String = new CHAR[bufferSize];
	WideCharToMultiByte( CP_UTF8, 0, unicodeString, -1, utf8String, bufferSize, nullptr, nullptr);
	utf8Str.clear();
	utf8Str.append(utf8String);
	delete[] utf8String;
	delete[] unicodeString;
}

#endif // _WIN32


#endif // !__TOOLS_H_

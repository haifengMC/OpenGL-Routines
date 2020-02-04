#ifndef __TOOLS_H_
#define __TOOLS_H_
#include "vgl.h"

#ifdef _WIN32

//utf8转ansi
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

//ansi转utf8
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

//用于将GLSL类型转换为存储大小
size_t TypeSize(GLenum type)
{
	size_t size = 0;
#define  CASE(Enum, Count, Type) case Enum: size = Count * sizeof(Type);break
	switch (type)
	{
		CASE(GL_FLOAT,				1,	GLfloat);
		CASE(GL_FLOAT_VEC2,			2,	GLfloat);
		CASE(GL_FLOAT_VEC3,			3,	GLfloat);
		CASE(GL_FLOAT_VEC4,			4,	GLfloat);
		CASE(GL_INT,				1,	GLint);
		CASE(GL_INT_VEC2,			2,	GLint);
		CASE(GL_INT_VEC3,			3,	GLint);
		CASE(GL_INT_VEC4,			4,	GLint);
		CASE(GL_UNSIGNED_INT,		1,	GLuint);
		CASE(GL_UNSIGNED_INT_VEC2,	2,	GLuint);
		CASE(GL_UNSIGNED_INT_VEC3,	3,	GLuint);
		CASE(GL_UNSIGNED_INT_VEC4,	4,	GLuint);
		CASE(GL_BOOL,				1,	GLboolean);
		CASE(GL_BOOL_VEC2,			2,	GLboolean);
		CASE(GL_BOOL_VEC3,			3,	GLboolean);
		CASE(GL_BOOL_VEC4,			4,	GLboolean);
		CASE(GL_FLOAT_MAT2,			4,	GLfloat);
		CASE(GL_FLOAT_MAT2x3,		6,	GLfloat);
		CASE(GL_FLOAT_MAT2x4,		8,	GLfloat);
		CASE(GL_FLOAT_MAT3,			9,	GLfloat);
		CASE(GL_FLOAT_MAT3x2,		6,	GLfloat);
		CASE(GL_FLOAT_MAT3x4,		12, GLfloat);
		CASE(GL_FLOAT_MAT4,			16, GLfloat);
		CASE(GL_FLOAT_MAT4x2,		8,	GLfloat);
		CASE(GL_FLOAT_MAT4x3,		12, GLfloat);
	}
#undef CASE
	return size;
}

#endif // !__TOOLS_H_

//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __LOAD_SHADERS_H__
#define __LOAD_SHADERS_H__

#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

//----------------------------------------------------------------------------
//
//  LoadShaders() takes an array of ShaderFile structures, each of which
//    contains the type of the shader, and a pointer a C-style character
//    string (i.e., a NULL-terminated array of characters) containing the
//    entire shader source.
//
//  The array of structures is terminated by a final Shader with the
//    "type" field set to GL_NONE.
//
//  LoadShaders() returns the shader program value (as returned by
//    glCreateProgram()) on success, or zero on failure. 
//
enum : GLenum
{
	GLLS_FILENAME_TEXT,
	GLLS_FILENAME_BINARY,
    GLLS_SOURCE,
};

enum : GLenum
{
	GLLS_ERRMSG_ALL,		//显示所有错误信息
	GLLS_ERRMSG_FILE,		//显示所有文件操作错误信息
	GLLS_ERRMSG_SHADER,		//显示所有着色器编译错误信息
	GLLS_ERRMSG_PROGRAM,	//显示所有程序编译错误信息
	GLLS_ERRMSG_GLSL,		//显示所有着色器错误信息
};

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

typedef struct _ShaderSource
{
	GLenum      type;
	const char* filename;
	std::string source;
} ShaderSource;

typedef struct _ErrMsg
{
	GLenum		type;
	std::string msg;
} ErrMsg;

typedef struct _ShaderInfoEx
{
	GLenum       type;
	GLuint       shader;
	ShaderSource source;
	ErrMsg*		 errMsg;//输出错误信息
} ShaderInfoEx;

typedef struct {
	GLenum type;
	const GLchar* suffix;
} ShaderSuffix;

const ShaderSuffix suffixes[] =
{
	{GL_VERTEX_SHADER, ".vert"},
	{GL_TESS_CONTROL_SHADER, ".tesc"},
	{GL_TESS_EVALUATION_SHADER, ".tese"},
	{GL_GEOMETRY_SHADER, ".geom"},
	{GL_FRAGMENT_SHADER, ".frag"},
	{GL_COMPUTE_SHADER, ".comp"},
};



GLuint LoadShader(ShaderInfo*, std::string* = NULL, GLenum = GLLS_ERRMSG_ALL);
//GLuint LoadShaderEx(ShaderInfoEx*);

GLuint LoadShaders(ShaderInfo*, GLenum = GLLS_FILENAME_TEXT);
//GLuint LoadShadersEx(ShaderInfoEx*);

GLuint LoadShadersBySpirV(ShaderInfo*);
//GLuint LoadShadersBySpirVEx(ShaderInfoEx*);

GLuint LoadProgramByBinary(const std::string& fileName, ErrMsg* errMsg = NULL);

//----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __LOAD_SHADERS_H__

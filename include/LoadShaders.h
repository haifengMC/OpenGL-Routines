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
enum
{
    GLLS_FILENAME,
    GLLS_SOURCE,
};

enum
{
	GLLS_ERRMSG_ALL,		//��ʾ���д�����Ϣ
	GLLS_ERRMSG_FILE,		//��ʾ�����ļ�����������Ϣ
	GLLS_ERRMSG_SHADER,		//��ʾ������ɫ�����������Ϣ
	GLLS_ERRMSG_PROGRAM,	//��ʾ���г�����������Ϣ
	GLLS_ERRMSG_GLSL,		//��ʾ������ɫ��������Ϣ
};

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

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
GLuint LoadShaders(ShaderInfo*, GLenum = GLLS_FILENAME);
GLuint LoadShadersBySpirV(ShaderInfo*);

//----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __LOAD_SHADERS_H__

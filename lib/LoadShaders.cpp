//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.cxx ---
//
//////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <sstream>

#include <GL3/gl3w.h>
#include "LoadShaders.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//----------------------------------------------------------------------------

static const GLchar*
ReadShader( const char* filename )
{
#ifdef WIN32
	FILE* infile;
	fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::ostringstream os;
        os << "Unable to open file '" << filename << "'" << std::endl;
#ifdef WIN32
		OutputDebugString(os.str().c_str());
#endif // WIN32
        std::cerr << os.str();
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

//----------------------------------------------------------------------------

GLuint
LoadShaders(ShaderInfo* shaders, GLenum usage)
{
    if ( shaders == NULL ) { return 0; }

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while ( entry->type != GL_NONE ) {
        GLuint shader = glCreateShader( entry->type );

        entry->shader = shader;

        const GLchar* source;
        if (GLLS_FILENAME == usage) source = ReadShader(entry->filename);
        else if (GLLS_SOURCE == usage) source = entry->filename;
       
        if ( source == NULL ) {
            for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
                glDeleteShader( entry->shader );
                entry->shader = 0;
            }

            return 0;
        }

        glShaderSource( shader, 1, &source, NULL );
        if (GLLS_FILENAME == usage) delete [] source;

        glCompileShader( shader );

        GLint compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
            std::ostringstream os;
            os << "Shader[" << shader << "] compilation failed: " << log << std::endl;
#ifdef WIN32
            OutputDebugString(os.str().c_str());
#endif // WIN32
            std::cerr << os.str();
            delete [] log;
#endif /* DEBUG */

            return 0;
        }

        glAttachShader( program, shader );
        
        ++entry;
    }

    glLinkProgram( program );

    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
#ifdef _DEBUG
        GLsizei len;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog(program, len, &len, log);
		std::ostringstream os;
		os << "Program[" << program << "] shader linking failed: " << log << std::endl;
#ifdef WIN32
		OutputDebugString(os.str().c_str());
#endif // WIN32
		std::cerr << os.str();
        delete [] log;
#endif /* DEBUG */

        for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
            glDeleteShader( entry->shader );
            entry->shader = 0;
        }
        
        return 0;
    }

    return program;
}

//----------------------------------------------------------------------------

static const GLchar*
ReadShaderBySpirV(const char* filename, GLuint& len)
{
#ifdef WIN32
	FILE* infile;
	fopen_s(&infile, filename, "rb");
#else
	FILE* infile = fopen(filename, "rb");
#endif // WIN32

	if (!infile) {
#ifdef _DEBUG
		std::ostringstream os;
		os << "Unable to open file '" << filename << "'" << std::endl;
#ifdef WIN32
		OutputDebugString(os.str().c_str());
#endif // WIN32
		std::cerr << os.str();
#endif /* DEBUG */
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
    len = ftell(infile);
	fseek(infile, 0, SEEK_SET);
    if (!len) return NULL;

	GLchar* source = new GLchar[len];

	fread(source, 1, len, infile);
	fclose(infile);

	return const_cast<const GLchar*>(source);
}

//----------------------------------------------------------------------------

GLuint
LoadShadersBySpirV(ShaderInfo* shaders)
{
	if (shaders == NULL) { return 0; }

	GLuint program = glCreateProgram();

    for (ShaderInfo* entry = shaders; GL_NONE != entry->type; ++entry)
    {
        GLuint len = 0;
        const GLchar* binary = ReadShaderBySpirV(entry->filename, len);
        if (!binary || !len)
            return 0;

		entry->shader = glCreateShader(entry->type);
        GLint num = 0;
        glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &num);
		if (!num)
			return 0;

        glShaderBinary(1, &entry->shader, GL_SHADER_BINARY_FORMAT_SPIR_V, binary, len);
        glSpecializeShader(entry->shader, "main", 0, NULL, NULL);

		GLint compiled = 0;
		glGetShaderiv(entry->shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv(entry->shader, GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetShaderInfoLog(entry->shader, len, &len, log);
            std::ostringstream os;
            os << "Shader[" << entry->shader << "] compilation failed: " << log << std::endl;
#ifdef WIN32
            OutputDebugString(os.str().c_str());
#endif // WIN32
            std::cerr << os.str();
            delete[] log;
#endif /* DEBUG */

            return 0;
        }
        glAttachShader(program, entry->shader);
    }

	glLinkProgram(program);

	GLint linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) 
    {
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		std::ostringstream os;
		os << "Program[" << program << "] shader linking failed: " << log << std::endl;
#ifdef WIN32
		OutputDebugString(os.str().c_str());
#endif // WIN32
		std::cerr << os.str();
		delete[] log;
#endif /* DEBUG */

		for (ShaderInfo* entry = shaders; entry->type != GL_NONE; ++entry)
        {
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}

		return 0;
	}

	return program;
}

#ifdef __cplusplus
}
#endif // __cplusplus



#ifndef __VTOOLS_H_
#define __VTOOLS_H_
#include "vgl.h"

//���ڽ�GLSL����ת��Ϊ�洢��С
size_t TypeSize(GLenum type)
{
	size_t size = 0;
#define  CASE(Enum, Count, Type) case Enum: size = Count * sizeof(Type);break
	switch (type)
	{
		CASE(GL_FLOAT, 1, GLfloat);
		CASE(GL_FLOAT_VEC2, 2, GLfloat);
		CASE(GL_FLOAT_VEC3, 3, GLfloat);
		CASE(GL_FLOAT_VEC4, 4, GLfloat);
		CASE(GL_INT, 1, GLint);
		CASE(GL_INT_VEC2, 2, GLint);
		CASE(GL_INT_VEC3, 3, GLint);
		CASE(GL_INT_VEC4, 4, GLint);
		CASE(GL_UNSIGNED_INT, 1, GLuint);
		CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
		CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
		CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
		CASE(GL_BOOL, 1, GLboolean);
		CASE(GL_BOOL_VEC2, 2, GLboolean);
		CASE(GL_BOOL_VEC3, 3, GLboolean);
		CASE(GL_BOOL_VEC4, 4, GLboolean);
		CASE(GL_FLOAT_MAT2, 4, GLfloat);
		CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
		CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
		CASE(GL_FLOAT_MAT3, 9, GLfloat);
		CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
		CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
		CASE(GL_FLOAT_MAT4, 16, GLfloat);
		CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
		CASE(GL_FLOAT_MAT4x3, 12, GLfloat);
	}
#undef CASE
	return size;
}

#endif // !__VTOOLS_H_

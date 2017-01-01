#ifndef UTILS_HPP
#define UTILS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

template <size_t _Size> inline int length(int(& array)[_Size])
{
    return _Size;
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

GLuint loadBMP_custom(const char * imagepath);

GLuint loadPNG(const char *imagepath);

#endif /* end of include guard: UTILS_HPP */
